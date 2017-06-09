#include "../include/Route.h"
#include "../include/Airport.h"
#include "../include/Node.h"
#include "../include/Flight.h"
Route::Route() : m_code(""), m_flight(NULL), m_level(0){}

Route::~Route(void)
{
	/*for (int i=0;i<m_point_list.size();i++){
		Point *p=m_point_list[i];
		p->~Point();
		
	}*/
}
Route::Route(String code, Flight *f, int level) : m_code(code), m_flight(f), m_level(level)
{
}
const String& Route::getCode() const
{
	return m_code ;
}

Flight* Route::getFlight()
{
	return m_flight;
}
const PointVector& Route::getPointList(void) const
{
	return m_point_list;
}

const int Route::getLevel(void) const
{
	return m_level;
}
Point*	Route::getPoint(unsigned int index)
{
	if (index >= 0 && index < m_point_list.size()) // valide range
		return m_point_list[index];
	else
		return NULL;
}
double Route::probabConflit(Point *p1,Point *p2, Route * r2){
	double prob;
	Point *p3,*p4;
	if (p1->approx(p2)){
		//The conflict point of r1 et r2
		Position n1=p1->getNode()->getPosition();
		Time t1=p1->getTime();
		Time t2=p2->getTime();
		//The previous point of r1 et r2
		Position n3,n4;
		Time t3,t4;
		
		if (p1->getPosInRoute()==0)// conflict point at first point of r1
		{
			n3 = this->getFlight()->getDeparture()->getPosition(); 
			t3 = this->getFlight()->getDepartureTime();
		}
		else{
			p3=m_point_list[p1->getPosInRoute()-1];
			n3=p3->getNode()->getPosition();
			t3=p3->getTime();
		}
		if (p2->getPosInRoute()==0)// conflict point at first point of r2
		{
			n4=r2->getFlight()->getDeparture()->getPosition();
			t4=r2->getFlight()->getDepartureTime();
		}
		else{
			p4=r2->getPoint(p2->getPosInRoute()-1);
			n4=p4->getNode()->getPosition();
			t4=p4->getTime();
		}
		const double k = 1.8520108;              // constant : 1(nmi)=k*1(km)
		const double s = 5.0;                    //minimum allowed horizontal separation is 5 (nmi)
		const double a0 = distanceBetween(n3,n4);      // distance between two precedent nodes in routes r1,r2 of conflict note
		const double b0 = distanceBetween(n3,n1);      // distance of precedent node in route r1 from the conflict node
		const double c0 = distanceBetween(n4,n1);      // distance of precedent node in route r2 from the conflict node
		double d1,d2;// distance at moment t of 2 aircraft from the conflict point
		const double v1=fabs(b0/(t3-t1));// velocity of aircraft on route r1
		const double v2=fabs(c0/(t4-t2));// velocity of aircraft on route r2
		const double p=v2/v1;

		const double cosa=(b0*b0 + c0*c0 - a0*a0)/(2*b0*c0); // cos of angle between  two routes
		const double sina=sqrt(1-cosa*cosa); // sin of angle between two routes
		if (t1 > t2){
			d1=b0;
			d2=c0-v2*(t1-t2);
		}else{
			d2=c0;
			d1=b0-v1*(t2-t1);
		}
		if ((p*p - 2*p*cosa + 1) < EPSILON )
		{
			//std::cout << "Probab is p =1.0" << std::endl;
			/*const double muy=(d2-p*d1)/k;
			if (fabs(muy) > s){
				return 0;
			}*/
			//return 1 - fabs(muy)/s;
			return 1;
		}
		const double lamda = sina/sqrt(p*p - 2*p*cosa + 1);
		if (sina < EPSILON )
		{
			//std::cout << "Probab is p =1.0" << std::endl;
			const double muy=(d2-p*d1)/k;
			if (fabs(muy) > s){
				return 0;
			}
			//return 1 - fabs(muy)/s;
			return 1;
		}
		const double muy=lamda*(d2-p*d1)/k;// en nmi
		const double sigma = 0.5*lamda*sqrt(10*(1 + p*p));// 10??? 
		if (sigma < EPSILON )
		{
			if (fabs(muy) > s){
				return 0;
			}
			return 1;
			//return 1 - fabs(muy)/s;
			//std::cout << "Probab is p =1.0" << std::endl;
		}
		const double f2=(s-muy)/(sqrt(2.0)*sigma);
		const double f1=(-s-muy)/(sqrt(2.0)*sigma);
		const double PIx = 3.141592653589793; 
		prob = (erf(f2)-erf(f1))/sqrt(PIx);
	}
	if (prob > 1){
		return 1.0;
	}
	return fabs(prob);
}
double Route::delaiSansConflit(Point *p1, Point *p2, Route * r2,int * ind)
{
	double delai=0;
	Point *p3,*p4;
	if (p1->approx(p2)){
		//The conflict point of r1 et r2
		Position n1=p1->getNode()->getPosition();
		Time t1=p1->getTime();
		Time t2=p2->getTime();
		//The previous point of r1 et r2
		Position n3,n4;
		Time t3,t4;
		
		if (p1->getPosInRoute()==0)// conflict point at first point of r1
		{
			n3 = this->getFlight()->getDeparture()->getPosition(); 
			t3 = this->getFlight()->getDepartureTime();
		}
		else{
			p3=m_point_list[p1->getPosInRoute()-1];
			n3=p3->getNode()->getPosition();
			t3=p3->getTime();
		}
		if (p2->getPosInRoute()==0)// conflict point at first point of r2
		{
			n4=r2->getFlight()->getDeparture()->getPosition();
			t4=r2->getFlight()->getDepartureTime();
		}
		else{
			p4=r2->getPoint(p2->getPosInRoute()-1);
			n4=p4->getNode()->getPosition();
			t4=p4->getTime();
		}
		const double k = 1.8520108;              // constant : 1(nmi)=k*1(km)
		const double s = 5.0;                    //minimum allowed horizontal separation is 5 (nmi)
		const double a0 = distanceBetween(n3,n4);      // distance between two precedent nodes in routes r1,r2 of conflict note
		const double b0 = distanceBetween(n3,n1);      // distance of precedent node in route r1 from the conflict node
		const double c0 = distanceBetween(n4,n1);      // distance of precedent node in route r2 from the conflict node
		double d1,d2;// distance at moment t of 2 aircraft from the conflict point
		const double v1=fabs(b0/(t3-t1));// velocity of aircraft on route r1
		const double v2=fabs(c0/(t4-t2));// velocity of aircraft on route r2
		const double p=v2/v1;

		const double cosa=(b0*b0 + c0*c0 - a0*a0)/(2*b0*c0); // cos of angle between  two routes
		const double sina=sqrt(1-cosa*cosa); // sin of angle between two routes
		if (t1 > t2){
			d1=b0;
			d2=c0-v2*(t1-t2);
			*ind=1;
		}else{
			d2=c0;
			d1=b0-v1*(t2-t1);
			*ind=2;
		}
		if ((p*p - 2*p*cosa + 1) < EPSILON )
		{
			return 1;
		}
		const double lamda = sina/sqrt(p*p - 2*p*cosa + 1);
		if (sina < EPSILON )
		{
			if (t1 > t2){
				return (s - d1)/v1;
			} else{
				return (s - d2)/v2;
			}
		}
		if (t1 > t2){
			delai = (s*k/lamda + c0 -p*d1)/v2 +t2 - t1;
		}else{
			delai = (s*k/lamda +p*b0 - c0)/v2 +t1 -t2;
		}
	}
	return delai;
}
double Route::erf(double X){
	/*C'est une méthode de Newton-Cotes : On subdivise l'intervalle d'intégration [a ;b] 
	en n sous intervalles de longueur égale h.
	h=(b-a)/n
	Sur chaque sous intervalle [xi;xi+1] on remplace f par son polynôme d'interpolation de degré 1 (sa corde) passant par les points (xi,f(xi)) et 
(xi+1, f(xi+1)).
	Algorithme : Intégrale f(x)dx entre [a,b] = h[ (f(a)+f(b))/2 + SOMME de i=1 à n-1 de f(a+i*h)]
	*/
	int i,n;
	double s,t,y;
	double a,b,h;
	a=0;
	b=fabs(X);
	n=ITMAX;

	h=(b-a)/n;
	s=fonctionF(a)+fonctionF(b);

	for(i=1;i<=n-1;i++){
		y=a+i*h;
		s=s+2*fonctionF(y);
	}
	t=s*h/2;
	if (X > 0){
	 return t;
	}else{
	 return -t;
	}
}
double Route::fonctionF(double X){
     return exp(-X*X);
}
void Route::addPoint( Point* point)
{
	m_point_list.push_back(point);
}

