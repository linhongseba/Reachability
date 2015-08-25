/**
@ author: Linhong
@ email: smileofice@gmail.com
**/
#ifndef query_h_
#define query_h
#include"readindex.h"
#include"../Autopart/Runtimecounter.h"
class Query{
public:
	int iotimes;
	int globaltime;
	int numlabel;
	double inmemoryquerytime;
	int nodenum;
	int condensednode;
	int partnum;
	int Gsn;
	int *sid;
	int *smap;
	partmap M;
	partmap PID;
	bool *istree;
	vector <ILabel> *Gpintervals;
	vector <int> *GpLins;
	vector <int> *GpLouts;
	vector <ILabel> *Gsintervals;
	vector <int> *GsLins;
	vector <int> *GsLouts;
	int type1;
	int type2;
public:
	Query(const char *graphname, int ttype,int querynum,double thre,int maxbuffersize);
	~Query();
	void Setentryvalue(Entry &u, Entry &target);
	void contaentry(Entry &target, Entry &source);
	void clearindex(Entry &Index);
	bool Isreachable(int u,int v,int ttype,Buffer &b, vector <MyPage> &pagelist);
	bool singleinterval(int u,int v,Buffer &b, vector <MyPage> &pagelist);
	bool singlehopi(int u,int v,Buffer &b, vector <MyPage> &pagelist);
	bool multiindex(int u,int v,Buffer &b, vector <MyPage> &pagelist);
	bool checkhopi(int u,int v, vector<int> &linu, vector<int> &linv, vector<int> &loutu, vector<int> &loutv);
	bool checkinterval(vector<ILabel> &u, vector<ILabel> &v);
	bool checkmulti(Entry &u,Entry &v);
	bool checkGp(int u,int v);
	bool checkGs(int u,int v);
	void retrivalpage(Buffer &b,vector <MyPage> &pagelist,int u, Entry &entryu);
	void accessbuffer(Buffer &b, MyPage &page,vector <MyPage> &pagelist);
	void simulateIO(Buffer &b, vector <MyPage> &pagelist, int u, int v, Entry &uentry, Entry &ventry);
};
Query::Query(const char *graphname, int ttype,int querynum,double thre,int maxbuffersize){
	Utility uti;
	string s(graphname);
	parse doparser;
	doparser.parsecomp(s);
	int *comp=doparser.comp;
	this->nodenum=doparser.nodenum;
	if(ttype==1){
		doparser.parsesketch(s);
		this->Gsn=doparser.Gsn;
		this->partnum=doparser.partnum;
		this->istree=doparser.istree;
		this->Gpintervals=doparser.Gpintervals;
		this->GpLins=doparser.GpLins;
		this->GpLouts=doparser.GpLouts;
		this->Gsintervals=doparser.Gsintervals;
		this->GsLins=doparser.GsLins;
		this->GsLouts=doparser.GsLouts;
		this->sid=doparser.sid;
		this->smap=doparser.smap;
		this->M=doparser.M;
		this->type1=doparser.type1;
		this->type2=doparser.type2;
		this->condensednode=doparser.condensednode;
	}
	Buffer b(maxbuffersize*1024*1024);
	vector <MyPage> pagelist;
	switch(ttype){
		case 1:
			doparser.readmulti(s,pagelist,4*1024);
			break;
		case 2:
			doparser.readhopi(s,pagelist,4*1024);
			break;
		case 3:
			doparser.readinterval(s,pagelist,4*1024);
			break;
		default:
			cout<<"error in parsing command line"<<endl;
			cout<<"please choose t value from {1,2,3}"<<endl;
			exit(0);
	}
	this->PID=doparser.PID;
	this->globaltime=0;
	this->iotimes=0;
	this->inmemoryquerytime=0;
	this->numlabel=0;
	EdgeSet *query=new EdgeSet[querynum];
	cout<<"generate the queries"<<endl;
	for(int i=0;i<querynum;i++){
		int u,v;
		do{
			u=uti.RandInt(0,this->nodenum);
			v=uti.RandInt(0,this->nodenum);
		}while(u==v);
		query[i].startnode=u;
		query[i].endnode=v;
	}
	Runtimecounter RC;
	RC.start();
	for(int i=0;i<querynum;i++){
		int u=query[i].startnode;
		int v=query[i].endnode;
		bool isreach;
		int compu=comp[u];
		int compv=comp[v];
		if(compu==compv)
			isreach=true;
		else
			isreach=this->Isreachable(compu,compv,ttype,b,pagelist);

	}
	RC.stop();
	ofstream outfile("time",ios::app);
	string filename(graphname);
	outfile<<filename<<"\t"<<querynum<<"\t";
	if(ttype==1)
		outfile<<"Multiple"<<"\t";
	else
		if(ttype==2)
			outfile<<"HOPI"<<"\t";
		else
			outfile<<"Interval"<<"\t";
	outfile<<"total time(ms)\t"<<RC.GetRuntime()<<"\t";
	outfile<<"IO number\t"<<this->iotimes<<"\t";
	outfile<<"number of labels accessed\t"<<this->numlabel<<"\t";
	outfile<<"in memory query time (ms)\t"<<this->inmemoryquerytime<<endl;
}
Query::~Query(){
}

