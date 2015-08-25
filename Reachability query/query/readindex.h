/**
@ author: Linhong
@ email: smileofice@gmail.com
**/
#ifndef readindex_h_
#define readindex_h_
#include"Buffer.h"
#include"../Autopart/Utility.h"

class parse{
public:
	int condensednode;
	int *comp;
	int nodenum;
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
	parse();
	~parse();
	void parsemulti(char *buf,Entry &Index,bool *&istree);
	void parsecomp(string graphname);
	void parsesketch(string graphname);
	void parsehopi(char *buf, vector<int> &u, vector<int> &v);
	void parseinterval(char *buf, vector<ILabel> &u);
	void readinterval (string graphname, vector <MyPage> &pagelist,int maxpage);
	void readhopi(string graphname, vector <MyPage> &pagelist,int maxpage);
	void readmulti(string graphname,vector <MyPage> &pagelist,int maxpage);
	void readmemoryinterval(string graphname, vector <ILabel> *&intervals);
	void readmemoryhopi(string graphname, vector <int> *&Lins, vector <int> *&Louts);
};

parse::parse(){
}
parse::~parse(){
}
void parse::readmemoryhopi(string graphname, vector <int> *&Lins, vector <int> *&Louts){
	string indexfile="Indexes\\"+graphname;
	indexfile=indexfile+".index_HOPI.txt";
	ifstream infile;
	infile.open(indexfile.c_str());
	if(infile.is_open()==false){
		cout<<"can not find the files"<<endl;
		cout<<"please check the path of the files"<<endl;
		exit(0);
	}
	char *buf=new char[20000];
	int id=0;
	while(infile.getline(buf,20000)){
		this->parsehopi(buf,Lins[id],Louts[id]);
		id++;
	}
	infile.close();
	infile.clear();
}
void parse::readmemoryinterval(std::string graphname, std::vector<ILabel> *&intervals){
	string indexfile="Indexes\\"+graphname;
	indexfile=indexfile+".index_Interval.txt";
	ifstream infile;
	infile.open(indexfile.c_str());
	if(infile.is_open()==false){
		cout<<"can not find the files"<<endl;
		cout<<"please check the path of the files"<<endl;
		exit(0);
	}
	char *buf=new char[20000];
	int id=0;
	while(infile.getline(buf,20000)){
		this->parseinterval(buf,intervals[id]);
		id++;
	}
	infile.close();
	infile.clear();
}
void parse::readhopi(string graphname, vector <MyPage> &pagelist,int maxpage){
	Utility uti;
	string indexfile="Indexes\\"+graphname;
	indexfile=indexfile+".index_HOPI.txt";
	ifstream infile;
	infile.open(indexfile.c_str());
	if(infile.is_open()==false){
		cout<<"can not find the files"<<endl;
		cout<<"please check the path of the files"<<endl;
		exit(0);
	}
	char *buf=new char[20000];
	int pagenum=0;
	MyPage page(pagenum,maxpage);
	int id=0;
	while(infile.getline(buf,20000)){
		Entry u;
		this->parsehopi(buf,u.Lin,u.Lout);
		u.id=id;
		u.size=(int)u.Lin.size()+(int)u.Lout.size();
		vector <Entry> decu;
		if(u.size<=page.maxsize){
			decu.push_back(u);
		}
		else{
			Entry v;
			v.id=id;
			v.size=0;
			for(int i=0;i<(int)u.Lin.size();i++){
				while(v.size<page.maxsize){
					v.Lin.push_back(u.Lin[i]);
					v.size++;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
			for(int i=0;i<(int)u.Lout.size();i++){
				while(v.size<page.maxsize){
					v.Lout.push_back(v.Lout[i]);
					v.size++;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
		}
		for(int i=0;i<(int)decu.size();i++){
			if(page.Insertentry(decu[i])==true)
				this->PID.insert(make_pair(id,pagenum));
			else{
				pagelist.push_back(page);
				pagenum++;
				page.resetcontent();
				page.resetid(pagenum);
				page.Insertentry(u);
				this->PID.insert(make_pair(id,pagenum));
			}
		}
		id++;
	}
	if(uti.FindPage(page,pagelist)==false)
		pagelist.push_back(page);
	infile.close();
	infile.clear();
}
void parse::readmulti(std::string graphname, vector <MyPage> &pagelist,int maxpage){
	Utility uti;
	string indexfile="Indexes\\"+graphname;
	indexfile=indexfile+"_multi.txt";
	ifstream infile;
	infile.open(indexfile.c_str());
	if(infile.is_open()==false){
		cout<<"can not find the files"<<endl;
		cout<<"please check the path of the files"<<endl;
		exit(0);
	}
	char *buf=new char[20000];
	int pagenum=0;
	MyPage page(pagenum,maxpage);
	int id=0;
	while(infile.getline(buf,20000)){
		Entry u;
		u.id=id;
		this->parsemulti(buf,u,this->istree);
		u.size=(int)u.Lin.size()+(int)u.Lout.size()+2*(int)u.intervals.size();
		vector <Entry> decu;
		if(u.size<=page.maxsize){
			decu.push_back(u);
		}
		else{
			Entry v;
			v.id=id;
			v.size=0;
			for(int i=0;i<(int)u.intervals.size();i++){
				while(v.size<page.maxsize){
					v.intervals.push_back(u.intervals[i]);
					v.size=v.size+2;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
			for(int i=0;i<(int)u.Lin.size();i++){
				while(v.size<page.maxsize){
					v.Lin.push_back(u.Lin[i]);
					v.size++;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
			for(int i=0;i<(int)u.Lout.size();i++){
				while(v.size<page.maxsize){
					v.Lout.push_back(v.Lout[i]);
					v.size++;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
		}
		for(int i=0;i<(int)decu.size();i++){
			if(page.Insertentry(decu[i])==true)
				this->PID.insert(make_pair(id,pagenum));
			else{
				pagelist.push_back(page);
				pagenum++;
				page.resetcontent();
				page.resetid(pagenum);
				page.Insertentry(u);
				this->PID.insert(make_pair(id,pagenum));
			}
		}
		id++;
	}
	if(uti.FindPage(page,pagelist)==false)
		pagelist.push_back(page);
	infile.close();
	infile.clear();
}
void parse::readinterval(std::string graphname,vector <MyPage> &pagelist,int maxpage){
	Utility uti;
	string indexfile="Indexes\\"+graphname;
	indexfile=indexfile+".index_Interval.txt";
	ifstream infile;
	infile.open(indexfile.c_str());
	if(infile.is_open()==false){
		cout<<"can not find the files"<<endl;
		cout<<"please check the path of the files"<<endl;
		exit(0);
	}
	char *buf=new char[20000];
	int pagenum=0;
	MyPage page(pagenum,maxpage);
	int id=0;
	while(infile.getline(buf,20000)){
		Entry u;
		this->parseinterval(buf,u.intervals);
		u.id=id;
		u.size=2*(int)u.intervals.size();
		vector <Entry> decu;
		if(u.size<=page.maxsize){
			decu.push_back(u);
		}
		else{
			Entry v;
			v.id=id;
			v.size=0;
			for(int i=0;i<(int)u.intervals.size();i++){
				while(v.size<page.maxsize){
					v.intervals.push_back(u.intervals[i]);
					v.size=v.size+2;
				}
				if(v.size==page.maxsize){
					decu.push_back(v);
					v.size=0;
				}
			}
		}
		for(int i=0;i<(int)decu.size();i++){
			if(page.Insertentry(decu[i])==true)
				this->PID.insert(make_pair(id,pagenum));
			else{
				pagelist.push_back(page);
				pagenum++;
				page.resetcontent();
				page.resetid(pagenum);
				page.Insertentry(u);
				this->PID.insert(make_pair(id,pagenum));
			}
		}
		id++;
	}
	if(uti.FindPage(page,pagelist)==false)
		pagelist.push_back(page);
	infile.close();
	infile.clear();
}
void parse::parsecomp(string graphname){
	ifstream infile;
	string s="Indexes\\"+graphname+".comp";
	infile.open(s.c_str());
	if(infile.is_open()==false){
		cout<<"can not open the SCC index files"<<endl;
		exit(0);
	}
	infile>>this->nodenum;
	if(this->nodenum<=0){
		cout<<"error exits during write the SCC index files"<<endl;
		exit(0);
	}
	this->comp=new int[this->nodenum];
	for(int i=0;i<this->nodenum;i++)
		infile>>comp[i];
}
void parse::parsehopi(char *buf, vector<int> &u, vector<int> &v){
	char *pch=strtok(buf," ");
	bool flag=false;
	while(pch!=NULL){
		if(strcmp(pch,":")==0)
			flag=true;
		else{
			if(flag==false){
				int c=atoi(pch);
				u.push_back(c);
			}
			else{
				int c=atoi(pch);
				v.push_back(c);
			}
		}
		pch=strtok(NULL," ");
	}
}
void parse::parseinterval(char *buf, vector<ILabel> &u){
	char *pch;
	int count=0;
	int start=-1;
	int end=-1;
	pch=strtok(buf," ");
	while(pch!=NULL){
		if(count%2==0){
			if(start!=-1){
				ILabel s;
				s.start=start;
				s.end=end;
				u.push_back(s);
			}
			start=atoi(pch);
		}
		else{
			end=atoi(pch);
		}
		pch=strtok(NULL," ");
		count++;
	}
}
void parse::parsesketch(string graphname){
	string ssid="Indexes\\"+graphname+".sid";
	string ssmap="Indexes\\"+graphname+".smap";
	string sM="Indexes\\"+graphname+".M";
	ifstream infile;

	infile.open(ssid.c_str());
	if(infile.is_open()==false){
		cout<<"can not open the Sid index files"<<endl;
		exit(0);
	}
	infile>>this->condensednode;
	this->sid=new int[this->condensednode];
	for(int i=0;i<this->condensednode;i++)
		infile>>this->sid[i];
	infile.close();
	infile.clear();

	infile.open(ssmap.c_str());
	if(infile.is_open()==false){
		cout<<"can not open the Smap index files"<<endl;
		exit(0);
	}
	infile>>this->Gsn;
	if(this->Gsn<0){
		cout<<"error exits during write the Smap index files"<<endl;
		exit(0);
	}
	if(this->Gsn==0)
		this->smap=NULL;
	else{
		this->smap=new int[this->Gsn];
		for(int i=0;i<Gsn;i++)
			infile>>this->smap[i];
	}
	infile.close();
	infile.clear();
	infile.open(sM.c_str());
	if(infile.is_open()==false){
		cout<<"can not open the M index files"<<endl;
		exit(0);
	}
	infile>>this->partnum;
	this->istree=new bool[this->partnum];
	for(int i=0;i<this->partnum;i++){
		infile>>this->istree[i];
	}
	infile>>this->type1;
	infile>>this->type2;
	int u,v;
	for(int i=0;i<this->Gsn;i++){
		infile>>u;
		infile>>v;
		this->M.insert(make_pair(u,v));
	}
	string name1=graphname+"_Gp";
	string name2=graphname+"_Gs";
	if(type1==2){
		this->GpLins=new vector <int>[this->partnum];
		this->GpLouts=new vector <int>[this->partnum];
		this->readmemoryhopi(name1,this->GpLins,this->GpLouts);
		this->Gpintervals=NULL;
	}else{
		this->Gpintervals=new vector <ILabel>[this->partnum];
		this->readmemoryinterval(name1,this->Gpintervals);
		this->GpLins=NULL;
		this->GpLouts=NULL;
	}
	if(type2==2){
		this->GsLins=new vector <int>[this->Gsn];
		this->GsLouts=new vector <int>[this->Gsn];
		this->readmemoryhopi(name2,this->GsLins,this->GsLouts);
		this->Gsintervals=NULL;
	}else{
		this->GsLins=NULL;
		this->GsLouts=NULL;
		this->Gsintervals=new vector <ILabel>[this->Gsn];
		this->readmemoryinterval(name2,this->Gsintervals);
	}
}
void parse::parsemulti(char *buf, Entry &Index, bool *&istree){
	char *pch=strtok(buf," ");
	Index.pid=atoi(pch);
	if(istree[Index.pid]==true){
		int count=0;
		int start=-1;
		int end=-1;
		pch=strtok(NULL," ");
		while(pch!=NULL){
			if(count%2==0){
				if(start!=-1){
					ILabel s;
					s.start=start;
					s.end=end;
					Index.intervals.push_back(s);
				}
				start=atoi(pch);
			}
			else{
				end=atoi(pch);
			}
			pch=strtok(NULL," ");
			count++;
		}
	}
	else{
		pch=strtok(NULL," ");
		bool flag=false;
		while(pch!=NULL){
			if(strcmp(pch,":")==0)
				flag=true;
			else{
				if(flag==false){
					int c=atoi(pch);
					Index.Lin.push_back(c);
				}
				else{
					int c=atoi(pch);
					Index.Lout.push_back(c);
				}
			}
			pch=strtok(NULL," ");
		}
	}
}

#endif

