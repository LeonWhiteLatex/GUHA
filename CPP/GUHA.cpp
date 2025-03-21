#include <iostream>
#include <iomanip>
#include "guha.hpp"

int main(){
	rnd(std::vector<std::string>{"A", "B", "C", "D", "E"}, 10);
	//Связи формируются случайно
	//A------>B там где есть А должно быть B (зависимость)
	//A-------B А и B часто вместе (ассоциация)
	//A - - ->B там где нету А должно быть B (конкуренция)
	//A - - - B А и B часто порозень (избегание)
	//mask_gen(std::vector<std::string>{"A", "B", "C"});
	for(auto i:names) std::cout<<i<<" "; std::cout<<"\n"; //вывод названий атрибутов
	for(auto i:nodes){
		for(unsigned int l=0; l<msk.size(); l++)
			std::cout<<(i&msk[l]&&1)<<std::setw(names[l].size()+1); //вывод объектов (бинарный)
		std::cout<<"\n";
	}std::cout<<"\n";
	guha(); //построение правил
	for(auto i:rules) std::cout<<i; //вывод правил
}

