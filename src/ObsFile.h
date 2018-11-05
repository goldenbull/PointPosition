// ObsFile.h: interface for the CObsFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBSFILE_H__B2457C2A_8EE6_4B04_BC9C_AB9F6DAD2B19__INCLUDED_)
#define AFX_OBSFILE_H__B2457C2A_8EE6_4B04_BC9C_AB9F6DAD2B19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObsFileStruct.h"
#include <iostream>

using namespace std;

const string rinex_version_type("RINEX VERSION / TYPE");
const string mark_name("MARKER NAME");
const string mark_num("MARKER NUMBER");
const string rec_num_type_vers("REC # / TYPE / VERS");
const string ant_num_type("ANT # / TYPE");
const string approx_pos("APPROX POSITION XYZ");
const string antenna("ANTENNA: DELTA H/E/N");
const string wavelen_fact("WAVELENGTH FACT L1/2");
const string obs_type("# / TYPES OF OBSERV");
const string interval("INTERVAL");
const string start_time("TIME OF FIRST OBS");
const string end_time("TIME OF LAST OBS");
const string end_mark("END OF HEADER");

class CObsFile  
{
public:
	bool ReadObsHeader(ifstream& instream);
	bool ReadObsData(ifstream& inStream);
	bool ReadObsFile(const string& FileName);

    ObsFileHeader        GetFileHeader();
	vector<ObsFileData>  GetFileData();
    int                  GetObsTypeNum();
	vector<string>       GetObsTypeList();

	CObsFile();
	CObsFile(const string& FileName);
	virtual ~CObsFile();
private:
	ObsFileHeader         header;
	vector<ObsFileData>   data;

};

inline int CObsFile::GetObsTypeNum()
{
	return header.obsTypeNum;
}

inline vector<string> CObsFile::GetObsTypeList()
{
	return header.obsTypeList;
}

#endif // !defined(AFX_OBSFILE_H__B2457C2A_8EE6_4B04_BC9C_AB9F6DAD2B19__INCLUDED_)
