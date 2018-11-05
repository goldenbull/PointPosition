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
	//�������A0��A1(����ʽϵ��),T(UTC���ݵĲο�ʱ��)��W(UTC�ο�����)
	double         A0;
	double         A1;
	int            T;
	int            W;
};

struct NavFileHeader
{
	//RINEX��ʽ�İ汾�š��ļ�����
	float          version;
	char           fileType;
	
	//�����еĵ�������A0��A3
	double         IonAlpha[4];
	
	//�����еĵ�������B0--B3
	double         IonBeta[4];
	
	//DELTA-UTC
    DeltaUTC      deltaUTC;
	
	//�����������ɵ�ʱ���
	int            leapSecond;
};

struct OneNavData
{
	//���ǵ�PRN��
	int           satPRN;

	//������Ԫʱ��
	CTime         satTime;
	
	//�����ӵ�ƫ�Ư�ơ�Ư���ٶ�
	double        clockBias;
	double        clockDrift;
	double        driftRate;
	
	//�㲥�����1����
	double        IODE;
	double        Crs;
	double        delta_n;
	double        M0;
	
	//�㲥�����2 ����
	double        Cuc;
	double        e;
	double        Cus;
	double        sqrt_A;
	
	//�㲥�����3 ����
	CTime         epochTime;
	double        Cic;
	double        OMEGA;
	double        Cis;
	
	//�㲥�����4 ����
	double        i0;
	double        Crc;
	double        omega;
	double        OMEGA_DOT;
	
	//�㲥�����5 ����
	double        IDOT;
	double        L2_codes;
	//	double        GPS_week;
	double        L2P_flag;
	
	//�㲥�����6 ����
	double        accuracy;
	double        health;
	double        TGD;
	double        IODC;
	
	//�㲥�����7 ����
	double        transTime;
	double        fitInterval;
    double        spare1;
	double        spare2;
};

struct NavFileData 
{
	//��Ԫ��TOC��Time of Clock��
	CTime               satClockTime;
	vector<OneNavData>  satNavRecord;
};

#endif