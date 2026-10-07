#include "apriori.hpp"
#include "guha.hpp"
#include <string>
#include <iostream>

class counter{
	counter *A=0, *B=0;
	std::string mask="", result="";
	int count=0, tmask; bool active=0;
public:
	counter(int tmask, std::string name):tmask(tmask), mask(name){}
	counter(counter* A, counter* B):A(A), B(B){
		active=1;
		mask=A->mask+B->mask;
		tmask=A->tmask|B->tmask;
	}
	void operator+(int t){ if((t&tmask)==tmask) count++; }
	
	void operator()(int _){
		if(active&&A&&B&&eee()>all){ 
			float a=A->eee(),b=B->eee(),c=eee();
			if(!(a&&b)) return;
			
			float confA=c/a, confB=c/b,
				convA=(1.0f-b)/(1.0f-confA), convB=(1.0f-a)/(1.0f-confB);
			std::string pA=A->mask, pB=B->mask;
			result="for "+mask+" with supp "+std::to_string(c)+":\n"+
					"supp "+pA+": "+std::to_string(a)+
					" supp "+pB+": "+std::to_string(b)+
					"\nconf "+pA+": "+std::to_string(confA)+
					"; conf "+pB+": "+std::to_string(confB)+
					";\nconv "+pA+": "+std::to_string(convA)+
					"; conv "+pB+": "+std::to_string(convB)+
					"\nlift "+mask+": "+std::to_string(c/(a*b))+
					"\nleverage "+mask+": "+std::to_string(c-a*b)+
					"\n";
		}
	}
	
	float eee(){
		return 1.0*count/nodes.size();
	}
	
	explicit operator std::string() const{
		return result;
	}
	
	bool online(){return active;}
	int get_mask(){ return tmask; }
	bool check(counter *a, counter *b){if(A->tmask==a->tmask && B->tmask==b->tmask) 
	return 1; return 0;}
};
//below needs reworking
counter* run_assoc(std::vector<counter*> &mir, counter *a, counter *b){
	if(a->get_mask()&b->get_mask()) return nullptr;
	if(a->get_mask()>b->get_mask()) {counter *t=a;a=b;b=t;}
	for(auto i:mir) if(i->check(a,b)) return nullptr;
	//if(a->eee()<all&&b->eee()<all) return counter(0,"");
	counter* N=new counter(a,b);
	for(auto i:nodes) (*N)+i;
	return N;
}
#include <cmath>
void apriori(unsigned int a){
	std::vector<counter*> cct;
	for(unsigned int i=0; i<names.size(); i++) cct.push_back(new counter(msk[i],names[i]));
	for(auto &j:cct) for(auto i:nodes) (*j)+i;
	std::vector<counter*> temp, //будущие маски
	mask=cct; //прошлые маски
	unsigned int allowed=((a>msk.size()-1)||!a)? msk.size()-1:a; 
	for(unsigned int i=0; i<allowed; i++){ //цикл в котором происходят доформирования
		for(unsigned int j=0; j<cct.size(); j++){ if(cct[j]->eee()<all) break;
			for(unsigned int k=0; k<mask.size(); k++){ if(mask[k]->eee()<all) break;
				counter *t=run_assoc(temp,cct[j],mask[k]); if(!t) continue;
				if(t->online() && (cct[j]->get_mask()|mask[k]->get_mask()!=std::pow(2, msk.size())))
					temp.push_back(t);
			}
		}
		cct=temp;//перенос будущих масок в текущие
		mask.insert(mask.end(), temp.begin(), temp.end());//запись будущих масок в прошлые
		temp.clear();//очищение будущих масок
		//допустим сначала: A B C и D => в collect A,B,C,D; mask A,B,C,D; temp AB,AC,AD,BC,BD,DC;;
		//вторая итерация: A B C D AB AC AD BC BD CD => 
		//в collect AB,AC,AD,BC,BD,DC; mask A,B,C,D,AB,AC,AD,BC,BD,CD; 
		//temp ABC,ABD,ABCD,ACB,ACD,ACDB,ADB,ADC,ADBC,BCA,BCD,BCAD,BDA,BDC,BDAC,DCA,DCB,DCAB;
		//итд...
	}//цикл поиска правил
	for(auto i:mask) {
		(*i)(nodes.size()); //формирование письменных правил
		rules.push_back((std::string)(*i)); //хранение
	}//цикл описания правил
}//функция формирует правила по данным, которые произвелись
