#ifndef INPUT_H
#define INPUT_H

#include "define.h"
#include "Position.h"
#include "Node.h"
#include "Route.h"
#include "Airport.h"
#include "Flight.h"
#include "Network.h"
#include <algorithm>

//=======================================================================================
//add by chenghao wang
static IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450,
                           490};
static IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470,
                           510};
static IntVector LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
static IntVector LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};
//=======================================================================================
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

    IntVector findFeasibleLevels(int iDefaultLevel);
};

#endif
