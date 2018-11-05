// Time.cpp: implementation of the CTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Time.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//缺省构造函数，指针都指向空NULL
CTime::CTime()
{
	this->m_pct  = new CommonTime;
	this->m_pgt  = new GpsTime;
	this->m_pmjd = new ModifyJulianDay;
}

//复制构造函数，用另一个对象构造一个新对象
CTime::CTime(const CTime& tobj)
{
	this->m_pct = new CommonTime(*tobj.m_pct);
	this->m_pgt = new GpsTime(*tobj.m_pgt);
	this->m_pmjd = new ModifyJulianDay(*tobj.m_pmjd);
}

//用gps时构造对象
CTime::CTime(const GpsTime& gt)
{
	this->m_pgt  = new GpsTime;
	this->m_pct  = new CommonTime;
	this->m_pmjd = new ModifyJulianDay;

	//根据gps时为各指针赋值
	*m_pgt = gt;
	this->GpsTimeToCommonTime(this->m_pgt, this->m_pct);
	this->GpsTimeToModifyJulianDay(this->m_pgt, this->m_pmjd);
}

//用普通时构造对象
CTime::CTime(const CommonTime& ct)
{
	this->m_pgt  = new GpsTime;
	this->m_pct  = new CommonTime;
	this->m_pmjd = new ModifyJulianDay;

	//根据普通时为各指针赋值
	*m_pct = ct;
	if(this->m_pct->year >= 80 && this->m_pct->year < 100)
	{
		this->m_pct->year += 1900;
	}
	else
		if(this->m_pct->year < 80)
		{
			this->m_pct->year += 2000;
		}
	this->CommonTimeToGpsTime(this->m_pct, this->m_pgt);
	this->CommonTimeToModifyJulianDay(this->m_pct, this->m_pmjd);
}

//用儒略日构造对象
CTime::CTime(const ModifyJulianDay& mjd)
{
	this->m_pgt  = new GpsTime;
	this->m_pct  = new CommonTime;
	this->m_pmjd = new ModifyJulianDay;

	//根据儒略日为各指针赋值
	*m_pmjd = mjd;
	this->ModifyJulianDayToGpsTime(this->m_pmjd, this->m_pgt);
	this->ModifyJulianDayToCommonTime(this->m_pmjd, this->m_pct);
}

//析构函数，释放各指针指向的内存
CTime::~CTime()
{
	if(this->m_pct != 0)
		delete this->m_pct;

	if(this->m_pgt != 0)
		delete this->m_pgt;

	if(this->m_pmjd != 0)
		delete this->m_pmjd;
}

void CTime::CommonTimeToGpsTime(const CommonTime* pct, GpsTime* pgt)
{
	ModifyJulianDay mjd;
	this->CommonTimeToModifyJulianDay(pct, &mjd);
	this->ModifyJulianDayToGpsTime(&mjd, pgt);
}

void CTime::GpsTimeToCommonTime(const GpsTime* pgt, CommonTime* pct)
{
	ModifyJulianDay mjd;
	this->GpsTimeToModifyJulianDay(pgt, &mjd);
	this->ModifyJulianDayToCommonTime(&mjd, pct);
}

void CTime::GpsTimeToModifyJulianDay(const GpsTime* pgt, ModifyJulianDay* pmjd)
{
	int day;
	pmjd->tod.tos = pgt->tow.tos;
	day = pgt->tow.sn / 86400;
	pmjd->day = pgt->wn * 7 + day + 44244;
	pmjd->tod.sn = pgt->tow.sn - day * 86400;
}

void CTime::ModifyJulianDayToGpsTime(const ModifyJulianDay* pmjd, GpsTime* pgt)
{
	int rd;
	pgt->wn = (pmjd->day - 44244) / 7;
	rd = pmjd->day - 44244 - pgt->wn * 7;
	pgt->tow.sn = rd * 86400 + pmjd->tod.sn;
	pgt->tow.tos = pmjd->tod.tos;
}

void CTime::CommonTimeToModifyJulianDay(const CommonTime* pct, ModifyJulianDay* pmjd)
{
	int y;
	int m;
	int temp;

	y = pct->year;
	m = pct->month;
	
	if(y >= 80 && y < 100)
	{
		y += 1900;
	}
	else
		if(y < 80)
		{
			y += 2000;
		}

	if(m <= 2)
	{
		y -= 1;;
		m += 12;
	}

	temp  = static_cast<int>(365.25 * y);
	temp += static_cast<int>(30.6001 * (m + 1));
	temp += pct->day;
	temp -= 679019;

	pmjd->day     = temp;
	pmjd->tod.sn  = pct->hour * 3600 + pct->minute * 60 + static_cast<int>(pct->second);
	pmjd->tod.tos = pct->second - static_cast<int>(pct->second);
}

