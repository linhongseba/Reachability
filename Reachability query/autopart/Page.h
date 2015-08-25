#ifndef Page_h_
#define Page_h_
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
using namespace std;
typedef  std::map <int, int> pagemap;
struct ILabel{
	int start;
	int end;
};
struct Entry{
	vector<int> Lin;
	vector<int> Lout;
	vector<ILabel> intervals;
	int size;
	int id;
	int pid;
};
class MyPage{
private:
	int cur;
	bool isfull;
public:
	int ID;
	int maxsize; //usually, MyPage size=4kbytes=1024(#of integers)
	int time;
	int bufpos;
	vector <Entry> content; //content stores the entry info
	pagemap pagehead;    //page head stores the pos of each entry in this page;
public:
	MyPage();
	~MyPage();
	MyPage(int idf, int maxs);     //initialize the MyPage 
	int Getcur(); //get the offset cur of the MyPage
	bool Insertentry(Entry &v); //insert an entry into the MyPage
	void Deleteentry(Entry &v); //delete an entry from the MyPage
	bool empty();
	bool full();
	void resetid(int newid);
	void resetcontent();
	int BinarySearchMyPage(vector<MyPage> MyPages,int startpos, int endpos);
};
MyPage::MyPage(){
}
MyPage::~MyPage(){
}

MyPage::MyPage(int idf, int maxs){
	this->ID=idf;
	this->maxsize=maxs;
	this->cur=0;
	this->isfull=false;
	this->time=0;
	this->bufpos=-1;
}
void MyPage::resetcontent(){
	this->cur=0;
	this->isfull=false;
	this->content.clear();
	this->pagehead.clear();
	this->bufpos=-1;
}
void MyPage::resetid(int newid){
	this->ID=newid;
}
bool MyPage::empty(){
	if(this->cur==0)
		return true;
	else
		return false;
}
bool MyPage::full(){
	return this->isfull;
}
int MyPage::Getcur(){
	return this->cur;
}
bool MyPage::Insertentry(Entry &v){
	int minsize=(int)sizeof(Entry);
	if(this->isfull==false){
		if(this->cur+v.size>this->maxsize)
			return false;
		else{
			this->content.push_back(v);
			this->pagehead.insert(make_pair(v.id,this->content.size()-1));
			this->cur=this->cur+v.size;
		}
		if(cur==this->maxsize||cur+minsize>this->maxsize)
			this->isfull=true;
		return true;
	}
	return false;
}
void MyPage::Deleteentry(Entry &v){
	pagemap::iterator pos=this->pagehead.find(v.id);
	int entrycur;
	if(pos!=pagehead.end()){
		entrycur=pos->second;
		this->content.erase(content.begin()+entrycur);
		cur=(int)this->content.size();
	}
}
int MyPage::BinarySearchMyPage(std::vector<MyPage> MyPages, int startpos, int endpos){
	int pos=(endpos-startpos)/2;
	if(MyPages[pos].ID==this->ID)
		return pos;
	else
		if(pos<endpos&&pos>startpos){
			if(MyPages[pos].ID>this->ID)
				return BinarySearchMyPage(MyPages,startpos,pos);
			else
				return BinarySearchMyPage(MyPages,pos,endpos);
		}
	return -1;
}
#endif
