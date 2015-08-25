/**
@ author: Linhong
@ email: smileofice@gmail.com
DFSAPP: implements all the linear time tarjan algorithm, which includs DFS traversal on undirected graph and digraph,
finding bridges and cut vertices on undirected graph, finding strongly connected component in digraph, and so on. 
**/


#ifndef DFSAPP_h_
#define DFSAPP_h_
#include"graph.h"
#include <vector>
#include <stack>
#include<queue>
#include"Utility.h"
class DFSAPP{
private:
	int *L;
	int *H;
	int *ND;
	int *low;
	vector <int> *adjlist;  //Given the Directed g, ingnoring the directiong--->g', 
	                       //the adjacent matrix of g', used in finding bridges and cut vertices
	vector <int>* treelist;
	vector <int> *nontreelist;
	vector <int> *children;
	int *postorder;
	int *rev_postorder;
	std::stack<int>stack_nodes;
public:
	Graph compressedG;      //Merge SCC if G is cyclic
	int *topologicalorder; //if g is a DAG, return the topogicalorder
	                      //(ordered by the finish time of dfs),otherwise, return NULL
	
	
	/*===============================================================*/
	/*-------------------used in do tarjia---------------------------*/
	/*===============================================================*/
	int *index;
	int *comp; //maintain the component index during double dfs
	int C;
	int nolevel;
	int *lowlink;

	int num;
	int dfstime;
	int count;
	bool *instack;

	/*-------- store the bridges and cut vertices found---------------------------------------*/
	vector <EdgeSet> Bridgeset;
	vector <int> Cutset;
public:
	DFSAPP();
	~DFSAPP();
	/**
	  @ different DFS traversals
	**/
	void doubleDFS(Graph &g, int start);//double dfs traversal without recursion

	void singleDFS(Graph &g,int v);  //single dfs traversal, maintains the finish time, 
	                                //and topological order by finish time


	void DFS(Graph &g,int v,int *&partid,int id);
	void colorDFS(Graph &g, int *&partid, int v);
	void undirectedDFS(Graph &g,int v); //traversal the graph when ignoring the edge direction
    vector<int> DFS_res(Graph &g, int v); //return the set of nodes visited by DFS traversal from v
	vector<int> BFS_res(Graph &g, int v);//return the set of nodes visited by BFS traversal from v

	/**
	 @ two kinds of tarjan algorithms
	**/
	void dotarjar(Graph &g); // traversal the graph twice to find the strongly connected component,
	                         //slower but can support huge graphs
	void dotarjar2(Graph &g); //the tarjan algorithm which use iterative inside, 
	                          //faster, but can not work on some huge graphs
	
	/**
	  @ find bridges and cut vertices on the undirected graph
	**/
	void FindBridges(Graph &h);
	void FindCutVertices(Graph &g);
	void assignpostorder(int N, Graph &g);
	void dfs_iterative(int root, int parent, Graph &g);

	/**
	 @ find the edges on cycle
	**/
	int getnumerofedgeoncycle(Graph &g);

	/**
     @ reset colors
	**/
	void Reset(Graph &g); //after each traversal, reset the color of g to white.
	void ResetWhite(Graph &g);

	void Docompressed(Graph &g);
};

DFSAPP::DFSAPP(){
	comp=NULL;
	this->topologicalorder=NULL;
	postorder=NULL;
	rev_postorder=NULL;
	treelist=NULL;
	nontreelist=NULL;
	children=NULL;
	L=NULL;
	H=NULL;
	ND=NULL;
	low=NULL;
}
DFSAPP::~DFSAPP(){
}

