#ifndef Buffer_h_
#define Buffer_h_
#include"Page.h"
#include<list>
class Buffer{
public:
	int maxsize;
	vector<MyPage> MyPagelist;
	Buffer();
	~Buffer();
	bool isfull;
	Buffer(int maxs);
	int insertandreplace(MyPage p);
};
Buffer::Buffer(){
}
Buffer::Buffer(int maxs){
	this->maxsize=maxs;
	this->isfull=false;
}
Buffer::~Buffer(){
}
int Buffer::insertandreplace(MyPage p){
	int mintime=1000000;
	int minpos=0;
	for(int i=0;i<(int)this->MyPagelist.size();i++){
		if(mintime<MyPagelist[i].time){
			mintime=MyPagelist[i].time;
			minpos=i;
		}
	}
	MyPage temp=this->MyPagelist[minpos];
	MyPagelist[minpos]=p;
	MyPagelist[minpos].bufpos=temp.bufpos;
	p.bufpos=temp.bufpos;
	return temp.ID;
}
#endif

