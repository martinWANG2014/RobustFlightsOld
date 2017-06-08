#include "Input.h"
#include <strstream>
Input::Input(Network* pNetwork): m_network( pNetwork)
{
}
Input::~Input(void)
{
}
bool Input::initNetwork(String config_filename, int period_length){
	readConfigFile(config_filename);
	readAirportFile();
	readNodeFile();
	readFlightFile(period_length);
	m_network->setPeriodLength(period_length);
	return 1;
}
bool Input::readConfigFile(const String filename)
{
	
	if ( filename.empty() )
		return false;
	
	std::ifstream ifile ;
	char c[MAXCHAR];
	// lire le fichier listedges.txt, qui contient les liens du Networke 
	ifile.open(filename.c_str());
	if (!ifile.is_open())
	{
		std::cout << "ERREUR: \tfichier " << filename << "n'existe pas\n" ;
		exit(0);
	}
	
	// skip the first line
	ifile.getline(c,MAXCHAR);
	
	// read number of periods at the second line
	ifile >> m_num_periods;
	
	// skip the the rest of this line
	ifile.getline(c,MAXCHAR);
	
	// read airport file name
	ifile >> m_airport_filename ;
	
	// skip the the rest of this line
	ifile.getline(c,MAXCHAR);

	// read node file name
	ifile >> m_node_filename;
	
	// skip the the rest of this line
	ifile.getline(c,MAXCHAR);
	
	// read flight file name
	ifile >> m_flight_filename;
	
    // skip the the rest of this line
	ifile.getline(c,MAXCHAR);
	
	if ( ifile.is_open() )
		ifile.close();

	std::cout << "Number of periods = " << m_num_periods << "\n" ;
	return true;	
}

bool Input::readAirportFile(void)
{
	if ( m_airport_filename.empty() )
		return false;
	
	std::ifstream ifile ;
	
	ifile.open(m_airport_filename.c_str());
	
	if (!ifile.is_open())
	{
		std::cout << "ERREUR: \tfichier " << m_airport_filename << "n'existe pas\n" ;
		exit(0);
	}
	
	char line[MAXCHAR];
	
	// skip the first line
	ifile.getline(line, MAXCHAR);
	
	// get number of nodes
	int num_nodes;	
	ifile >> num_nodes ;
	
	// skip the rest of the first line
	ifile.getline(line, MAXCHAR);
	
	int n = 0 ;

	// get the new line
	int numline = 2 ;

	while ( !ifile.eof() && (n < num_nodes) )
	{
		// get the next line
		ifile.getline(line,MAXCHAR) ;

		numline++ ;
		std::istringstream	istreamNode(line) ;		
		
		String  icaoCode;
		double latitude, longitude;
		// get airport icao code, latitude and longitude
		istreamNode >> icaoCode >> latitude >> longitude ;
		char token[MAXCHAR];
		istreamNode.getline(token,MAXCHAR);
		String name(token);

		// if airport icao code is not blank, enter the airport into the network
		if (!icaoCode.empty())
		
		{
			Position p(latitude, longitude);
			m_network->addNewAirport(icaoCode, name, latitude, longitude);
			n++ ;
		}		
	}
	
	std::cout << "Read airports file sucessfully.\n" ;
	std::cout << "Number of airports = " << n << "\n";
	if ( ifile.is_open() )
		ifile.close();
	return true;	
}

