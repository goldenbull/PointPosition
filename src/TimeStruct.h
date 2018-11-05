//TimeStruct.h

/*

  Define various time structure, include:

  Common Time

  Julian Day

  Modify Julian Day

  GPS Time

  Day of Year

*/


#ifndef      TIME_STRUCTURE

#define      TIME_STRUCTURE


//********************************************
struct TOD{//time of day(s)
	long   sn;
	double tos;
};
//********************************************


//********************************************
struct TOW{//time of week(s)
	long   sn;
	double tos;

	bool operator == (const TOW& tow)
	{
		return ((sn == tow.sn) && (tos == tow.tos));
	};
};
//********************************************


//********************************************
struct CommonTime
{
	CommonTime(){};

	CommonTime
		(
		unsigned short v1, 
		unsigned short v2, 
		unsigned short v3, 
		unsigned short v4, 
		unsigned short v5, 
		double         v6
		)
	{
		year   = v1;
		month  = v2;
		day    = v3;
		hour   = v4;
		minute = v5;
		second = v6;
	};

	~CommonTime(){};

	unsigned short year;
	unsigned short month;
	unsigned short day;
	unsigned short hour;
	unsigned short minute;
	double         second;
};

typedef CommonTime* pCT;
//********************************************


//********************************************
struct ModifyJulianDay
{
	ModifyJulianDay(){};

	ModifyJulianDay(long v1, double v2)
	{
		day     = v1;
		tod.sn  = static_cast<int>(v2);
		tod.tos = v2 - tod.sn;
	};

	ModifyJulianDay(long v1, long v2, double v3)
	{
		day     = v1;
		tod.sn  = v2;
		tod.tos = v3;
	};

	~ModifyJulianDay(){};

	long day;
	TOD  tod;
};

typedef ModifyJulianDay* pMJ;
//********************************************


//********************************************
struct GpsTime
{
	GpsTime(){};

	GpsTime(int v1, double v2)
	{
		wn      = v1;
		tow.sn  = static_cast<int>(v2);
		tow.tos = v2 - tow.sn;
	};

	GpsTime(int v1, long v2, double v3)
	{
		wn      = v1;
		tow.sn  = v2;
		tow.tos = v3;
	};

	~GpsTime(){};

	int wn;
	TOW tow;
};

typedef GpsTime* pGT;
//********************************************



#endif