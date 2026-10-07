#include "fisher.hpp"
#include <cmath>
#include <random>

#define SLG std::lgamma

std::random_device rd;
int rn(int a, int b){
	std::uniform_int_distribution<int> dist(a,b);
	std::mt19937 mt(rd()); return dist(mt);
}

double min(double a, double b){
	return (a<b)? a:b;
}

#include <iostream>
double fFET(double a, double b, double c, double d){
	int start=-min(a,d), end=min(b,c), period=end-start; double sum=0,
	initial=SLG(a+b+1)+SLG(a+c+1)+SLG(d+c+1)+SLG(b+d+1)-SLG(a+b+c+d+1),
	observed=std::exp(initial-(SLG(a+1)+SLG(b+1)+SLG(c+1)+SLG(d+1)));
	int I; for(I=start; I<end; I++){
		double K=std::exp(initial-(SLG(a+I+1)+SLG(b-I+1)+SLG(c-I+1)+SLG(d+I+1)));
		if(K>observed) break; sum+=K;
	}for(int i=end; i>=I; i--){
		double K=std::exp(initial-(SLG(a+i+1)+SLG(b-i+1)+SLG(c-i+1)+SLG(d+i+1)));
		if(K>observed) break; sum+=K;
	}return sum;
}

double coeff(double& a, double& b, double& c, double& d){
	int start=-min(a,d), end=min(b,c), period=end-start; double eff=10000.0/period;
	a=(int)(a*eff);b=(int)(b*eff);c=(int)(c*eff);d=(int)(d*eff); return eff;
}

double afFET(double a, double b, double c, double d){ coeff(a,b,c,d); return fFET(a,b,c,d); }

bool FET(double a, double b, double c, double d){ return fFET(a,b,c,d)>0.05; }
bool aFET(double a, double b, double c, double d){ return afFET(a,b,c,d)>0.05; }

#ifdef TESTING
#include <iostream>
int main(){
	int count=0, counta=0;
	double a=rn(0,500000),b=rn(0,500000),c=rn(0,500000),d=rn(0,500000);
	for(unsigned int i=0; i<1000; i++,a=rn(0,500000),b=rn(0,500000),c=rn(0,500000),d=rn(0,500000)){
		count+=FET(a,b,c,d); counta+=aFET(a,b,c,d); std::cout<<i<<"\n";
	}std::cout<<"without: "<<count<<"\nwith: "<<counta<<"\n";
}
#endif
