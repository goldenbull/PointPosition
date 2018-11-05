//ObsFileStruct.h
/*
This file defined the struct of the observation file.
*/

#ifndef OBS_FILE_STRUCT_H
#define OBS_FILE_STRUCT_H

#include "Coor.h"
#include "Coordinate.h"
#include "Time.h"
#include <string>
#include <vector>

using namespace std;

struct WAVELENFACT
{
	int                 L1;
	int                 L2;
	int                 satNum;
	vector<string>      satList;
};

struct ObsFileHeader
{
	//版本号，观测值所属的卫星系统；
	float              rinexVer;
	char               satType;
	
	//点名，点号；
    string             markName;
	string             markNum;
	
    //接收机序列号、类型和版本号
	string             recNum;
	string             recType;
	string             recVers;
	
	//天线序列号及类型
	string             antNum;
	string             antType;
	
	//标志的近似点位坐标(WGS84)
	CCoor              appoxPosition;
	
	//天线高，天线中心偏差
	CRDTOPOCENTRIC     antDelta;
	
	//L1和L2载波缺省的波长因子
	int                 L1;
	int                 L2;
	
	//L1和L2变化的波长因子
    vector<WAVELENFACT> wavelenList;
    
	//观测值类型的数量和列表
	int                 obsTypeNum;
	vector<string>      obsTypeList;
	
	//起始观测时间，终止观测时间，时间系统类型
	CTime               startTime;
	string              startTmSys;
	CTime               endTime;
	string              endTmSys;
};

struct ObsDataUnit //一个观测值数据（包括观测值、失锁标识符和信号强度）
{
	double                   obsValue;
	int                      LLI;
	int                      signalStrength;
};

struct OneSatData
{
	string                  satPRN;
	vector<ObsDataUnit>     oneValue;
};

struct ObsFileData
{
	//观测历元时刻
	CTime                   epochTime;
	
	//历元标志
	int                     epochFlag;
	
    //当前历元所观测到的卫星数
	int                     epochSatNum;
	
	//接收机的时钟偏差
	//	double                  RecClkOffset;
	
	//当前历元所有卫星的观测值数据列表
	vector<OneSatData>      epochObsData;
};

#endif