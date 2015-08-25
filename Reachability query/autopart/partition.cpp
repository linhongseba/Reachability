#include"autopart.h"
#include"Utility.h"
int type=2;
int iterationnum=100;
void exit_with_help(){
	 cout<<"usage:"<<endl;
	 cout<<"Partition -g graphfilename -t cost_function_type[option]"<<endl;
	 cout<<"option:"<<endl;
	 cout<<"-f 1: use entropy function"<<endl;
	 cout<<"-f 2: use estimate size function"<<endl;
	 cout<<"-f 3: use the lower bound of estimate size"<<endl;
	 cout<<"-f 4: use the query time estimation cost model"<<endl;
	 cout<<"-f 5: use the index size estimation cost model"<<endl;
	 cout<<"-l: log file name"<<endl;
	 cout<<"-i: iteration num"<<endl;
	 exit(1);
}
int main(int argc, char*argv[]){
	int i;
	srand( (unsigned)time( NULL ) );
	char *infilename=new char[50];
	char *outname=new char[50];
	for(i=1;i<argc;i++){
		if(argv[i][0] != '-') break;
		if(++i>=argc)
			exit_with_help();
		switch(argv[i-1][1]){
			case 'g':
				strcpy(infilename,argv[i]);
				break;
			case 'l':
				strcpy(outname,argv[i]);
				break;
			case 'f':
				type=atoi(argv[i]);
				break;
			case 'i':
				iterationnum=atoi(argv[i]);
				break;
			default:
				exit_with_help();
		}
	}
	Graph g(infilename);
	int *partid=new int[g.sizenode()];
	for(int i=0;i<g.sizenode();i++)
		partid[i]=-1;
	autopart p(g,partid,type,iterationnum);
	ofstream outfile(".pid");
	for(int i=0;i<g.sizenode();i++)
		outfile<<partid[i]<<endl;
}
