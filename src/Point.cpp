#include "../include/define.h"
#include "../include/Point.h"
#include "../include/Node.h"
Point::Point(void)
: m_time(0)
, m_node(NULL) 
{
}

Point::~Point(void)
{
}

Point::Point( Node* node, Time period,int period_time,int position)
: m_time(period)
, m_node(node),m_pos(position),period_time(period_time),m_period(period/period_time)
{
}
Node* Point::getNode(void) const
{
	return m_node;
}
Time Point::getTime(void) const
{
	return m_time;
}

int Point::getPosInRoute(void) const
{
	return m_pos;
}

void Point::setPosInRoute(int pos)
{
	m_pos = pos;
}
int Point::getPeriodTime() const{
	return period_time;
}
int Point::getPeriod() const{
	return m_period;
}
bool Point::approx(Point * p2){
	double k=(this->getTime()- p2->getTime());
	double m=fabs(k);
	if (this->getNode()->getICAOcode()==p2->getNode()->getICAOcode()&&m<= this->getPeriodTime()){
		return true;
	}
	return false;
}