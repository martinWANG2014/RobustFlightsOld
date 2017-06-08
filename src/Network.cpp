#include "../include/Network.h"

Network::Network(void)
{
}

Network::~Network(void)
{
}
void Network::addRoute(Route *r)
{
	m_route_list.push_back(r);
	r->setOrder(m_route_list.size() - 1);
}

const int Network::getNodeNumber() const
{
	return m_num_nodes;
}
Node* Network::findNodeByCode(const String& code) const
{
	
	for (NodeVector::const_iterator it = m_node_list.begin() ; it != m_node_list.end() ; it++)
	{
		if ( (*it)->getICAOcode()== code )
			return (*it);
	}
	return NULL;
}
int Network::addNewNode(const Node& node)
{
	// not add a node with name empty
	if ( node.getICAOcode().empty() )
	{
		std::cerr << "WARNING: try to add a node without code has failed ! This node was ignored \n ";
		return m_num_nodes;
	}
	Node* tmp_node = new Node(node);
	m_node_list.push_back(tmp_node) ;
	tmp_node->setOrder(m_num_nodes);
	m_num_nodes++;
	return m_num_nodes;
}
const NodeVector& Network::getNodeList(void) const
{
	return m_node_list;
}
const int Network::getFlightNumber() const
{
	return m_num_flights;
}
Flight* Network::findFlight(const String& code) const
{
	if  (code == "")  
		return NULL ;	
	for ( FlightVector::const_iterator it = m_flight_list.begin() ;it != m_flight_list.end() ;it++)
	{
		if ( (*it)->getCode() == code  )
			return (*it);
	}
	return NULL;
}
const FlightVector& Network::getFlightList(void) const
{
	return m_flight_list;
}

Airport* Network::findAirport(const String& icaoCode) const
{
	
	if  (icaoCode.empty())  
		return NULL ;
	
	for ( AirportVector::const_iterator it = m_airport_list.begin() ;it != m_airport_list.end() ;it++)
	{
		if ( (*it)->getICAOcode() == icaoCode  )
			return (*it);
	}
	return NULL;
}

void Network::setPeriodLength(int period)
{
	m_periodLength = period;
}

int	Network::getPeriodLength() const
{
	return m_periodLength;
}
int	Network::getNumLevels() const
{
	return m_numLevels;
}
const IntVector& Network::getActiveLevels()
{
	return m_activeLevels; //levels;
}
const IntVector& Network::getInActiveLevels()
{
	return m_inactiveLevels;
}
const IntVector& Network::getLevelList()
{
	return m_level_list;
}

const RouteVector& Network::getRouteList(void) const
{
	return m_route_list;
}
Airport* Network::addNewAirport(const String& icaoCode, const String& name, double lat, double longi)
{
	if (icaoCode.empty() ) // code empty
		return NULL;

	Airport* tmp_airport = new Airport(icaoCode, name, lat, longi);	
	m_airport_list.push_back(tmp_airport);
	tmp_airport->setOrder(m_num_airports);
	m_num_airports++;
	return tmp_airport;
}
Flight* Network::addNewFlight(Airport* startp, Airport* endp,const String& fID, int fl, Time time)
{
	if (fID.empty() ) // code empty
		return NULL;
	if (startp == endp ) // same node
		return NULL;	
	Flight* tmp_flight = new Flight(fID, startp,  endp, fl, time);
	m_flight_list.push_back(tmp_flight);
	tmp_flight->setOrder(m_num_flights);
	m_num_flights++;
	return tmp_flight;
}
Point* Network::addNewPoint(Node* node, Time time,int period_len,int pos)
{
	if ((node==NULL) || (time <= 0))
		return NULL;
	Point* point = new Point(node, time,period_len,pos); 
	m_point_list.push_back (point);
	point->setOrder(m_point_list.size() -1);
	return point;
}
void Network::setLevelList(IntVector& level_list){
	m_level_list=level_list;
}

