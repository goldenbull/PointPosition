// NavFile.cpp: implementation of the CNavFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NavFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNavFile::CNavFile()
{

}

CNavFile::CNavFile(const string& FileName)
{
	this->ReadNavFile(FileName);
}

CNavFile::~CNavFile()
{

}

bool CNavFile::ReadNavHeader(ifstream &inStream)
{
	string GPSNFileInfor;
	for(;;)
	{	
		string GPSNFileFlag;
		getline(inStream,GPSNFileInfor);
		
		GPSNFileFlag = GPSNFileInfor.substr(60,20);
		
		if(GPSNFileFlag.find( versin_type ) != string::npos)
		{
			header.version = atof(GPSNFileInfor.substr(0,9).c_str());
			header.fileType = GPSNFileInfor[20];
			continue;
		}
		
		if(GPSNFileFlag.find( ion_alpha ) != string::npos)
		{
			for(int i = 0; i < 4; i++)
				header.IonAlpha[i] = atof(GPSNFileInfor.substr(2 + 12 * i,12).c_str());
			continue;
		}
		
		if(GPSNFileFlag.find( ion_beta ) != string::npos)
		{
			for(int i = 0; i < 4; i++)
				header.IonBeta[i] = atof(GPSNFileInfor.substr(2 + 12 * i,12).c_str());
			continue;
		}
		
		if(GPSNFileFlag.find( delta_UTC ) != string::npos)
		{
			header.deltaUTC.A0 = atof(GPSNFileInfor.substr(3,19).c_str());
			header.deltaUTC.A1 = atof(GPSNFileInfor.substr(22,19).c_str());
			header.deltaUTC.T = atoi(GPSNFileInfor.substr(41,9).c_str());
			header.deltaUTC.W = atoi(GPSNFileInfor.substr(50,9).c_str());
			continue;
		}
		
		if(GPSNFileFlag.find( leap_seconds ) != string::npos)
		{
			header.leapSecond = atoi(GPSNFileInfor.substr(0,6).c_str());
			continue;
		}
		
		if(GPSNFileFlag.find( end_flag ) != string::npos)
			break;
	}
	
	return true;
}

bool CNavFile::ReadNavData(ifstream &inStream)
{
	string GPSNFileInfor;
	NavFileData navFileData;
	CTime markTime;
	int count = 0;
    int mark = 0;

	for(;;)
	{
		getline(inStream,GPSNFileInfor);
		
		if( inStream.eof() ) 
		{
			if(navFileData.satNavRecord.size() > 0)
			{
				navFileData.satClockTime = markTime;
				data.push_back(navFileData);
			}
			return true;
		} 

		OneNavData  oneNavData;

		//由时间来控制提取，首先提取时间
        CommonTime ct;
		CTime      readTime;

		ct.year = atoi( GPSNFileInfor.substr(3,2).c_str() );
		ct.month = atoi( GPSNFileInfor.substr(6,2).c_str() );
		ct.day = atoi( GPSNFileInfor.substr(9,2).c_str() );
		ct.hour = atoi( GPSNFileInfor.substr(12,2).c_str() );
		ct.minute = atoi( GPSNFileInfor.substr(15,2).c_str() );
		ct.second = atof( GPSNFileInfor.substr(17,5).c_str() );
        readTime = ct;
		
		if(count == 0) markTime = readTime;
		
loop:	if(fabs((markTime - readTime) * 1) < 3640)
		{
			double TOE_GPS_week, TOE_sec;
			
			oneNavData.satTime = readTime;
			oneNavData.satPRN = atoi( GPSNFileInfor.substr(0,2).c_str() );
			oneNavData.clockBias= atof( GPSNFileInfor.substr(22,19).c_str() );
			oneNavData.clockDrift = atof( GPSNFileInfor.substr(41,19).c_str() );
			oneNavData.driftRate = atof( GPSNFileInfor.substr(60,19).c_str() );
			
			double broad_orbit[7][4] = {0};
			for(int i = 0; i < 7; i++)
			{
				getline(inStream,GPSNFileInfor);
				int num;
				num = GPSNFileInfor.length() / 19;
				for(int j = 0; j < num; j++)
				{
					broad_orbit[i][j] = atof( GPSNFileInfor.substr(3 + j * 19,19).c_str());
				}
			}
			
			oneNavData.IODE = broad_orbit[0][0];
			oneNavData.Crs = broad_orbit[0][1];
			oneNavData.delta_n = broad_orbit[0][2];
			oneNavData.M0 = broad_orbit[0][3];
			oneNavData.Cuc = broad_orbit[1][0];
			oneNavData.e = broad_orbit[1][1];
			oneNavData.Cus = broad_orbit[1][2];
			oneNavData.sqrt_A = broad_orbit[1][3];
			TOE_sec = broad_orbit[2][0];
			oneNavData.Cic = broad_orbit[2][1];
			oneNavData.OMEGA = broad_orbit[2][2];
			oneNavData.Cis = broad_orbit[2][3];
			oneNavData.i0 = broad_orbit[3][0];
			oneNavData.Crc = broad_orbit[3][1];
			oneNavData.omega = broad_orbit[3][2];
			oneNavData.OMEGA_DOT = broad_orbit[3][3];
			oneNavData.IDOT = broad_orbit[4][0];
			oneNavData.L2_codes = broad_orbit[4][1];
			TOE_GPS_week = broad_orbit[4][2];
			oneNavData.L2P_flag = broad_orbit[4][3];
			oneNavData.accuracy = broad_orbit[5][0];
			oneNavData.health = broad_orbit[5][1];
			oneNavData.TGD = broad_orbit[5][2];
			oneNavData.IODC = broad_orbit[5][3];
			oneNavData.transTime = broad_orbit[6][0];
			oneNavData.fitInterval = broad_orbit[6][1];
			oneNavData.spare1 = broad_orbit[6][2];
			oneNavData.spare2 = broad_orbit[6][3];
			
			GpsTime GPSTime;
			
			GPSTime.wn = static_cast<int>(TOE_GPS_week);
			GPSTime.tow.sn = static_cast<int>(TOE_sec + (TOE_GPS_week - GPSTime.wn) * 604800);
			GPSTime.tow.tos = TOE_sec + (TOE_GPS_week - GPSTime.wn) * 604800 - GPSTime.tow.sn;
			oneNavData.epochTime = GPSTime;
			
			count ++;
			navFileData.satNavRecord.push_back(oneNavData);
		}
		else                 
		{
			navFileData.satClockTime = markTime;
			data.push_back(navFileData);
			markTime = readTime;
			navFileData.satNavRecord.clear();
			goto loop;			
		}
	}                      
	
	return true;
}

bool CNavFile::ReadNavFile(const string& FileName)
{
	ifstream inStream;
	inStream.open(FileName.c_str());
	
	if(!inStream.is_open())                        //打开文件
	{
		cout << "Sorry,can't open the file!" << endl;
		return false;
	}
	
	if(!this->ReadNavHeader(inStream))
	{
		cout << "Sorry,can't read the file's header!" << endl;
		return false;
	}
	
	if(!this->ReadNavData(inStream))
	{
		cout << "Sorry,can't read the file's record!" << endl;
		return false;
	}
	
	return true;
}