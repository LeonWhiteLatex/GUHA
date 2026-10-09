#include "fisher.hpp"
#include "guha.hpp"
#include <random>
#include <iostream>

float all=0.2, character=3, depends=1.5;

std::vector<float> norm(std::vector<float> v){
	float sum=0.0;
	for(auto i:v) sum+=i;
	for(auto &i:v) i/=sum;
	return v;
}

float FIMPLE(float a, float b){
	//if(a<all) return 0.0;
	return a/(a+b);
}

std::vector<std::string> rules={}, names={};
std::vector<int> nodes={
	8*0+4*0+2*1+1*1,
	8*0+4*0+2*1+1*1,
	8*1+4*0+2*0+1*1,
	8*1+4*1+2*0+1*0,
	8*0+4*1+2*0+1*1,
	8*1+4*0+2*0+1*1,
	8*0+4*1+2*1+1*0,
	8*0+4*1+2*1+1*1,
	8*1+4*0+2*1+1*0,
	8*1+4*1+2*1+1*0,
	8*0+4*1+2*1+1*1,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*0+1*0,
	8*0+4*1+2*0+1*0,
	8*0+4*1+2*1+1*0,
	8*0+4*0+2*0+1*0,
	8*0+4*1+2*0+1*0,
	8*0+4*0+2*1+1*0,
	8*1+4*1+2*1+1*0,
	8*0+4*0+2*0+1*1,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*1+1*0,
	8*1+4*1+2*1+1*0,
	8*1+4*1+2*1+1*0,
	8*1+4*1+2*0+1*0,
	8*1+4*1+2*1+1*1,
	8*0+4*0+2*0+1*1,
}, msk={1, 2, 4, 8};

struct noded{
	int a, b, lvl;
	std::vector<float> rule={0.0,0.0,0.0,0.0};
	std::string formatted="", name1="", name2="";
	
	noded(int a, int b, int lvl):a(a), b(b), lvl(lvl){
		for(unsigned int l=0; l<names.size(); l++)
			name1+=(a&msk[l]&&1)? names[l]:"";
		for(unsigned int l=0; l<names.size(); l++)
			name2+=(b&msk[l]&&1)? names[l]:"";
	}
	explicit operator std::vector<float>() const{ return norm(rule); } 
	explicit operator std::vector<int>() const{ return std::vector<int>{(int)rule[0],(int)rule[1],(int)rule[2],(int)rule[3]}; } 
	explicit operator int() const{ return a|b; }
	explicit operator bool() const{ return cFET(rule[3],rule[2],rule[1],rule[0]); }
	explicit operator std::string() const{return formatted;}
	explicit operator std::vector<std::string>() const{ return std::vector<std::string>{"11",name2,"00",name1};}
	void operator+(int o){ bool phi=((a&o)==a), psi=((b&o)==b); rule[phi|(psi<<1)]++; }
	void operator()(){
		if(!((rule[3]+rule[1]) && (rule[3]+rule[2]))) return;
		std::vector<float> nn=norm(rule); //if(nn[3]<all) return;
		
		bool fat=cFET(rule[3],rule[2],rule[1],rule[0]);
		float AB=FIMPLE(nn[3],nn[2]), AC=FIMPLE(nn[3],nn[1]),
		DB=FIMPLE(nn[2],nn[0]), DC=FIMPLE(nn[1],nn[0]),
		AD=(AB*AC)/(DB*DC),
		T=(AD>=1)? AB/AC:DC/DB;
		std::string color=(fat)? "black":"red";
		if(AD>character){
			if(T>depends) formatted+="{edge [color="+color+"] "+name2+"->"+name1+"\n";
			else if(T<1.0/depends) formatted+="{edge [color="+color+"] "+name1+"->"+name2+"\n";
			else formatted+="{edge [dir=none, color="+color+"] "+name1+"->"+name2+"}\n";
		}else{
			if(T>depends) formatted+="{edge [style=dashed, color="+color+"] "+name1+"->"+name2+"}\n";
			else if(T<1.0/depends) formatted+="{edge [style=dashed, color="+color+"] "+name2+"->"+name1+"}\n";
			else formatted+="{edge [style=dashed, dir=none, color="+color+"] "+name1+"->"+name2+"}\n";
		}/*else{//отношение показывает что связь слабая и скорее А и Б случайно разбросаны
			formatted=""; //тут все обнуляется
			rule=std::vector<float>{0.0,0.0,0.0,0.0};
		}*/
	}
};

std::vector<noded> assoc;

std::random_device rd;
int random_num(int a, int b){
	std::uniform_int_distribution<int> dist(a,b);
	std::mt19937 mt(rd()); return dist(mt);
}
void mask_gen(std::vector<std::string> n){
	msk.clear();
	for(unsigned int i=0; i<n.size(); i++) msk.push_back(std::pow(2,i));
	names=n;
}

void rnd(std::vector<std::string> a, int o){
	mask_gen(a); nodes.clear(); assoc.clear();
	for(unsigned int i=0; i<o; i++) nodes.push_back(random_num(1,std::pow(2,a.size())-1));
}

bool run_assoc(int a, int b, int lvl){
	if(a&b) return 0; if(a>b){ int t=a; a=b; b=t; }
	bool flag1=0, flag2=0, lol1=0, lol2=0;
	for(unsigned int i=0; i<assoc.size(); i++)
		if((int)assoc[i].a==a && (int)assoc[i].b==b) return 0;
	else if(((int)assoc[i])==a){ flag1|=(bool)assoc[i]; lol1=1; }
	else if(((int)assoc[i])==b){ flag2|=(bool)assoc[i]; lol2=1; }
	if(lol1 && !flag1 || lol2 && !flag2) return 0;
	noded tt(a,b, lvl); for(auto i:nodes) tt+i;
	assoc.push_back(tt); return (bool)tt;
}

void guha(unsigned int a){ assoc.clear();
	std::vector<int> collect=msk, temp, mask=msk;
	unsigned int allowed=((a>msk.size()-1)||!a)? msk.size()-1:a; 
	for(unsigned int i=0; i<allowed; i++){
		for(auto j:collect) for(auto k:mask)
			if(run_assoc(j, k, i) && (j|k!=std::pow(2, msk.size()))) 
				temp.push_back(j|k);
		collect=temp;
		mask.insert(mask.end(), temp.begin(), temp.end());
		temp.clear();
	}for(auto i:assoc) { i(); rules.push_back((std::string)i); }
}

std::vector<int> get_node(int t){
	if(t<0) return std::vector<int>{static_cast<int>(t)};
	if(t>=assoc.size()) return std::vector<int>{static_cast<int>(t-assoc.size()+1)};
	return std::vector<int>(assoc[t]);
}

std::vector<std::string> get_names(long unsigned int t){ 
	if(t>=assoc.size()) return std::vector<std::string>{"","","",""};
	return std::vector<std::string>(assoc[t]);
}

