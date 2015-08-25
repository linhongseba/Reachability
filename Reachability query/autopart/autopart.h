/**
@ author:  Linhong
@ email:  smileofice@gmail.com
autopart: partition a given digraph into a set of subgraphs with differenent non-tree density
          as well as the size of inter-partition edges is minimal
**/
#ifndef autopart_h_
#define autopart_h_
#include"graph.h"
#include"DFSAPP.h"
#include"mymath.h"
#endif
struct Part{
	int partid;
	int nodenum;
	int edgenum;
};
class autopart{
private:
	partmap partindex;
	int numpart;
	int connectcompnum;
	int *edges;
	int **finalmatrix;
public:
	/**
	Algorithm 1:
	 choose a partition "currentpart", 
	 move a set of vertices in "currentpart" into a new partition
	**/
	autopart(Graph &g,int *&partid,int type,int iterationum);
	~autopart();
	Part choosepartiton(Graph &g, int *&partid, int type);
	double initialcost(Graph &g, int type, int *&partid);
	void updatepartition(Graph &g, Part &currentpart, int *&partid,int type);
	void FindConnectedComponent(Graph &g, int *&partid);

	/**
	Algorithm 2:
	Refinement Partition Procedure
	For each vertex v, 
	find a better target partition such that the cost will be reduced by the new assignment.
	cost function 1:  
	E(G_i)= entropy( graphdensity(Gi));
	E(P(G))=\sum E(G_i) /k +\sum E_{ij}/|V_i|/|V_j|

	cost function 2:
	E(G_i)=|G_i|*(1-dnt)+V_i*sqrt(E_i)*dnt;
	E(P(G))=\sum E(G_i)+\sum |E_{ij}|+k

	cost function 3:
	E(G_i)=C_1|E|*log(|V|^2/|E|)
	E(H(G))=\sum E(G_i)+\sum  |E_{ij}|+klog k
	
	cost function 4:
	E(G_i)=min(C_1*|V|,C_2*|E|^{1/2})
	E(H(G))=\sum E(G_i)+\sum |E_{ij}|+logk

	cost function 5:
	E(G_i)=min(C_1*{V}^2, C_2*|V|*|E|^1/2)
	E(H(G))=\sum E(G_i)+\sum |E_{ij}|+2k
	**/

	double refinementpartition(Graph &g, int *&partid,int type,int iterationum);
	void partitionmatrix(int **&matrix,Graph &g, int *&partid);
	double encodingcost(int *&partid, int **&matrix, int type);
	double costfunction1(int *&partid, int **&matrix);
	double costfunction2(int *&partid, int **&matrix);
	double costfunction3(int *&partid, int **&matrix);
	double costfunction4(int *&partid, int **&matrix);
	double costfunction5(int *&partid, int **&matrix);
	double encodingcostdiff(Graph &g,int *&partid, int **&matrix,int type,int v, int targetpid);
	double costdiff1(Graph &g,int *&partid, int **&matrix,int v, int targetpid);
	double costdiff2(Graph &g,int *&partid, int **&matrix,int v, int targetpid,int type);
	void updatematrix(Graph &g, int *&partid, int targetpid, int v,int **&matrix);
	void newmatrix(Graph &g, int *&partid, int targetpid, int v,int **&nmatrix,int *&newedges);
	void updatepartmap(int v ,int sourcepid, int targetpid);

