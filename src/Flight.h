#ifndef FLIGHT_H
#define FLIGHT_H

#include "define.h"
#include "_Base.h"
#include "Route.h"
#include "Airport.h"
class Flight : public _Base
{
private:

	String		m_ICAOcode;				// ICAO code
	Airport*	m_departure ;           // airport of departure
	Airport*	m_destination ;         // airport of arrival
	Level		m_flight_level ;		// default flight level
	Level		m_actual_fl;			// actual flight level
	Time		m_departure_period ;	// schedulled take off time
	LevelSet	m_levelSet;				// set of availble level for this flight.
	RouteVector m_route_list;			// list of preferred routes
	int			m_num_routes ;			// number of available preffered routes 
	Route*		m_activeRoute;			// the chosen route of this flight

public:
	Flight(void);
	virtual ~Flight(void);
	Flight(String code, Airport* startp, Airport* endp, int fl, int period );

	/**
	* Copy Constructor
	* @param f : Flight to be copied
	*/
	Flight(const Flight& f);
/**
* get the departure period
* @return @var  m_departure_period
*/
	const Time	getDepartureTime() const;

/**
* get the flight level 
* @return @var  m_flight_level
*/
	const int	getFlightLevel() const;

/**
* get the destination airport 
* @return @var  m_destination
*/
	Airport*	getDestination() const;

/**
* get the departure airport
* @return @var  m_departure
*/
	Airport*	getDeparture() const;

/**
* get the icao code airport
* @return @var  m_ICAOcode
*/	
	String		getCode(void) const;
	
/**
* get the actual flight level 
* @return @var  m_actual_fl
*/	
	const int	getActualFlightLevel() const;

/**
* change the actual flight level, see @var  m_actual_fl
*/	
	void		setActualFlightLevel(int fl);
Route*		addNewRoute(const String code, int level);

/**
* Select the route @param r as a new active route 
*/			
	void		setActiveRoute(Route* r) ;

/**
* Select the route @param r as a new active route 
*/				
	Route*		getActiveRoute() const ;
	const LevelSet&	getLevelSet() const;
};
/**
* List of Flights
*/		
typedef std::vector<Flight*>		FlightVector ;
typedef std::map<Flight*, bool>					FlightAfectationMap;

#endif
