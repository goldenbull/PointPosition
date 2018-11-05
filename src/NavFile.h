// NavFile.h: interface for the CNavFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAVFILE_H__5ACB4866_8A51_4427_8B29_5C2A3D9ECC73__INCLUDED_)
#define AFX_NAVFILE_H__5ACB4866_8A51_4427_8B29_5C2A3D9ECC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NavFileStruct.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

const string versin_type("RINEX VERSION / TYPE");
const string PGM_runBy_date("PGM / RUN BY / DATE");
const string ion_alpha("ION ALPHA");
const string ion_beta("ION BETA");
const string delta_UTC("DELTA-UTC: A0,A1,T,W");
const string leap_seconds("LEAP SECONDS");
const string end_flag("END OF HEADER");

class CNavFile  
{
public:
	CNavFile();
	CNavFile(const string& FileName);

	bool ReadNavData(ifstream& inStream);
	bool ReadNavHeader(ifstream& inStream);
	bool ReadNavFile(const string& FileName);
    
	NavFileHeader       GetNavHeader();
    vector<NavFileData> GetNavData();

	virtual ~CNavFile();
private:
    NavFileHeader       header;
    vector<NavFileData> data;
};

inline NavFileHeader CNavFile::GetNavHeader()
{
	return header;
}

inline vector<NavFileData> CNavFile::GetNavData()
{
	return data;
}

#endif // !defined(AFX_NAVFILE_H__5ACB4866_8A51_4427_8B29_5C2A3D9ECC73__INCLUDED_)
