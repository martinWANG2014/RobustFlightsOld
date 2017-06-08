#ifndef NETWORK_H
#define NETWORK_H

#include "define.h"
#include "Node.h"
#include "Airport.h"
#include "Flight.h"
class Network
{
	friend class Input;
public:
	int				m_num_nodes;		 // number of nodes
	int				m_num_flights;		 // number of flights
	int				m_num_airports;		// number of airports	
	int				m_periodLength;		//  number of periods
	int				m_numLevels;		//  number of levels 
	NodeVector		m_node_list;		// list of nodes
	FlightVector	m_flight_list;		// list of flights
	IntVector		m_level_list;		// list of levels
	RouteVector		m_route_list;		// list of routes
	PointVector		m_point_list;		// list of points
	AirportVector	m_airport_list;		// list of aiports
	IntVector		m_activeLevels;		// list of active levels
	IntVector		m_inactiveLevels; // list of inactive levels

	Network(void);
	~Network(void);
	void				addRoute(Route* r);
	const int			getNodeNumber() const;
	Node*				findNodeByCode(const String& code) const;
	/**
* Add new node to the network
*/			
	int					addNewNode(const Node& node);
/**
* Get the list of nodes
*/			
	const NodeVector&	getNodeList(void) const;
	const int			getFlightNumber() const;	

/**
* Search a Flight by code name.
* @return the pointer to Flight if found one, otherwise return null
*/			
	Flight*				findFlight(const String& code) const;
	const FlightVector&	getFlightList(void) const;
	/**
* Search an airport by icao code name.
*/			
	Airport*			findAirport(const String& icaoCode) const ;
		void				setPeriodLength(int period);

/**
* get the period length 
*/
	int					getPeriodLength() const;
		int					getNumLevels() const;
		/**
* Get a set of levels having conflicts 
*/
const IntVector&		getActiveLevels();

/**
* Get a set of levels having no conflicts 
*/
const IntVector&		getInActiveLevels();
const IntVector&		getLevelList();
const RouteVector&		getRouteList(void) const;
void setLevelList(IntVector& level_list);
/**
* Add a new airport into the network
*/			
	Airport*				addNewAirport(const String& icaoCode, const String& name, double lat, double longi);
	Flight*				addNewFlight(Airport* startp, Airport* endp, const String& fID, int fl, int time);
	Point*					addNewPoint(Node* node, int time,int period_len, int pos);
};

#endif