void CTime::ModifyJulianDayToCommonTime(const ModifyJulianDay* pmjd, CommonTime* pct)
{
	int a, b, c, d, e;

	a = pmjd->day  + 2400001;
	b = a + 1537;
	c = static_cast<int>((b - 122.1) / 365.25);
	d = static_cast<int>(365.25 * c);
	e = static_cast<int>((b - d) / 30.6001);

	pct->day = b - d - static_cast<int>(30.6001 * e);
	pct->month = e - 1 - 12 * (e / 14);
	pct->year = c - 4715 - ((7 + pct->month) / 10);
	pct->hour = pmjd->tod.sn / 3600;
	pct->minute = (pmjd->tod.sn % 3600) / 60;
	pct->second = (pmjd->tod.sn % 3600) % 60 + pmjd->tod.tos;
}

const GpsTime& CTime::GetGpsTime() const
{
	return *this->m_pgt;
}

const CommonTime& CTime::GetCommonTime() const
{
	return *this->m_pct;
}

const ModifyJulianDay& CTime::GetModifyJulianDay() const
{
	return *this->m_pmjd;
}

void* CTime::operator [] (int index)
{
	switch(index)
	{
	case 0:
		return m_pct;
	case 1:
		return m_pgt;
	case 2:
		return m_pmjd;
	default:
		cerr<<"下标越界!(只能取0、1、2)\n";
		return NULL;
	}
}

CTime& CTime::operator = (const CTime& tobj)
{
	if(&tobj != this)
	{
		delete this->m_pct;
		delete this->m_pgt;
		delete this->m_pmjd;

		this->m_pct  = new CommonTime(*tobj.m_pct);
		this->m_pgt  = new GpsTime(*tobj.m_pgt);
		this->m_pmjd = new ModifyJulianDay(*tobj.m_pmjd);
	}

	return *this;
}

CTime& CTime::operator = (const CommonTime& ct)
{
	CTime t(ct);
	*this = t;

	return *this;
}

double CTime::operator * (double v)
{
	return (this->value() * v);
}

bool CTime::operator == (CTime& t)
{
	return (fabs(this->value() - t.value()) < 1.0e-5);
}

bool CTime::operator < (double v)
{
	return (this->value() < v);
}

bool CTime::operator > (double v)
{
	return (this->value() > v);
}

double CTime::value()
{
	return this->m_pgt->wn * 604800 + this->m_pgt->tow.sn + this->m_pgt->tow.tos;
}

CTime CTime::operator + (CTime& t)
{
	double  dt;
	GpsTime gt;

	dt = this->value() + t.value();
	gt.tow.sn = static_cast<int>(dt);
	gt.tow.tos = dt - gt.tow.sn;
	gt.wn = gt.tow.sn / 604800;
	gt.tow.sn = gt.tow.sn % 604800;

	return CTime(gt);
}

CTime CTime::operator + (double t)
{
	double  dt;
	GpsTime gt;

	dt = this->value() + t;
	gt.tow.sn = static_cast<int>(dt);
	gt.tow.tos = dt - gt.tow.sn;
	gt.wn = gt.tow.sn / 604800;
	gt.tow.sn = gt.tow.sn % 604800;

	return CTime(gt);
}

CTime CTime::operator - (CTime& t)
{
	double  dt;
	GpsTime gt;

	dt = this->value() - t.value();
	gt.tow.sn = static_cast<int>(dt);
	gt.tow.tos = dt - gt.tow.sn;
	gt.wn = gt.tow.sn / 604800;
	gt.tow.sn = gt.tow.sn % 604800;

	return CTime(gt);
}

CTime CTime::operator - (double t)
{
	double  dt;
	GpsTime gt;

	dt = this->value() - t;
	gt.tow.sn = static_cast<int>(dt);
	gt.tow.tos = dt - gt.tow.sn;
	gt.wn = gt.tow.sn / 604800;
	gt.tow.sn = gt.tow.sn % 604800;

	return CTime(gt);
}

//输出时间，普通时
void CTime::PrintTime()
{

	cout<<this->m_pct->year<<"年"<<this->m_pct->month<<"月"<<this->m_pct->day<<"日";
	cout<<this->m_pct->hour<<"时"<<this->m_pct->minute<<"分";
	cout<<fixed<<this->m_pct->second<<"秒"<<endl;

//	cout<<this->m_pgt->wn<<"   "<<this->m_pgt->tow.sn + this->m_pgt->tow.tos<<endl;
}

void CTime::WriteTime(ofstream& os)
{
	os<<this->m_pgt->wn<<" "<<fixed<<this->m_pgt->tow.sn + this->m_pgt->tow.tos;
}