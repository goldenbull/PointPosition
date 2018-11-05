// Position.h: interface for the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITION_H__5FB0F777_0857_47E4_97CE_1CEC0599EAF1__INCLUDED_)
#define AFX_POSITION_H__5FB0F777_0857_47E4_97CE_1CEC0599EAF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObsFile.h"
#include "NavFile.h"
#include "Result.h"

#include "matrix.h"

#ifndef _NO_NAMESPACE
using namespace std;
using namespace math;
#define STD std
#else
#define STD
#endif

#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
#else
typedef matrix Matrix;
#endif

#ifndef _NO_EXCEPTION
#  define TRYBEGIN()	try {
#  define CATCHERROR()	} catch (const STD::exception& e) { \
cerr << "Error: " << e.what() << endl; }
#else
#  define TRYBEGIN()
#  define CATCHERROR()
#endif     //¾ØÕóÀàÔ¤±àÒë

const double GM = 3.9860047e+14;
const double F  = -4.442807633e-10;
const double f1 = 1575.42;
const double f2 = 1227.6;
const double c  = 299792458.458;
const double we  = 7.2921151467e-5;

class CPosition  
{
public:
	bool       PointPosition(CObsFile& obsFile, CNavFile& navFile, CCoor& sitCoor, double&  Tr);
	double     GetIonDelay(const double& P1, const double& P2);
	double     GetTropDelay(CCoor& satCoor, CCoor& sitCoor);
	bool       GetSatCoor(CTime obsTime, CTime& sendTime, OneNavData& navData, CCoor& sitCoor, CCoor& satCoor, double& satClkBias);
    bool       ComputeOneEpoch(ObsFileData& obsData, NavFileData navData, CCoor& sitCoor, double &Tr);

	bool       GetObsFileData(CObsFile& obsFile);
	bool       GetNavFileData(CNavFile& navFile);
	NavFileData GetNavData(CTime& obsTime);
    OneNavData GetOneNavData(string& satPRN, NavFileData& navData);
   
	CPosition();
	virtual ~CPosition();
private:
	double    ComSatClkBias(CTime& sendTime, OneNavData navData);
	bool      ComputeSatCoor(CTime& sendTime, CCoor& satCoor, OneNavData navData );
	bool      ComputeFactors(CCoor& sitCoor, CCoor& satCoor, double& deltaTs,double& Tr, double tropDelay, double ionDelay, double p1, double& l, double&m, double& n, double& W);
	
	int                    obsTypeNum;
	vector<string>         obsTypeList;
	vector<ObsFileData>    obsFileData;

	vector<NavFileData>    navFileData;
    int                    mark;

	OneEpochResult         oneEpochResult;
	Result                 result;
};

#endif // !defined(AFX_POSITION_H__5FB0F777_0857_47E4_97CE_1CEC0599EAF1__INCLUDED_)
