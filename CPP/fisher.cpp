#include "fisher.hpp"
#include <cmath>

#define SLG std::lgamma

int min(int a, int b){ return (a<b)? a:b; }

int mean(int a, int b, int c){ return (a+b+c)/3.0; }

#include <iostream>
double fFET(int a, int b, int c, int d){
	int start=-min(a,d), end=min(b,c), period=end-start; double sum=0,
	initial=SLG(a+b+1)+SLG(a+c+1)+SLG(d+c+1)+SLG(b+d+1)-SLG(a+b+c+d+1),
	observed=std::exp(initial-(SLG(a+1)+SLG(b+1)+SLG(c+1)+SLG(d+1)));
	int I; for(I=start; I<end; I++){
		double K=std::exp(initial-(SLG(a+I+1)+SLG(b-I+1)+SLG(c-I+1)+SLG(d+I+1)));
		if(K>observed) break; sum+=K;
	}for(int i=end; i>=start; i--){
		double K=std::exp(initial-(SLG(a+i+1)+SLG(b-i+1)+SLG(c-i+1)+SLG(d+i+1)));
		if(K>observed) break; sum+=K;
	}return sum;
}

double coeff(int& a, int& b, int& c, int& d){
	int start=-min(a,d), end=min(b,c), period=end-start; double eff=1000.0/((float)period);
	a=(int)(a*eff);b=(int)(b*eff);c=(int)(c*eff);d=(int)(d*eff); return eff;
}

double afFET(int a, int b, int c, int d){ coeff(a,b,c,d); return fFET(a,b,c,d); }

bool FET(int a, int b, int c, int d){ return fFET(a,b,c,d)<0.0005; }
bool aFET(int a, int b, int c, int d){ return afFET(a,b,c,d)<0.0005; }

bool cFET(int a, int b, int c, int d){
	int meaner; if(!aFET(a,b,c,d)) return 0;
	meaner=mean(a,b,c); if(!aFET(a,b,c,meaner)) return 0;
	meaner=mean(a,b,d); if(!aFET(a,b,meaner,d)) return 0;
	meaner=mean(a,c,d); if(!aFET(a,meaner,c,d)) return 0;
	meaner=mean(b,c,d); if(!aFET(meaner,b,c,d)) return 0;
	return 1;
}

#ifdef TESTING
#include <iostream>
#include <random>

void putout(double a, double b, double c, double d){
	double meaner=(a+b+c+d);
	std::cout<<a<<" "<<b<<" "<<c<<" "<<d<<"\n";
}

std::random_device rd;
int rn(int a, int b){
	std::uniform_int_distribution<int> dist(a,b);
	std::mt19937 mt(rd()); return dist(mt);
}

int main(){
	int county=0, p1=0,p2=0,pc1=0, pc2=0;
	double a=rn(400,50000),b=rn(400,50000),c=rn(400,50000),d=rn(400,50000);
	for(unsigned int i=0; i<10000; i++,a=rn(400,50000),b=rn(400,50000),c=rn(400,50000),d=rn(400,50000)){
		county+=((p1=aFET(a,b,c,d))==(p2=cFET(a,b,c,d))); 
		pc1+=p1; pc2+=p2;
		if(p1) { putout(a,b,c,d); std::cout<<"FET: "<<p1<<"; context FET: "<<p2<<"\n";
					std::cout<<afFET(a,b,c,d)<<"\n\n"; }
	}
	std::cout<<"precision: "<<1.0*county/100.0<<"%\n";
	std::cout<<"FET: "<<1.0*pc1/100.0<<"%; context FET:"<<1.0*pc2/100.0<<"%\n";
}
#endif