	/**
	return the partition results
	**/
	int **Getmatrix();
	partmap Getmapping();
	int Getpartnum();
};
void autopart::FindConnectedComponent(Graph &g, int *&partid){
	DFSAPP app;
	//ofstream outtest("bugtest.txt");
	app.dfstime=0;
	app.count=g.sizenode()-1;
	app.topologicalorder=new int[g.sizenode()];
	this->numpart=0;
	for(int i=0;i<g.sizenode();i++){
		if(g.vertices[i].color=='w'){
			app.DFS(g,i,partid,this->numpart);
			this->numpart++;
		}
	}
	for(int i=0;i<g.sizenode();i++)
		g.vertices[i].color='w';
	for(int i=0;i<g.sizenode();i++){
		this->partindex.insert(make_pair(partid[i],i));
		//outtest<<i<<" "<<partid[i]<<endl;
	}
	this->numpart--;
	this->connectcompnum=this->numpart;
	this->edges=new int[this->connectcompnum+1];
	memset(edges,0,sizeof(int)*(this->connectcompnum+1));
	for(int i=0;i<g.sizenode();i++){
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			if(partid[i]==partid[p->adjvex]){
				int pid=partid[i];
				this->edges[pid]++;
			}
		}
	}
}
double autopart::initialcost(Graph &g,int type,int *&partid){
	double cost=0;
	mymath mathf;
	switch(type){
		case 1:
			for(int i=0;i<=this->connectcompnum;i++){
				int v=(int)this->partindex.count(i);
				int e=this->edges[i];
				double nt=mathf.graphdensity(v,e);
				cost=cost+mathf.entropy(nt);
			}
			cost=cost/(this->connectcompnum+1);
			break;
		case 2:
			for(int i=0;i<=this->connectcompnum;i++){
				int v=(int)this->partindex.count(i);
				int e=this->edges[i];
				cost=cost+mathf.estimatesize(v,e);
			}
			cost=cost+(this->connectcompnum+1);
			break;
		case 3:
			for(int i=0;i<=this->connectcompnum;i++){
				int v=(int)this->partindex.count(i);
				int e=this->edges[i];
				cost=cost+mathf.lowerbound(v,e);
			}
			cost=cost+(this->connectcompnum+1)*log((double)(this->connectcompnum+1));
			break;
		case 4:
			for(int i=0;i<=this->connectcompnum;i++){
				int v=(int)this->partindex.count(i);
				int e=this->edges[i];
				cost=cost+mathf.querycost(v,e);
			}
			cost=cost+(double)(this->connectcompnum+1);
			break;
		case 5:
			for(int i=0;i<=this->connectcompnum;i++){
				int v=(int)this->partindex.count(i);
				int e=this->edges[i];
				cost=cost+mathf.sizecost(v,e);
			}
			cost=cost+(this->connectcompnum+1)*2;
			break;
		default:
			cout<<"please choose the cost function -f from {1,2,3,4,5}"<<endl;
			exit(0);
	}
	return cost;
}
autopart::autopart(Graph &g, int *&partid,int type,int iterationum){
	this->FindConnectedComponent(g,partid);
	mymath mathf;
	double descencoding=0;
	double preencoding=this->initialcost(g,type,partid);
	do{
		descencoding=0;
		Part p=this->choosepartiton(g,partid,type);
		this->updatepartition(g,p,partid,type);
		double nowencoding=this->refinementpartition(g,partid,type,iterationum);
		descencoding=nowencoding-preencoding;
		preencoding=nowencoding;
		cout<<"number of partitions now..."<<this->numpart+1<<endl;
	}while(descencoding<0);
	this->finalmatrix=new int *[this->numpart+1];
	for(int i=0;i<=this->numpart;i++)
		this->finalmatrix[i]=new int[this->numpart+1];
	for(int i=0;i<this->numpart+1;i++)
		for(int j=0;j<this->numpart+1;j++)
			this->finalmatrix[i][j]=0;
	for(int i=0;i<(int)g.sizenode();i++){
		int pid=partid[i];
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			int tid=partid[p->adjvex];
			this->finalmatrix[pid][tid]++;
		}
	}
}
autopart::~autopart(){

}
/**
choose a partition , the cost of which is maximum
**/
Part autopart::choosepartiton(Graph &g, int *&partid,int type){
	Part p;
	p.partid=0;
	p.nodenum=0;
	p.edgenum=0;
	double maxencoding=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int n=0;
		int e=0;
		for(partmap::iterator pos=this->partindex.lower_bound(i);pos!=this->partindex.upper_bound(i);++pos){
			n++;
			for(ArcNode *p=g.vertices[pos->second].firstarc;p!=NULL;p=p->nextarc)
				if(partid[p->adjvex]==i)
					e++;
		}
		double temp=0;
		switch(type){
			case 1:
				temp=mathf.entropy((double)n/e);
				break;
			case 2:
				temp=mathf.estimatesize(n,e);
				break;
			case 3:
				temp=mathf.lowerbound(n,e);
				break;
			case 4:
				temp=mathf.querycost(n,e);
				break;
			case 5:
				temp=mathf.sizecost(n,e);
				break;
			default:
				cout<<"please choose the cost function -f from {1,2,3,4,5}"<<endl;
				exit(0);
		}
		if(temp>maxencoding){
			maxencoding=temp;
			p.partid=i;
			p.edgenum=e;
			p.nodenum=n;
		}
	}
	return p;
}
/**
Input: Graph g, a chosen partition currentpart G_i, a k-partition P(G)
Output: a k+1 partition by moving a set of vertices in the partition "currentpart" into 
a new partition with pid=k+1;
we move a vertex v into a new partition "K+1" if and only if
Cost(G_i\v)< Cost(G_i)
**/
void autopart::updatepartition(Graph &g, Part &currentpart, int *&partid,int type){
	int i=currentpart.partid;
	int n=currentpart.nodenum;
	int e=currentpart.edgenum;
	mymath mathf;
	double desencoding=0;
	vector <int> temp;
	for(partmap::iterator pos=this->partindex.lower_bound(i);pos!=this->partindex.upper_bound(i);){
		int v=pos->second;
		int re=0;
		for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
			if(partid[p->adjvex]==i)
				re++;
		}
		for(int j=0;j<(int)g.parents[v].size();j++){
			int parent=g.parents[v][j];
			if(partid[parent]==i)
				re++;
		}
		double pre,now;
		switch(type){
			case 1:
				pre=mathf.entropy(mathf.graphdensity(n,e));
				now=mathf.entropy(mathf.graphdensity(n-1,e-re));
				if(pre>now){
					this->partindex.erase(pos++);
					temp.push_back(v);
				}else
					++pos;
				break;
			case 2:
				pre=mathf.estimatesize(n,e)+this->numpart+1;
				now=mathf.estimatesize(n-1,e-re)+this->numpart+2+re;
				if(pre>now){
					this->partindex.erase(pos++);
					temp.push_back(v);
				}
				else
					++pos;
				break;
			case 3:
				pre=mathf.lowerbound(n,e)+log(double(this->numpart)+1)*(this->numpart+1);
				now=mathf.lowerbound(n-1,e-re)+log(double(this->numpart)+2)*(this->numpart+2)+re;
				if(pre>now){
					this->partindex.erase(pos++);
					temp.push_back(v);
				}else
					++pos;
				break;
			case 4:
				pre=mathf.querycost(n,e)+log((double)(this->numpart+1));
				now=mathf.querycost(n-1,e-re)+log((double)(this->numpart+2))+re;
				if(pre>now){
					this->partindex.erase(pos++);
					temp.push_back(v);
				}
				else
					++pos;
				break;
			case 5:
				pre=mathf.sizecost(n,e)+2*(this->numpart+1);
				now=mathf.sizecost(n-1,e-re)+2*(this->numpart+2)+re;
				if(pre>now){
					this->partindex.erase(pos++);
					temp.push_back(v);
				}
				else
					++pos;
				break;
			default:
				cout<<"please choose the cost function from {1,2,3,4,5}"<<endl;
				exit(0);
		}
	}
	if((int)temp.size()>0){
		this->numpart=this->numpart+1;
		for(int j=0;j<(int)temp.size();j++){
			int v=temp[j];
			partid[v]=this->numpart;
			partindex.insert(make_pair(this->numpart,v));
		}
	}
}
/**
return a matrix where each entry matrix[i][j] records the number of edges from partition i to partition j
**/
void autopart::partitionmatrix(int **&matrix,Graph &g, int *&partid){
	int ppnum=this->numpart-this->connectcompnum+1;
	for(int i=0;i<g.sizenode();i++){
		int pid1=partid[i]-this->connectcompnum-1;
		for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
			int pid2=partid[p->adjvex]-this->connectcompnum-1;
			if((pid1==pid2) && (pid1<0))
				this->edges[pid1+this->connectcompnum+1]++;
			else{
				if(pid1<0)
					pid1=ppnum-1;
				if(pid2<0)
					pid2=ppnum-1;
				matrix[pid1][pid2]++;
			}
		}
	}
}
/**
 update the partition matrix if a vertex v is moved into a partition with pid="targetpid"
**/
void autopart::updatematrix(Graph &g,int *&partid,int targetpid,int v,int **&matrix){
	int ppnum=this->numpart-this->connectcompnum+1;
	int sourcepid=partid[v]-this->connectcompnum-1;
	int target=targetpid-this->connectcompnum-1;
	for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
		int tid=partid[p->adjvex]-this->connectcompnum-1;
		if((sourcepid==tid)&&(sourcepid<0))
			this->edges[sourcepid+this->connectcompnum+1]--;
		else{
			if(sourcepid<0)
				sourcepid=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			matrix[sourcepid][tid]--;
		}
		if((target==tid)&&(target<0))
			this->edges[target+this->connectcompnum+1]++;
		else{
			if(target<0)
				target=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			matrix[target][tid]++;
		}
	}
	for(int i=0;i<(int)g.parents[v].size();i++){
		int w=g.parents[v][i];
		int tid=partid[w]-this->connectcompnum-1;
		if((sourcepid==tid)&&(sourcepid<0))
			this->edges[sourcepid+this->connectcompnum+1]--;
		else{
			if(sourcepid<0)
				sourcepid=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			matrix[tid][sourcepid]--;
		}
		if((target==tid)&&(target<0))
			this->edges[target+this->connectcompnum+1]++;
		else{
			if(target<0)
				target=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			matrix[tid][target]++;
		}
	}
	partid[v]=targetpid;
}
/**
@ return the new partition matrix if a vertex v is moved into a partition with pid="targetpid";
**/
void autopart::newmatrix(Graph &g, int *&partid, int targetpid, int v,int **&nmatrix,int *&newedges){
	int ppnum=this->numpart-this->connectcompnum+1;
	int sourcepid=partid[v]-this->connectcompnum-1;
	int target=targetpid-this->connectcompnum-1;
	for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
		int tid=partid[p->adjvex]-this->connectcompnum-1;
		if((sourcepid==tid)&&(sourcepid<0))
			newedges[sourcepid+this->connectcompnum+1]--;
		else{
			if(sourcepid<0)
				sourcepid=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			nmatrix[sourcepid][tid]--;
		}
		if((target==tid)&&(target<0))
			newedges[target+this->connectcompnum+1]++;
		else{
			if(target<0)
				target=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			nmatrix[target][tid]++;
		}
	}
	for(int i=0;i<(int)g.parents[v].size();i++){
		int w=g.parents[v][i];
		int tid=partid[w]-this->connectcompnum-1;
		if((sourcepid==tid)&&(sourcepid<0))
			newedges[sourcepid+this->connectcompnum+1]--;
		else{
			if(sourcepid<0)
				sourcepid=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			nmatrix[tid][sourcepid]--;
		}
		if((target==tid)&&(target<0))
			newedges[target+this->connectcompnum+1]++;
		else{
			if(target<0)
				target=ppnum-1;
			if(tid<0)
				tid=ppnum-1;
			nmatrix[tid][target]++;
		}
	}
}
/**
@   compute the first cost function for a partitioning
    E_{P(G)}=\sum_{i=1}^{k}entropy(dnt(G_i))/k+\sum |E_{ij}|/|V_i|/|V_j|
	the first part estimate the structural information of a partition
	the second part denotes the inter-partition edge density
**/
double autopart::costfunction1(int *&partid,int **&matrix){
	double cost=0;
	double sum=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int v1=(int)this->partindex.count(i);
		int e;
		if(i<=this->connectcompnum)
			e=this->edges[i];
		else
			e=matrix[i-this->connectcompnum-1][i-this->connectcompnum-1];
		double nt=mathf.graphdensity(v1,e);
		cost=cost+mathf.entropy(nt);
		for(int j=0;j<=this->numpart;j++){
			if(j!=i){
				int v2=(int)this->partindex.count(j);
				int interedges;
				if(i<=this->connectcompnum&&j<=this->connectcompnum)
					interedges=0;
				else{
					if(i<=this->connectcompnum)
						interedges=matrix[this->numpart-this->connectcompnum][j-this->connectcompnum-1];
					else
						if(j<=this->connectcompnum)
							interedges=matrix[i-this->connectcompnum-1][this->numpart-this->connectcompnum];
						else
							interedges=matrix[i-this->connectcompnum-1][j-this->connectcompnum-1];
				}
				sum=sum+mathf.Interpartitionedgedensity(interedges,v1,v2);
			}
		}
	}
	cost=sum+cost/(this->numpart+1);
	return cost;
}
/**
@  compute the second cost function for a partitioning, that is,
   E_{P(G)}=\sum_{i=1}^{k}|G_i|*(1-dnt)+V_i*sqrt(E_i)*dnt+\sum |E_{i,j}|+k
   the first part: estimates the index size of each partition
   the second part: k estimates the index size for partiton-level graph
   |E_{ij}| estimates the index size for partition-level skeleton graph
**/
double autopart::costfunction2(int *&partid, int **&matrix){
	double cost=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int v1=(int)this->partindex.count(i);
		int e;
		if(i<=this->connectcompnum)
			e=this->edges[i];
		else
			e=matrix[i-this->connectcompnum-1][i-this->connectcompnum-1];
		cost=cost+mathf.estimatesize(v1,e);
		for(int j=0;j<=this->numpart;j++){
			if(j!=i){
				int interedges;
				if(i<=this->connectcompnum&&j<=this->connectcompnum)
					interedges=0;
				else{
					if(i<=this->connectcompnum)
						interedges=matrix[this->numpart-this->connectcompnum][j-this->connectcompnum-1];
					else
						if(j<=this->connectcompnum)
							interedges=matrix[i-this->connectcompnum-1][this->numpart-this->connectcompnum];
						else
							interedges=matrix[i-this->connectcompnum-1][j-this->connectcompnum-1];
				}
				cost=cost+interedges;
			}
		}
	}
	cost=cost+this->numpart+1;
	return cost;
}
double autopart::costfunction3(int *&partid, int **&matrix){
	double cost=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int v1=(int)this->partindex.count(i);
		int e;
		if(i<=this->connectcompnum)
			e=this->edges[i];
		else
			e=matrix[i-this->connectcompnum-1][i-this->connectcompnum-1];
		cost=cost+mathf.lowerbound(v1,e);
		for(int j=0;j<=this->numpart;j++){
			if(j!=i){
				int interedges;
				if(i<=this->connectcompnum&&j<=this->connectcompnum)
					interedges=0;
				else{
					if(i<=this->connectcompnum)
						interedges=matrix[this->numpart-this->connectcompnum][j-this->connectcompnum-1];
					else
						if(j<=this->connectcompnum)
							interedges=matrix[i-this->connectcompnum-1][this->numpart-this->connectcompnum];
						else
							interedges=matrix[i-this->connectcompnum-1][j-this->connectcompnum-1];
				}
				cost=cost+interedges;
			}
		}
	}
	cost=cost+log((double)(this->numpart+1))*(this->numpart+1);
	return cost;
}
double autopart::costfunction4(int *&partid, int **&matrix){
	double cost=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int v1=(int)this->partindex.count(i);
		int e;
		if(i<=this->connectcompnum)
			e=this->edges[i];
		else
			e=matrix[i-this->connectcompnum-1][i-this->connectcompnum-1];
		cost=cost+mathf.querycost(v1,e);
		for(int j=0;j<=this->numpart;j++){
			if(j!=i){
				int interedges;
				if(i<=this->connectcompnum&&j<=this->connectcompnum)
					interedges=0;
				else{
					if(i<=this->connectcompnum)
						interedges=matrix[this->numpart-this->connectcompnum][j-this->connectcompnum-1];
					else
						if(j<=this->connectcompnum)
							interedges=matrix[i-this->connectcompnum-1][this->numpart-this->connectcompnum];
						else
							interedges=matrix[i-this->connectcompnum-1][j-this->connectcompnum-1];
				}
				cost=cost+interedges;
			}
		}
	}
	cost=cost+(double)(this->numpart+1);
	return cost;
}

