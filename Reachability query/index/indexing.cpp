#include"indexframework.h"
void exit_with_help(){
	cout<<"please put the bin and command.bat files in the same path with the index.exe"<<endl;
	cout<<"usage:"<<endl;
	cout<<"index [option]"<<endl;
	cout<<"option:"<<endl;
	cout<<"-t [1|2|3]"<<endl;
	cout<<"1: read the graph, do partitioning on it, and build multiple indexes on it"<<endl;
	cout<<"2: read the graph and build the hopi on the whole graph"<<endl;
	cout<<"3: read the graph and build the compressed TC on the whole graph"<<endl;
	cout<<"-g: input graph file name (required)"<<endl;
	cout<<"-a density threshold"<<endl;
	cout<<"-i number of iterations"<<endl;
	cout<<"-f cost function type"<<endl;
	cout<<"4: query cost function"<<endl;
	cout<<"5: index size cost function"<<endl;
	exit(1);
}
int main(int argc, char*argv[]){
	int ttype=1;
	int htype=4;
	int iterationnum=100;
	double threshold=0.15;
	int i;
	srand( (unsigned)time( NULL ) );
	char *infilename=new char[50];
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
			case 'a':
				threshold=atof(argv[i]);
				break;
			case 'i':
				iterationnum=atoi(argv[i]);
				break;
			case 'f':
				htype=atoi(argv[i]);
				break;
			default:
				exit_with_help();
		}

	}
	cout<<"start reading the graph into memeory"<<endl;
	Graph g(infilename);
	cout<<"end of reading graph"<<endl;

	cout<<"start to construct the condensed graph"<<endl;
	DFSAPP app;
	app.dotarjar(g);
	app.Docompressed(g);
	cout<<"end of construction"<<endl;
	cout<<"size of condensed graph:"<<endl;
	cout<<"node number: "<<app.compressedG.sizenode();
	cout<<" edge number: "<<app.compressedG.sizeedge()<<endl;

	string graphname(infilename);
	string s="Indexes\\"+graphname+".comp";
	ofstream outcomp(s.c_str());
	outcomp<<g.sizenode()<<endl;
	for(int i=0;i<g.sizenode();i++)
		outcomp<<app.comp[i]<<endl;
	indexing singleindex;
	indexframework FI;
	int indexsize;
	double indextime;
	ofstream outsize("indexsize",ios::app);
	ofstream outtime("indextime",ios::app);
	switch(ttype){
		case 1:
			FI.doindexframework(app.compressedG,infilename,threshold,iterationnum,htype);
			indexsize=FI.indexsize;
			indextime=FI.indextime;
			outsize<<graphname<<"\t"<<"framework\t";
			outtime<<graphname<<"\t"<<"framework\t";
			break;
		case 2:
			singleindex.singleindex(app.compressedG,ttype,infilename);
			indexsize=singleindex.returnindexsize();
			indextime=singleindex.returnruntime();
			outsize<<graphname<<"\t"<<"HOPI\t";
			outtime<<graphname<<"\t"<<"HOPI\t";
			break;
		case 3:
			singleindex.singleindex(app.compressedG,ttype,infilename);
			indexsize=singleindex.returnindexsize();
			indextime=singleindex.returnruntime();
			outsize<<graphname<<"\t"<<"Interval\t";
			outtime<<graphname<<"\t"<<"Interval\t";
			break;
		default:
			exit_with_help();
	}
	outtime<<"construct time(ms)\t"<<indextime<<endl;
	outsize<<"index size(#ofintegers)"<<"\t"<<indexsize<<endl;
}

