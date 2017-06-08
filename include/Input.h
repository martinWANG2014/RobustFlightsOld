#ifndef INPUT_H
#define INPUT_H

#include "define.h"
#include "Position.h"
#include "Node.h"
#include "Route.h"
#include "Airport.h"
#include "Flight.h"
#include "Network.h"

/**
* This class perform the input tasks for the network
* We need 4 files
* 1. Configuration file, tell us which names are used for Airport , Waypoint, Flight plan data files 
* 2. Airport data file
* 3. Waypoint data file
* 4. Flight plan data file 
*/
class Input {
private:
    Network *m_network;
    String m_airport_filename;
    String m_node_filename;
    String m_flight_filename;
    int m_num_periods;
    IntVector m_VectList;
    int m_numpluslevel; //additional level
public:
    Input(Network *pNetwork);

    ~Input(void);

    bool initNetwork(String config_filename, int period_length);

    /**
* Parse the configuration file 	 
* @param filename name of configuration file.
* @return true if function success, false otherwise.
* this function will call 
* @fn readAirportFile
* @fn readNodeFile
* @fn readFlighfFile 
*/
    bool readConfigFile(const String filename);

    /**
* Read Airport data 	 
*/
    bool readAirportFile(void);

/**
* Read Waypoint data 	 
*/
    bool readNodeFile(void);
// read Level list

    bool readLevelSet();

/**
* Read Flight data 	 
*/
    bool readFlightFile(int period_length);

    int maxLevel(int *NombreVols, IntVector temp_list, LevelSet level);

    bool appartient0(int i, IntVector Contrainte_list);
};

#endif
