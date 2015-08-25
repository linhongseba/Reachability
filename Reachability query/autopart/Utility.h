/**
@ author: Linhong
@ email: smileofice@gmail.com
Utility: implements all the basic operations, such as random number generator, 
sets intersetion, finding nodes, edges or strings from a vector, and find, insert or delete a pointer from link list
**/
#ifndef Utility_h_
#define Utility_h_
#include <fstream>
#include <time.h>
#include "Page.h"
using namespace std;
typedef  std::multimap <string, int> Mytable;
typedef  std::multimap <int, int> partmap;
Mytable labeltable;
struct ArcNode{
        int	adjvex;
        ArcNode	*nextarc;
};

struct EdgeSet{
	int startnode;
	int endnode;
	
};
struct VNode{
	string data;
	ArcNode *firstarc;
	int start;
	int finish;
	char color;
};
class Utility{
public:
	Utility();
	~Utility();
	int RandInt(int min, int max); //random generate an integer between min and max
	double randdouble();
	int FindEdge(EdgeSet temp, vector<EdgeSet> &Edges);
	bool FindLabel(ILabel temp, vector<ILabel> &labels);
	int  FindNode(int v, vector <int> &nodes);
	int FindNode(int v,int *nodes,int length);
	bool FindPage(MyPage &p, vector <MyPage> &pagelist);
	bool Isequal (string s, char a);
	int Findstring(string s, vector<string> &stringlist);// whether the stringlist contains string s
	bool Iscontain(vector <string> &L, vector <string> &S);// whether set S is in set L
	vector <int> setminus(vector <int> &set1, vector <int> &set2); //return set1-set2
	bool Isinterset(vector <int> &set1, vector<int>&set2); //test whether two sets intersection
	bool containment(ILabel u, vector<ILabel> &v, int pos1,int pos2);
	void setverticesvalue(VNode &v1, VNode &v2);
	bool Delete(VNode &v1, int v);//delete the  v from the adjacent list of v1
	bool Insert(VNode &v1, int v);//add an ArcNode v into the adjacent list of v1
	ArcNode * Previous(VNode &v1,ArcNode *v);//return the previous ArcNode of v1 in the adjacent list of v1
};

Utility::Utility(){
}
double Utility::randdouble(){
	double a=(double)rand();
	double b=(double)rand();
	return a/(a+b);
}
int Utility::FindEdge(EdgeSet temp, vector<EdgeSet> &Edges){
	int index=0;
	while(index<(int)Edges.size()){
		if(Edges[index].startnode==temp.startnode&&Edges[index].endnode==temp.endnode)
			return index;
		else
			index++;
	}
	return -1;
}
bool Utility::FindLabel(ILabel temp,vector <ILabel>&labels){
	int index=0;
	while(index<(int)labels.size()){
		if(labels[index].start==temp.start&&labels[index].end==temp.end)
			return true;
		else
			index++;
	}
	return false;
}
bool Utility::FindPage(MyPage &p, vector <MyPage> &pagelist){
	int i=0;
	while(i<(int)pagelist.size()){
		if(pagelist[i].ID==p.ID)
			return true;
		else
			i++;
	}
	return false;
}
bool Utility::containment(ILabel u, vector<ILabel> &v,int pos1,int pos2){
	int m=(pos2-pos1)/2;
	if(u.start<v[m].start&&u.end>=v[m].end)
		return true;
	else{
		if(u.start>v[m].start)
			return containment(u,v,pos1,m);
		if(u.end<v[m].end)
			return containment(u,v,m,pos2);
	}
	return false;
}
int Utility::FindNode(int v, std::vector<int> &nodes){
	int index=0;
	while(index<(int)nodes.size()){
		if(nodes[index]==v)
			return index;
		else
			index++;
	}
	return -1;
}
int Utility::FindNode(int v, int *nodes,int length){
	int index=0;
	while(index<length){
		if(nodes[index]==v)
			return index;
		else
			index++;
	}
	return -1;
}
int Utility::Findstring(string s,vector<string> &stringlist){
	int index=0;
	while(index<(int)stringlist.size()){
		if(stringlist[index]==s)
			return index;
		else
			index++;
	}
	return -1;
}
bool Utility::Iscontain(vector<string> &L, vector<string> &S){
	if((int)S.size()>(int)L.size())
		return false;
	else{
		int length=0;
		for(int i=0;i<(int)S.size();i++){
			if(Findstring(S[i],L)>0)
				length++;
		}
		if(length==(int)S.size())
			return true;
		else
			return false;
	}
}

