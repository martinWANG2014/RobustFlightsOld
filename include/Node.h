#ifndef NODE_H
#define NODE_H

#include "define.h"
#include "Position.h"
#include "_Base.h"

class Node : public _Base {
protected:
    String m_ICAOCode;        // unique required
    String m_name;            // optional
    Position m_position;        // required
public:
    const String &getICAOcode() const;

    const String &getName() const;

    const Position &getPosition() const;

    Node(String icaoCode, String name, double lat, double longi);

    Node(void);

    virtual ~Node(void);
};

typedef std::vector<Node *> NodeVector;

#endif