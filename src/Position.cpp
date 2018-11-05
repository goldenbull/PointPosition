// Position.cpp: implementation of the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Position.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosition::CPosition()
{

}

CPosition::~CPosition()
{

}

bool CPosition::GetObsFileData(CObsFile& obsFile)
{
	obsFileData = obsFile.GetFileData();
    obsTypeNum = obsFile.GetObsTypeNum();
	obsTypeList = obsFile.GetObsTypeList();
	return true;
}

bool CPosition::GetNavFileData(CNavFile& navFile)
{
	navFileData = navFile.GetNavData();
	return true;
}

NavFileData CPosition::GetNavData(CTime& obsTime)
{
	double deltaTime;
	for(int i = 0; i < navFileData.size(); i++)
	{
		deltaTime = fabs((obsTime - navFileData[i].satClockTime) * 1);
		if (deltaTime <= 7200)
		{
			mark = i;
			return navFileData[i];
		}
	}
	cout << "Sorry, there are some problems in the navigation file! Please check it." << endl;
}

OneNavData CPosition::GetOneNavData(string& satPRN, NavFileData& navData)
{
	int prn;
	prn = atoi(satPRN.substr(1,2).c_str());

loop:	for(int i = 0; i < navFileData[mark].satNavRecord.size(); i++)
	{
		if (prn == navFileData[mark].satNavRecord[i].satPRN)
			return navFileData[mark].satNavRecord[i];
	}
	mark ++;
	goto loop;
	cout << "Sorry, can't find this satellite's navigation. Please check!" << endl;
}

bool CPosition::ComputeSatCoor(CTime& sendTime, CCoor& satCoor, OneNavData navData )
{
	//1.计算卫星运动的平均角速度n
	double n0,n;
	n0 = sqrt(GM) / pow(navData.sqrt_A,3);
	n = n0 + navData.delta_n;
	
	//计算相对于星历参考时刻的时间
    CTime tk;
	tk = sendTime - navData.epochTime;
	if(tk > 302400) tk = tk - 604800;
	if(tk < -302400) tk = tk + 604800;
	
	//2.计算观测瞬间卫星的平近点角M
	double M;
	M = navData.M0 + tk * n;
				
	//3.计算偏近点角(弧度)
	double E0,E;
	E  = 0;
	do
	{
		E0 = E;
		E = M + navData.e * sin(E0);		
	}
	while(fabs(E - E0) > 1.0e-14);
	
	//4.计算真近点角f
	double f,cosf,sinf;
	cosf = (cos(E) - navData.e); // (1 - e * cos(E));
	sinf = sqrt(1 - pow(navData.e,2)) * sin(E); // (1 - navData.e * cos(E));
	f = atan2(sinf , cosf);
	
	//5.计算升交距角u'
	double uP;
	uP = navData.omega + f;
				
	//6.计算摄动改正项
	double deltau, deltar, deltai;
	deltau = navData.Cuc * cos(2 * uP) + navData.Cus * sin(2 * uP);
	deltar = navData.Crc * cos(2 * uP) + navData.Crs * sin(2 * uP);
	deltai = navData.Cic * cos(2 * uP) + navData.Cis * sin(2 * uP);
				
	//7.对u'、r'、i0进行摄动改正
	double u, r, i;
	u = uP + deltau;
	r = pow(navData.sqrt_A, 2) * (1 - navData.e * cos(E)) + deltar;
	i = navData.i0 + deltai + tk * navData.IDOT;
	
	//8.计算卫星在轨道面坐标系中的位置
	double x, y;
	x = r * cos(u);
	y = r * sin(u);
	
	//9.计算观测瞬间升交点的经度L
	double L, toe;
	toe = navData.epochTime.GetGpsTime().tow.sn + navData.epochTime.GetGpsTime().tow.tos;
	L = navData.OMEGA + tk * (navData.OMEGA_DOT - we) - we * toe;
				
	//10.计算卫星在瞬时地球坐标系(地固系)中的位置
	satCoor.X( x * cos(L) - y * cos(i) * sin(L));
	satCoor.Y( x * sin(L) + y * cos(i) * cos(L));
	satCoor.Z( y * sin(i));
	
	return true;
}

