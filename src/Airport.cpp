#include "../include/Airport.h"

Airport::Airport(void):
m_ICAOCode(""), m_name (""), m_position(Position(0.0,0.0)) 
{
}

Airport::~Airport(void)
{
}

const String& Airport::getICAOcode() const
{
	return m_ICAOCode;
}

Airport::Airport(String icaoCode, String name, double lat, double longi):
m_ICAOCode(icaoCode), m_name(name), m_position(lat,longi)
{
	
}


const Position& Airport::getPosition() const
{
	return m_position ;
}

const String& Airport::getName() const
{
	return m_name ;
}