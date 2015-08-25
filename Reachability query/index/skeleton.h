/**
@ author:  Linhong
@ email:  smileofice@gmail.com
skeleton:
Given the partitioning information of a graph G, P(G), construcut the graph framework H(G)
In H(G), there are:
the partition-level graph PLG
the partition-level skeleton graph PLSG
the mapping function M
**/

#include"../Autopart/graph.h"
/**
class skeleton:
PLG graph construction
PLSG graph construction
mapping M construction
and 
superflous edges removal
**/
void itoa( int value, char* &result, int base ) {
	
	// check that the base if valid
	
	if (base < 2 || base > 16) { *result = 0; }
	

	
	char* out = result;
	
	int quotient = value;
	

	
	do {
	
		*out = "0123456789abcdef"[ std::abs( quotient % base ) ];
	
		++out;
	
		quotient /= base;
	
	} while ( quotient );
	

	
	// Only apply negative sign for base 10
	
	if ( value < 0 && base == 10) *out++ = '-';
	

	
	std::reverse( result, out );
	
	*out = 0;
	
	
}

class skeleton{
private:
	Graph PLG;
	Graph PLSG;
	partmap M;
public:
	skeleton(int *&pid,int k,int **&matrix,vector<EdgeSet> &Ei,int Gsn, int *&sid, int *&smap);
	void ConstructPLG(int k, int **&matrix);
	void ConstructPLSG(int Gsn, vector<EdgeSet> &Ei,int *&sid, int *&smap);
	void constructM(int *&pid,int Gsn,int *&smap);
	~skeleton();
	Graph GetPLG();
	Graph GetPLSG();
	void removesuperflous();
	partmap Getmapping();
};
skeleton::skeleton(int *&pid,int k,int **&matrix,vector<EdgeSet> &Ei,int Gsn, int *&sid, int *&smap){
	this->ConstructPLG(k,matrix);
	this->ConstructPLSG(Gsn,Ei,sid,smap);
	this->constructM(pid,Gsn,smap);
	this->removesuperflous();
}
void skeleton::ConstructPLG(int k, int **&matrix){
	Utility uti;
	this->PLG.setvn(k);
	this->PLG.vertices=new VNode[k];
	this->PLG.parents=new vector <int>[k];
	for(int i=0;i<k;i++){
		this->PLG.vertices[i].color='w';
		string s("");
		this->PLG.vertices[i].data=s;
		this->PLG.vertices[i].firstarc=NULL;
		this->PLG.vertices[i].start=0;
		this->PLG.vertices[i].finish=0;
	}
	int edgenum=0;
	for(int i=0;i<k;i++){
		for(int j=0;j<k;j++){
			if(j!=i&&matrix[i][j]>0){
				bool flag=uti.Insert(this->PLG.vertices[i],j);
				if(flag==true){
					edgenum++;
					this->PLG.parents[j].push_back(i);
				}
			}
		}
	}
	this->PLG.setan(edgenum);
}
void skeleton::ConstructPLSG(int Gsn, vector<EdgeSet> &Ei, int *&sid, int *&smap){
	this->PLSG.setvn(Gsn);
	this->PLSG.vertices=new VNode[Gsn];
	this->PLSG.parents=new vector <int>[Gsn];
	for(int i=0;i<Gsn;i++){
		PLSG.vertices[i].color='w';
		char * s=new char[65];
		itoa(smap[i],s,10);
		string dat(s);
		this->PLSG.vertices[i].data=dat;
		this->PLSG.vertices[i].firstarc=NULL;
		this->PLSG.vertices[i].start=0;
		this->PLSG.vertices[i].finish=0;
	}
	int edgenum=0;
	for(int i=0;i<(int)Ei.size();i++){
		int u=sid[Ei[i].startnode];
		int v=sid[Ei[i].endnode];
		bool flag=uti.Insert(this->PLSG.vertices[u],v);
		if(flag==true){
			edgenum++;
			this->PLSG.parents[v].push_back(u);
		}
	}
	this->PLSG.setan(edgenum);
}
void skeleton::constructM(int *&pid,int Gsn,int *&smap){
	for(int i=0;i<Gsn;i++){
		int v=smap[i];
		this->M.insert(make_pair(pid[v],i));
	}
}
skeleton::~skeleton(){
}

Graph skeleton::GetPLG(){
	return this->PLG;
}

Graph skeleton::GetPLSG(){
	return this->PLSG;
}

partmap skeleton::Getmapping(){
	return this->M;
}

void skeleton::removesuperflous(){

}

