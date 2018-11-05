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
	//�汾�ţ��۲�ֵ����������ϵͳ��
	float              rinexVer;
	char               satType;
	
	//��������ţ�
    string             markName;
	string             markNum;
	
    //���ջ����кš����ͺͰ汾��
	string             recNum;
	string             recType;
	string             recVers;
	
	//�������кż�����
	string             antNum;
	string             antType;
	
	//��־�Ľ��Ƶ�λ����(WGS84)
	CCoor              appoxPosition;
	
	//���߸ߣ���������ƫ��
	CRDTOPOCENTRIC     antDelta;
	
	//L1��L2�ز�ȱʡ�Ĳ�������
	int                 L1;
	int                 L2;
	
	//L1��L2�仯�Ĳ�������
    vector<WAVELENFACT> wavelenList;
    
	//�۲�ֵ���͵��������б�
	int                 obsTypeNum;
	vector<string>      obsTypeList;
	
	//��ʼ�۲�ʱ�䣬��ֹ�۲�ʱ�䣬ʱ��ϵͳ����
	CTime               startTime;
	string              startTmSys;
	CTime               endTime;
	string              endTmSys;
};

struct ObsDataUnit //һ���۲�ֵ���ݣ������۲�ֵ��ʧ����ʶ�����ź�ǿ�ȣ�
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
	//�۲���Ԫʱ��
	CTime                   epochTime;
	
	//��Ԫ��־
	int                     epochFlag;
	
    //��ǰ��Ԫ���۲⵽��������
	int                     epochSatNum;
	
	//���ջ���ʱ��ƫ��
	//	double                  RecClkOffset;
	
	//��ǰ��Ԫ�������ǵĹ۲�ֵ�����б�
	vector<OneSatData>      epochObsData;
};

#endif