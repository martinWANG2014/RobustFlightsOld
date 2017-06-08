#include "../include/define.h"
#include "../include/Network.h"
#include "../include/Input.h"
#include "../include/ProcessClock.h"
using namespace std;
typedef std::map<Level, bool> LevelExamine;
ILOSTLBEGIN
ofstream ofile1;

/*afficher tous les combinaisons dans un fichier*/
void combinaison(vector<int> L,vector<int> F,int k){
	if (k > F.size()){
		return ;
	}else if (k==0){
		for (unsigned int i=0;i<L.size();i++){
			ofile1 << L[i] << " ";
			//cout << L[i]<<" ";
		}
		ofile1 << "\n";
		//cout <<"\n";
	}else {
		for (unsigned int i=0;i<F.size();i++){
			vector<int> G;
			for (unsigned int j=i+1;j<F.size();j++){
				G.push_back(F[j]);
			}
			vector<int> L2;
			for (unsigned int j=0;j<L.size();j++){
				L2.push_back(L[j]);
			}
			L2.push_back(F[i]);
			combinaison(L2,G,k-1);
		}
	}
}
//fonction calcul factorielle
int factorielle(int i)
 {
  if (i>1) return(i*factorielle(i-1));
  else return(1);
 }
int nombreCombinaison(int n,int k){
	return factorielle(n)/(factorielle(k)*factorielle(n-k));
}
/*Tester si indice i est appartient à la liste de contraintes*/
bool appartient(int i,IntVector Contrainte_list){
	for (unsigned int j=0;j<Contrainte_list.size();j++){
		if (i==Contrainte_list[j]){
			return true;
		}
	}
	return false;
}
/*C'est une strong contrainte pour choisir le premier indice pour ajouter dans la liste des contraintes*/
int strong_contraint0(int size, double ** prob,double ** delai,VectorDouble Pi){
	int temp_indice;
	double temp_value=MAXDOUBLE;
	for (int i=0;i < size;i++){
		double temp_somme=0;
		for (int j=0;j< size;j++ )
		{
			temp_somme = temp_somme + delai[i][j]*prob[i][j]/2;
		}
		temp_somme=Pi[i] - temp_somme;
		if (temp_somme < temp_value){
			temp_value = temp_somme;
			temp_indice = i;
		}
	}
	//std::cout << "temp_indice = " << temp_indice << std::endl;
	return temp_indice;
}
/*c'est le contraint pour choisir l'indice pour ajouter dans la liste des contraintes*/
int strong_contraint(int size,double **probability,IntVector Contrainte_list,double** delai,VectorDouble Pi,IloNumArray x){
	int temp_indice;
	double temp_value=MAXDOUBLE;
	for (int i=0;i < size;i++){
		if (appartient(i,Contrainte_list)){
			continue;
		}
		double temp_somme=0;
		for (int j=0;j< size;j++ )
		{
			if (probability[i][j] > 0){
			temp_somme = temp_somme + delai[i][j]*(probability[i][j])*x[j]/2;
			}
		}
		temp_somme=Pi[i] - temp_somme;
		if (temp_somme < temp_value){
			temp_value = temp_somme;
			temp_indice = i;
		}
	}
	//std::cout << "temp_indice = " << temp_indice << std::endl;
	return temp_indice;
}
/*Tester la feasibilité de la solution, en utilisant la formule Hoeffding*/
bool FeasibilityHoeffding(double epsilon,IloNumArray x,VectorDouble Pi,double Coef_Pij,double **delai,int size,double **probability)
{
	for (int i=0;i<size;i++){
		double temp=0;
		if (x[i]==1){		
			for (int j=0;j<size;j++){
				if (probability[i][j] > 0){
					temp = temp + probability[i][j]*delai[i][j]*x[j]/2;
				}
			}
			temp = Pi[i] - temp;
			temp = 2* temp*temp;
			double temp2=0;
			for (int j=0;j<size;j++){
				if (probability[i][j] > 0){
					temp2 = temp2 + delai[i][j]*delai[i][j]*x[j];
				}
			}
			temp=temp/temp2;
			temp=exp(-temp);
			//cout << "temp="<< temp  <<endl;
			if (temp > epsilon){
				return false;
			}
		}
	}
	return true;
}
double randomUniform()
{
	return rand()/(double)RAND_MAX;
}
double randomUniform(double a,double b)
{
	return (b-a)*randomUniform()+a;
}
/*tirer un chiffre y dans une loi normale de moyenne mean et d’écart-type ecart,méthode de Box-Muller*/
double getRandom(double mean,double ecart){
	double x1, x2, w, y;
	do {
		x1 = 2.0 * randomUniform(0,1) - 1.0;
		x2 = 2.0 * randomUniform(0,1) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while ( w >= 1.0);

	w = sqrt( (-2.0 * log( w ) ) / w );
	y = x1 * w;
	y = (y * ecart) + mean;
	return y;
}

double TempDepart(double temp){
	double nombre1;
	double nombre2;
	double heure;
	nombre1=randomUniform(0,100);
	if (nombre1 <=19.0){
		nombre2=randomUniform(0,100);
		if (nombre2<=10.0){
			heure=randomUniform(-15,-13);
		}
		if (10.0< nombre2&& nombre2<=30.0){
			heure=randomUniform(-12,-10);
		}
		if (30.0< nombre2&& nombre2<=60.0){
			heure=randomUniform(-9,-8);
		}
		if (nombre2>60.0){
			heure=randomUniform(-7,-5);
		}
	}
	if (19.0 < nombre1 &&nombre1<= 63.0){
		nombre2=randomUniform(0,100);
		if (nombre2 <= 25.0){
			heure=randomUniform(-4,-2);
		}
		if (25.0 <nombre2&&nombre2<= 75.0){
			heure=randomUniform(-1,1);
		}
		if (nombre2 > 75.0){
			heure=randomUniform(2,4);
		}
	}
	if (63.0 < nombre1&&nombre1 <= 83.0){
		nombre2=randomUniform(0,100);
		if (nombre2 <= 40){
			heure=randomUniform(5,7);
		}
		if (40 < nombre2 &&nombre2<= 70){
			heure=randomUniform(8,9);
		}
		if (70 < nombre2 &&nombre2<=90){
			heure=randomUniform(10,12);
		}
		if (nombre2 > 90){
			heure=randomUniform(13,15);
		}
	}
	if (83.0 < nombre1 &&nombre1<= 93.0){
		nombre2=randomUniform(0,100);
		if (nombre2<=50){
			heure=randomUniform(16,20);
		}
		if (50 < nombre2 &&nombre2<= 80){
			heure=randomUniform(21,25);
		}
		if (80 < nombre2){
			heure=randomUniform(26,30);
		}
	}
	if (nombre1 > 93){
		nombre2=randomUniform(0,100);
		if (nombre2<=30){
			heure=randomUniform(31,35);
		}
		if (30<nombre2&&nombre2<=55){
			heure=randomUniform(36,40);
		}
		if (55<nombre2&&nombre2<=75){
			heure=randomUniform(41,45);
		}
		if (75< nombre2&&nombre2<=85){
			heure=randomUniform(46,50);
		}
		if (85 < nombre2&&nombre2 <= 95){
			heure=randomUniform(51,55);
		}
		if (95< nombre2){
			heure=randomUniform(56,60);
		}
	}
	return (heure + temp);
}
double TempDepart2(double temp){
	double nombre1;
	double nombre2;
	nombre1=randomUniform(0,100);
	if (nombre1<=50){
		nombre2=getRandom(temp,3/3);
		if (nombre2 > 0){
			nombre2=-nombre2;
		}
		return (temp + nombre2);
	}
	if (nombre1 > 50){
		nombre2=getRandom(temp,2/3);
		if (nombre2 < 0){
			nombre2=-nombre2;
		}
		return (temp + nombre2);
	}
}
/* Test de feasibility par simulation MonteCarlo*/
bool FeasibilityMonteCarlo(double epsilon,IloNumArray x,FlightVector flCandLev_list,int size,int period_length,int * ind,IntVector Contrainte_list,VectorDouble Pi){
	double ecart = 5.0;//écart type pour générer nouveau départ
	int count=0;//compteur pour calculer nombre de itérations qui ont conflicts
	int max_conflict = -1;
	int max_indice = -1;
	int iteration=1000;
	double coefProb=0.001;
	double** delai=new double *[size];
	for (int i=0;i<size;i++){
			delai[i]=new double[size];
		}
	
	*ind=-1;
	int *table=new int[size];
	for (int i=0;i<size;i++){
		table[i]=0;
	}
	for (int a=0;a< iteration;a++){// On fait 1000 itération
		for (int i=0;i<size;i++){
			for (int j=0;j<size;j++){
				delai[i][j]=0;
			}
		}
		bool test=true;
		FlightVector ListFlight;
		for (int j=0;j< size;j++){// Parcour la liste des vols
			if (x[j]==1&&!appartient(j,Contrainte_list)){// Pour les vols qui ont x=1 et qui n'appartient pas à contrainte list
				Flight * fj=flCandLev_list[j];
				Route *rj=fj->getActiveRoute();
				
				double dept=(double)fj->getDepartureTime();// prend le depart time
				//double newdept=getRandom(dept,ecart);// générer le nouveau depart time
				//double newdept=TempDepart(dept);//simulation méthode 1:donnée réel
				double newdept=TempDepart2(dept);//simulation méthode 2:loi normal
				Position depposition=fj->getDeparture()->getPosition();//la position depart
				const PointVector& pointlist = rj->getPointList();
				const Point *firstwp=pointlist[0];//le premier way point
				Position fwp=firstwp->getNode()->getPosition();
				Flight *f=new Flight(fj->getCode(),fj->getDeparture(),fj->getDestination(),1,int(newdept));
				f->setOrder(j);
				Route *r=new Route("route",f,1);
				double d1=distanceBetween(depposition,fwp);// la distance entre point départ et premier way point
				double fwpt=(double)firstwp->getTime();
				/*Calcul la vitesse,la vitesse varie entre -1% et 1% */
				double vitesse;
				if ((fwpt-dept)!=0){
					vitesse = d1/(fwpt - dept);
					//vitesse=randomUniform(vitesse*99/100,vitesse*101/100);
					vitesse=getRandom(vitesse,vitesse/300);
				}else{
					vitesse=0;
				}
				double deltat=fwpt-dept;
				int t0=(int)newdept;
				/*Parcourir la liste des point de 1 à n-1 pour ajouter nouveau point dans nouvelle route*/
				for (unsigned int k=0;k<pointlist.size()-1;k++){
					Point *p1=pointlist[k];
					Position n1=p1->getNode()->getPosition();
					Point *p2=pointlist[k+1];
					Position n2=p2->getNode()->getPosition();
					Point *p=new Point(p1->getNode(),t0+deltat,period_length,k);
					r->addPoint(p);
					/*new waypoint time and deltat*/
					t0=t0+deltat;
					d1=distanceBetween(n1,n2);
					if (vitesse!=0){
						deltat=d1/vitesse;
					}
				}//fin boucle
				/*Pour la dernière point du route*/
				Point *p1=pointlist[pointlist.size()-1];
				Point *p=new Point(p1->getNode(),t0+deltat,period_length,pointlist.size()-1);
				r->addPoint(p);
				//Associer le vol avec route
				f->setActiveRoute(r);
				//ajouter le vol dans la liste
				ListFlight.push_back(f);
			}//fin boucle x[i]=1
		}//fin boucle for pour parcourir tous les vols
		
		//Parcourir les nouveaux vols,trouver les nouveaux probability de conflict
		int sizeFlight=ListFlight.size();
		if (sizeFlight==0){
			continue;
		}
		for (unsigned int j=0 ; j < sizeFlight-1 ; j++){
			Flight *fj = ListFlight[j];
			Route *rj = fj->getActiveRoute();
			const PointVector pointlistj = rj->getPointList();
			
			for (unsigned int k=j+1 ; k<sizeFlight ; k++){
				bool test2=true;
				Flight *fk = ListFlight[k];
				Route *rk = fk->getActiveRoute();
				const PointVector pointlistk=rk->getPointList();
				for (unsigned int l=0;l<pointlistj.size();l++){
					Point *p1=pointlistj[l];
					for (unsigned int m=0;m<pointlistk.size();m++){
						Point *p2=pointlistk[m];
						if (p1->approx(p2)){// pass same point same period
								double prob=rj->probabConflit(p1,p2,rk);
								if (prob > coefProb){//si prob > % on déduit qu'il y conflict entre fj et fk
									int *indi=new int();
									double delay=rj->delaiSansConflit(p1,p2,rk,indi);
									if (*indi==1){
										delai[k][j]=delay;
										delai[j][k]=0;
									}else
									{
										delai[j][k]=delay;
										delai[k][j]=0;
									}
									test2=false;
									delete indi;
									break;
								}
						}//fin if
					}//fin for
					if (test2==false){
						break;
					}
				}//fin for
			}//fin boucle for fk
		}//fin boucle for fj
		//Tester la faisaibilité
		for (int i=0;i<size;i++){
			double sum=0;
			for (int j=0;j<size;j++){
				sum=sum +delai[i][j];
			}
			if (sum > Pi[i]){
				test = false;
				table[i]=table[i]+1;
				break;
			}
		}
		//Deallocation mémoire
		for (int j=0;j<ListFlight.size();j++){
			Flight *fj=ListFlight[j];
			Route *rj=fj->getActiveRoute();
			for (int k=rj->getPointList().size()-1;k>=0;k--){
				Point* p = rj->getPoint(k);
				delete p;
			}
			delete rj;
			delete fj;
		}
		if (test==false){//1 contraint est violée
			count ++;
		}
	}//fin boucle 1000 itération
	/*calculer l'indice pour ajouter prochainement à la liste de contraintes*/
	for (int i=0;i<size;i++){
		if (x[i]==1){
			if (table[i]> max_conflict&&!appartient(i,Contrainte_list)){
				max_conflict = table[i];
				max_indice = i;
			}
		}
	}
	*ind=max_indice;
	//cout <<"max conflict ="<<max_conflict<<endl;
	//cout << "max_indice = "<<max_indice<<endl;
	delete table;
	for (int i=0;i<size;i++){
				delete[] delai[i];
			}
	delete[] delai;
	//cout << "count = "<<count<<endl;
	if (epsilon==0.1){
		if (1000-count < 915){
			return false;
		}
	}
	if (epsilon==0.05){
		if (1000-count < 961)
		{
			return false;
		}
	}
	if (epsilon==0.15){
		if (1000-count <868){
			return false;
		}
	}
	if (epsilon==0.2){
		if (1000-count < 821){
			return false;
		}
	}
	if (epsilon==0.25){
		if (1000-count <772){
			return false;
		}
	}
	/*if ((count/iteration) > epsilon){
		return false;
	}*/
	return true;
}
/*Test de feasibilité par l'énumération de tous les événements qui peuvent se produisent*/
bool FeasibilityEnumeration(double epsilon,IloNumArray x,VectorDouble Pi,double Coef_Pij,double **probability,int size,int * ind,IntVector Contrainte_list){
	*ind=-1;
	double max_somme=-1;
	//double min_somme=MAXDOUBLE;
	for (int i=0;i<size;i++){//parcourir les vols dans cette niveau
		if (x[i]==1){//traiter seulement les vols qui ont x[i]=1
			vector<int> first;
			vector<vector<int> > result;
			int nombre=0;
			double somme=0;
			double temp_somme=0;
			for (int j=0;j<size;j++){
				if (probability[i][j]!=0){
					temp_somme=temp_somme + Coef_Pij*x[j];
					first.push_back(j);//ajouter ce vol dans la liste
					nombre++;//compter le nombre total des vols qui ont conflict avec vol i
				}
			}
			//Si la somme est supérieur à Pi
			if (temp_somme>Pi[i]){
				int k = (int)(Pi[i]/Coef_Pij);
				if (Pi[i]-k*Coef_Pij > 0){
					k=k+1;
				}
				int compteur=0;
				//les combinaisons dont on a besoin est la somme des combinaison de k à nombre
				for (int count=k;count<=nombre;count++){
					//cout << "aaa"<<endl;
					vector<int> last;
					//on ouvre le fichier pour écrire les combinaison count
					ofile1.open("outProb.txt",fstream::trunc);
					//compter le nombre de combinaison
					compteur=nombreCombinaison(nombre,count);
					//cout << "compteur="<<compteur<<endl;
					//afficher les combinaison dans le fichier
					combinaison(last,first,count);
					//Sleep(30000);
					//fermer le fichier
					if ( ofile1.is_open() )
							ofile1.close();
					//ouvrir ce fichier pour lire les données
					std::ifstream ifile ;
					ifile.open("outProb.txt");
					if (!ifile.is_open())
					{
						std::cout << "ERREUR: \tfichier n'existe pas\n" ;
						exit(0);
					}
					char line[LONGLINE];
					int compteur1=0;
					/*lire contenu fichier pour prendre vector of combinaison*/
					while (!ifile.eof()&&compteur1<compteur){
						vector<int> temp;
						ifile.getline(line, MAXCHAR);
						std::istringstream	istream(line) ;
						int compteur2=0;
						while (!istream.eof()&&compteur2<count){
							int tempIndice;
							{			 
								string tmp_s;
								istream >> tmp_s;
								std::istringstream	isstr_temp( tmp_s) ;
								isstr_temp >> tempIndice ;
								temp.push_back(tempIndice);
							}
							compteur2++;
						}
						result.push_back(temp);
						compteur1++;
					}
					//fermer le fichier
					if ( ifile.is_open() )
						ifile.close();
				}//fin de la boucle pour lire les combinaisons et les ajouter dans le vector result
				//parcourir le vector result pour calculer la somme des probabilités d'occurence
				for (unsigned int l=0;l<result.size();l++){
					double temp_prod=1;
					for (int j=0;j<size;j++){
						if(probability[i][j]!=0){
							if (appartient(j,result[l])){
								temp_prod=temp_prod*probability[i][j];
							//	cout << "temp_pro1="<<temp_prod<<endl;
							}
							else{
							temp_prod=temp_prod*(1-probability[i][j]);
							//cout << "temp_pro2="<<temp_prod<<endl;
							}
						//	cout << "temp_pro="<<temp_prod<<endl;
						}
					}
					somme=somme+temp_prod;
					//cout << somme<<endl;
				}//fin boucle for
				//cout <<"size " <<result.size()<<endl;
				//cout <<"somme= "<<somme<<endl;
				//si la somme de probabilité d'occurence > epsilon, la feasibilité n'est pas suffisant, retourne false
				if (somme > epsilon){
					//cout << "aa"<<endl;
					return false;
				}
				
			}//fin boucle if somme > pi
			if (somme!=0 && somme>max_somme&&!appartient(i,Contrainte_list)){
				max_somme=somme;
				*ind=i;
			}
		}//fin boucle x[i]=1
	}//fin boucle parcourir les vols dans cette niveau
	//cout << "i ="<<*ind<<endl;
	return true;
}

int clevel; 

int main(int argc, char * argv[]) {

	//std::cout << "Choose number of levels: (2 or 4):";
	//cin >> clevel;

	if (argc < 4)
	{
		std::cout << "Usage: mms config_filename period_length mode\n";
		std::cout << "periode_length is the time in minutes of a period .\n";
		std::cout << "mode:0--> test feasibility par Hoeffding\n";
		std::cout << "mode:1--> test feasibility par Enumeration\n";
		std::cout << "mode:2--> test feasibility par Monte Carlo\n";
		return 1;
	}
	// Create network instance
	Network net;

	// Initiate input instance
	Input input(&net);
	// get the configuration filename from the command line
	std::string config_filename(argv[1]);

	// get the period length from the command line
	int period_length;
	{
		std::istringstream stringStream(argv[2]);
		stringStream >> period_length;
	}
	int mode;
	{
		std::istringstream stringStream(argv[3]);
		stringStream >> mode;
	}
	std::cout << config_filename << ",	period=" << period_length << ",  mode =" << mode << std::endl;
	/*initialiser le réseau, lire les fichiers*/
	input.initNetwork(config_filename, period_length);
	//initialiser randomer
	srand(time(0));
	//initialiser les constants pour traiter le problème
	double Coef_Pi = 5;
	double Coef_Pij = 1;//0.7
	double epsilon = 0.05;
	double somme_objectif = 0;
	int maxConflict = 0;
	LevelExamine levelEx;
	ProcessClock pclock;
	pclock.start();
	IloEnv env;
	try {
		//Prende la liste des flights
		const FlightVector& flist = net.getFlightList();
		//Prendre la liste de level
		IntVector VectorLevels = net.getLevelList();
		//initialiser l'affectation les vols, au début, aucun vol est affecté
		FlightAfectationMap affectation;
		for (FlightVector::const_iterator itf = flist.begin(); itf != flist.end(); itf++) {
			Flight *fi = (*itf);
			affectation[fi] = false;
		}
		for (IntVector::iterator itA = VectorLevels.begin(); itA != VectorLevels.end(); itA++) {
			int lev = (*itA);
			levelEx[lev] = false;
		}
		//Parcourir la liste de level, traiter le problème robuste par level
		for (IntVector::iterator itA = VectorLevels.begin(); itA != VectorLevels.end(); itA++) {
			int lev = (*itA);
			FlightVector CandidateFlightList;
			FlightVector flCandLev_list;
			VectorDouble Mi, Pi;
			cout << "level  = " << lev << endl;
			// trouver les flights dans cette level,ajouter les vols qui a ce level comme level préféré ou
			// les vols qui n'est pas affecté à son niveau préféré qui est déjà traité
			for (FlightVector::const_iterator itf = flist.begin(); itf != flist.end(); itf++)
			{
				Flight* fi = (*itf);
				const LevelSet& levels = fi->getLevelSet();
				Level lprefere = fi->getFlightLevel();
				if (lprefere == lev) {
					CandidateFlightList.push_back(fi);
				}
				else {
					for (LevelSet::const_iterator il = levels.begin(); il != levels.end(); il++)
					{
						Level l = (*il);
						//ajouter dans la liste si le vol n'est pas encore affecté et il contient ce level dans sa liste de level
						if (l == lev && affectation[fi] == false && levelEx[lprefere] == true)
						{
							CandidateFlightList.push_back(fi);
							continue;
						}
					}
				}
			}
			int size = CandidateFlightList.size();
			//calcul les Mi, et trouver les candidate flights--> les vols qui ont conflict avec les autres sur ce level
			for (int i = 0; i < size; i++) {
				Flight* fi = CandidateFlightList[i];
				Route* ri = fi->getActiveRoute();
				const PointVector& pointlist1 = ri->getPointList();
				double SumConffi = 0.0;
				bool test2 = false;
				for (int j = 0; j < size; j++) {
					Flight * fj = CandidateFlightList[j];
					bool test = false;
					if (fi == fj) {
						continue;
					}
					Route* rj = fj->getActiveRoute();

					const PointVector& pointlist2 = rj->getPointList();
					for (unsigned int k = 0; k < pointlist1.size(); k++) {
						Point* p1 = pointlist1[k];
						for (unsigned int l = 0; l < pointlist2.size(); l++) {
							Point * p2 = pointlist2[l];
							//2 vols ont conflict quand il passe le meme point en meme period
							if (p1->approx(p2)) {
								double prob = ri->probabConflit(p1, p2, rj);
								if (prob > 0.001) {
									SumConffi = SumConffi + Coef_Pij;//calculer Mi
									test = true;
									test2 = true;
									break;
								}
							}
						}
						if (test) {// if the conflict is known we pass the loop
							break;
						}
					}

					if (test) {// if the conflict is known we pass the loop
						continue;
					}
				}
				if (test2) {
					flCandLev_list.push_back(fi);
					Mi.push_back(SumConffi);
				}
			}// end loop calcul Mi
			levelEx[lev] = true;
			/*S'il y a pas de conflicts, affecter à ce niveau les vols qui ont ce level comme level préferé*/
			//if (flCandLev_list.size()==0){
			//	cout << "no conflict" << endl;
			//	for (int i=0;i<size;i++){
			//		Flight *fi=CandidateFlightList[i];
			//		//if (fi->getFlightLevel()==lev){
			//			fi->setActualFlightLevel(lev);
			//			affectation[fi]=true;
			//		//}
			//	}
			//	continue;
			//}
			/*affecter à ce niveau les vols qui est dans la liste en train d'être traité
			et qui n'est pas dans la liste condidat*/
			for (int i = 0; i < size; i++) {
				Flight* fi = CandidateFlightList[i];
				bool test = false;
				for (unsigned j = 0; j < flCandLev_list.size(); j++) {
					Flight* fj = flCandLev_list[j];
					if (fi == fj) {//tester si flight est dans la liste flight actuel
						test = true;
						break;
					}
				}
				//if (test==false&&fi->getFlightLevel()==lev){
				if (test == false) {//si le vol n'est pas dans la liste de vols actuel
					if (fi->getFlightLevel() == lev) {
						somme_objectif = somme_objectif + 3;
					}
					else {
						somme_objectif = somme_objectif + 1;
					}
					fi->setActualFlightLevel(lev);
					affectation[fi] = true;
				}
			}
			size = flCandLev_list.size();
			/*initialiser la liste de probabilité de conflict des vols*/
			double** probability = new double *[size];
			double** delai = new double *[size];
			for (int i = 0; i < size; i++) {
				probability[i] = new double[size];
				delai[i] = new double[size];
			}
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					probability[i][j] = 0;
					delai[i][j] = 0;
				}
			}
			/* calcul probabilité pij*/
			for (int i = 0; i < size - 1; i++) {
				Flight* fi = flCandLev_list[i];
				Route* ri = fi->getActiveRoute();
				const PointVector& pointlist1 = ri->getPointList();
				for (int j = i + 1; j < size; j++) {
					Flight * fj = flCandLev_list[j];
					bool test = false;
					if (fi == fj) {
						continue;
					}
					Route* rj = fj->getActiveRoute();
					const PointVector& pointlist2 = rj->getPointList();
					for (unsigned int k = 0; k < pointlist1.size(); k++) {
						Point* p1 = pointlist1[k];
						for (unsigned int l = 0; l < pointlist2.size(); l++) {
							Point * p2 = pointlist2[l];
							if (p1->approx(p2)) {// pass same point same period
								double prob = ri->probabConflit(p1, p2, rj);
								if (prob > 0.001) {
									probability[i][j] = prob;
									probability[j][i] = prob;
									int * ind = new int();
									double delay = ri->delaiSansConflit(p1, p2, rj, ind);
									if (*ind == 1) {
										delai[j][i] = delay;
										delai[i][j] = 0;
									}
									else {
										delai[i][j] = delay;
										delai[j][i] = 0;
									}
									test = true;
									delete ind;
									break;
								}
							}
						}
						if (test) {// if the conflict is known we pass the loop
							break;
						}
					}
					if (test) {// if the conflict is known we pass the loop
						continue;
					}
				}
			}// end loop calcul probability between flight
			/*calcul Pi*/
			for (int i = 0; i < size; i++)
			{
				Flight* fi = flCandLev_list[i];
				Route* ri = fi->getActiveRoute();
				const PointVector& pointlist = ri->getPointList();
				const Time& tstart = fi->getDepartureTime();
				Point* plast = pointlist[pointlist.size() - 1];
				const Time& tDestination = plast->getTime();
				double hour = (tDestination - tstart) / 60.0;
				if (hour <= 1.0)
				{
					Pi.push_back(Coef_Pi);
				}
				else
				{
					Pi.push_back(Coef_Pi + (hour - 1.0)*0.1); // c=0.1
				}
			}// end loop calcul Pi
			if (flCandLev_list.size() == 0) {
				cout << "no conflict" << endl;
				continue;
			}
			// si tous les vols dans la liste de candidate n'a cette level comme le level préféré, on résoudre pas le problème à cet niveau
			bool test = false;
			for (int i = 0; i < size; i++) {
				Flight *fi = flCandLev_list[i];
				if (fi->getFlightLevel() == lev) {
					test = true;
				}
			}
			if (!test) {
				std::cout << " Tous les vols n'a pas cette niveau commme le niveau prefere" << std::endl;
				//continue;
			}
			for (int i = 0; i < size; i++) {
				int temp = 0;
				for (int j = 0; j < size; j++) {
					if (probability[i][j]>0) {
						temp++;
					}
				}
				if (temp > maxConflict) {
					maxConflict = temp;
				}
			}
			/*Chercher le premier indice pour ajouter dans la liste des contraintes*/
			//////int indice=strong_contraint0(size,probability,Coef_Pij,Pi);
			int indice = strong_contraint0(size, probability, delai, Pi);
			IntVector Contrainte_list;
			Contrainte_list.push_back(indice);
			IloModel model(env);
			IloNumVarArray x(env);
			//creation variable
			for (int i = 0; i < size; i++) {
				stringstream name; name << "x(" << i << ")";
				x.add(IloBoolVar(env, 0, 1, name.str().c_str()));
			}

			//objective
			IloExpr obj(env);
			for (int i = 0; i < size; i++) {
				Flight *fi = flCandLev_list[i];
				if (fi->getFlightLevel() == lev) {
					obj += 3 * x[i];// si level est son level plus préféré
				}
				else {
					obj += x[i];
				}
				//obj +=  x[i];
			}
			model.add(IloMaximize(env, obj));
			// contraintes
			for (unsigned int i = 0; i < Contrainte_list.size(); i++) {
				int k = Contrainte_list[i];
				IloExpr contraint(env);
				for (int j = 0; j < size; j++) {
					if (k == j) {
						contraint += Mi[j] * x[j];
					}
					else if (probability[k][j]>0) {
						//contraint+=Coef_Pij*x[j]*probability[k][j]; 
						contraint += Coef_Pij*x[j] * delai[k][j];
					}
				}
				model.add(contraint <= Mi[k] + Pi[k]);
			}
			IloCplex solver(model); // solver
			solver.setOut(env.getNullStream()); // no output
			solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
			solver.solve();
			solver.exportModel("model.lp");
			solver.writeSolution("sol.sl");
			IloNumArray vals(env);
			solver.getValues(vals, x);
			/*for (int i=0;i<size;i++){
				cout << "x["<<i<<"]="<<vals[i]<<endl;
			}*/
			double objectif_value = solver.getObjValue();
			model.end();
			solver.end();
			obj.end();

			int * ind = new int();
			/*Test de feasibilité*/
			switch (mode) {
			case 0:
				/*Hoeffding*/
				while (!FeasibilityHoeffding(epsilon, vals, Pi, Coef_Pij, delai, size, probability) && Contrainte_list.size() != size) {
					int indice;
					indice = strong_contraint(size, probability, Contrainte_list, delai, Pi, vals);
					Contrainte_list.push_back(indice);
					IloModel model(env);
					IloNumVarArray x(env);
					//creation variable
					for (int i = 0; i < size; i++) {
						stringstream name; name << "x(" << i << ")";
						x.add(IloBoolVar(env, 0, 1, name.str().c_str()));
					}

					//objective
					IloExpr obj(env);
					for (int i = 0; i < size; i++) {
						Flight *fi = flCandLev_list[i];
						if (fi->getFlightLevel() == lev) {
							obj += 3 * x[i];
						}
						else {
							obj += x[i];
						}
						//obj +=  x[i];
					}
					model.add(IloMaximize(env, obj));
					// contraintes
					for (unsigned int i = 0; i < Contrainte_list.size(); i++) {
						int k = Contrainte_list[i];
						IloExpr contraint(env);
						for (int j = 0; j < size; j++) {
							if (k == j) {
								contraint += Mi[j] * x[j];
							}
							else if (probability[k][j]>0) {
								//	contraint+=Coef_Pij*x[j]*probability[k][j]; 
								contraint += Coef_Pij*x[j] * delai[k][j];
							}
						}
						model.add(contraint <= Mi[k] + Pi[k]);
					}
					IloCplex solver(model); // solver
					solver.setOut(env.getNullStream()); // no output
					solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
					solver.solve();
					solver.exportModel("model.lp");
					solver.writeSolution("sol.sl");
					objectif_value = solver.getObjValue();
					//IloNumArray vals(env);
					solver.getValues(vals, x);
					model.end();
					solver.end();
					obj.end();
				}//end loup test de feasibilite
				break;
			case 1:/*Enumeration*/
				while (!FeasibilityEnumeration(epsilon, vals, Pi, Coef_Pij, probability, size, ind, Contrainte_list) && Contrainte_list.size() != size) {
					int indice;
					//Si on ne trouve pas une bonne indice dans la test, on utilise la formule
					if (*ind == -1) {
						indice = strong_contraint(size, probability, Contrainte_list, delai, Pi, vals);
					}
					else {
						indice = *ind;
					}
					Contrainte_list.push_back(indice);
					IloModel model(env);
					IloNumVarArray x(env);
					//creation variable
					for (int i = 0; i < size; i++) {
						stringstream name; name << "x(" << i << ")";
						x.add(IloBoolVar(env, 0, 1, name.str().c_str()));
					}

					//objective
					IloExpr obj(env);
					for (int i = 0; i < size; i++) {
						Flight *fi = flCandLev_list[i];
						if (fi->getFlightLevel() == lev) {
							obj += 3 * x[i];
						}
						else {
							obj += x[i];
						}
						//obj +=  x[i];
					}
					model.add(IloMaximize(env, obj));
					// contraintes
					for (unsigned int i = 0; i < Contrainte_list.size(); i++) {
						int k = Contrainte_list[i];
						IloExpr contraint(env);
						for (int j = 0; j < size; j++) {
							if (k == j) {
								contraint += Mi[j] * x[j];
							}
							else if (probability[k][j]>0) {
								contraint += Coef_Pij*x[j] * probability[k][j];
							}
						}
						model.add(contraint <= Mi[k] + Pi[k]);
					}
					IloCplex solver(model); // solver
					solver.setOut(env.getNullStream()); // no output
					solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
					solver.solve();
					solver.exportModel("model.lp");
					solver.writeSolution("sol.sl");
					objectif_value = solver.getObjValue();
					//IloNumArray vals(env);
					solver.getValues(vals, x);
					model.end();
					solver.end();
					obj.end();
				}//end loup test de feasibilite par enumeration
				break;
			case 2:/*MonteCarlo*/
				while (!FeasibilityMonteCarlo(epsilon, vals, flCandLev_list, size, period_length, ind, Contrainte_list, Pi) && Contrainte_list.size() != size) {
					int indice;
					//Si on ne trouve pas une bonne indice dans la test, on utilise la formule
					if (*ind == -1) {
						indice = strong_contraint(size, probability, Contrainte_list, delai, Pi, vals);
					}
					else {
						indice = *ind;
					}
					Contrainte_list.push_back(indice);
					IloModel model(env);
					IloNumVarArray x(env);
					//creation variable
					for (int i = 0; i < size; i++) {
						stringstream name; name << "x(" << i << ")";
						x.add(IloBoolVar(env, 0, 1, name.str().c_str()));
					}

					//objective
					IloExpr obj(env);
					for (int i = 0; i < size; i++) {
						Flight *fi = flCandLev_list[i];
						if (fi->getFlightLevel() == lev) {
							obj += 3 * x[i];
						}
						else {
							obj += x[i];
						}
						//obj +=  x[i];
					}
					model.add(IloMaximize(env, obj));
					// contraintes
					for (unsigned int i = 0; i < Contrainte_list.size(); i++) {
						int k = Contrainte_list[i];
						IloExpr contraint(env);
						for (int j = 0; j < size; j++) {
							if (k == j) {
								contraint += Mi[j] * x[j];
							}
							else if (probability[k][j]>0) {
								//contraint+=Coef_Pij*x[j]*probability[k][j];
								contraint += Coef_Pij*x[j] * delai[k][j];
							}
						}
						model.add(contraint <= Mi[k] + Pi[k]);
					}
					IloCplex solver(model); // solver
					solver.setOut(env.getNullStream()); // no output
					solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
					solver.solve();
					solver.exportModel("model.lp");
					solver.writeSolution("sol.sl");
					objectif_value = solver.getObjValue();
					//IloNumArray vals(env);
					solver.getValues(vals, x);
					model.end();
					solver.end();
					obj.end();
				}//end loup test de feasibilite par enumeration
				break;
			case 3:
				for (int i = 0; i < size; i++) {
					if (!appartient(i, Contrainte_list)) {
						Contrainte_list.push_back(i);
					}
				}
				IloModel model(env);
				IloNumVarArray x(env);
				//creation variable
				for (int i = 0; i < size; i++) {
					stringstream name; name << "x(" << i << ")";
					x.add(IloBoolVar(env, 0, 1, name.str().c_str()));
				}

				//objective
				IloExpr obj(env);
				for (int i = 0; i < size; i++) {
					Flight *fi = flCandLev_list[i];
					if (fi->getFlightLevel() == lev) {
						obj += 3 * x[i];
					}
					else {
						obj += x[i];
					}
					//obj +=  x[i];
				}
				model.add(IloMaximize(env, obj));
				// contraintes
				for (unsigned int i = 0; i < Contrainte_list.size(); i++) {
					int k = Contrainte_list[i];
					IloExpr contraint(env);
					for (int j = 0; j < size; j++) {
						if (k == j) {
							contraint += Mi[j] * x[j];
						}
						else if (probability[k][j]>0) {
							//contraint+=Coef_Pij*x[j]*probability[k][j]; 
							contraint += Coef_Pij*x[j] * delai[k][j];
						}
					}
					model.add(contraint <= Mi[k] + Pi[k]);
				}
				IloCplex solver(model); // solver
				solver.setOut(env.getNullStream()); // no output
				solver.setParam(IloCplex::Threads, 2); // no multi-threading (less performance)
				solver.solve();
				solver.exportModel("model.lp");
				solver.writeSolution("sol.sl");
				objectif_value = solver.getObjValue();
				//IloNumArray vals(env);
				solver.getValues(vals, x);
				model.end();
				solver.end();
				obj.end();
				break;
			}
			delete ind;
			if (Contrainte_list.size() == size) {
				cout << "No solution feasible for this level" << endl;
			}

			for (int i = 0; i < size; i++) {
				//cout << "x["<<i<<"]="<<vals[i]<<endl;
				if (vals[i] == 1) {
					Flight * fi = flCandLev_list[i];
					fi->setActualFlightLevel(lev);
					affectation[fi] = true;
				}
			}

			somme_objectif = somme_objectif + objectif_value;
			for (int i = 0; i < size; i++) {
				delete[] probability[i];
				delete[] delai[i];
			}
			delete[] probability;
			delete[] delai;
		}// end loop of level list
		int compteur = 0;
		int compteur2 = 0;
		for (FlightVector::const_iterator itf = flist.begin(); itf != flist.end(); itf++) {
			Flight *fi = (*itf);
			if (fi->getActualFlightLevel() != fi->getFlightLevel()) {
				compteur++;
			}
			if (affectation[fi] == false) {
				compteur2++;
			}
		}
		cout << "Nombre de flight change level:" << compteur << endl;
		cout << "Nombre de flight non affecte:" << compteur2 << endl;
		cout << "La somme des valeurs d'objectifs:" << somme_objectif << endl;
		cout << "Max conflict:" << maxConflict << endl;
	}
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
	catch (...) {
		cerr << "Error: " << endl;
	}
	env.end();
	pclock.end();
	cout << "elapsed time = " << pclock.getCpuTime() << endl;
	system("pause");
	return 0;
}
