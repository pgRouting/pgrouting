cd build/
#make clean
#rm -rf *
#cmake -DBOOST_ROOT:PATH=/usr/local/boost_1_58_0 ..
#cmake -DWITH_DOC=ON -DBUILD_DOXY ..
cmake ..
make
sudo make install
#make doc
#make doxy
cd ..
# before testing everything
#tools/test-runner.pl -alg common 
#tools/test-runner.pl -alg dijkstra 
#tools/test-runner.pl -alg myfeature
# test everything
#tools/test-runner.pl -alg