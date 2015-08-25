#ifndef mymath_h_
#define mymath_h_
#include "stdlib.h"
#include "math.h"
#include <stdio.h>
using namespace std;
class mymath{
public:
	mymath();
	~mymath();
	double entropy( double p);
	double nontreedensity(int nodenum, int edgenum);
	double estimatesize(int nodenum,int edgenum);
	double Interpartitionedgedensity (int edgenum, int vnum1, int vnum2);
	double graphdensity(int nodenum,int edgenum);
	double lowerbound(int nodenum,int edgenum);
	double querycost(int nodenum, int edgenum);
	double sizecost(int nodenum,int edgenum);
};


mymath::mymath(){
}

mymath::~mymath(){
}
double mymath::lowerbound(int nodenum,int edgenum){
	if(nodenum==0||edgenum==0)
		return 0;
	else
		return log((double)nodenum*nodenum/edgenum)*edgenum;
		
}
double mymath::graphdensity(int nodenum,int edgenum){
	return (double)edgenum*2/nodenum/(nodenum-1);
}
double mymath::querycost(int nodenum, int edgenum){
	if(nodenum==0||edgenum==0)
		return 0;
	double c1=nodenum/2;
	double c2=sqrt((double)edgenum);
	double cq=min(c1,c2);
	return cq;
}

double mymath::sizecost(int nodenum,int edgenum){
	if(nodenum==0||edgenum==0)
		return 0;
	double c1=nodenum*nodenum/2;
	double c2=(double)nodenum*sqrt((double)edgenum);;
	double cs=min(c1, c2);
	return cs;
}
double mymath::entropy(double p){
	if(p<0){
		cout<<"p is in the range (0,1)"<<endl;
			exit(0);
	}
	if(p==0||p==1)
		return 0;
	else{
		double r=-log(p)*p-log(1-p)*(1-p);
		return r;
	}
}
double mymath::nontreedensity(int nodenum, int edgenum){
	double n;
	if(edgenum==0)
		return 0;
	else
		n=(double)(edgenum+1-nodenum)/edgenum;
	if(n>=0)
		return n;
	else
		return 0;
}
double mymath::Interpartitionedgedensity(int edgenum, int vnum1, int vnum2){
	double q;
	if(vnum1==0||vnum2==0)
		return 0;
	else
		q=(double)edgenum/vnum1/vnum2;
	return q;
}
double mymath::estimatesize(int nodenum,int edgenum){
	double nt=this->nontreedensity(nodenum,edgenum);
	double esize=(nodenum+edgenum)*(1-nt)+nodenum*sqrt((double)edgenum)*nt;
	return esize;
}

#endif

