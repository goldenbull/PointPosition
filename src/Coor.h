// Coor.h: interface for the CCoor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOR_H__8D5DA141_AA24_4CDE_BFE2_CDF71DCBC306__INCLUDED_)
#define AFX_COOR_H__8D5DA141_AA24_4CDE_BFE2_CDF71DCBC306__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoorStruct.h"
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

const double PI = acos(-1);
const double ee = 0.006693421622966;
const double a  = 6378245.0000000000;

class CCoor  
{
public:
	CCoor();
	CCoor(const CCoor& );
	CCoor(double array[3]);
	CCoor(const CoorGeodesy& );
	CCoor(const CoorCartesian& );
	CCoor(double , double , double );
	virtual ~CCoor();

	void PrintCoor();
    void WriteCartesianCoor(ofstream& out);
	void WriteGeodesyCoor(ofstream& out);

	double X();
	double Y();
	double Z();
	void   X(double );
	void   Y(double );
	void   Z(double );

	double L();
	double B();
	double H();
	void   L(double );
	void   B(double );
	void   H(double );

	CCoor& operator = (const CCoor& );
	CCoor& operator = (double array[3]);
	CCoor& operator = (const CoorGeodesy& );
	CCoor& operator = (const CoorCartesian& );

	//两点间的直线距离
	double DistanceTo(CCoor& );
	double DistanceTo(const CoorGeodesy& );
	double DistanceTo(const CoorCartesian& );

	//到测站的高度角
	double ElevationAngleTo(CCoor& );
	double ElevationAngleTo(const CoorGeodesy& );
	double ElevationAngleTo(const CoorCartesian& );

	
private:
	void CartesianToGeodesy();
	void GeodesyToCartesian();

	CoorSite CoorToSite(CCoor& );
	CoorSite CoorToSite(const CoorGeodesy& );
	CoorSite CoorToSite(const CoorCartesian& );

private:
	CoorCartesian* pCoorCar;
	CoorGeodesy*   pCoorGeo;
};

#endif // !defined(AFX_COOR_H__8D5DA141_AA24_4CDE_BFE2_CDF71DCBC306__INCLUDED_)
