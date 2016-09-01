#!/bin/bash


#dont put pgr_ on MY_FUNCTION_NAME that is done automatically
MY_FUNCTION_NAME="funnyDijkstra"
DEVELOPER_NAME="Celia Virginia Vergara Castillo"
DEVELOPER_EMAIL="vicky_vergara@hotmail.com" 
YEAR="2016"

# Notice the "\n     " (change line and four spaces) after each comma
# first line are the inputs
# second line are the outputs
MY_QUERY_LINE1="edges_sql TEXT,\n    start_pid BIGINT,\n    end_pids ANYARRAY,\n    directed BOOLEAN DEFAULT true,"
MY_QUERY_LINE2="OUT seq INTEGER,\n    OUT path_seq INTEGER,\n    OUT start_vid BIGINT,\n    OUT end_vid BIGINT,\n    OUT node BIGINT,\n    OUT edge BIGINT,\n    OUT cost FLOAT,\n    OUT agg_cost FLOAT"


MY_RETURN_VALUE_TYPE="General_path_element_t" 
MY_FUNCTION_NAME_UPPER=$(echo $MY_FUNCTION_NAME | tr 'a-z' 'A-Z')

# Available types to store the edge information:
#  http://docs.pgrouting.org/doxy/dev/structpgr__edge__t.html
#  http://docs.pgrouting.org/doxy/dev/structPgr__edge__xy__t.html
MY_EDGE_TYPE="pgr_edge_t"

# Available functions that read the edge information:
# http://docs.pgrouting.org/doxy/dev/edges__input_8h.html#
MY_EDGE_FUNCTION="pgr_get_edges"





#the above variables must be defined

if test -z "$MY_FUNCTION_NAME"; then 
    exit
fi

#if a directory named MY_FUNCTION_NAME exist we stop
if [ -d "$MY_FUNCTION_NAME" ]; then
    echo "directory exists please delete first"
    exit 1
fi
if [ -d ../../src/"$MY_FUNCTION_NAME" ]; then
    echo "directory exists please delete first"
    exit 1
fi

mkdir "$MY_FUNCTION_NAME"

#copy the subdirectories
cp -r sql  "$MY_FUNCTION_NAME"
cp -r doc  "$MY_FUNCTION_NAME"
cp -r test  "$MY_FUNCTION_NAME"
cp -r  src  "$MY_FUNCTION_NAME"



#####   SQL   #########
echo "updating the /sql/CMakeLists.txt"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/sql/CMakeLists.txt

# /sql/function1.sql   
echo "Updating the .sql in sql"
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/sql/function1.sql
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/sql/function1.sql
mv "$MY_FUNCTION_NAME"/sql/function1.sql "$MY_FUNCTION_NAME"/sql/"$MY_FUNCTION_NAME".sql


#####   SRC   #########
echo "updating the /sql/CMakeLists.txt"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/CMakeLists.txt

echo "updating the function1.c file in src"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_EDGE_TYPE/$MY_EDGE_TYPE/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_EDGE_FUNCTION/$MY_EDGE_FUNCTION/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1.c
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1.c
mv "$MY_FUNCTION_NAME"/src/function1.c "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME".c

echo "updating the src/function1_driver.h"
sed -i "s/MY_FUNCTION_NAME_UPPER/$MY_FUNCTION_NAME_UPPER/g" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_EDGE_TYPE/$MY_EDGE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1_driver.h
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.h
mv "$MY_FUNCTION_NAME"/src/function1_driver.h "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME"_driver.h

echo "updating the src/function1_driver.cpp"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/DEVELOPER_NAME/$DEVELOPER_NAME/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/DEVELOPER_EMAIL/$MY_DEVELOPER_EMAIL/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_EDGE_TYPE/$MY_EDGE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
sed -i "s/MY_RETURN_VALUE_TYPE/$MY_RETURN_VALUE_TYPE/" "$MY_FUNCTION_NAME"/src/function1_driver.cpp
mv "$MY_FUNCTION_NAME"/src/function1_driver.cpp "$MY_FUNCTION_NAME"/src/"$MY_FUNCTION_NAME"_driver.cpp

#####   DOC   #########

echo "updating the doc/pgr_function1.cpp"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/doc/pgr_function1.rst
sed -i "s/YEAR/$YEAR/" "$MY_FUNCTION_NAME"/doc/pgr_function1.rst
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/doc/pgr_function1.rst
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/doc/pgr_function1.rst
mv "$MY_FUNCTION_NAME"/doc/pgr_function1.rst "$MY_FUNCTION_NAME"/doc/pgr_"$MY_FUNCTION_NAME".rst

#####   DOC   #########

echo "updating test/test.conf"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/test/test.conf

echo "updating test/test.conf"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/test/doc-function1.test.sql
sed -i "s/MY_QUERY_LINE1/$MY_QUERY_LINE1/" "$MY_FUNCTION_NAME"/test/doc-function1.test.sql
sed -i "s/MY_QUERY_LINE2/$MY_QUERY_LINE2/" "$MY_FUNCTION_NAME"/test/doc-function1.test.sql

echo "updating the test/pgtap/types-check.sql"
sed -i "s/MY_FUNCTION_NAME/$MY_FUNCTION_NAME/" "$MY_FUNCTION_NAME"/test/pgtap/types-check.sql

mv "$MY_FUNCTION_NAME"/test/doc-function1.test.sql "$MY_FUNCTION_NAME"/test/doc-"$MY_FUNCTION_NAME".test.sql
mv "$MY_FUNCTION_NAME"/test/doc-function1.result "$MY_FUNCTION_NAME"/test/doc-"$MY_FUNCTION_NAME".result

#move the whole structure to its place
mv "$MY_FUNCTION_NAME" ../../src/"$MY_FUNCTION_NAME"
exit