bool Utility::Isequal(string s, char a){
	if(s.length()!=1)
		return false;
	else{
		char t=s[0];
		if(t==a)
			return true;
		else
			return false;
	}
}
int Utility::RandInt(int min, int max){
	int temp=rand()%max+min;
	return temp;
}
vector <int> Utility::setminus(std::vector<int> &set1, std::vector<int> &set2){
	vector <int> result;
	for(int i=0;i<(int)set1.size();i++){
		if(this->FindNode(set1[i],set2)<0)
			result.push_back(set1[i]);
	}
	return result;
}
ArcNode *Utility::Previous(VNode &v1, ArcNode *v){
	 ArcNode *temp=v1.firstarc;
	 if(v==v1.firstarc) { //special case, index IS the head :)
		 return v1.firstarc;
	 }
	 while(temp->nextarc != v){
		 temp=temp->nextarc;
	 }
	 return temp;
}
bool Utility::Delete(VNode &v1, int v){
	ArcNode *corpse=v1.firstarc;
	while(corpse!=NULL){
		if(corpse->adjvex==v)
			break;
		else
			corpse=corpse->nextarc;
	}
	ArcNode *temp;
	ArcNode *tail=v1.firstarc;
	while(tail!=NULL&&tail->nextarc!=NULL)
		tail=tail->nextarc;
	if(corpse!=NULL){
		if(corpse == v1.firstarc) {//case 1 corpse = Head
			temp=v1.firstarc;
			v1.firstarc=v1.firstarc->nextarc;
			delete temp;
		}
		else if(corpse ==tail ) { //case 2 corpse is at the end
			temp = tail;
			tail=this->Previous(v1,tail);
			tail->nextarc=NULL;
			delete temp;
		} else{//case 3 corpse is in middle somewhere
			temp=this->Previous(v1,corpse);
			temp->nextarc=corpse->nextarc;
			delete corpse;
		}
		return true;
	}
	else
		return false;
}
bool Utility::Insert(VNode &v1, int v){
	ArcNode *p=v1.firstarc;
	while(p!=NULL){
		if(p->adjvex==v)
			break;
		else
			p=p->nextarc;
	}
	if(p==NULL){
		ArcNode *q=new ArcNode;
		q->adjvex=v;
		q->nextarc=NULL;
		ArcNode *head=v1.firstarc;;
		v1.firstarc=q;
		q->nextarc=head;
		return true;
	}
	else
		return false;
}
void Utility::setverticesvalue(VNode &v1,VNode &v2){
	v1.color=v2.color;
	v1.firstarc=NULL;
	for(ArcNode *p=v2.firstarc;p!=NULL;p=p->nextarc){
		ArcNode *q=new ArcNode;
		q->adjvex=p->adjvex;
		q->nextarc=NULL;
		ArcNode *head=v1.firstarc;
		v1.firstarc=q;
		q->nextarc=head;
	}
	v1.data=v2.data;
}

bool Utility::Isinterset(vector<int> &set1,vector<int> &set2){
	if((int)set1.size()==0||(int)set2.size()==0)
		return false;
	else{
		int min=(int)set1.size();
		if(min<(int)set2.size())
			min=(int)set2.size();
		if(min==(int)set1.size()){
			for(int i=0;i<min;i++){
				if(this->FindNode(set1[i],set2)!=-1)
					return true;
			}
			return false;
		}
		else{
			for(int i=0;i<min;i++){
				if(this->FindNode(set2[i],set1)!=-1)
					return true;
			}
			return false;
		}
	}
}
Utility::~Utility(){
}

#endif

