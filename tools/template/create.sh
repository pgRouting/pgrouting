#!/bin/bash

function usage() {
if [  "$#" -lt 1 ] ; then
    echo "Usage: sh ./create.sh <action>"
    echo "  <action> = clean, overwrite, missing"
    echo "  clean action will:   ONLY ACTIVE ONE"
    echo "    - remove the directories:"
    echo "      src/MY_FUNCTION_NAME/src"
    echo "      src/MY_FUNCTION_NAME/doc"
    echo "      src/MY_FUNCTION_NAME/test"
    echo "      include/drivers/MY_FUNCTION_NAME"
    echo "    - and start from scratch"
    echo "  overwrite action will: TODO"
    echo "    -  overwite the files if they exist"
    echo "  missing will: TODO"
    echo "    - skip files that exist"
    echo "    - write the missing files only"
    exit 1
fi
}

ACTION="$1"

if [[  ! ( "$1" == "clean" || "$1" == "overwrite" || "$1" == "missing" ) ]] ; then
    usage
fi



#dont put pgr_ on MY_FUNCTION_NAME that is done automatically
MY_FUNCTION_NAME="funnyDijkstra"
DEVELOPER_NAME="Celia Virginia Vergara Castillo"
DEVELOPER_EMAIL="vicky_vergara\@hotmail.com"
YEAR="2017"

# Note: the "\n     " (change line and four spaces) after each comma
# first line are the inputs
#   - the compulsory parameters with no name
#   - the optional parameters with name
# second line are the outputs
#   - all with a name

MY_QUERY_LINE1="TEXT,\n    BIGINT,\n    BIGINT,\n    directed BOOLEAN DEFAULT true,\n    only_cost BOOLEAN DEFAULT false,"
MY_QUERY_LINE2="    OUT seq INTEGER,\n    OUT path_seq INTEGER,\n    OUT node BIGINT,\n    OUT edge BIGINT,\n    OUT cost FLOAT,\n    OUT agg_cost FLOAT"


MY_FUNCTION_NAME_UPPER=$(echo $MY_FUNCTION_NAME | tr 'a-z' 'A-Z')
MY_FUNCTION_NAME_LOWER=$(echo $MY_FUNCTION_NAME | tr 'A-Z' 'a-z')

# Available types to store the edge information:
#  http://docs.pgrouting.org/doxy/dev/structpgr__edge__t.html
#  http://docs.pgrouting.org/doxy/dev/structPgr__edge__xy__t.html
MY_RETURN_VALUE_TYPE="General_path_element_t"
MY_RETURN_VALUE_FILE=$(echo $MY_RETURN_VALUE_TYPE | tr 'A-Z' 'a-z')

MY_EDGE_TYPE="pgr_edge_t"
MY_EDGE_FILE=$(echo $MY_EDGE_TYPE | tr 'A-Z' 'a-z')

# Available functions that read the edge information:
# http://docs.pgrouting.org/doxy/dev/edges__input_8h.html#
MY_EDGE_FUNCTION="pgr_get_edges"





#the above variables must be defined

if test -z "$MY_FUNCTION_NAME"; then
    exit
fi

if [ -d ./$MY_FUNCTION_NAME ]; then
    rm -rf ./$MY_FUNCTION_NAME
fi

mkdir "$MY_FUNCTION_NAME"

#copy to a working subdirectory
cp -r sql  "$MY_FUNCTION_NAME"
cp -r doc  "$MY_FUNCTION_NAME"
cp -r test  "$MY_FUNCTION_NAME"
cp -r src  "$MY_FUNCTION_NAME"
cp -r pgtap  "$MY_FUNCTION_NAME"

