/**
@ author: Linhong
@ email: smileofice@gmail.com
**/
#ifndef indexing_h_
#define indexing_h_
#include"../Autopart/Runtimecounter.h"
#include"../Autopart/DFSAPP.h"
Utility uti;
class indexing{
private:
	int indexsize;
	double constructtime;
	vector <int> *Lin;
	vector <int> *Lout;
	vector <ILabel> *intervallabels;
public:
	indexing();
	~indexing();
	void interval(Graph &g);
	void hopi(string graphname);
	void subinterval(Graph &g,int pid,partmap &partindex,int *&partid);
	void subhopi(Graph &g,int pid,partmap &partindex,int *&partid,string graphname);
	void singleindex(Graph &g,int type, string graphname);
	void multipleindex(int k, bool *&istree,Graph &g,partmap &partindex,int *&partid,string graphname);
	void Printintervalindexfile(string graphname,int nodenum);
	void Printhopiindexfile(string graphname,int nodenum);
	void Printmultiindexfile(string graphname,int nodenum,bool *&istree,int*&partid);
	double readtimefromlogfile(string graphname);
	double returnruntime();
	int returnindexsize();
};

indexing::indexing(){
}
indexing::~indexing(){
}
void indexing::interval(Graph &g){
	DFSAPP app;
	Utility uti;
	app.dfstime=0;
	app.count=g.sizenode()-1;
	app.topologicalorder=new int[g.sizenode()];
	for(int i=0;i<g.sizenode();i++)
		g.vertices[i].color='w';
	for(int i=0;i<g.sizenode();i++){
		if(g.vertices[i].color=='w')
			app.singleDFS(g,i);
		ILabel l;
		l.start=g.vertices[i].start;
		l.end=g.vertices[i].finish;
		intervallabels[i].push_back(l);
		this->indexsize=this->indexsize+2;
	}
	if(g.sizenode()!=g.sizeedge()+1){
		for(int i=g.sizenode()-1;i>=0;i--){
			int u=app.topologicalorder[i];
			for(ArcNode *p=g.vertices[u].firstarc;p!=NULL;p=p->nextarc){
				bool flag=true;
				for(int k=0;k<(int)intervallabels[u].size();k++){
					if(intervallabels[u][k].start<g.vertices[p->adjvex].start&&intervallabels[u][k].end>=g.vertices[p->adjvex].finish){
						flag=false;
						break;
					}
				}
				if(flag==false)
					;
				else{
					ILabel temp;
					temp.start=g.vertices[p->adjvex].start;
					temp.end=g.vertices[p->adjvex].finish;
					if(uti.FindLabel(temp,intervallabels[u])==false)
						intervallabels[u].push_back(temp);
					this->indexsize=this->indexsize+2;
					}
			}
		}
	}
	app.Reset(g);
	delete []app.topologicalorder;
	app.topologicalorder=NULL;
}
void indexing::subinterval(Graph &g,int pid,partmap &partindex,int *&partid){
	DFSAPP app;
	app.dfstime=0;
	int nodenum=(int)partindex.count(pid);
	app.count=nodenum-1;
	app.topologicalorder=new int[nodenum];
	for(partmap::iterator pos=partindex.lower_bound(pid);pos!=partindex.upper_bound(pid);++pos){
		int v=pos->second;
		if(g.vertices[v].color=='w')
			app.colorDFS(g,partid,v);
		ILabel l;
		l.start=g.vertices[v].start;
		l.end=g.vertices[v].finish;
		intervallabels[v].push_back(l);
	}
	for(int i=nodenum-1;i>0;i--){
		int u=app.topologicalorder[i];
		for(ArcNode *p=g.vertices[u].firstarc;p!=NULL;p=p->nextarc){
			if(partid[p->adjvex]==pid){
					bool flag=true;
					for(int k=0;k<(int)intervallabels[u].size();k++){
						if(intervallabels[u][k].start<g.vertices[p->adjvex].start&&intervallabels[u][k].end>=g.vertices[p->adjvex].finish){
							flag=false;
							break;
						}
					}
					if(flag==false)
						;
					else{
						ILabel temp;
						temp.start=g.vertices[p->adjvex].start;
						temp.end=g.vertices[p->adjvex].finish;
						if(uti.FindLabel(temp,intervallabels[u])==false){
							intervallabels[u].push_back(temp);
							this->indexsize=this->indexsize+2;
						}
					}
				}
			}
	}
	delete []app.topologicalorder;
	app.topologicalorder=NULL;
}
void indexing::singleindex(Graph &g,int type, string graphname){
	this->indexsize=0;
	this->constructtime=0;
	Runtimecounter RC;
	string indexfile(".index");
	indexfile="Indexes\\"+graphname+indexfile;
	if(type==3){
		RC.start();
		this->intervallabels=new vector<ILabel>[g.sizenode()];
		this->Lin=NULL;
		this->Lout=NULL;
		this->interval(g);
		RC.stop();
		this->constructtime=this->constructtime+RC.GetRuntime();
		this->Printintervalindexfile(indexfile,g.sizenode());
	}
	else
		if(type==2){
			this->Lin=new vector<int>[g.sizenode()];
			this->Lout=new vector<int>[g.sizenode()];
			this->intervallabels=NULL;
			ofstream outfile("bin\\.dagedge");
			for(int i=0;i<g.sizenode();i++){
				for(ArcNode *p=g.vertices[i].firstarc;p!=NULL;p=p->nextarc){
					outfile<<i+1<<" "<<p->adjvex+1<<" "<<1<<endl;    //hopi read the graph where id start at 1
				}
			}
			this->hopi(graphname);
			this->Printhopiindexfile(indexfile,g.sizenode());
		}
		else{
			cout<<"error occures during parsing command line"<<endl;
			cout<<"please choose -t value from {1,2,3}"<<endl;
			exit(0);
		}
}
void indexing::hopi(string graphname){
	string s="command.bat "+graphname;
	system(s.c_str());
	ifstream infile;
	infile.open("bin\\.hops");
	if(!infile.is_open()){
		cout<<"An error occurs when constructing the hopi labels"<<endl;
		exit(0);
	}
	char *buf=new char[50];
	int linenum=0;
	while(infile.getline(buf,50))
		linenum++;
	infile.close();
	infile.clear();
	infile.open("bin\\.hops");
	if(!infile.is_open()){
		cout<<"can not read the hops files"<<endl;
		exit(0);
	}
	for(int i=0;i<linenum;i++){
		int u, v,w;
		infile>>u;
		infile>>v;
		infile>>w;
		if(u==v){
			Lin[w-1].push_back(u-1);
			this->indexsize=this->indexsize+2;
		}
		else
			if(u==w){
				Lout[v-1].push_back(u-1);
				this->indexsize=this->indexsize+2;
			}
			else{
				cout<<"An error occurs when constructing the hopi labels"<<endl;
				exit(0);
			}
	}
	this->constructtime=this->constructtime+this->readtimefromlogfile(graphname);
	string ss="del bin\\"+graphname+".log";
	system(ss.c_str());
}
void indexing::subhopi(Graph &g,int pid,partmap &partindex,int *&partid,string graphname){
	ofstream outfile("bin\\.dagedge");
	for(partmap::iterator pos=partindex.lower_bound(pid);pos!=partindex.upper_bound(pid);++pos){
		int v=pos->second;
		for(ArcNode *p=g.vertices[v].firstarc;p!=NULL;p=p->nextarc){
			if(partid[p->adjvex]==pid){
				outfile<<v+1<<" "<<p->adjvex+1<<" "<<1<<endl; 
			}
		}
	}
	this->hopi(graphname);
}
void indexing::multipleindex(int k, bool *&istree,Graph &g,partmap &partindex,int *&partid,string graphname){
	this->constructtime=0;
	this->indexsize=0;
	this->Lin=new vector<int>[g.sizenode()];
	this->Lout=new vector<int>[g.sizenode()];
	this->intervallabels=new vector<ILabel>[g.sizenode()];
	Runtimecounter RC;
	for(int i=0;i<k;i++){
		if(istree[i]==true){
			RC.start();
			this->subinterval(g,i,partindex,partid);
			RC.stop();
			this->constructtime=this->constructtime+RC.GetRuntime();
		}else{
			this->subhopi(g,i,partindex,partid,graphname);
		}
	}
}
void indexing::Printintervalindexfile(string graphname,int nodenum){
	string indexfile=graphname+"_Interval.txt";
	ofstream output(indexfile.c_str());
	for(int i=0;i<nodenum;i++){
		for(int j=0;j<(int)intervallabels[i].size();j++){
			output<<intervallabels[i][j].start<<" "<<intervallabels[i][j].end<<" ";
		}
		output<<endl;
	}
}
void indexing::Printhopiindexfile(string graphname,int nodenum){
	string indexfile=graphname+"_HOPI.txt";
	ofstream output(indexfile.c_str());
	for(int i=0;i<nodenum;i++){
		for(int j=0;j<(int)Lin[i].size();j++){
			output<<Lin[i][j]<<" ";
		}
		output<<":";
		for(int j=0;j<(int)Lout[i].size();j++){
			output<<" "<<Lout[i][j];
		}
		output<<endl;
	}
}
void indexing::Printmultiindexfile(string graphname,int nodenum,bool *&istree,int*&partid){
	string indexfile="Indexes\\"+graphname+"_multi.txt";
	ofstream output(indexfile.c_str());
	for(int i=0;i<nodenum;i++){
		int pid=partid[i];
		output<<pid<<" ";
		if(istree[pid]==true){
			for(int j=0;j<(int)this->intervallabels[i].size();j++)
				output<<intervallabels[i][j].start<<" "<<intervallabels[i][j].end<<" ";
		}else{
			for(int j=0;j<(int)Lin[i].size();j++)
				output<<Lin[i][j]<<" ";
			output<<":";
			for(int j=0;j<(int)Lout[i].size();j++)
				output<<" "<<Lout[i][j];
		}
		output<<endl;
	}
}
double indexing::readtimefromlogfile(string graphname){
	string s="bin\\"+graphname+".log";
	ifstream infile;
	double t=0;
	infile.open(s.c_str());
	if(!infile.is_open()){
		return 0;
	}
	else{
		char ch;
		while(infile.get(ch)){
			if(ch==':'){
				vector <char> ss;
				infile.get(ch);
				while(infile.get(ch)&&ch!=' ')
					ss.push_back(ch);
				char *tt=new char[(int)ss.size()];
				for(int i=0;i<(int)ss.size();i++)
					tt[i]=ss[i];
				double t1=atof(tt);
				t=t+t1;
			}
		}
	return 1000*t;
	}
}
double indexing::returnruntime(){
	return this->constructtime;
}
int indexing::returnindexsize(){
	return this->indexsize;
}
#endif

