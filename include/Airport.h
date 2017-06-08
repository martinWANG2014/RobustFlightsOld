#ifndef AIRPORT_H
#define AIRPORT_H

#include "define.h"
#include "Position.h"
#include "_Base.h"

class Airport : public _Base {
protected:
    String m_ICAOCode;        // unique required
    String m_name;            // required
    Position m_position;        // required
public:
    const String &getICAOcode() const;

    const String &getName() const;

    const Position &getPosition() const;

    Airport(String icaoCode, String name, double lat, double longi);

    Airport(void);

    virtual ~Airport(void);
};

/**
* Definition of a list of airports 
*/
typedef std::vector<Airport *> AirportVector;

#endif
