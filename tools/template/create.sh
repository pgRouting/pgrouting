#!/bin/bash


#dont put pgr_ on MY_FUNCTION_NAME that is done automatically
MY_FUNCTION_NAME="dijkstraViaVertex"
DEVELOPER_NAME="Celia Virginia Vergara Castillo"
DEVELOPER_EMAIL="vicky_vergara@hotmail.com" 
YEAR="2015"

MY_QUERY_LINE1="CREATE OR REPLACE FUNCTION pgr_dijkstraViaVertices(sql text, vertices anyarray, directed boolean default true,"
MY_QUERY_LINE2="   OUT seq BIGINT, OUT path_id BIGINT, OUT path_seq BIGINT, OUT start_vid BIGINT, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost FLOAT, OUT agg_cost FLOAT, OUT route_agg_cost FLOAT)"


MY_RETURN_VALUE_TYPE="Routes_t" 
MY_FUNCTION_NAME_UPPER=$(echo $MY_FUNCTION_NAME | tr 'a-z' 'A-Z')

#the above variables must be defined

if test -z "$MY_FUNCTION_NAME"; then 
	exit
fi

#if a directory named MY_FUNCTION_NAME exist we stop
if [ -d "$MY_FUNCTION_NAME" ]; then
        echo "directory exists please delete first"
	exit
fi
if [ -d ../../src/"$MY_FUNCTION_NAME" ]; then
        echo "directory exists please delete first"
	exit
fi

mkdir "$MY_FUNCTION_NAME"

#copy the subdirectories
cp -r sql  "$MY_FUNCTION_NAME"
cp -r doc  "$MY_FUNCTION_NAME"
cp -r test  "$MY_FUNCTION_NAME"
cp -r  src  "$MY_FUNCTION_NAME"
cp CMakeLists.txt  "$MY_FUNCTION_NAME"


echo "Updating the .sql in sql"
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/sql/function1.sql
mv "$MY_FUNCTION_NAME"/sql/function1.sql "$MY_FUNCTION_NAME"/sql/"$MY_FUNCTION_NAME".sql

echo "updating the CmaKeLists.txt file in src"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/sql/CMakeLists.txt
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/CMakeLists.txt

echo "updating the function1.c file in src"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1.c
mv "$MY_FUNCTION_NAME"/src/function1.c "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME".c

echo "updating the function1_driver.h file in src"
sed -i "s/MY_FUNCTION_NAME_UPPER/$MY_FUNCTION_NAME_UPPER/g" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.h
mv "$MY_FUNCTION_NAME"/src/function1_driver.h "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME"_driver.h

echo "updating the function1_driver.cpp file in src"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
mv "$MY_FUNCTION_NAME"/src/function1_driver.cpp "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME"_driver.cpp


#move the whole structure to its place
mv "$MY_FUNCTION_NAME" ../../src/"$MY_FUNCTION_NAME"
exit
/*!
  perform the folloing substitutions
  :%s/MY_FUNCTION_NAME/pgroutingTest/
  :%s/MY_RETURN_VALUE_TYPE/General_path_element_t/
*/





EXAMPLE PROCESS BASED ON WARSHALL

cp -r template warshall
cd warshall/sql/
mv function1.sql warshall.sql
vi warshall.sql
	change date, name email
	-- copy old functions signature or make a new signature using this one as example (V2)
	add new function signature
cd ../../common/sql/	
vi pgr_parameter_check.sql
	-- add warshall where appropiate (because "sql" its going be the same as dijkstra, put where dikstra appears)
cd ../../warshall/src/
mv function1.c warshall.c
mv function1_driver.cpp warshall_driver.cpp 
mv function1_driver.h warshall_driver.h
vi warshall.c
        35: change the name of the driver
	change the names of the functions
	(kind of work like the program will work)
        process starts around line 130
	153: adjust to the parameters recieved from the query
	50: and adjust to the parameters recieved from call in 153
	68: no start/end vertex check needed change to -1, -1);
	surround with #if 0 / #endif conditions that probably dont apply to the problem
	aprox 110: send the data to the cpp code (fix name and parameters)
                    // no need for rcost flag its already being used
        aprox 163: we are returning 2 bigs nd 1 float so: __pgr_2b1f
	aprox 190: fix to what we are returning
vi warshall_driver.h
	change date, name email
	change constant of #define
	fix parameters to what its sent to us in (aprox 110)
vi warshall_driver.cpp
	change to have  #include warshall_driver.h  (instead of function1_driver)
	surround with #if 0 / #endif conditions that probably dont apply to the problem
	change Pgr_dijkstra to Pgr_warshall  (depends on decision made in DECISION)
	addjust the calls to be: (as no start_v nor end_v are needed remove the reference to them)
        	digraph.warshall(paths) & undigraph.warshall(paths)
	The results can be stored as a deque<Path> paths  or as a Path (so I am thinking out loud)
		Warshall returns a matrix but we are going to return a table, 
		if deque<Path> paths is used then each path[i] can represent a row of the table
			anyway it will have to be collapsed to a single Path
		if Path is used then it must have all the values
        I prefer the deque<Path> paths and then do the code that is already there:
		    *ret_path = pgr_get_memory3(count, (*ret_path)); // getting memory to store the result
		    int sequence(collapse_paths(ret_path, paths));   // store the result

For warshall we need either:
  (1)	Pgr_warshall  class defined, so we can put the functions:
		initialize_graph
		warshall
		 (and auxiliary private functions for warshall)
  (2)   Pgr_dijkstra  where we will put:
		warshall
		 (and auxiliary private functions for warshall)
For the moment I am deciding for the first one.
*********************
vi warshall_driver.cpp
	#include "./pgr_warshal.hpp"   (need to code this)

preparing a commit
cd ../../
git add warshall
git commit -a -m 'added warshall directory with connection code'


Codded pgr_warshall

vi CMakelists.txt
    changed function1 to warshall

vi ../../CMakelists.txt
   added in alphabetical order

   ADD_SUBDIRECTORY(warshall)
   LIST(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")


vi ../../../CMakelists.txt

    174: in alphabetical order:
    ${PGROUTING_SOURCE_DIR}/src/warshall/src

    261: in alphabetical order
    add_library(routing-2.1

vi ../sql/CMakelists.txt





after release 2.1
#324
Documentations says:
" is a graph analysis algorithm for finding the shortest paths between all pairs of nodes in a weighted graph"
as it does find the shortest path it does not return the shortest path, just the cost of the shortest path.
So documentation should be fixed.
