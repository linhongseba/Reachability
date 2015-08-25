/**
@ author: Linhong
@ email: smileofice@gmail.com
partstatistics:
1. compute the partitioning information, such as the partid assigned to each vertex v in G;
the set of inter-partition edges, the matrix stores the number of inter-partition edges, 
and the set of source and target vertices of inter-partition edges
2.Analyze the peformance of partition procedure:
number of tree-like structure
number of graph structure
the maximum non-tree density
the minimum non-tree density
the average non-tree density
the non-tree density of original graph
**/
#include"graph.h"
class statispart{
private:
	int *sid;
	int *smap;
	vector <EdgeSet> Ei;
	double maxdensity;
	double mindensity;
	double originaldensity;
	int treenum;
	int graphnum;
	bool* istree;
	int Gsn;
public:
	statispart();
	~statispart();
	statispart(Graph &g, int *&partid,int k,partmap &partindex, int **&matrix,double thres);
	int * GetSID();
	int * GetSMAP();
	void InitGsinfo(Graph &g,int *&partid);
	void Initstatistic(Graph &g,int k,partmap &partindex,int **&matrix,double thres);
	vector <EdgeSet> Getinterpartedges();
	double Getmaxdensity();
	double Getmindensity();
	double Getoriginaldensity(Graph &g);
	int Gettreenum();
	int Getgraphnum();
	int GetnodenumofGs();
	bool * Getistreeinfo();
};
statispart::statispart(Graph &g, int *&partid, int k,partmap &partindex, int **&matrix,double thres){
	this->maxdensity=0;
	this->mindensity=1;
	this->treenum=0;
	this->graphnum=0;
	this->InitGsinfo(g,partid);
	this->Initstatistic(g,k,partindex,matrix,thres);
}
void statispart::InitGsinfo(Graph &g,int *&partid){
	vector <int> subnodes;
	for(int i=0;i<g.sizenode();i++){
		int spid=partid[i];
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			int tpid=partid[p->adjvex];
			if(spid!=tpid){
				EdgeSet temp;
				temp.startnode=i;
				temp.endnode=p->adjvex;
				this->Ei.push_back(temp);
				if(g.vertices[i].color=='w')
					g.vertices[i].color='g';
				if(g.vertices[p->adjvex].color=='w')
					g.vertices[p->adjvex].color='g';
			}
		}
	}
	for(int i=0;i<g.sizenode();i++){
		if(g.vertices[i].color=='g'){
			subnodes.push_back(i);
			g.vertices[i].color='w';
		}
	}
	this->smap=new int[(int)subnodes.size()];
	this->sid=new int[g.sizenode()];
	memset(this->sid,-1,4*g.sizenode());
	this->Gsn=(int)subnodes.size();
	for(int i=0;i<(int)subnodes.size();i++){
		smap[i]=subnodes[i];
		sid[subnodes[i]]=i;
	}
}
void statispart::Initstatistic(Graph &g,int k,partmap &partindex,int **&matrix,double thres){
	mymath mathf;
	this->istree=new bool[k];
	for(int i=0;i<k;i++){
		int nodenum=(int)partindex.count(i);
		int edgenum=matrix[i][i];
		double density=mathf.nontreedensity(nodenum,edgenum);
		if(density<this->mindensity)
			this->mindensity=density;
		if(density>this->maxdensity)
			this->maxdensity=density;
		if(density<=thres){
			istree[i]=true;
			this->treenum++;
		}else{
			istree[i]=false;
			this->graphnum++;
		}
	}
}
statispart::statispart(){
	this->sid=NULL;
	this->smap=NULL;
	this->istree=NULL;
}
statispart::~statispart(){
}
int * statispart::GetSID(){
	return this->sid;

}
int * statispart::GetSMAP(){
	return this->smap;
}
vector <EdgeSet> statispart::Getinterpartedges(){
	return this->Ei;
}

double statispart::Getmaxdensity(){
	return this->maxdensity;
}
double statispart::Getmindensity(){
	return this->mindensity;
}
double statispart::Getoriginaldensity(Graph &g){
	mymath mathf;
	return mathf.nontreedensity(g.sizenode(),g.sizeedge());
}
int statispart::Gettreenum(){
	return this->treenum;
}
int statispart::Getgraphnum(){
	return this->graphnum;
}
bool *statispart::Getistreeinfo(){
	return this->istree;
}
int statispart::GetnodenumofGs(){
	return this->Gsn;
}