double autopart::costfunction5(int *&partid,int **&matrix){
	double cost=0;
	mymath mathf;
	for(int i=0;i<=this->numpart;i++){
		int v1=(int)this->partindex.count(i);
		int e;
		if(i<=this->connectcompnum)
			e=this->edges[i];
		else
			e=matrix[i-this->connectcompnum-1][i-this->connectcompnum-1];
		cost=cost+mathf.sizecost(v1,e);
		for(int j=0;j<=this->numpart;j++){
			if(j!=i){
				int interedges;
				if(i<=this->connectcompnum&&j<=this->connectcompnum)
					interedges=0;
				else{
					if(i<=this->connectcompnum)
						interedges=matrix[this->numpart-this->connectcompnum][j-this->connectcompnum-1];
					else
						if(j<=this->connectcompnum)
							interedges=matrix[i-this->connectcompnum-1][this->numpart-this->connectcompnum];
						else
							interedges=matrix[i-this->connectcompnum-1][j-this->connectcompnum-1];
				}
				cost=cost+interedges;
			}
		}
	}
	cost=cost+2*(this->numpart+1);
	return cost;
}
double autopart::encodingcost(int *&partid,int **&matrix, int type){
	switch(type){
		case 1:
			return this->costfunction1(partid,matrix);
			break;
		case 2:
			return this->costfunction2(partid,matrix);
			break;
		case 3:
			return this->costfunction3(partid,matrix);
			break;
		case 4:
			return this->costfunction4(partid,matrix);
			break;
		case 5:
			return this->costfunction5(partid,matrix);
			break;
		default:
			cout<<"please choose the cost function  -f from {1,2,3,4,5}"<<endl;
			exit(0);
	}
	return -1;
}
double autopart::encodingcostdiff(Graph &g,int *&partid, int **&matrix, int type, int v, int targetpid){
	if(type==1)
		return this->costdiff1(g,partid,matrix,v,targetpid);
	else
		return this->costdiff2(g,partid,matrix,v,targetpid,type);
}
/**
computer the first cost function value difference when v is moved into a new partition targetpid
**/
double autopart::costdiff1(Graph &g,int *&partid, int **&matrix, int v, int targetpid){
	int ppnum=this->numpart-this->connectcompnum+1;
	int **nmatrix=new int *[ppnum];
	for(int i=0;i<ppnum;i++)
		nmatrix[i]=new int[ppnum];
	for(int i=0;i<ppnum;i++)
		for(int j=0;j<ppnum;j++)
			nmatrix[i][j]=matrix[i][j];
	int *newedges=new int[this->connectcompnum+1];
	for(int i=0;i<=this->connectcompnum;i++)
		newedges[i]=this->edges[i];
	this->newmatrix(g,partid,targetpid,v,nmatrix,newedges);
	mymath mathf;
	int sourcepid=partid[v];
	int n1=(int)this->partindex.count(sourcepid);
	int n2=(int)this->partindex.count(targetpid);
	sourcepid=sourcepid-connectcompnum-1;
	int target=targetpid-connectcompnum-1;
	double pre=0;
	double now=0;
	if(sourcepid<0){
		pre=pre+mathf.entropy(mathf.graphdensity(n1,this->edges[sourcepid+connectcompnum+1]));
		now=now+mathf.entropy(mathf.graphdensity(n1-1,newedges[sourcepid+connectcompnum+1]));
		sourcepid=ppnum-1;
	}else{
		pre=pre+mathf.entropy(mathf.graphdensity(n1,matrix[sourcepid][sourcepid]));
		now=now+mathf.entropy(mathf.graphdensity(n1-1,nmatrix[sourcepid][sourcepid]));
	}
	if(target<0){
		pre=pre+mathf.entropy(mathf.graphdensity(n2,this->edges[target+connectcompnum+1]));
		now=now+mathf.entropy(mathf.graphdensity(n2+1,newedges[target+connectcompnum+1]));
		target=ppnum-1;
	}else{
		pre=pre+mathf.entropy(mathf.graphdensity(n2,matrix[target][target]));
		now=now+mathf.entropy(mathf.graphdensity(n2+1,nmatrix[target][target]));
	}
	for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
		int tid=partid[p->adjvex]-connectcompnum-1;
		if(tid<0)
			tid=ppnum-1;
		if(tid==sourcepid){
			int n3=n1;
			pre=pre+mathf.Interpartitionedgedensity(matrix[target][tid],n2,n3);
			now=now+mathf.Interpartitionedgedensity(nmatrix[target][tid],n2+1,n3-1);
		}
		else if(tid==target){
			int n3=n2;
			pre=pre+mathf.Interpartitionedgedensity(matrix[sourcepid][target],n1,n3);
			now=now+mathf.Interpartitionedgedensity(nmatrix[sourcepid][tid],n1-1,n3+1);
		}
		else{
			int n3=(int)this->partindex.count(partid[p->adjvex]);
			pre=pre+mathf.Interpartitionedgedensity(matrix[sourcepid][tid],n1,n3)
			+mathf.Interpartitionedgedensity(matrix[target][tid],n2,n3);
			now=now+mathf.Interpartitionedgedensity(nmatrix[sourcepid][tid],n1-1,n3)+mathf.Interpartitionedgedensity(nmatrix[targetpid][tid],n2+1,n3);
		}	
	}
	for(int i=0;i<(int)g.parents[v].size();i++){
		int w=g.parents[v][i];
		int tid=partid[w]-connectcompnum-1;
		if(tid<0)
			tid=ppnum-1;
		if(tid==sourcepid){
			int n3=n1;
			pre=pre+mathf.Interpartitionedgedensity(matrix[tid][target],n3,n2);
			now=now+mathf.Interpartitionedgedensity(nmatrix[target][tid],n2+1,n3-1);
		}
		else if(tid==target){
			int n3=n2;
			pre=pre+mathf.Interpartitionedgedensity(matrix[tid][sourcepid],n3,n1);
			now=now+mathf.Interpartitionedgedensity(nmatrix[sourcepid][tid],n1-1,n3+1);
		}
		else{
			int n3=(int)this->partindex.count(partid[w]);
			pre=pre+mathf.Interpartitionedgedensity(matrix[tid][sourcepid],n3,n1)+mathf.Interpartitionedgedensity(matrix[tid][targetpid],n3,n2);
			now=now+mathf.Interpartitionedgedensity(nmatrix[tid][sourcepid],n3,n1-1)+mathf.Interpartitionedgedensity(matrix[tid][sourcepid],n3,n2+1);
		}			
	}
	for(int i=0;i<ppnum;i++){
		delete []nmatrix[i];
		nmatrix[i]=NULL;
	}
	return now-pre;
}
/**
computer the second cost function value difference when v is moved into a new partition targetpid
**/
double autopart::costdiff2(Graph &g,int *&partid, int **&matrix, int v, int targetpid,int type){
	int ppnum=this->numpart-this->connectcompnum+1;
	int **nmatrix=new int *[ppnum];
	for(int i=0;i<ppnum;i++)
		nmatrix[i]=new int[ppnum];
	for(int i=0;i<ppnum;i++)
		for(int j=0;j<ppnum;j++)
			nmatrix[i][j]=matrix[i][j];
	int *newedges=new int[this->connectcompnum+1];
	for(int i=0;i<=this->connectcompnum;i++)
		newedges[i]=this->edges[i];
	this->newmatrix(g,partid,targetpid,v,nmatrix,newedges);
	mymath mathf;
	int sourcepid=partid[v];
	int n1=(int)this->partindex.count(sourcepid);
	int n2=(int)this->partindex.count(targetpid);
	sourcepid=sourcepid-connectcompnum-1;
	int target=targetpid-connectcompnum-1;
	double pre=0;
	double now=0;
	switch(type){
		case 2:
			if(sourcepid<0){
				pre=pre+mathf.estimatesize(n1,this->edges[sourcepid+connectcompnum+1]);
				now=now+mathf.estimatesize(n1-1,newedges[sourcepid+connectcompnum+1]);
				sourcepid=ppnum-1;
			}else{
				pre=pre+mathf.estimatesize(n1,matrix[sourcepid][sourcepid]);
				now=now+mathf.estimatesize(n1-1,nmatrix[sourcepid][sourcepid]);
			}
			if(target<0){
				pre=pre+mathf.estimatesize(n2,this->edges[target+connectcompnum+1]);
				now=now+mathf.estimatesize(n2+1,newedges[target+connectcompnum+1]);
				target=ppnum-1;
			}else{
				pre=pre+mathf.estimatesize(n2,matrix[target][target]);
				now=now+mathf.estimatesize(n2+1,nmatrix[target][target]);
			}
			break;
		case 3:
			if(sourcepid<0){
				pre=pre+mathf.lowerbound(n1,this->edges[sourcepid+connectcompnum+1]);
				now=now+mathf.lowerbound(n1-1,newedges[sourcepid+connectcompnum+1]);
				sourcepid=ppnum-1;
			}else{
				pre=pre+mathf.lowerbound(n1,matrix[sourcepid][sourcepid]);
				now=now+mathf.lowerbound(n1-1,nmatrix[sourcepid][sourcepid]);
			}
			if(target<0){
				pre=pre+mathf.lowerbound(n2,this->edges[target+connectcompnum+1]);
				now=now+mathf.lowerbound(n2+1,newedges[target+connectcompnum+1]);
				target=ppnum-1;
			}else{
				pre=pre+mathf.lowerbound(n2,matrix[target][target]);
				now=now+mathf.lowerbound(n2+1,nmatrix[target][target]);
			}
			break;
		case 4:
			if(sourcepid<0){
				pre=pre+mathf.querycost(n1,this->edges[sourcepid+connectcompnum+1]);
				now=now+mathf.querycost(n1-1,newedges[sourcepid+connectcompnum+1]);
				sourcepid=ppnum-1;
			}else{
				pre=pre+mathf.querycost(n1,matrix[sourcepid][sourcepid]);
				now=now+mathf.querycost(n1-1,nmatrix[sourcepid][sourcepid]);
			}
			if(target<0){
				pre=pre+mathf.querycost(n2,this->edges[target+connectcompnum+1]);
				now=now+mathf.querycost(n2+1,newedges[target+connectcompnum+1]);
				target=ppnum-1;
			}else{
				pre=pre+mathf.querycost(n2,matrix[target][target]);
				now=now+mathf.querycost(n2+1,nmatrix[target][target]);
			}
			break;
		case 5:
			if(sourcepid<0){
				pre=pre+mathf.sizecost(n1,this->edges[sourcepid+connectcompnum+1]);
				now=now+mathf.sizecost(n1-1,newedges[sourcepid+connectcompnum+1]);
				sourcepid=ppnum-1;
			}else{
				pre=pre+mathf.sizecost(n1,matrix[sourcepid][sourcepid]);
				now=now+mathf.sizecost(n1-1,nmatrix[sourcepid][sourcepid]);
			}
			if(target<0){
				pre=pre+mathf.sizecost(n2,this->edges[target+connectcompnum+1]);
				now=now+mathf.sizecost(n2+1,newedges[target+connectcompnum+1]);
				target=ppnum-1;
			}else{
				pre=pre+mathf.sizecost(n2,matrix[target][target]);
				now=now+mathf.sizecost(n2+1,nmatrix[target][target]);
			}
			break;
		default:
			cout<<"please choose the cost function -f from {1,2,3,4,5}"<<endl;
			exit(0);

	}
	for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
		int tid=partid[p->adjvex]-this->connectcompnum-1;
		if(tid<0)
			tid=ppnum-1;
		if(tid==sourcepid){
			pre=pre+matrix[target][tid];
			now=now+nmatrix[target][tid];
		}
		else if(tid==targetpid){
			pre=pre+matrix[sourcepid][tid];
			now=now+nmatrix[sourcepid][tid];
		}
		else{
			pre=pre+matrix[sourcepid][tid]+matrix[target][tid];
			now=now+nmatrix[sourcepid][tid]+nmatrix[target][tid];
		}
	}
	for(int i=0;i<(int)g.parents[v].size();i++){
		int w=g.parents[v][i];
		int tid=partid[w]-this->connectcompnum-1;
		if(tid<0)
			tid=ppnum-1;
		if(tid==sourcepid){
			pre=pre+matrix[tid][target];
			now=now+nmatrix[tid][target];
		}
		else if(tid==targetpid){
			pre=pre+matrix[tid][sourcepid];
			now=now+nmatrix[tid][sourcepid];
		}
		else{
			pre=pre+matrix[tid][sourcepid]+matrix[tid][target];
			now=now+nmatrix[tid][sourcepid]+nmatrix[tid][target];
		}
	}
	for(int i=0;i<ppnum;i++){
		delete []nmatrix[i];
		nmatrix[i]=NULL;
	}
	return now-pre;
}