/*assume that we have already done tarjia before calling do compressed*/
void DFSAPP::Docompressed(Graph &g){
	Utility uti;
	compressedG.setvn(C);
	compressedG.vertices=new VNode[C];
	compressedG.parents=new vector<int>[C];
	for(int i=0;i<C;i++){
		compressedG.vertices[i].color='w';
		compressedG.vertices[i].start=0;
		compressedG.vertices[i].finish=0;
		compressedG.vertices[i].firstarc=NULL;
		compressedG.vertices[i].data="";
	}
	int numberofedge=0;
	for(int i=0;i<g.sizenode();i++){
		int u=comp[i];
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			int v=comp[p->adjvex];
			if(u!=v){
				bool flag=uti.Insert(compressedG.vertices[u], v);
				if(flag==true){
					numberofedge++;
					compressedG.parents[v].push_back(u);
				}
			}
		}
	}
	compressedG.setan(numberofedge);
}
void DFSAPP::undirectedDFS(Graph &g,int v){
	stack<int> visitS;
	g.vertices[v].color='g';
	int u;
	visitS.push(v);
	g.vertices[v].start=dfstime;
	dfstime++;
	while(visitS.empty()==false){
		u=visitS.top();
		if(g.vertices[u].color=='w'){
			g.vertices[u].color='g';
			g.vertices[u].start=dfstime;
			dfstime++;
		}
		if(g.vertices[u].color=='b')
			visitS.pop();
		else{
			bool end=true;
			for(int i=0;i<(int)adjlist[u].size();i++){
				int w=adjlist[u][i];
				if(g.vertices[w].color=='w'){
					visitS.push(w);
					end=false;
				}
			}
			if(end==true){
				g.vertices[u].color='b';
				visitS.pop();
				g.vertices[u].finish=dfstime;
				dfstime++;
			}
		}
	}
}
void DFSAPP::dfs_iterative(int root,int parent, Graph &g){
	int i,j;
	stack <int> dfs_stack;
	dfs_stack.push(root);
	dfs_stack.push(parent);
	dfs_stack.push(0);
	while (!dfs_stack.empty()) {
		i=dfs_stack.top(); 
		dfs_stack.pop();
		parent=dfs_stack.top(); 
		dfs_stack.pop();
		root=dfs_stack.top(); 
		dfs_stack.pop();
		if (i==0) g.vertices[root].color='g';
		if (i==(int)adjlist[root].size()) {
			j=0;
			for (i=0; i<(int)adjlist[root].size(); i++) {
				int w=adjlist[root][i];
				if ((j<(int)children[root].size()) && (children[root][j]==w)) {
					treelist[num].push_back(postorder[w]);
					j++;
				}
				else if (w!=parent) {
					nontreelist[num].push_back(w);
				}
			}
			postorder[root]=num;
			rev_postorder[num]=root;
			num++;
		}
		else {
			int w=adjlist[root][i];
			dfs_stack.push(root);
			dfs_stack.push(parent);
			dfs_stack.push(i+1);
			if (g.vertices[w].color=='w') {
				children[root].push_back(w);
				dfs_stack.push(w);
				dfs_stack.push(root);
				dfs_stack.push(0);			
				continue;
			}
		}
	}
	this->Reset(g);
}
void DFSAPP::assignpostorder(int N,Graph &g){
	num=0;
	memset(postorder,-1,sizeof(postorder));
	dfs_iterative(0,-1,g);
	int i,j;
	for (i=0; i<N; i++) {
		for (j=0; j<(int)nontreelist[i].size(); j++) {
			nontreelist[i][j]=postorder[nontreelist[i][j]];
		}
	}
}
void DFSAPP::FindBridges(Graph &h){
	int i,j;
	this->adjlist=new vector <int> [h.sizenode()];
	this->treelist=new vector <int>[h.sizenode()];
	this->nontreelist=new vector <int>[h.sizenode()];
	this->postorder=new int[h.sizenode()];
	this->rev_postorder=new int[h.sizenode()];
	this->L=new int[h.sizenode()];
	this->H=new int[h.sizenode()];
	this->ND=new int[h.sizenode()];
	this->children=new vector <int>[h.sizenode()];
	for(int i=0;i<h.sizenode();i++){
		for(ArcNode *p=h.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			adjlist[i].push_back(p->adjvex);
			adjlist[p->adjvex].push_back(i);
		}
	}
	assignpostorder(h.sizenode(),h);
	for (i=0; i<h.sizenode(); i++) {
		ND[i]=1;
			for (j=0; j<(int)treelist[i].size(); j++) {
				int w=treelist[i][j];
				ND[i]+=ND[w];
			}
			L[i]=i-ND[i]+1;
			for (j=0; j<(int)treelist[i].size(); j++) {
				int w=treelist[i][j];
				if (L[i]>L[w]) {
					L[i]=L[w];
				}
			}
			for (j=0; j<(int)nontreelist[i].size(); j++) {
				int w=nontreelist[i][j];
				if (L[i]>w) {
					L[i]=w;
				}
			}
			H[i]=i;
			for (j=0; j<(int)treelist[i].size(); j++) {
				int w=treelist[i][j];
				if (H[i]<H[w]) {
					H[i]=H[w];
				}
			}
			for (j=0; j<(int)nontreelist[i].size(); j++) {
				int w=nontreelist[i][j];
				if (H[i]<w) {
					H[i]=w;
				}
			}
			for (j=0; j<(int)treelist[i].size(); j++) {
				int w=treelist[i][j];
				if ((H[w]<=w) && (L[w]>w-ND[w]) ) {
					int m=rev_postorder[i];
					int n=rev_postorder[w];
					EdgeSet temp;
					temp.startnode=m;
					temp.endnode=n;
					this->Bridgeset.push_back(temp);
				}
			}
	}
	delete []this->adjlist;
	delete []this->L;
	delete []this->H;
	delete []this->children;
	delete []this->nontreelist;
	delete []this->treelist;
	delete []this->postorder;
	delete []this->rev_postorder;
	delete []this->children;
}
void DFSAPP::FindCutVertices(Graph &g){
	cout<<"begin to find the cut vertices"<<endl;
	this->low=new int[g.sizenode()];
	this->adjlist=new vector <int> [g.sizenode()];
	for(int i=0;i<g.sizenode();i++){
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			this->adjlist[i].push_back(p->adjvex);
			this->adjlist[p->adjvex].push_back(i);
		}
	}
	stack<int> visitS;
	visitS.push(-1);
	visitS.push(0);
	dfstime=0;
	while(visitS.empty()==false){
		int v=visitS.top();//child
		visitS.pop();
		int u=visitS.top();//parent
		visitS.pop();
		visitS.push(u);
		visitS.push(v);
		if(g.vertices[v].color=='w'){
			g.vertices[v].color='g';
			g.vertices[v].start=dfstime;
			this->low[v]=dfstime;
			dfstime++;
		}
		if(g.vertices[v].color=='b'){
			visitS.pop();
			visitS.pop();
			if(u!=-1)
				this->low[u]=min(this->low[u],this->low[v]);
		}
		else{
			bool end=true;
			for(int i=0;i<(int)adjlist[v].size();i++){
				int w=adjlist[v][i];
				if(g.vertices[w].color=='w'){
					visitS.push(v);
					visitS.push(w);
					end=false;
				}
				else{
					this->low[v]=min(this->low[v],g.vertices[w].start);
				}
			}
			if(end==true){
				g.vertices[v].color='b';
				visitS.pop();
				visitS.pop();
				g.vertices[v].finish=dfstime;
				dfstime++;
				if(u!=-1)
					this->low[u]=min(this->low[u],this->low[v]);
			}
		}
	}
	for(int i=g.sizenode()-1;i>=0;i--){
		if(i==0){
			if(g.outdegree(i)>=2)
				this->Cutset.push_back(i);
		}
		else{
			for(int k=0;k<(int)adjlist[i].size();k++){
				int u=adjlist[i][k];
				if(low[u]>=g.vertices[i].start){
					this->Cutset.push_back(i);
					break;
				}
			}
		}
	}
	delete []this->adjlist;
	this->Reset(g);
}
void DFSAPP::singleDFS(Graph &g,int v){
	stack<int> visitS;
	g.vertices[v].color='g';
	int u;
	visitS.push(v);
	g.vertices[v].start=dfstime;
	dfstime++;
	while(visitS.empty()==false){
		u=visitS.top();
		if(g.vertices[u].color=='w'){
			g.vertices[u].start=dfstime;
			g.vertices[u].color='g';
			dfstime++;
		}
		if(g.vertices[u].color=='b')
			visitS.pop();
		else{
			ArcNode *p=g.vertices[u].firstarc;
			bool end=true;
			while(p!=NULL){
				if(g.vertices[p->adjvex].color=='w'){
					visitS.push(p->adjvex);
					end=false;
				}
				p=p->nextarc;
			}
			if(end==true){
				visitS.pop();
				g.vertices[u].color='b';
				this->topologicalorder[count]=u;
				count--;
				g.vertices[u].finish=dfstime;
				dfstime++;
			}
		}
	}
}
void DFSAPP::DFS(Graph &g,int v,int *&partid,int id){
	stack<int> visitS;
	g.vertices[v].color='g';
	int u;
	visitS.push(v);
	g.vertices[v].start=dfstime;
	dfstime++;
	while(visitS.empty()==false){
		u=visitS.top();
		if(g.vertices[u].color=='w'){
			g.vertices[u].start=dfstime;
			g.vertices[u].color='g';
			dfstime++;
		}
		if(g.vertices[u].color=='b')
			visitS.pop();
		else{
			ArcNode *p=g.vertices[u].firstarc;
			bool end=true;
			while(p!=NULL){
				if(g.vertices[p->adjvex].color=='w'){
					visitS.push(p->adjvex);
					end=false;
				}
				p=p->nextarc;
			}
			for(int i=0;i<(int)g.parents[u].size();i++){
				int w=g.parents[u][i];
				if(g.vertices[w].color=='w'){
					visitS.push(w);
					end=false;
				}
			}
			if(end==true){
				visitS.pop();
				g.vertices[u].color='b';
				this->topologicalorder[count]=u;
				count--;
				if(partid[u]==-1)
					partid[u]=id;
				g.vertices[u].finish=dfstime;
				dfstime++;
			}
		}
	}
}
void DFSAPP::colorDFS(Graph &g,int *&partid,int v){
	stack<int> visitS;
	g.vertices[v].color='g';
	int u;
	visitS.push(v);
	g.vertices[v].start=dfstime;
	dfstime++;
	int sourcepid=partid[v];
	while(visitS.empty()==false){
		u=visitS.top();
		if(g.vertices[u].color=='w'){
			g.vertices[u].color='g';
			g.vertices[u].start=dfstime;
			dfstime++;
		}
		if(g.vertices[u].color=='b')
			visitS.pop();
		else{
			ArcNode *p=g.vertices[u].firstarc;
			bool end=true;
			while(p!=NULL){
				if(g.vertices[p->adjvex].color=='w'&&partid[p->adjvex]==sourcepid){
					visitS.push(p->adjvex);
					end=false;
				}
				p=p->nextarc;
			}
			if(end==true){
				g.vertices[u].color='b';
				visitS.pop();
				topologicalorder[count]=u;
				count--;
				g.vertices[u].finish=dfstime;
				dfstime++;
			}
		}
	}
}
void DFSAPP::dotarjar(Graph &g){
	this->Reset(g);
	this->dfstime=0;
	this->count=g.sizenode()-1;
	comp=new int[g.sizenode()];
	this->topologicalorder=new int[g.sizenode()];
	for(int i=0;i<g.sizenode();i++){
		if(g.vertices[i].color=='w')
			this->singleDFS(g,i);
	}
	C=0;
	this->Reset(g);
	for(int i=0;i<g.sizenode();i++){
		int child=this->topologicalorder[i];
		if(g.vertices[child].color=='w'){
			stack <int> visitnode;
			visitnode.push(child);
			g.vertices[child].color='g';
			while(visitnode.empty()==false){
				int u=visitnode.top();
				if(g.vertices[u].color=='w'){
					g.vertices[u].color='g';
				}
				if(g.vertices[u].color=='b'){
					visitnode.pop();
					this->comp[u]=C;
				}
				else{
					bool flag=true;
					for(int j=0;j<(int)g.parents[u].size();j++){
						int par=g.parents[u][j];
						if(g.vertices[par].color=='w'){
							flag=false;
							visitnode.push(par);
						}
					}
					if(flag==true){
						visitnode.pop();
						g.vertices[u].color='b';
						this->comp[u]=C;
					}
				}
			}
			C++;
		}
	}
	delete[]this->topologicalorder;
	this->topologicalorder=NULL;
}
void DFSAPP::doubleDFS(Graph &g,int start){
	this->index[start]=nolevel;
	this->lowlink[start]=nolevel;
	nolevel++;
	comp[start]=-1;
	stack_nodes.push(start);
	this->instack[start]=true;
	int cur;
	for (ArcNode *p=g.vertices[start].firstarc;p!=NULL;p=p->nextarc) {
		cur=p->adjvex;
		if (index[cur]==-1) {
			doubleDFS(g,cur);
			lowlink[start]=min(lowlink[start],lowlink[cur]);
		}
		else
			if(this->instack[cur]==true)
				lowlink[start]=min(lowlink[start],index[cur]);
	}
	if (lowlink[start]==index[start]) {
		do {
			cur=stack_nodes.top(); stack_nodes.pop();
			this->instack[cur]=false;
			comp[cur]=C;
		} while (cur!=start);
		C++;
	}
}
void DFSAPP::dotarjar2(Graph &g){
	cout<<"begin dotarjar"<<endl;
	Reset(g);
	int i;
	C=0;
	this->nolevel=0;
	this->index=new int[g.sizenode()];
	comp=new int[g.sizenode()];
	this->lowlink=new int[g.sizenode()];
	this->instack=new bool[g.sizenode()];
	memset(instack,false,g.sizenode());
	memset(index,-1,g.sizenode()<<2);
	for (i=0; i<g.sizenode(); i++) {
		if (index[i]==-1) {
			this->doubleDFS(g,i);
		}
	}
	delete []this->index;
	delete []this->lowlink;
	delete []this->instack;
	this->index=NULL;
	this->lowlink=NULL;
	this->instack=NULL;
}
vector<int> DFSAPP::DFS_res(Graph &g, int v){
	vector <int> visitednodes;
	stack<int> visitS;
	g.vertices[v].color='g';
	int u;
	visitS.push(v);
	visitednodes.push_back(v);
	while(visitS.empty()==false){
		u=visitS.top();
		if(g.vertices[u].color=='w'){
			g.vertices[u].color='g';
			visitednodes.push_back(u);
		}
		if(g.vertices[u].color=='f')
			visitS.pop();
		else{
			ArcNode *p=g.vertices[u].firstarc;
			bool end=true;
			while(p!=NULL){
				if(g.vertices[p->adjvex].color=='w'){
					visitS.push(p->adjvex);
					end=false;
				}
				p=p->nextarc;
			}
			if(end==true){
				visitS.pop();
				g.vertices[u].color='f';
			}
		}
	}
	return visitednodes;
}
vector<int>DFSAPP::BFS_res(Graph &g, int v){
	queue <int> visitqueue;
	vector <int> accessnodes;
	g.vertices[v].color='g';
	visitqueue.push(v);
	accessnodes.push_back(v);
	while(visitqueue.size()!=0){
		int u=visitqueue.front();
		visitqueue.pop();
		ArcNode *p=g.vertices[u].firstarc;
		if(p!=NULL){
			do{
				if(g.vertices[p->adjvex].color=='w'){
					g.vertices[p->adjvex].color='g';
					accessnodes.push_back(p->adjvex);
					visitqueue.push(p->adjvex);
				}
				p=p->nextarc;
			}while(p!=NULL);
		}
		g.vertices[u].color='b';	

	}
	return accessnodes;
}
void DFSAPP::Reset(Graph &g){
	for(int i=0;i<g.sizenode();i++)
		g.vertices[i].color='w';
}
void DFSAPP::ResetWhite(Graph &g){
	for(int i=0;i<g.sizenode();i++){
		if(g.vertices[i].color=='f')
			g.vertices[i].color='w';
	}
}
int DFSAPP::getnumerofedgeoncycle(Graph &g){
	this->dotarjar(g);
	int count=0;
	for(int i=0;i<g.sizenode();i++){
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			if(this->comp[i]==this->comp[p->adjvex])
				count++;
		}
	}
	this->Reset(g);
	return count;
}
#endif