bool Input::readNodeFile(void)
{
	if ( m_node_filename.empty() )
		return false;
	std::ifstream ifile ;
	ifile.open(m_node_filename.c_str());
	if (!ifile.is_open())
	{
		std::cout << "ERREUR: \tfichier " << m_node_filename << "n'existe pas\n" ;
		exit(0);
	}
	char line[MAXCHAR];
	// skip the first line
	ifile.getline(line, MAXCHAR);
	// get number of nodes
	int num_nodes;	
	ifile >> num_nodes ;
	// skip the rest of the first line
	ifile.getline(line, MAXCHAR);
	int n = 0 ;
	// get the new line
	int numline = 2 ;
	while ( !ifile.eof() && (n < num_nodes) )
	{
		// get a whole line
		ifile.getline(line, MAXCHAR);
		numline++ ;
		// create a stream for input
		std::istringstream	istreamNode(line) ;		
		String  code;
		double latitude, longitude;
		// get waypoint codename, latitude and longitude
		istreamNode >> code >> latitude >> longitude ;

		char token[MAXCHAR];
		istreamNode.getline(token,MAXCHAR);
		String name(token);
		// if codename is not blank, enter the node into the network
		if (!code.empty())
		{
			Node node(code, name, latitude,longitude);
			n++;
			m_network->addNewNode(node);
		}	
	}
	std::cout << "Read nodes file sucessfully.\n" ;
	std::cout << "Number of nodes = " << n << "\n";
	if ( ifile.is_open() )
		ifile.close();
	return true;	
}
bool Input::appartient0(int i,IntVector Contrainte_list){
	for (unsigned int j=0;j<Contrainte_list.size();j++){
		if (i==Contrainte_list[j]){
			return true;
		}
	}
	return false;
}
int Input::maxLevel(int * NombreVols,IntVector temp_list,LevelSet level){
	int maxV=-1;
	int maxIndice=-1;
	int index=0;
	for (LevelSet::iterator itB=level.begin();itB!=level.end();itB++,index++){
		if (NombreVols[index] > maxV && !appartient0(index,temp_list)){
			maxV=NombreVols[index];
			maxIndice=index;
		}
	}
	if (maxIndice!=-1){
		return maxIndice;
	}
	return -1;
}
bool Input::readFlightFile(int period_length)
{
	if ( m_flight_filename.empty() )
		return false;
	std::ifstream ifile ;
	ifile.open(m_flight_filename.c_str());
	if (!ifile.is_open())
	{
		std::cout << "ERREUR: \tfichier " << m_flight_filename << "n'existe pas\n" ;
		exit(0);
	}
	int	num ;
	IntVector temp_list;
	LevelSet level;
	char	line[LONGLINE] ;
	// skip the first line
	ifile.getline(line,MAXCHAR) ;
	// the second line is number of flights
	ifile >> num;
	//num = 0.4*num;
	ifile.getline(line,MAXCHAR) ;
	// the remain lines are flight information, each line corresponds to a flight
	int num_flights =0;
	// 1:DEPA;2:DEST;3:Aircraft ID;4:Off Time;5:Level;6:Point Profile (Node code:Time)	
	int numline = 2 ;
	while (!ifile.eof() &&	num_flights < num )
	{
		// get the next line
		ifile.getline(line,LONGLINE) ;
		numline++ ;
		// convert to fields using ';' as separator 
		std::istringstream	isline(line) ;
		// get airport of departure
		char	token[MAXCHAR];		
		isline.getline(token, MAXCHAR, ';');
		String departure(token);
		// airport of departure must be non empty
		if (departure.empty()) 
		{
			std::cerr << numline<< " Warning: departure is empty. Skip this line.\n" ;
			continue ;
		}
		Airport* depAirport = m_network->findAirport(departure);
		if (depAirport == NULL) // must verify also the existence of airport
		{
			std::cerr << numline << " Warning: departure " << departure << " not exist. Skip this line.\n" ;
			continue ;
		}
		// get airport of destination
		isline.getline(token, MAXCHAR, ';');
		String destination(token);		
		if (destination.empty())
		{
			std::cerr << numline << " Warning: destination is empty. Skip this line.\n" ;
			continue ;
		}
		if (departure == destination)
		{
			std::cerr << numline << " Warning: the destination is identical with the departure. Skip this flight.\n" ;
			continue;
		}
		Airport* destAirport = m_network->findAirport(destination);
		if (destAirport == NULL) // must verify also the existence of airport
		{
			std::cerr << numline << " Warning: destination " << destination << " not exist. Skip this line.\n" ;
			continue ;
		}
		// 3:FlightID;4:OffTime;5:FL;6:Point Profile (Point:Time)	
		// get FlighID
		isline.getline(token, MAXCHAR, ';');
		String flightID(token);
		// get time of departure 
		isline.getline(token, MAXCHAR, ';');
		Time departureTime;
		{			 
			String tmp_s(token);
			std::istringstream	isstr_temp( tmp_s) ;
			isstr_temp >> departureTime ;		
		}
		
		//departureTime = departureTime / period_length + 1;
		isline.getline(token, MAXCHAR, ';');
		int flightLevel;
		{			 
			String tmp_s(token);
			std::istringstream	isstr_temp( tmp_s) ;
			isstr_temp >> flightLevel ;
		}
		
		char pointlist[LONGLINE];
		isline.getline(pointlist, LONGLINE, ';');
		if (strlen(pointlist) <= 1) // pointlist is empty
			continue;  
		// input for a flight

		Flight* pf = m_network->findFlight(flightID);
		if (pf != NULL)
			continue;

		pf = m_network->addNewFlight( depAirport, destAirport, flightID, flightLevel, departureTime);

		// fail to create a flight
		if ( pf == NULL )
			continue;     // skip the rest of line, flight info is non valid		
		// Flight is created successfully
		// set the flight order in the list

		num_flights++ ;
		//std::cout << "Flight from " << departure << " to " << destination << "\n";
		std::istringstream	islist(pointlist) ;
		while ( !islist.eof())
		{
			char tmp[MAXCHAR];
			islist.getline(tmp, MAXCHAR, ' ');
			// pointdata is "pointcode:inner_time:pointcode:inner_time: " 
			std::istringstream	isfl(tmp) ;
			Time inner_time;
			String pointcode;
			
			{ // using block to free string stream	after existing the block

				// get pointcode
				isfl.getline(tmp, MAXCHAR,':');		
				{
					std::istringstream	pointstream(tmp) ;
					pointstream >> pointcode ;
				}
				
				// get inner_time				
				isfl.getline(tmp, MAXCHAR, ':');		
				{
					std::istringstream	pointstream(tmp) ;
					pointstream >> inner_time ;
				}
				//inner_time =  inner_time / period_length + 1 ;	
			}
			

			if ( pointcode.empty() )
				continue;
			char buffer[MAXCHAR] ;
			sprintf(buffer, "FL%02d%s" , num_flights-1, flightID.c_str());		
			String rcode (buffer);
			// attach route to Flight pf
			Route** pr = new Route*[3];
			int index=0;
			
			//extern int clevel;

			for ( int i= 0,l = -1 ; l <= 1 ; l++, i++)
			{	
				// we consider levels �0
				pr[i] = pf->addNewRoute(rcode, flightLevel + 40*l);		
						level.insert(flightLevel+ 40*l);
				// set active route is scheduled route
				if (l==0){
					pf->setActiveRoute(pr[i]) ;
				}

				// aad route to list of route in nework
				m_network->addRoute(pr[i]);
			}
			do
			{
				// add point to Route
				Node* pnode = m_network->findNodeByCode(pointcode);
				Point* point = m_network->addNewPoint(pnode, inner_time,period_length,index);					
				point->setOrder(index++); // update the order of point in the route

				for (int i = 0, l = -1 ; l <= 1  ; l++, i++)
				{
					pr[i]->addPoint (point);
					// add flight into passing flight list of node 
					//pnode->update(inner_time , pr[index]);
				}

				// get pointcode
				isfl.getline(tmp, MAXCHAR,':');		
				{
					std::istringstream	pointstream(tmp) ;
					pointstream >> pointcode ;
				}
				// get inner_time
				isfl.getline(tmp, MAXCHAR, ':');		
				{
					std::istringstream	pointstream(tmp) ;
					pointstream >> inner_time ;
					//inner_time =  inner_time / period_length + 1 ;
				}				
			}while (!isfl.eof());		
			delete[] pr;
			//delete nom;
		}
		/*std::cout << "Flight code:		" << flightID		<< "\n";
		std::cout << "Start point:		" << departure		<< "\n";
		std::cout << "Endp point:		" << destination	<< "\n";
		std::cout << "Flight level:		" << flightLevel	<< "\n";
		std::cout << "departure_time:	" << departureTime	<< "\n";*/	
	} // end loop for each line
		
	if ( ifile.is_open() )
		ifile.close();
	int* NombreVols=new int[level.size()];
	FlightVector flist=m_network->getFlightList();
	int indexL=0;
	for (LevelSet::iterator itA = level.begin() ; itA != level.end() ; itA++,indexL++){
		int l=(*itA);
		NombreVols[indexL]=0;
		for (int i=0;i< (int)flist.size();i++){
			Flight *f=flist[i];
			LevelSet lset=f->getLevelSet();
			/*for (LevelSet::iterator itB=lset.begin();itB!=lset.end();itB++){
				int k=(*itB);
				if (l==k){
					NombreVols[indexL]=NombreVols[indexL]+1;
				}
			}*/
			Level k=f->getFlightLevel();
			if (l==k){
					NombreVols[indexL]=NombreVols[indexL]+1;
				}
		}
		//temp_list.push_back(l);
	}
	IntVector temp_list2;
	while (temp_list.size()!=level.size()){
		int k=maxLevel(NombreVols,temp_list,level);
		if (k!=-1){
			temp_list.push_back(k);
		indexL=0;
		for (LevelSet::iterator itB=level.begin();itB!=level.end();itB++,indexL++){
			if (indexL==k){
				int m=(*itB);
				temp_list2.push_back(m);
			}
		}
		}
	}
	m_network->setLevelList(temp_list2);
	std::cout << "Read flight file sucessfully.\n" ;
	std::cout << "Number of flights = " << num_flights << "\n";		
	return true;	
}