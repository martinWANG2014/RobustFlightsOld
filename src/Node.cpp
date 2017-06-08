#include "../include/Node.h"

Node::Node(void):
m_ICAOCode(""), m_name (""), m_position(Position(0.0,0.0)) 
{
}

Node::~Node(void)
{
}

const String& Node::getICAOcode() const
{
	return m_ICAOCode;
}

Node::Node(String icaoCode, String name, double lat, double longi):
m_ICAOCode(icaoCode), m_name(name), m_position(lat,longi)
{
	
}


const Position& Node::getPosition() const
{
	return m_position ;
}

const String& Node::getName() const
{
	return m_name ;
}