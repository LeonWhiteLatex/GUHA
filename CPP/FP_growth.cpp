#include "FP_growth.hpp"
#include "guha.hpp"

struct pair{
	int ind, mask;
	long unsigned int place;
};

std::vector<pair> sort(std::vector<pair> T){
	std::vector<pair> _[256]; int I=1, J=256;
	for(unsigned int l=0; l<4; l++){ //std::cout<<"power: "<<l<<"; I:"<<I<<"; J:"<<J<<"\n";
		for(auto i:T) _[(i.ind/I)%J].push_back(i);
		I*=256; T.clear();
		for(unsigned int i=0, j=255; i<256; i++, j--){
			for(auto k:_[j]) T.push_back(k); _[j].clear();
		}
	}return T;
}
/*
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
};*/

node* ynsort(node* _start, std::vector<pair> PID, int t=0, int rm=0){//if(t>1) return _start;
	if(!_start||!_start->get_sib().size()) return _start;
	std::vector<node*> _[4];_[3]=_start->get_sib();
	int RM=rm|PID[t].mask; node *left=0, *right=0; long posl=0,posr=0; std::vector<node*> ns;
	for(unsigned int i=0;i<(ns=_start->get_sib()).size(); i++){ 
		left=(rm==ns[i]->mask)? ns[i]:left; posl=(rm==ns[i]->mask)? _[0].size():posl;
		right=(RM==ns[i]->mask)? ns[i]:right; posr=(RM==ns[i]->mask)? _[1].size():posr;
		_[bool(PID[t].mask&ns[i]->mask)].push_back(ns[i]);
	}if(t<PID.size()){
		_start->place_sib(_[2]);_start->place_cld(_[2]);
		if(left){
			_[0].erase(_[0].begin()+posl); left->place_sib(_[0]);
			left=ynsort(left,PID,t+1,left->mask); _start->add_sib(left);
		}else{
			_start->place_sib(_[0]);
			_start=ynsort(_start,PID,t+1,_start->mask);
		}if(right){
			_[1].erase(_[1].begin()+posr); right->place_sib(_[1]);
			right=ynsort(right,PID,t+1,right->mask); _start->add_cld(right);
		}else{ 
			_start->place_sib(_[3]);
			_start=ynsort(_start,PID,t+1,_start->mask);
		}
	}return _start;
}

node* fp_growth(){ std::vector<pair> pid;
	std::vector<int> dep(msk.size(),0);
	for(unsigned int i=0; i<msk.size(); i++){
		for(auto j:nodes) if(j&msk[i]) dep[i]++;
		pid.push_back(pair{dep[i], msk[i], i});
	}//std::cout<<pid.size()<<"\n";
	
	//for(auto i:pid) std::cout<<"masks: "<<i.mask<<" "; std::cout<<"\n";
	pid=sort(pid);
	//for(auto i:pid) std::cout<<"masks: "<<i.mask<<" "; std::cout<<"\n";
	unsigned int size=1; for(auto i:msk) size+=i;
	std::vector<pair> dip[size], TT;
	//std::cout<<nodes.size()<<"\n";
	for(auto i:nodes) dip[i].push_back(pair{0,i,0});
	//for(unsigned int i=0; i<size; i++) {for(auto j:dip[i]) std::cout<<j.mask<<" "; std::cout<<"\n";}
	for(unsigned int i=0; i<size; i++) if(dip[i].size()) TT.push_back(pair{0,dip[i][0].mask, dip[i].size()});
	std::vector<node*> k;
	for(auto i:TT) k.push_back(new node(i.mask, i.place));
	node* _start=new node(0,0); _start->place_sib(k);
	_start=ynsort(_start,pid);
	//_start->get_tree();
	_start->reverse_sum();
	//_start->get_tree();
	return _start;
	//for(auto i:TT) std::cout<<std::bitset<4>(i.mask)<<" "<<i.place<<"\n";
}
