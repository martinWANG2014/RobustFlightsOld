#include "../include/Flight.h"

Flight::Flight() : 
m_ICAOcode(""),
 m_departure(NULL) , m_destination (NULL) , 
m_flight_level(0), m_actual_fl(0),m_departure_period(0), 
m_levelSet()
{

}
Flight::Flight(const Flight &f): m_ICAOcode(f.m_ICAOcode), m_departure(f.m_departure) , m_destination (f.m_destination) , 
m_flight_level(f.m_flight_level),m_actual_fl(f.m_actual_fl), m_departure_period(f.m_departure_period), 
m_levelSet(f.m_levelSet)
{	
	
}
Flight::Flight(String code, Airport* startp, Airport* endp, int fl, int period):
m_ICAOcode(code), m_departure(startp) , m_destination (endp) , 
m_flight_level(fl), m_actual_fl(fl),m_departure_period(period),  
 m_levelSet()
{
}
Flight::~Flight(void)
{
}

Airport* Flight::getDeparture() const
{
	return m_departure;
}

Airport* Flight::getDestination() const
{
	return m_destination;
}

const int Flight::getFlightLevel() const
{
	return m_flight_level;
}

const int Flight::getActualFlightLevel() const
{
	return m_actual_fl;
}


void Flight::setActualFlightLevel(int fl) 
{
	m_actual_fl = fl;
}

const Time Flight::getDepartureTime() const
{
	return m_departure_period;
}
String Flight::getCode(void) const
{
	return m_ICAOcode;
}
Route* Flight::addNewRoute(const String code, int level)
{
	Route* pr = new Route(code,this, level);
	if (pr != NULL )
	{
		m_route_list.push_back(pr);
		m_num_routes++;
		m_levelSet.insert(level);
	}
	return pr;
}
void Flight::setActiveRoute(Route* r) 
{
	m_activeRoute = r;
}

Route* Flight::getActiveRoute() const 
{
	return m_activeRoute ;
}
const LevelSet&	Flight::getLevelSet() const
{
	return m_levelSet;
}
