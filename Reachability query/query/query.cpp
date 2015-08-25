#include "query.h"
void exit_with_help(){
	 cout<<"please put folder \"Indexes\" in the same path with the query.exe"<<endl;
	 cout<<"usage:"<<endl;
	 cout<<"query [option]"<<endl;
	 cout<<"option:"<<endl;
	 cout<<"-q: the number of queries"<<endl;
	 cout<<"-t [1|2|3|]"<<endl;
	 cout<<"1:read the multiple index and do the query"<<endl;
	 cout<<"2: read the hopi index and do the query"<<endl;
	 cout<<"3: read the interval index and do the query"<<endl;
	 cout<<"-a densitry threshold"<<endl;
	 cout<<"-g: input graph file name "<<endl;
	 cout<<"-b maximum buffer size (Mega)"<<endl;
	 exit(1);
}
int main(int argc, char*argv[]){
	int ttype=1;
	int qnum=1000000;
	double thre=0.3;
	int bufsize=8;
	int i;
	srand((unsigned)time(NULL));
	char *infilename=new char[50];
	strcpy(infilename,"test.txt");
	for(i=1;i<argc;i++){
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();
		switch(argv[i-1][1]){
			case 't':
				ttype=atoi(argv[i]);
				break;
			case 'g':
				strcpy(infilename,argv[i]);
				break;
			case 'q':
				qnum=atoi(argv[i]);
				break;
			case 'a':
				thre=atof(argv[i]);
				break;
			case 'b':
				bufsize=atoi(argv[i]);
				break;
			default:
				exit_with_help();
		}
	}
	Query doquery(infilename,ttype,qnum,thre,bufsize);
}