bool CPosition::GetSatCoor( CTime obsTime, CTime& sendTime, OneNavData& navData, CCoor& sitCoor, CCoor& satCoor, double& satClkBias)
{
	double         deltaT0, deltaT, R;
	double         deltaAlpha;
    matrix<double> A(3, 3);
	matrix<double> crdSat(3, 1);
	
	deltaT = 0.075;
	double a;
	
	do
	{
		deltaT0 = deltaT;
		sendTime = obsTime - deltaT0;
		this->ComputeSatCoor(sendTime,satCoor,navData);
        deltaAlpha = we * deltaT0;
		
		satCoor.X( satCoor.X() * cos(deltaAlpha) + satCoor.Y() * sin(deltaAlpha));
		satCoor.Y( satCoor.Y() * cos(deltaAlpha) - satCoor.X() * sin(deltaAlpha));
		
		satClkBias = this->ComSatClkBias(sendTime,navData);
		R = sitCoor.DistanceTo(satCoor);
		deltaT = R / c - satClkBias;
		a = fabs(deltaT0 - deltaT);
	}
	while ( a > 1.0e-15 );
	
	return true;
}

double CPosition::GetTropDelay(CCoor &satCoor, CCoor &sitCoor)
{
	double E;
	double bias;
	
	E = satCoor.ElevationAngleTo(sitCoor);
	bias = 2.47 / (sin(E) + 0.0121);
	
	return bias;
}

double CPosition::GetIonDelay(const double& P1, const double& P2)
{
	return 1.54573 * ( P1 - P2 );
}

double CPosition::ComSatClkBias(CTime& sendTime, OneNavData navData)
{
	//1.计算卫星运动的平均角速度n
	double n0,n;
	n0 = sqrt(GM) / pow(navData.sqrt_A,3);
	n = n0 + navData.delta_n;
	
	//计算相对于星历参考时刻的时间
    CTime tk;
	tk = sendTime - navData.epochTime;
	if(tk > 302400) tk = tk - 604800;
	if(tk < -302400) tk = tk + 604800;
	
	//2.计算观测瞬间卫星的平近点角M
	double M;
	M = navData.M0 + tk * n;
				
	//3.计算偏近点角(弧度)
	double E0,E;
	E  = 0;
	do
	{
		E0 = E;
		E = M + navData.e * sin(E0);		
	}
	while(fabs(E - E0) > 1.0e-14);

	double deltaTr;
	double satClkBias;
	deltaTr = F * navData.e * navData.sqrt_A * sin(E);
	satClkBias = navData.clockBias + (sendTime - navData.satTime) * navData.clockDrift
		+ navData.driftRate* pow((sendTime - navData.satTime) * 1, 2) + deltaTr - navData.TGD;
	return satClkBias;
}

bool CPosition::ComputeFactors(CCoor& sitCoor, CCoor& satCoor, double& deltaTs,double& Tr, double tropDelay, double ionDelay, double p1, double& l, double&m, double& n, double& W)
{
	double R0;
	R0 = sitCoor.DistanceTo(satCoor);

	l = (satCoor.X() - sitCoor.X()) / R0;
	m = (satCoor.Y() - sitCoor.Y()) / R0;
	n = (satCoor.Z() - sitCoor.Z()) / R0;
	W = p1 - (R0 + c * Tr - c * deltaTs + tropDelay + ionDelay);

	return true;
}


