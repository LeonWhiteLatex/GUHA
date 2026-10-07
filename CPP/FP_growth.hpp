#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <bitset>

struct node{
	int mask=0;
	long unsigned int p;
	node* parent=0;
	std::vector<node*> sibling, child;
public:
	node(int mask, long unsigned int p):mask(mask),p(p){}
	void add_sib(node* s){sibling.push_back(s);s->parent=parent;}
	void add_cld(node* c){child.push_back(c);c->parent=this;}
	void insert_sib(std::vector<node*> s){ sibling.insert(sibling.end(),s.begin(), s.end()); }
	void insert_cld(std::vector<node*> c){ child.insert(child.end(),c.begin(), c.end()); }
	void place_sib(std::vector<node*> s){ sibling=s; }
	void place_cld(std::vector<node*> c){ child=c; }
	std::vector<node*> get_cld(){ return child; }
	std::vector<node*> get_sib(){ return sibling; }
	void get_tree(int in=0){//if(in>1) return;
		std::cout<<std::bitset<4>(mask)<<": "<<p<<"\n";
		//std::cout<<child.size()<<" "<<sibling.size()<<"\n";
		//if(child.size())std::cout<<"children\n";
		for(auto i:child) { for(int j=0; j<in+1; j++) std::cout<<"|"; i->get_tree(in+1); }
		//std::cout<<"sibling time!\n";
		//if(child.size())std::cout<<"siblings\n";
		for(auto i:sibling){ for(int j=0; j<in+1; j++) std::cout<<"|"; i->get_tree(in); }
		//std::cout<<"\n";
	}
	long unsigned int reverse_sum(){
		for(auto i:child) p+=i->reverse_sum();
		for(auto i:sibling) p+=i->reverse_sum();
		return p;
	}
};
node* fp_growth();
