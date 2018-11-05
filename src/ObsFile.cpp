// ObsFile.cpp: implementation of the CObsFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObsFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObsFile::CObsFile()
{

}

CObsFile::CObsFile(const string& FileName)
{
	this->ReadObsFile(FileName);
}

CObsFile::~CObsFile()
{

}

/**************************************************************
读取文件头信息
***************************************************************/

bool CObsFile::ReadObsHeader(ifstream &inStream)
{
	string strObsFileInfor;
	string strMarkInfor;
	int strLength = 0;
	int count_fact = 0;

	for(;;)
	{
		getline(inStream,strObsFileInfor);
		strLength = strObsFileInfor.size();
		strMarkInfor = strObsFileInfor.substr(60, strLength - 60);
		
		if(strMarkInfor.find( rinex_version_type ) != string::npos)
		{
			header.rinexVer = atof(strObsFileInfor.substr(0,9).c_str());
			header.satType= strObsFileInfor[40];
			continue;
		}
		
		if(strMarkInfor.find( mark_name) != string::npos)
		{
			header.markName = strObsFileInfor.substr(0,60);
			continue;
		}
		
		if(strMarkInfor.find( mark_num ) != string::npos)
		{
			header.markNum = strObsFileInfor.substr(0,20);
			continue;
		}
		
		if(strMarkInfor.find( rec_num_type_vers ) != string::npos)
		{
			header.recNum = strObsFileInfor.substr(0,20);
			header.recType = strObsFileInfor.substr(20,20);
			header.recVers = strObsFileInfor.substr(40,20);
			continue;
		}
		
		if(strMarkInfor.find( ant_num_type ) != string::npos)
		{
			header.antNum = strObsFileInfor.substr(0,20);
			header.antType = strObsFileInfor.substr(20,20);
			continue;
		}
		
		if(strMarkInfor.find( approx_pos ) != string::npos)
		{
			CoorCartesian position;
			position.X = atof(strObsFileInfor.substr(0,14).c_str());
			position.Y = atof(strObsFileInfor.substr(14,14).c_str());
			position.Z = atof(strObsFileInfor.substr(28,14).c_str());
			header.appoxPosition = position;
			continue;
		}
		
		if(strMarkInfor.find( antenna ) != string::npos)
		{
			header.antDelta.upping = atof(strObsFileInfor.substr(0,14).c_str());
			header.antDelta.easting = atof(strObsFileInfor.substr(14,14).c_str());
			header.antDelta.northing = atof(strObsFileInfor.substr(28,14).c_str());
			continue;
		}
		
		if(strMarkInfor.find( wavelen_fact ) != string::npos)
		{
			count_fact++;
			
			if(count_fact == 1)
			{
				header.L1 = atoi(strObsFileInfor.substr(0,6).c_str());
				header.L2 = atoi(strObsFileInfor.substr(6,6).c_str());
				continue;
			}
			else
			{	
				WAVELENFACT wavelenfact;
				
				wavelenfact.L1 = atoi(strObsFileInfor.substr(0,6).c_str());
				wavelenfact.L2 = atoi(strObsFileInfor.substr(6,6).c_str());
				wavelenfact.satNum = atoi(strObsFileInfor.substr(12,6).c_str());
				for(int j = 0; j < wavelenfact.satNum; j++)
				{
					wavelenfact.satList.push_back(strObsFileInfor.substr(21 + 6 * j,3).c_str());
				}
				header.wavelenList.push_back(wavelenfact);
			}
			
			continue;
		}
		
		if(strMarkInfor.find( obs_type ) != string::npos)
		{
			header.obsTypeNum = atoi(strObsFileInfor.substr(0,6).c_str());
			
			if(header.obsTypeNum  <= 9)
			{
				for(int j = 0; j < header.obsTypeNum; j++)
				{
					header.obsTypeList.push_back(strObsFileInfor.substr(10 + 6 * j,2));
				}
			}
			else
			{
				for(int j = 0; j < 9; j ++)
				{
					header.obsTypeList.push_back(strObsFileInfor.substr(10 + 6 * j,2));
				}
				
				getline(inStream,strObsFileInfor);
				
				for(int j = 0; j < (header.obsTypeNum - 9); j ++)
				{
					header.obsTypeList.push_back(strObsFileInfor.substr(10 + 6 *j,2));
				}
				
			}
			continue;
		}
		
		if(strMarkInfor.find( start_time ) != string::npos)
		{
			CommonTime ct;
			ct.year = atoi(strObsFileInfor.substr(0,6).c_str());
			ct.month = atoi(strObsFileInfor.substr(6,6).c_str());
			ct.day = atoi(strObsFileInfor.substr(12,6).c_str());
			ct.hour = atoi(strObsFileInfor.substr(18,6).c_str());
			ct.minute = atoi(strObsFileInfor.substr(24,6).c_str());
			ct.second = atof(strObsFileInfor.substr(30,13).c_str());
			header.startTime= ct;

			header.startTmSys = strObsFileInfor.substr(48,3);
			
			continue;
		}
		
		if(strMarkInfor.find( end_time ) != string::npos)
		{
			CommonTime ct;
			ct.year = atoi(strObsFileInfor.substr(0,6).c_str());
			ct.month = atoi(strObsFileInfor.substr(6,6).c_str());
			ct.day = atoi(strObsFileInfor.substr(12,6).c_str());
			ct.hour = atoi(strObsFileInfor.substr(18,6).c_str());
			ct.minute = atoi(strObsFileInfor.substr(24,6).c_str());
			ct.second = atof(strObsFileInfor.substr(30,13).c_str());
            header.endTime = ct;

			header.endTmSys = strObsFileInfor.substr(48,3);
			
			continue;
		}
		
		if(strMarkInfor.find( end_mark ) != string::npos)
		{
			break;
		}
	}
	return true;
}

