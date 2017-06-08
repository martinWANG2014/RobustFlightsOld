#include "../include/Position.h"

//constructeur
Position::Position(): m_latitude(0.0), m_longitude(0.0)
{
	
}

Position::Position(double latitude, double longitude) : m_latitude(latitude) , m_longitude(longitude)
{
}
Position::Position(const Position& p)
{
	m_latitude = p.m_latitude ;
	m_longitude = p.m_longitude ;
	
}
//destructeur
Position::~Position(void)
{
}
//ascenceur
double Position::getLongitude() const
{
	return m_longitude ;
}

double Position::getLatitude() const
{	
	return m_latitude ;
}

double distanceBetween(const Position & p1, const Position & p2)
{
/*Haversine formula:R = earth’s radius (mean radius = 6,378.16km)
					Δlat = lat2− lat1
					Δlong = long2− long1
					a = sin²(Δlat/2) + cos(lat1).cos(lat2).sin²(Δlong/2)
					c = 2.atan2(√a, √(1−a))
					d = R.c
(Note that angles need to be in radians to pass to trig functions).
*/
	const double PIx = 3.141592653589793; 
    const double RADIO = 6378.16;
	double lat1=p1.getLatitude();
	double lat2=p2.getLatitude();
	double long1=p1.getLongitude();
	double long2= p2.getLongitude();

	double dlat=lat1-lat2;
	double dlon=long1-long2;
	double dlatRadian=dlat*PIx/180;
	double dlonRadian=dlon*PIx/180;
	double lat1Radian=lat1*PIx/180;
	double lat2Radian=lat2*PIx/180;
	double a = sin(dlatRadian/2)*sin(dlatRadian/2)+cos(lat1Radian)*cos(lat2Radian)*sin(dlonRadian/2)*sin(dlonRadian/2);
	double c = 2*atan2(sqrt(a),sqrt(1-a));
	double d=RADIO*c;
	return d;
}

std::ostream& operator << (std::ostream& os, Position& p)
{
	os << p.getLatitude() << " "  << p.getLatitude();
	return os;
}