void Query::clearindex(Entry &Index){
	Index.intervals.clear();
	Index.Lin.clear();
	Index.Lout.clear();
}
void Query::Setentryvalue(Entry &u,Entry &target){
	u.id=target.id;
	u.intervals=target.intervals;
	u.size=target.size;
	u.pid=target.pid;
	u.Lin=target.Lin;
	u.Lout=target.Lout;
}
void Query::contaentry(Entry &target, Entry &source){
	target.id=source.id;
	for(int i=0;i<(int)source.intervals.size();i++)
		target.intervals.push_back(source.intervals[i]);
	for(int i=0;i<(int)source.Lin.size();i++)
		target.Lin.push_back(source.Lin[i]);
	for(int i=0;i<(int)source.Lout.size();i++)
		target.Lout.push_back(source.Lout[i]);
	target.pid=source.pid;
	target.size=source.size+target.size;
}
void Query::accessbuffer(Buffer &b,MyPage &page,vector <MyPage> &pagelist){
	if(page.bufpos==-1){
		if(b.isfull==false){
			page.time=this->globaltime;
			this->iotimes++;
			b.MyPagelist.push_back(page);
			page.bufpos=(int)b.MyPagelist.size()-1;
			if((int)b.MyPagelist.size()==b.maxsize/1024)
				b.isfull=true;
		}
		else{
			int replaceid=b.insertandreplace(page);
			this->iotimes=this->iotimes+2;
			pagelist[replaceid].bufpos=-1;
			}
	}
	else
		b.MyPagelist[page.bufpos].time=this->globaltime;
	
}
bool Query::checkhopi(int u, int v, vector<int> &linu, vector<int> &linv, vector<int> &loutu, vector<int> &loutv){
	Utility uti;
	if(uti.FindNode(u,linv)!=-1)// u is in Lin of v
		return true;
	else
		if(uti.FindNode(v,loutu)!=-1)//v is in Lout of u
			return true;
		else{
			if(uti.Isinterset(loutu,linv)==true)
				return true;
			else
				return false;
		}
}
bool Query::checkinterval(vector<ILabel> &u, vector<ILabel> &v){
	for(int i=0;i<(int)u.size();i++){
		for(int j=0;j<(int)v.size();j++){
			if(u[i].start<v[j].start&&u[i].end>=v[j].end)
				return true;
		}
	}
	return false;
}
bool Query::checkmulti(Entry &u, Entry &v){
	int pid=u.pid;
	bool flag;
	Runtimecounter RC;
	RC.start();
	if(this->istree[pid]==true)
		flag=this->checkinterval(u.intervals,v.intervals);
	else
		flag=this->checkhopi(u.id,v.id,u.Lin,v.Lin,u.Lout,v.Lout);
	RC.stop();
	this->inmemoryquerytime=this->inmemoryquerytime+RC.GetRuntime();
	return flag;
}
bool Query::checkGp(int u,int v){
	bool flag;
	Runtimecounter RC;
	RC.start();
	if(this->type1==2){
		flag=this->checkhopi(u,v,this->GpLins[u],this->GpLins[v],this->GpLouts[u],this->GpLouts[v]);
		this->numlabel=this->numlabel+(int)this->GpLins[u].size()+(int)this->GpLins[v].size()
			+(int)this->GpLouts[u].size()+(int)this->GpLouts[v].size();
	}
	else{
		flag=this->checkinterval(this->Gpintervals[u],this->Gpintervals[v]);
		this->numlabel=this->numlabel+2*(int)this->Gpintervals[u].size()+2*(int)this->Gpintervals[v].size();
	}
	RC.stop();
	this->inmemoryquerytime=this->inmemoryquerytime+RC.GetRuntime();
	return flag;
}
bool Query::checkGs(int u,int v){
	bool flag;
	Runtimecounter RC;
	RC.start();
	if(this->type2==2){
		flag=this->checkhopi(u,v,this->GsLins[u],this->GsLins[v],this->GsLouts[u],this->GsLouts[v]);
		this->numlabel=this->numlabel+(int)this->GsLins[u].size()+(int)this->GsLins[v].size()
			+(int)this->GsLouts[u].size()+(int)this->GsLouts[v].size();
	}
	else{
		flag=this->checkinterval(this->Gsintervals[u],this->Gsintervals[v]);
		this->numlabel=this->numlabel+2*(int)this->Gsintervals[u].size()+2*(int)this->Gsintervals[v].size();
	}
	RC.stop();
	this->inmemoryquerytime=this->inmemoryquerytime+RC.GetRuntime();
	return flag;
}
void Query::retrivalpage(Buffer &b,vector <MyPage> &pagelist,int u, Entry &entryu){
	for(partmap::iterator pos=this->PID.lower_bound(u);pos!=this->PID.upper_bound(u);++pos){
		if(pos->first==u){
			int pidu=pos->second;
			this->globaltime++;
			this->accessbuffer(b,pagelist[pidu],pagelist);
			pagemap::iterator newpos=pagelist[pidu].pagehead.find(u);
			int uentrycur;
			if(newpos!=pagelist[pidu].pagehead.end())
				uentrycur=newpos->second;
			else{
				cout<<"error occurs in read entries into page"<<endl;
				exit(0);
			}
			Entry source=pagelist[pidu].content[uentrycur];
			this->contaentry(entryu,source);
		}
	}
}
void Query::simulateIO(Buffer &b,vector <MyPage> &pagelist,int u,int v,Entry &uentry,Entry &ventry){
	uentry.size=0;
	ventry.size=0;
	this->retrivalpage(b,pagelist,u,uentry);
	this->retrivalpage(b,pagelist,v,ventry);
}
bool Query::singleinterval(int u, int v,Buffer &b, vector <MyPage> &pagelist){
	Entry uentry,ventry;
	this->simulateIO(b,pagelist,u,v,uentry,ventry);
	Runtimecounter RC;
	RC.start();
	this->numlabel=this->numlabel+2*(int)uentry.intervals.size()+2*(int)ventry.intervals.size();
	bool flag=this->checkinterval(uentry.intervals,ventry.intervals);
	RC.stop();
	this->inmemoryquerytime=this->inmemoryquerytime+RC.GetRuntime();
	return flag;
}
bool Query::singlehopi(int u, int v,Buffer &b, vector <MyPage> &pagelist){
	Entry uentry,ventry;
	this->simulateIO(b,pagelist,u,v,uentry,ventry);
	Runtimecounter RC;
	RC.start();
	this->numlabel=this->numlabel+(int)uentry.Lin.size()+(int)uentry.Lout.size()+(int)ventry.Lout.size()+(int)ventry.Lin.size();
	bool flag=this->checkhopi(u,v,uentry.Lin,ventry.Lin,uentry.Lout,ventry.Lout);
	RC.stop();
	this->inmemoryquerytime=this->inmemoryquerytime+RC.GetRuntime();
	return flag;
}
bool Query::multiindex(int u, int v,Buffer &b, vector <MyPage> &pagelist){
	Entry uentry,ventry;
	this->simulateIO(b,pagelist,u,v,uentry,ventry);
	if(uentry.pid!=ventry.pid){
		int pi1=uentry.pid;
		int pi2=ventry.pid;
		bool flag=this->checkGp(pi1,pi2);//check whether the two partitions are reachable
		if(flag==false)   //if not reachable====> u and v are not reachable
			return false;
		else{
			for(partmap::iterator pos1=this->M.lower_bound(pi1);pos1!=this->M.upper_bound(pi1);++pos1){
				for(partmap::iterator pos2=this->M.lower_bound(pi2);pos2!=this->M.upper_bound(pi2);++pos2){
					if(this->checkGs(pos1->second,pos2->second)==true){
						int x=this->smap[pos1->second];
						int y=this->smap[pos2->second];
						Entry xentry,yentry;
						this->simulateIO(b,pagelist,x,y,xentry,yentry);
						if(this->checkmulti(uentry,xentry)==true||this->checkmulti(yentry,ventry)==true)
							return true;
					}
				}
			}
			return false;
		}
	}
	else{//if u and v are in the same partition, check the interval or hopi label of u and 
		this->numlabel=this->numlabel+2*(int)uentry.intervals.size()+(int)uentry.Lin.size()
			+(int)uentry.Lout.size()+2*(int)ventry.intervals.size()+(int)ventry.Lin.size()+(int)ventry.Lout.size();
		return this->checkmulti(uentry,ventry);
	}
}

bool Query::Isreachable(int u, int v, int ttype,Buffer &b, vector <MyPage> &pagelist){
	switch(ttype){
		case 1:
			this->multiindex(u,v,b,pagelist);
			break;
		case 2:
			this->singlehopi(u,v,b,pagelist);
			break;
		case 3:
			this->singleinterval(u,v,b,pagelist);
			break;
		default:
			cout<<"error in parsing command line"<<endl;
			cout<<"please choose t value from {1,2,3}"<<endl;
			exit(0);
	}
	return false;
}

#endif
