   Installing and Using autopart, index and query

1.1 Installing Dependency

    In order to install the autopart, index and query, you have to make sue that perl and gcc compiler has been installed in the PC already.

    In additon, please put the "temp" folder under the local driver C.


1.2 How to install

    1.2.1 Windows with gcc
          $ cd "path"  where "path" is the absolute path where the "autopart ,index and query" source code is
          $ cd autopart
          $ make 
          $ cd ..
          $ cd index
          $ make
          $ cd ..
          $ cd query
          $ make
   1.2.2  Linux
          For Linux machines with gcc, the installation is similar to 1.2.1

1.3 How to Use
    Make sure a folder with name "Indexes" has been put in the same path with the index.exe file. The folder "Indexes" is used to store the graph indexes in the disk.

   1.3.1 Autopart
   
      usage:
Partition -g graphfilename -t cost_function_type[option]
option:
-f 1: use entropy function
-f 2: use estimate size function
-f 3: use the lower bound of estimate size
-f 4: use the query time estimation cost model
-f 5: use the index size estimation cost model
-l: log file name
-i: iteration num

  1.3.2 Index

     usage:
index [option]
option:
-t [1|2|3]
1: read the graph, do partitioning on it, and build multiple indexes on it
2: read the graph and build the hopi on the whole graph
3: read the graph and build the compressed TC on the whole graph
-g: input graph file name (required)
-a density threshold
-i number of iterations
-f cost function type
4: query cost function
5: index size cost function

    1.3.3 Query
 
usage:
query [option]
option:
-q: the number of queries
-t [1|2|3|]
1:read the multiple index and do the query
2: read the hopi index and do the query
3: read the interval index and do the query
-a densitry threshold
-g: input graph file name
-b maximum buffer size (Mega)

