/**
@ author: Linhong
@ email: smileofice@gmail.com
graph: implements a in-memory graph structure, which is stored as link-list type
**/
#ifndef graph_h_
#define graph_h_
#include"Utility.h"
/**
class Graph:
the in-memeory link-list data structure
the function to get the indegree and outdegree of a vertex v
the naive algorithm to test whether u can reach ve by recursive traversal
**/
class Graph{
private:
	int vnum,arcnum;
public:
	VNode * vertices;
	vector<int> *parents;
public:
	Graph();
	Graph(const char *inname);
	~Graph();
	int sizenode();
	int sizeedge();
	int outdegree(int v);
	int indegree(int v);
	void setvn(int vn);
	void setan(int an);
	bool IsDes(int u, int v);
};
Graph::Graph(){
	vertices=NULL;
	vnum=0;
	arcnum=0;
}
Graph::Graph(const char *inname){
	Utility uti;
	ifstream infile;
	infile.open(inname);
	if(!infile.is_open()){
		cout<<"can not find the graph file"<<endl;
		exit(0);
	}
	int vn,an;
	infile>>vn;
	infile>>an;
	this->vnum=vn;
	vertices=new VNode[vn];
	parents=new vector<int> [vn];
	this->arcnum=0;
	for(int i=0;i<vn;i++){
		char *buffer=new char[300];
		infile.getline(buffer,300);
		string s(buffer,0,300);
		this->vertices[i].color='w';
		this->vertices[i].data=s;
		this->vertices[i].start=0;
		this->vertices[i].finish=0;
		this->vertices[i].firstarc=NULL;
	}
	char *buffer=new char[50];
	infile.getline(buffer,50);
	for(int j=0;j<an;j++){
		int v,w;
		infile.getline(buffer,50);
		char *pch;
		pch=strtok(buffer," ");
		v=atoi(pch);
		pch=strtok(NULL," ");
		w=atoi(pch);
		if(v!=w){
			bool flag=uti.Insert(this->vertices[v],w);
			if(flag==true){
				arcnum++;
				this->parents[w].push_back(v);
			}
				
		}
	}
	if(arcnum!=an){
		cout<<"the input graph has multiple edges"<<endl;
	}
}
int Graph::outdegree(int v){
	int count=0;
	for(ArcNode *p=vertices[v].firstarc;p!=NULL;p=p->nextarc){
		count++;
	}
	return count;
}
int Graph::indegree(int v){
	return (int)this->parents[v].size();
}
Graph::~Graph(){
}
int Graph::sizenode(){
	return this->vnum;
}
int Graph::sizeedge(){
	return this->arcnum;
}
void Graph::setan(int an){
	this->arcnum=an;
}
void Graph::setvn(int vn){
	this->vnum=vn;
}
bool Graph::IsDes(int u, int v)
{
  /* Have we been here already? */

	if (this->vertices[v].color=='g')
		return false;

  /*
   * Is this the destination?  If so,
   * we've reached it!
   */

  if (v == u)
	  return true;

  /* Don't come here again. */
  this->vertices[v].color='g';

  /*
   * See if we can get there from each
   * of the vertices we connect to.
   * If we can get there from at least
   * one of them, it is reachable.
   */
  for (ArcNode *p=vertices[v].firstarc;p!=NULL;p=p->nextarc) {
	  if (this->IsDes(u, p->adjvex)==true)
      return true;
  }

  /*
   * Couldn't get there from any of our
   * neighbors, so it is unreachable from
   * here.
   */
  return false;
}
#endif

