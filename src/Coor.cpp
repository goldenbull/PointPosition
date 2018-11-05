// Coor.cpp: implementation of the CCoor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Coor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoor::CCoor()
{
	this->pCoorCar = new CoorCartesian;
	this->pCoorGeo = new CoorGeodesy;
}

CCoor::CCoor(const CCoor& coor)
{
	this->pCoorCar = new CoorCartesian(*coor.pCoorCar);
	this->pCoorGeo = new CoorGeodesy(*coor.pCoorGeo);
}

CCoor::CCoor(double array[3])
{
	this->pCoorCar = new CoorCartesian;
	this->pCoorGeo = new CoorGeodesy;

	this->pCoorCar->X = array[0];
	this->pCoorCar->Y = array[1];
	this->pCoorCar->Z = array[2];

	this->CartesianToGeodesy();
}

CCoor::CCoor(const CoorGeodesy& coor)
{
	this->pCoorGeo = new CoorGeodesy(coor);
	this->pCoorCar = new CoorCartesian;
	this->GeodesyToCartesian();
}

CCoor::CCoor(const CoorCartesian& coor)
{
	this->pCoorCar = new CoorCartesian(coor);
	this->pCoorGeo = new CoorGeodesy;
	this->CartesianToGeodesy();
}

CCoor::CCoor(double x, double y, double z)
{
	this->pCoorCar = new CoorCartesian;
	this->pCoorGeo = new CoorGeodesy;

	this->pCoorCar->X = x;
	this->pCoorCar->Y = y;
	this->pCoorCar->Z = z;

	this->CartesianToGeodesy();
}

CCoor::~CCoor()
{
	if(pCoorCar)
		delete this->pCoorCar;
	if(pCoorGeo)
		delete this->pCoorGeo;
}

CoorSite CCoor::CoorToSite(CCoor& sit_coor)
{
	CoorSite result;
	double dx;
	double dy;
	double dz;

	double c00, c01, c02;
	double c10, c11, c12;
	double c20, c21, c22;

	dx = this->X() - sit_coor.X();
	dy = this->Y() - sit_coor.Y();
	dz = this->Z() - sit_coor.Z();

	c00 = -(sin(sit_coor.B()) * cos(sit_coor.L()));
	c01 = -(sin(sit_coor.B()) * sin(sit_coor.L()));
	c02 =  cos(sit_coor.B());

	c10 = -sin(sit_coor.L());
	c11 =  cos(sit_coor.L());
	c12 =  0;

	c20 =  (cos(sit_coor.B()) * cos(sit_coor.L()));
	c21 =  (cos(sit_coor.B()) * sin(sit_coor.L()));
	c22 =  sin(sit_coor.B());

	result.N = c00 * dx + c01 * dy + c02 * dz;
	result.E = c10 * dx + c11 * dy + c12 * dz;
	result.U = c20 * dx + c21 * dy + c22 * dz;

	return result;
}

CoorSite CCoor::CoorToSite(const CoorGeodesy& coor)
{
	CCoor sit_coor(coor);
	return this->CoorToSite(sit_coor);
}

CoorSite CCoor::CoorToSite(const CoorCartesian& coor)
{
	CCoor sit_coor(coor);
	return this->CoorToSite(sit_coor);
}

void CCoor::CartesianToGeodesy()
{
	double L;
	double B0, B;
	double H;

	L = atan2(this->pCoorCar->Y, this->pCoorCar->X);


	double r;
	double N;
	r = sqrt(this->pCoorCar->X * this->pCoorCar->X + this->pCoorCar->Y * this->pCoorCar->Y);
	B = atan2(this->pCoorCar->Z, r);

	do
	{
		B0 = B;
		N = a / sqrt(1 - ee * sin(B0) * sin(B0));
		B = atan2((this->pCoorCar->Z + N * ee * sin(B0)), r);
	}
	while(fabs(B - B0) > 1.0e-10);

	H = r / cos(B) - N;

	this->pCoorGeo->L = L;
	this->pCoorGeo->B = B;
	this->pCoorGeo->H = H;
}

void CCoor::GeodesyToCartesian()
{
	double N;
	N = a / sqrt(1 - ee * sin(this->pCoorGeo->B) * sin(this->pCoorGeo->B));

	this->pCoorCar->X = (N + this->pCoorGeo->H) * cos(this->pCoorGeo->B) * cos(this->pCoorGeo->L);
	this->pCoorCar->Y = (N + this->pCoorGeo->H) * cos(this->pCoorGeo->B) * sin(this->pCoorGeo->L);
	this->pCoorCar->Z = (N * (1 - ee) + this->pCoorGeo->H) * sin(this->pCoorGeo->B);
}

double CCoor::X()
{
	return this->pCoorCar->X;
}

double CCoor::Y()
{
	return this->pCoorCar->Y;
}

double CCoor::Z()
{
	return this->pCoorCar->Z;
}

