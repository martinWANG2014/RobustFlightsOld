#ifndef POSITION_H
#define POSITION_H

#include "define.h"
class Position
{
private:
	double m_latitude;
	double m_longitude;
public:
	//constructeur
	Position(double latitude,double longitude);
	Position();
	Position(const Position& p);
	//destructeur
	~Position();
	//ascenceur
	double getLatitude() const;
	double getLongitude() const;
	friend double distanceBetween(const Position & p1, const Position & p2);
	/**
	* A Friend function used to print the longitude and latitude of this Position.
	*/
	friend std::ostream& operator << (std::ostream& os, Position& p);
};

#endif
