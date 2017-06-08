#ifndef POINT_H
#define POINT_H

#include "define.h"
#include "_Base.h"
class Node;

/**
* An pair of @class Node and period makes a Point. 
*/
class Point : public _Base
{
public:
	Point(void);
	virtual ~Point(void);
protected:
	/// The moment that route arrive at Node
	Time m_time;
	/// The pointer to the Node that Point corresponds to
	Node* m_node;
	/// The period that route arrive at Node
	Time m_period;
	int period_time;//constant
	int m_pos;
	/// position of Point in the route
public:
	Point(Node* node, int period,int period_time,int position);
	int getTime(void) const;
	Node* getNode(void) const;
	int getPosInRoute(void) const;
	void setPosInRoute(int pos);
	int getPeriodTime() const;
	int getPeriod() const;
	bool approx(Point * p2);

};


/**
* PointVector is a list of Point, it is equivalent to an air route. 
*/
typedef std::vector<Point*> PointVector; 

#endif
