
set -e

# creating the template
#cd tools/template/
#sh create.sh
#cd ../../


#tools/testers/algorithm-tester.pl -documentation -alg topology
#tools/testers/algorithm-tester.pl -documentation
#tools/testers/algorithm-tester.pl -documentation -alg astar
#tools/testers/algorithm-tester.pl -documentation -alg pickDeliver

#developers documentation
#cd tools/doxygen
#make
#cd ../../

cd build/

#the following to make the doc using most recent results
#make doc

#make clean
#rm -rf *
#cmake -DPostgreSQL_DEBUG=ON \
#    -DPostgreSQL_EXTENSION_DIR="/my/extension/dir" ..
#    -DPostgreSQL_INCLUDE_DIR="/my/include/dir" \
#    -DPostgreSQL_LIBRARY_DIR="/my/library/dir" \
#    -DPOSTGRESQL_VERSION="9.3" ..

#cmake -DBOOST_ROOT:PATH=/usr/local/boost_1_58_0 ..
#cmake -DPOSTGRESQL_VERSION="9.3" ..
#cmake -LA ..
#cmake -DBUILD_LATEX=ON -DBUILD_MAN=ON -DWITH_DEV_DOC=ON -DWITH_DOC=ON ..
#cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..

#this ones work
#cmake -DBoost_DEBUG=On -DBOOST_ROOT:PATH=/usr/local/boost_1_58_0 ..
#cmake -DBoost_DEBUG=On  ..

cmake ..
#cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DBoost_DEBUG=On ..
#cmake -DWITH_DOC=ON ..
make 

sudo make install 
#make doc
cd ..



#tools/testers/algorithm-tester.pl -alg allpairs   
#tools/testers/algorithm-tester.pl -alg apsp_johnson
#tools/testers/algorithm-tester.pl -alg apsp_warshall
#tools/testers/algorithm-tester.pl -alg astar
#tools/testers/algorithm-tester.pl -alg bd_astar
#tools/testers/algorithm-tester.pl -alg bd_dijkstra
#tools/testers/algorithm-tester.pl -alg common 
#tools/testers/algorithm-tester.pl -alg convinience 
#tools/testers/algorithm-tester.pl -alg contraction 
#tools/testers/algorithm-tester.pl -alg dijkstra
#tools/testers/algorithm-tester.pl -alg doc
#tools/testers/algorithm-tester.pl -alg driving_distance
tools/testers/algorithm-tester.pl -alg internalQueryTests
#tools/testers/algorithm-tester.pl -alg kdijkstra
#tools/testers/algorithm-tester.pl -alg ksp 
#tools/testers/algorithm-tester.pl -alg pickDeliver
#tools/testers/algorithm-tester.pl -alg topology 
#tools/testers/algorithm-tester.pl -alg trsp
#tools/testers/algorithm-tester.pl -alg newTSP   
#tools/testers/algorithm-tester.pl -alg tsp   
#tools/testers/algorithm-tester.pl -alg vrp_basic
#tools/testers/algorithm-tester.pl -alg vrppdtw
#tools/testers/algorithm-tester.pl -alg withPoints
#tools/testers/algorithm-tester.pl -alg funnyDijkstra   


tools/testers/algorithm-tester.pl

createdb  ___pgr___test___
sh ./tools/testers/pg_prove_tests.sh vicky
dropdb  ___pgr___test___

