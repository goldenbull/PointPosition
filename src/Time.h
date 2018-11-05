// Time.h: interface for the CTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIME_H__BB0C9D61_B247_4198_82F3_C9CDA9919C77__INCLUDED_)
#define AFX_TIME_H__BB0C9D61_B247_4198_82F3_C9CDA9919C77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeStruct.h"
#include "math.h"
#include <iostream>
#include <fstream>

using namespace std;

class CTime
{
public:
	CTime();
	CTime(const CTime& );
	CTime(const GpsTime& );
	CTime(const CommonTime& );
	CTime(const ModifyJulianDay& );
	virtual ~CTime();

	const GpsTime& GetGpsTime() const;
	const CommonTime& GetCommonTime() const;
	const ModifyJulianDay& GetModifyJulianDay() const;

	
	void*  operator [] (int );
	CTime  operator + (CTime& );
	CTime  operator + (double );
	CTime  operator - (CTime& );
	CTime  operator - (double );
	CTime& operator = (const CTime& );
	CTime& operator = (const CommonTime& );
	double operator * (double );
	bool   operator == (CTime& );
	bool   operator < (double );
	bool   operator > (double );

	double value();
	void   PrintTime();
	void   WriteTime(ofstream& os);

private:
	void CommonTimeToGpsTime(const CommonTime* pct, GpsTime* pgt);
	void GpsTimeToCommonTime(const GpsTime* pgt, CommonTime* pct);
	void GpsTimeToModifyJulianDay(const GpsTime* pgt, ModifyJulianDay* pmjd);
	void ModifyJulianDayToGpsTime(const ModifyJulianDay* pmjd, GpsTime* pgt);
	void CommonTimeToModifyJulianDay(const CommonTime* pct, ModifyJulianDay* pmjd);
	void ModifyJulianDayToCommonTime(const ModifyJulianDay* pmjd, CommonTime* pct);
	


private:
	GpsTime*           m_pgt;
	CommonTime*        m_pct;
	ModifyJulianDay*   m_pmjd;
};

#endif // !defined(AFX_TIME_H__BB0C9D61_B247_4198_82F3_C9CDA9919C77__INCLUDED_)