void CCoor::X(double v)
{
	this->pCoorCar->X = v;
	this->CartesianToGeodesy();
}

void CCoor::Y(double v)
{
	this->pCoorCar->Y = v;
	this->CartesianToGeodesy();
}

void CCoor::Z(double v)
{
	this->pCoorCar->Z = v;
	this->CartesianToGeodesy();
}

double CCoor::L()
{
	return this->pCoorGeo->L;
}

double CCoor::B()
{
	return this->pCoorGeo->B;
}

double CCoor::H()
{
	return this->pCoorGeo->H;
}

void CCoor::L(double v)
{
	this->pCoorGeo->L = v;
	this->GeodesyToCartesian();
}

void CCoor::B(double v)
{
	this->pCoorGeo->B = v;
	this->GeodesyToCartesian();
}

void CCoor::H(double v)
{
	this->pCoorGeo->H = v;
	this->GeodesyToCartesian();
}

CCoor& CCoor::operator = (const CCoor& coor)
{
	if(this != &coor)
	{
		if(pCoorCar)
			delete this->pCoorCar;
		if(pCoorGeo)
			delete this->pCoorGeo;

		this->pCoorCar = new CoorCartesian(*coor.pCoorCar);
		this->pCoorGeo = new CoorGeodesy(*coor.pCoorGeo);
	}

	return *this;
}

CCoor& CCoor::operator = (double array[3])
{
	if(pCoorCar)
		delete this->pCoorCar;
	if(pCoorGeo)
		delete this->pCoorGeo;

	this->pCoorCar = new CoorCartesian;
	this->pCoorGeo = new CoorGeodesy;

	this->pCoorCar->X = array[0];
	this->pCoorCar->Y = array[1];
	this->pCoorCar->Z = array[2];

	this->CartesianToGeodesy();

	return *this;
}

CCoor& CCoor::operator = (const CoorGeodesy& coor)
{
	if(pCoorCar)
		delete this->pCoorCar;
	if(pCoorGeo)
		delete this->pCoorGeo;

	this->pCoorGeo = new CoorGeodesy(coor);
	this->pCoorCar = new CoorCartesian;
	this->GeodesyToCartesian();

	return *this;
}

CCoor& CCoor::operator = (const CoorCartesian& coor)
{
	if(pCoorCar)
		delete this->pCoorCar;
	if(pCoorGeo)
		delete this->pCoorGeo;

	this->pCoorCar = new CoorCartesian(coor);
	this->pCoorGeo = new CoorGeodesy;
	this->CartesianToGeodesy();

	return *this;
}

double CCoor::DistanceTo(CCoor& coor)
{
	double R(0);

	R += (X() - coor.X()) * (X() - coor.X());
	R += (Y() - coor.Y()) * (Y() - coor.Y());
	R += (Z() - coor.Z()) * (Z() - coor.Z());
	R  = sqrt(R);

	return R;
}

double CCoor::DistanceTo(const CoorGeodesy& coor)
{
	CCoor coorObj(coor);
	return DistanceTo(coorObj);
}

double CCoor::DistanceTo(const CoorCartesian& coor)
{
	double R(0);

	R += (X() - coor.X) * (X() - coor.X);
	R += (Y() - coor.Y) * (Y() - coor.Y);
	R += (Z() - coor.Z) * (Z() - coor.Z);
	R  = sqrt(R);

	return R;
}

double CCoor::ElevationAngleTo(CCoor& coor)
{
	double R;
	double rad;
	CoorSite coor_sit;

	R = this->DistanceTo(coor);
	coor_sit = this->CoorToSite(coor);
	rad = asin(coor_sit.U / R);

	return rad;
}

double CCoor::ElevationAngleTo(const CoorGeodesy& coor)
{
	double R;
	double rad;
	CoorSite coor_sit;

	R = this->DistanceTo(coor);
	coor_sit = this->CoorToSite(coor);
	rad = asin(coor_sit.U / R);

	return rad;
}

double CCoor::ElevationAngleTo(const CoorCartesian& coor)
{
	double R;
	double rad;
	CoorSite coor_sit;

	R = this->DistanceTo(coor);
	coor_sit = this->CoorToSite(coor);
	rad = asin(coor_sit.U / R);

	return rad;
}

void CCoor::PrintCoor()
{
	cout<<"\tX = "<<fixed<<X()<<"\tY = "<<Y()<<"\tZ = "<<Z()<<endl;
//	cout<<"\tL = "<<fixed<<L()<<"\tB = "<<B()<<"\tH = "<<H()<<endl;
}

void CCoor::WriteCartesianCoor(ofstream& out)
{
	out << fixed << this->pCoorCar->X << "  " << this->pCoorCar->Y << "  " << this->pCoorCar->Z << "  ";
}

void CCoor::WriteGeodesyCoor(ofstream& out)
{
	out << fixed << this->pCoorGeo->B << "  " << this->pCoorGeo->L << "  " << this->pCoorGeo->H << "  ";
}