/**
@ refinement partition: 
Input: an initial k-partition G^0;
Output: a better partition G';

**/
double autopart::refinementpartition(Graph &g, int *&partid,int type, int iterationum){
	int ppnum=this->numpart-this->connectcompnum+1;
	int **matrix=new int *[ppnum];
	for(int i=0;i<ppnum;i++)
		matrix[i]=new int[ppnum];
	for(int i=0;i<ppnum;i++)
		for(int j=0;j<ppnum;j++)
			matrix[i][j]=0;
	memset(this->edges,0,sizeof(int)*(this->connectcompnum+1));
	double desencoding=0;
	int round=0;
	do{
		desencoding=0;
		this->partitionmatrix(matrix,g,partid);
		for (int i=0;i<g.sizenode();i++){
			double minencoding=1;
			int sourcepid=partid[i];
			int targetpid=sourcepid;
			for(int k=0;k<=this->numpart,k!=sourcepid;k++){
				if(sourcepid<=this->connectcompnum&&k<=this->connectcompnum)
					continue;
				else{
					int source=sourcepid-this->connectcompnum-1;
					int tempk=k-this->connectcompnum-1;
					if(source<0)
						source=ppnum-1;
					if(tempk<0)
						tempk=ppnum-1;
					if(matrix[source][tempk]>0||matrix[tempk][source]>0){
						double diff=this->encodingcostdiff(g,partid,matrix,type,i,k);
						if(diff<minencoding){
							minencoding=diff;
							targetpid=k;
						}
					}
				}
			}
			if(sourcepid!=targetpid){
				desencoding=desencoding+minencoding;
				this->updatematrix(g,partid,targetpid,i,matrix);
				this->updatepartmap(i,sourcepid,targetpid);
			}
		}
		round++;
	}while(desencoding<0&&round<iterationum);
	double res=this->encodingcost(partid,matrix,type);
	for(int i=0;i<ppnum;i++){
		delete []matrix[i];
		matrix[i]=NULL;
	}
	return res;
}
void autopart::updatepartmap(int v,int sourcepid, int targetpid){
	if(sourcepid!=targetpid){
		for(partmap::iterator pos=this->partindex.lower_bound(sourcepid);pos!=this->partindex.upper_bound(sourcepid);){
			if(pos->second==v){
				partindex.erase(pos++);
				break;
			}
			else
				++pos;
		}
		partindex.insert(make_pair(targetpid,v));
	}
}

int ** autopart::Getmatrix(){
	return this->finalmatrix;
}
partmap autopart::Getmapping(){
	return this->partindex;
}

int autopart::Getpartnum(){
	return this->numpart+1;
}
