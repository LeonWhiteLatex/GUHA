#include <iostream>
#include <iomanip>
#include "guha.hpp"
#include "apriori.hpp"
#include "FP_growth.hpp"
#include "graphics.hpp"
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> chrono_clock;
#define NOW std::chrono::system_clock::now()

int main(){
	all=0.2; character=2;
	//rnd(std::vector<std::string>{"A", "B", "C", "D", "E"}, 100);
	//Связи формируются случайно
	//A------>B там где есть А должно быть B (зависимость)
	//A-------B А и B часто вместе (ассоциация)
	//A - - ->B там где нету А должно быть B (конкуренция)
	//A - - - B А и B часто порозень (избегание)
	mask_gen(std::vector<std::string>{"A", "B", "C", "D"});
	for(auto i:names) std::cout<<i<<" "; std::cout<<"\n"; //вывод названий атрибутов
	for(auto i:nodes){
		for(unsigned int l=0; l<msk.size(); l++)
			std::cout<<(i&msk[l]&&1)<<std::setw(names[l].size()+1); //вывод объектов (бинарный)
		std::cout<<"\n";
	}std::cout<<"\n";
	
	chrono_clock start, end;
	
	start=NOW;
	
	apriori(3); //построение правил
	
	end=NOW;
	
	std::cout<<"apriori time: "<<
		std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<<"mcs\n";
	std::cout<<"apriori size: "<<rules.size()<<"\n";
	for(auto i:rules) std::cout<<i; //вывод правил
	rules.clear();
	std::cout<<"\n\n";
	start=NOW;
	
	guha(1); //построение правил
	
	end=NOW;
	
	std::cout<<"GUHA time: "<<
		std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<<"mcs\n";
	std::cout<<"GUHA size: "<<rules.size()<<"\n";
	for(auto i:rules) std::cout<<i; //вывод правил
	std::cout<<"\n\n";
	//for(unsigned int i=0; i<40; i++) {out(i); std::cout<<"\n\n";}
	//out(6);
	update();
	
	node *p;
	start=NOW;
	
	p=fp_growth();
	
	end=NOW;
	
	std::cout<<"fp growth time: "<<
		std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()<<"mcs\n";
	p->get_tree();
}

