#ifndef DEFINE_H
#define DEFINE_H
#undef IL_STD
#define IL_STD

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include <ilcplex/ilocplex.h>
#include <math.h>
#include <cstdlib>
#include <ctime>

typedef std::string				String;
typedef int						Time;
typedef int						Level;
typedef std::set<Level>			LevelSet;
typedef std::vector<int> IntVector;
typedef std::vector<double>		VectorDouble;             //definition du vecteur double
const double	EPSILON				= 1.0e-10;			// precision of double variables
const int		ITMAX				= 1000 ;
const int		MAXCHAR				= 1024;				// maximum size of buffer
const int		LONGLINE			= 8*1024;   		// maximum size of a line in input file
const int		MAX_NAME_LENGTH 	= 30;
const double	MAXDOUBLE			= 1e30;	

#endif