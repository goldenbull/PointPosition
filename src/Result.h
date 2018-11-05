//Result.h
/*
This file defined the struct of the result.
*/
#ifndef RESULT_H

#define RESULT_H

#include "Coor.h"
#include "Time.h"
#include <vector>
using namespace std;

struct OneEpochResult
{
	CTime               epoch;
	int                 satNum;
    CCoor               sitCoor;
	double              receiverClkBias;
	double              unitError;
	double              PDOP;
};

struct Result
{
	int                    epochNum;
	vector<OneEpochResult> oneResult;
};

#endif