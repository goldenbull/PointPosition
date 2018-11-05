//NavFileStruct.h
/*
This file defined the navigation's struct.
*/
#ifndef NAV_FILE_STRUCT_H
#define NAV_FILE_STRUCT_H

#include "Time.h"
#include <string>
#include <vector>
using namespace std;

struct DeltaUTC
{
	//历书参数A0、A1(多项式系数),T(UTC数据的参考时刻)、W(UTC参考周数)
	double         A0;
	double         A1;
	int            T;
	int            W;
};

struct NavFileHeader
{
	//RINEX格式的版本号、文件类型
	float          version;
	char           fileType;
	
	//历书中的电离层参数A0—A3
	double         IonAlpha[4];
	
	//历书中的电离层参数B0--B3
	double         IonBeta[4];
	
	//DELTA-UTC
    DeltaUTC      deltaUTC;
	
	//由于跳秒而造成的时间差
	int            leapSecond;
};

struct OneNavData
{
	//卫星的PRN号
	int           satPRN;

	//卫星历元时刻
	CTime         satTime;
	
	//卫星钟的偏差、漂移、漂移速度
	double        clockBias;
	double        clockDrift;
	double        driftRate;
	
	//广播轨道—1数据
	double        IODE;
	double        Crs;
	double        delta_n;
	double        M0;
	
	//广播轨道—2 数据
	double        Cuc;
	double        e;
	double        Cus;
	double        sqrt_A;
	
	//广播轨道—3 数据
	CTime         epochTime;
	double        Cic;
	double        OMEGA;
	double        Cis;
	
	//广播轨道—4 数据
	double        i0;
	double        Crc;
	double        omega;
	double        OMEGA_DOT;
	
	//广播轨道—5 数据
	double        IDOT;
	double        L2_codes;
	//	double        GPS_week;
	double        L2P_flag;
	
	//广播轨道—6 数据
	double        accuracy;
	double        health;
	double        TGD;
	double        IODC;
	
	//广播轨道—7 数据
	double        transTime;
	double        fitInterval;
    double        spare1;
	double        spare2;
};

struct NavFileData 
{
	//历元：TOC（Time of Clock）
	CTime               satClockTime;
	vector<OneNavData>  satNavRecord;
};

#endif