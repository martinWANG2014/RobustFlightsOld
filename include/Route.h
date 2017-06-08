#ifndef ROUTE_H
#define ROUTE_H

#include "define.h"
#include "Point.h"
#include "_Base.h"

class Route;

class Flight;

/**
* List of routes
*/
typedef std::vector<Route *> RouteVector;

class Route : public _Base {
private:
    String m_code;            // Code to give a name of corresponding column in LP.
    PointVector m_point_list;    // Points exist in the route.
    Flight *m_flight;        // Flight to which this route belongs
    int m_level;        // Each route have a constant altitude.
    RouteVector m_conflicts;    // List of routes in conclict with it
public:
    Route(void);

    virtual ~Route(void);

    Route(const String code, Flight *f, int level);

    Flight *getFlight(void);

    const String &getCode() const;

    const PointVector &getPointList(void) const;

    const int getLevel(void) const;

    double probabConflit(Point *p1, Point *p2, Route *r2);

    double probabConflictViaDepartTime(Point *p1, Point *p2, Route *r2);

    double delaiSansConflit(Point *p1, Point *p2, Route *r2, int *ind);

    double delaiSansConflitViaDepartTime(Point *p1, Point *p2, Route *r2, int *ind);

    Point *getPoint(unsigned int index);

    double erf(double X);

    double fonctionF(double X);

    void addPoint(Point *point);
};

#endif