/******************************************************************
读取观测值信息
*******************************************************************/
bool CObsFile::ReadObsData(ifstream &inStream)
{
	string strObsFileInfor;
	string strMarkInfor;

	for(;;)
	{
		getline(inStream,strObsFileInfor);
		
		if(inStream.eof()) break;
		
		ObsFileData oneEpochData;
		CommonTime ct;
		
		ct.year = atoi(strObsFileInfor.substr(1,2).c_str());
		ct.month = atoi(strObsFileInfor.substr(4,2).c_str());
		ct.day = atoi(strObsFileInfor.substr(7,2).c_str());
		ct.hour = atoi(strObsFileInfor.substr(10,2).c_str());
		ct.minute = atoi(strObsFileInfor.substr(13,2).c_str());
		ct.second = atof(strObsFileInfor.substr(15,11).c_str());
		
		oneEpochData.epochTime = ct;
		
		oneEpochData.epochFlag = atoi(strObsFileInfor.substr(28,1).c_str());
		oneEpochData.epochSatNum = atoi(strObsFileInfor.substr(29,3).c_str());
		
BEGIN:
		if(oneEpochData.epochFlag != 0)
		{
			for(int i = 0; i < oneEpochData.epochSatNum; i++)
				getline(inStream,strObsFileInfor);
			goto BEGIN;
		}

		string otherline;
		vector<string> epochSatList;

		if(oneEpochData.epochSatNum > 12)   //观测历元的卫星数多于12颗续行
		{
			getline(inStream,otherline);
		    otherline = otherline.substr(32,otherline.size() - 32);
			strObsFileInfor += otherline;
		}

		for(int j = 0; j < (strObsFileInfor.size() - 32 ) / 3; j++)
		{
			epochSatList.push_back(strObsFileInfor.substr(32 + 3 * j,3));
		}
         
		int count = 0;

		for(int i = 0; i < oneEpochData.epochSatNum ; i++) 
			//外层循环读取一个观测历元的所有卫星的观测值
			
		{
			bool mark = true;

			getline(inStream,strObsFileInfor);
			
			OneSatData  oneSatData;
			
			string strObsData;
			
			if(header.obsTypeNum > 5)
			{
				getline(inStream,strObsData);
				strObsFileInfor += strObsData;
			}
		
			int max;
			max = strObsFileInfor.length() / 14;

			if(max < header.obsTypeNum)
			{
//				epochSatList.erase(&epochSatList[i]);
				count ++;
				continue;
			}
			
			for(int j = 0; j < max; j++)
			{
				ObsDataUnit oneValue; //当前历元一颗卫星的单个类型观测值数据  

				oneValue.obsValue = atof(strObsFileInfor.substr(0 + 16 * j,14).c_str()); 
				if(oneValue.obsValue == 0)
				{
		//			epochSatList.erase(&epochSatList[i]);
                    count ++;
					mark = false;
					break;
				}
				if(j == max - 1)  
				{
                    oneValue.LLI = 0;
					oneValue.signalStrength = 0;
				}
				else
				{
					oneValue.LLI = atoi(strObsFileInfor.substr(14 + 16 * j,1).c_str());
					oneValue.signalStrength = atoi(strObsFileInfor.substr(15 + 16 * j,1).c_str());
				}
				//将卫星的单个观测值存入当前历元的观测值序列中
				oneSatData.oneValue.push_back(oneValue);
			}
			
			//将当前历元单颗卫星的观测值存入当前历元所有卫星观测值数组中
			if(mark) 
			{
				oneSatData.satPRN = epochSatList[i];
				oneEpochData.epochObsData.push_back(oneSatData);
			}
		}
		
		//将当前历元观测值存入文件观测值数组中
		oneEpochData.epochSatNum -= count;
		data.push_back(oneEpochData);  
	}
	return true;
}

bool CObsFile::ReadObsFile(const string& FileName)
{
	ifstream inStream;
	inStream.open(FileName.c_str());
	
	if(!inStream.is_open())                        //打开文件
	{
		cout << "Sorry,can't open the file!" << endl;
		return false;
	}
	
	if(!this->ReadObsHeader(inStream))
	{
		cout << "Sorry,can't read the file's header!" << endl;
		return false;
	}
	
	if(!this->ReadObsData(inStream))
	{
		cout << "Sorry,can't read the file's record!" << endl;
		return false;
	}
	
	return true;
}

ObsFileHeader CObsFile::GetFileHeader()
{
	return header;
}

vector<ObsFileData>  CObsFile::GetFileData()
{
	return data;
}