bool CPosition::ComputeOneEpoch(ObsFileData& obsData, NavFileData navData, CCoor& sitCoor, double &Tr)
{
    double p1, p2;
	
	double satClkBias, tropDelay, ionDelay;
	double m, n, l, L;
	double deltax, deltay, deltaz, deltat;
	
	matrix<double> B(obsData.epochSatNum, 4);
	matrix<double> W(obsData.epochSatNum, 1);
	matrix<double> v(obsData.epochSatNum, 1);
	matrix<double> x(4, 1);
	matrix<double> Qxx;
	double sigma0;
	double PDOP;

    CCoor satCoor;
    
	if(obsData.epochSatNum >= 4)
	{
		int count = 0;

		do
		{
			for(int i = 0; i < obsData.epochSatNum; i++)
			{
				OneNavData oneNavData;
				oneNavData = this->GetOneNavData(obsData.epochObsData[i].satPRN,navData);

				CTime sendTime;

				this->GetSatCoor(obsData.epochTime, sendTime,oneNavData,sitCoor,satCoor,satClkBias);    
			//	satClkBias = this->ComSatClkBias(sendTime,oneNavData);
				tropDelay = this->GetTropDelay(satCoor,sitCoor);
				
				for(int j = 0; j < obsTypeNum; j++)  //采用双频观测值
				{
					if(obsTypeList[j] == "P1") 
					{
						p1 = obsData.epochObsData[i].oneValue[j].obsValue;
						continue;
					}
					if(obsTypeList[j] == "P2")
					{
						p2 = obsData.epochObsData[i].oneValue[j].obsValue;
						continue;
					}
				}
				
				ionDelay = this->GetIonDelay(p1, p2);

				this->ComputeFactors(sitCoor,satCoor,satClkBias,Tr,tropDelay,ionDelay,p1,l,m,n,L);
				
				B(i, 0) = -l;
				B(i, 1) = -m;
				B(i, 2) = -n;
				B(i, 3) = 1;
				W(i, 0) = L;
			}

			x = (!((~B) * B)) * (~B) * W;
			v = B * x - W;
			sigma0 = ((~v) * v)(0, 0) / (obsData.epochSatNum - 4.0);
			sigma0 = sqrt(sigma0);
			Qxx = !(~B * B);
			PDOP = sqrt( Qxx(0,0) + Qxx(1,1) + Qxx(2,2) );

			deltax = x(0, 0);
			deltay = x(1, 0);
			deltaz = x(2, 0);
			deltat = x(3, 0) / c;
			
			sitCoor.X(sitCoor.X() + deltax);
			sitCoor.Y(sitCoor.Y() + deltay);
			sitCoor.Z(sitCoor.Z() + deltaz);
			Tr += deltat;
			count ++;
			
			if(count > 7) break;   //不收敛情况
		}
		while(fabs(deltax) > 1.0e-8);
    }
	else cout << "Sorry, the number of satellites is less than 4, can't compute!" << endl;
	
    oneEpochResult.epoch = obsData.epochTime;
    oneEpochResult.sitCoor = sitCoor;
    oneEpochResult.receiverClkBias = Tr;
	oneEpochResult.satNum = obsData.epochSatNum;
	oneEpochResult.unitError = sigma0;
	oneEpochResult.PDOP = PDOP;
    oneEpochResult.epoch.PrintTime();
	cout << oneEpochResult.satNum << endl;
	return true;
}

bool CPosition::PointPosition(CObsFile& obsFile, CNavFile& navFile, CCoor& sitCoor, double& Tr)
{
	Tr = 0.0;
	CCoor iniSit(0.1,0.1,0.1);
	sitCoor = iniSit;
   
	result.epochNum = 0;

	ofstream outStream("结果.txt");
	outStream << "This file is the result of point position. The observation types are P1 and P2.\n";
	outStream << "The output followed by:\n";
	outStream << "观测历元 卫星数  测站的空间直角坐标  测站的大地坐标 测站的站心地平坐标";
	outStream << "接收机钟差 单位权中误差  PDOP" << endl;
	
	this->GetObsFileData(obsFile);
	this->GetNavFileData(navFile);

	for(int i = 0; i < obsFileData.size(); i++)
	{
		NavFileData navData;
		navData = this->GetNavData(obsFileData[i].epochTime);
		this->ComputeOneEpoch(obsFileData[i],navData,sitCoor,Tr);

        result.oneResult.push_back(oneEpochResult);
		result.epochNum ++;
		
		result.oneResult[i].epoch.WriteTime(outStream);
		outStream << "   " << result.oneResult[i].satNum << "   ";
		result.oneResult[i].sitCoor.WriteCartesianCoor(outStream);
		result.oneResult[i].sitCoor.WriteGeodesyCoor(outStream);
		outStream << fixed << "   " <<result.oneResult[i].receiverClkBias << "   " <<result.oneResult[i].unitError;
        outStream << "   " <<result.oneResult[i].PDOP << endl;
	}
	outStream.close();
	
	return true;
}