function substitute() {
    perl -pi -e "s/$1/$2/g" $MY_FUNCTION_NAME/src/*
    perl -pi -e "s/$1/$2/g" $MY_FUNCTION_NAME/doc/*
    perl -pi -e "s/$1/$2/g" $MY_FUNCTION_NAME/sql/*
    perl -pi -e "s/$1/$2/g" $MY_FUNCTION_NAME/test/*
    perl -pi -e "s/$1/$2/g" $MY_FUNCTION_NAME/pgtap/*
}

substitute "MY_FUNCTION_NAME_LOWER" "$MY_FUNCTION_NAME_LOWER"
substitute "MY_FUNCTION_NAME_UPPER" "$MY_FUNCTION_NAME_UPPER"
substitute "MY_FUNCTION_NAME" $MY_FUNCTION_NAME
substitute "MY_QUERY_LINE1" "$MY_QUERY_LINE1"
substitute "MY_QUERY_LINE2" "$MY_QUERY_LINE2"
substitute "DEVELOPER_NAME" "$DEVELOPER_NAME"
substitute "DEVELOPER_EMAIL" "$DEVELOPER_EMAIL"
substitute "MY_FUNCTION_NAME" $MY_FUNCTION_NAME
substitute "MY_FUNCTION_NAME" $MY_FUNCTION_NAME
substitute "YEAR" $YEAR
substitute "MY_EDGE_TYPE" $MY_EDGE_TYPE
substitute "MY_EDGE_FILE" $MY_EDGE_FILE
substitute "MY_EDGE_FUNCTION" $MY_EDGE_FUNCTION
substitute "MY_RETURN_VALUE_TYPE" $MY_RETURN_VALUE_TYPE
substitute "MY_RETURN_VALUE_FILE" $MY_RETURN_VALUE_FILE



#renaming the files
mv $MY_FUNCTION_NAME/src/function1.c                       $MY_FUNCTION_NAME/src/$MY_FUNCTION_NAME.c
mv $MY_FUNCTION_NAME/src/function1_driver.cpp              $MY_FUNCTION_NAME/src/"$MY_FUNCTION_NAME"_driver.cpp
mv $MY_FUNCTION_NAME/src/function1_driver.h                $MY_FUNCTION_NAME/src/"$MY_FUNCTION_NAME"_driver.h

mv $MY_FUNCTION_NAME/doc/doc-pgr_function1.queries         $MY_FUNCTION_NAME/doc/doc-pgr_$MY_FUNCTION_NAME.queries
mv $MY_FUNCTION_NAME/doc/pgr_function1.rst                 $MY_FUNCTION_NAME/doc/pgr_$MY_FUNCTION_NAME.rst

mv $MY_FUNCTION_NAME/test/doc-function1.test.sql           $MY_FUNCTION_NAME/test/doc-$MY_FUNCTION_NAME.test.sql
mv $MY_FUNCTION_NAME/test/doc-function1.result             $MY_FUNCTION_NAME/test/doc-$MY_FUNCTION_NAME.result

mv $MY_FUNCTION_NAME/sql/function1.sql                     $MY_FUNCTION_NAME/sql/$MY_FUNCTION_NAME.sql

mv $MY_FUNCTION_NAME/pgtap/function1-typesCheck.sql        $MY_FUNCTION_NAME/pgtap/$MY_FUNCTION_NAME-typesCheck.sql
mv $MY_FUNCTION_NAME/pgtap/function1-compare-dijkstra.sql  $MY_FUNCTION_NAME/pgtap/$MY_FUNCTION_NAME-compare-dijkstra.sql
mv $MY_FUNCTION_NAME/pgtap/function1-innerQuery.sql        $MY_FUNCTION_NAME/pgtap/$MY_FUNCTION_NAME-innerQuery.sql



if [[ ! ( -d ../../doc/$MY_FUNCTION_NAME ) ]] ; then
    mkdir ../../doc/$MY_FUNCTION_NAME
fi

if [[ ! ( -d ../../sql/$MY_FUNCTION_NAME ) ]] ; then
    mkdir ../../sql/$MY_FUNCTION_NAME
fi


if [[  "$1" == "clean"  ]] ; then
    echo "deleting old directories"
    rm -rf ../../sql/$MY_FUNCTION_NAME
    rm -rf ../../src/$MY_FUNCTION_NAME
    rm -rf ../../doc/$MY_FUNCTION_NAME
    rm -rf ../../include/drivers/$MY_FUNCTION_NAME
    rm -rf ../../test/$MY_FUNCTION_NAME
    rm -rf ../../pgtap/$MY_FUNCTION_NAME

    mkdir -p ../../sql/$MY_FUNCTION_NAME
    mkdir -p ../../doc/$MY_FUNCTION_NAME
    mkdir -p ../../include/drivers/$MY_FUNCTION_NAME
    mkdir -p ../../test/$MY_FUNCTION_NAME
    mkdir -p ../../pgtap/$MY_FUNCTION_NAME


    #moving the whole structure to its final location
    mv $MY_FUNCTION_NAME/pgtap/*              ../../pgtap/$MY_FUNCTION_NAME
    mv $MY_FUNCTION_NAME/src/*driver.h      ../../include/drivers/$MY_FUNCTION_NAME

    mv $MY_FUNCTION_NAME/doc/*              ../../doc/$MY_FUNCTION_NAME
    mv $MY_FUNCTION_NAME/sql/*              ../../sql/$MY_FUNCTION_NAME
    mv $MY_FUNCTION_NAME/test/*             ../../test/$MY_FUNCTION_NAME

    rm -rf  $MY_FUNCTION_NAME/doc
    rm -rf  $MY_FUNCTION_NAME/sql
    rm -rf  $MY_FUNCTION_NAME/test
    rm -rf  $MY_FUNCTION_NAME/pgtap

    mv $MY_FUNCTION_NAME                    ../../src
fi

exit 0;

