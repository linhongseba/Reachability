#ifndef indexframework_h_
#define indexframework_h_
#include"indexing.h"
#include"skeleton.h"
#include"../Autopart/autopart.h"
#include"../Autopart/partstatistics.h"
class indexframework{
public:
	double indextime;
	int indexsize;
public:
	indexframework();
	void doindexframework(Graph &g, const char *graphname,double thres,int iterationnum,int htype);
	~indexframework();
};

indexframework::indexframework(){
}
indexframework::~indexframework(){
}
void indexframework::doindexframework(Graph &g, const char *graphname,double thres,int iterationnum, int htype){
	string s(graphname);
	//do graph partitioning..........
	cout<<"doing graph partitioning"<<endl;
	int *partid=new int[g.sizenode()];
	for(int i=0;i<g.sizenode();i++)
		partid[i]=-1;
	Runtimecounter RC;
	RC.start();
	autopart p(g,partid,htype,iterationnum);
	int **matrix=p.Getmatrix();
	partmap partindex=p.Getmapping();
	int k=p.Getpartnum();
	string spid="Indexes\\"+s+".pid";
	ofstream outpid(spid.c_str());
	for(int i=0;i<g.sizenode();i++)
		outpid<<partid[i]<<endl;
	RC.stop();
	cout<<"end of graph partitioning"<<endl;

	//analyze the partitioning results
	cout<<"analyzing the partitioning results"<<endl;
	statispart statis(g,partid,k,partindex,matrix,thres);
	bool *istree=statis.Getistreeinfo();
	int treenumber=statis.Gettreenum();
	int graphnumber=statis.Getgraphnum();
	double maxdensity=statis.Getmaxdensity();
	double mindensity=statis.Getmindensity();
	double originaldensity=statis.Getoriginaldensity(g);
	vector <EdgeSet> Ei=statis.Getinterpartedges();
	int interedgenum=(int)Ei.size();
	int *sid=statis.GetSID();
	int *smap=statis.GetSMAP();
	int Gsn=statis.GetnodenumofGs();
	cout<<"number of interpartition edges"<<interedgenum<<endl;
	string ssid="Indexes\\"+s+".sid";
	string ssmap="Indexes\\"+s+".smap";
	ofstream outsid(ssid.c_str());
	ofstream outsmap(ssmap.c_str());
	outsmap<<Gsn<<endl;
	for(int i=0;i<Gsn;i++)
		outsmap<<smap[i]<<endl;
	outsid<<g.sizenode()<<endl;
	for(int i=0;i<g.sizenode();i++)
		outsid<<sid[i]<<endl;


	//output the partition results
	ofstream outfile("partresult.txt");
	outfile<<s<<"\t";
	outfile<<"partition time\t"<<RC.GetRuntime()<<"\t";
	outfile<<"number of tree-like subgraphs\t"<<treenumber<<"\t";
	outfile<<"number of graph-like subgraphs\t"<<graphnumber<<"\t";
	outfile<<"number of inter-partition edges\t"<<interedgenum<<"\t";
	outfile<<"min non-tree density\t"<<mindensity<<"\t";
	outfile<<"max non-tree density\t"<<maxdensity<<"\t";
	outfile<<"density of input graph\t"<<originaldensity<<endl;
	
	//indexing P(G)
	this->indexsize=0;
	this->indextime=0;
	cout<<"build reachability labeling in each partition in P(G)"<<endl;
	indexing BI;
	BI.multipleindex(k,istree,g,partindex,partid,s);
	BI.Printmultiindexfile(s,g.sizenode(),istree,partid);
	indexsize=indexsize+BI.returnindexsize();
	indextime=indextime+BI.returnruntime();
	cout<<"end of indexing on P(G)"<<endl;

	//do partition-level graph and partition-level skeleton graph construction
	cout<<"build the partition-level graph Gp"<<endl;
	cout<<"build the partition-level skeleton graph Gs"<<endl;
	skeleton sketch(partid,k,matrix,Ei,Gsn,sid,smap);
	Graph plg=sketch.GetPLG();
	Graph plsg=sketch.GetPLSG();
	partmap M=sketch.Getmapping();
	string sM="Indexes\\"+s+".M";
	ofstream outM(sM.c_str());
	outM<<k<<endl;
	for(int i=0;i<k;i++)
		outM<<istree[i]<<endl;
	//indexing PLG Gp and PLSG Gs
	cout<<"build the reachability lableing on Gp and Gs"<<endl;
	mymath mathf;
	double density1=mathf.nontreedensity(plg.sizenode(),plg.sizeedge());
	double density2=mathf.nontreedensity(plsg.sizenode(),plsg.sizeedge());
	string name1=s+"_Gp";
	string name2=s+"_Gs";
	int type1=2;
	int type2=2;
	if(density1<=thres)
		type1=3;
	if(density2<=thres)
		type2=3;
	BI.singleindex(plg,type1,name1);
	indexsize=indexsize+BI.returnindexsize();
	indextime=indextime+BI.returnruntime();
	BI.singleindex(plsg,type2,name2);
	indexsize=indexsize+BI.returnindexsize();
	indextime=indextime+BI.returnruntime();
	outM<<type1<<endl;
	outM<<type2<<endl;
	for(partmap::iterator pos=M.begin();pos!=M.end();++pos){
		outM<<pos->first<<" "<<pos->second<<endl;
	}
}

#endif

