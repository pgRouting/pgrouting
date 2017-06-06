#!/bin/bash

#
# base script for developers
# copy to the root of the repository
# 

set -e

if [ -z $1 ]; then
    echo "version missing"
    exit 1;
fi

VERSION=$1


function test_compile {                                                                                                                                                                                                          

echo ------------------------------------
echo ------------------------------------
echo Compiling with $1
echo ------------------------------------

sudo update-alternatives --set gcc /usr/bin/gcc-$1

cd build/
# Release RelWithDebInfo MinSizeRel Debug
cmake  -DDOC_USE_BOOTSTRAP=ON -DWITH_DOC=ON -DBUILD_DOXY=ON -DCMAKE_BUILD_TYPE=Debug ..


#cmake  -DWITH_DOC=ON -DBUILD_DOXY=ON ..
#cmake -DCMAKE_VERBOSE_MAKEFILE=ON ..
make doxy
make doc
make
sudo make install
cd ..


#tools/testers/algorithm-tester.pl -alg dijkstra
#tools/testers/algorithm-tester.pl -alg dijkstra -debug1 >result.txt

#  Verify with git diff that signatures did not change
#sh tools/release-scripts/get_signatures.sh $VERSION ____sigs_routing____ sql/sigs
#cp build/sql/pgrouting--*.sql tools/sql-update-scripts
if [[ $(git status | grep 'pgrouting--') ]]; then
    echo "**************************************************"
    echo "           WARNING"
    echo "the signatures changed, copying generated files"
    echo "Plese verify the changes are minimal"
    echo "**************************************************"
    git diff
fi


tools/testers/algorithm-tester.pl -documentation
cd build
rm -rf doc/*
make doc
cd ..


tools/testers/algorithm-tester.pl

dropdb --if-exists ___pgr___test___
createdb  ___pgr___test___
sh ./tools/testers/pg_prove_tests.sh vicky 
dropdb  ___pgr___test___

#tools/testers/update-tester.sh 

}

#test_compile 4.4
#rm -rf build/*
#test_compile 5
#rm -rf build/*
#test_compile 4.9
#rm -rf build/*
#test_compile 4.6
#sudo rm -f /usr/lib/postgresql/9.3/lib/libpgrouting-2.5.so
#sudo rm -f /usr/share/postgresql/9.3/extension/pgrouting*2.5.0*
#rm -rf build/*
test_compile 4.8

exit 0

gource --seconds-per-day 0.1 \
    --auto-skip-seconds 0.3 \
    --max-file-lag 0.1 \
    --logo doc/static/images/pgrouting-logo.png ---date-format "%B %Y" \
    --hide filenames,dirnames \
    --start-position 0.1 \
    --disable-auto-rotate \
    ----file-idle-time 10 \
    -f -1280x720 \
    --output-ppm-stream release-2.4.ppm 

ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i release-2.4.ppm -vcodec libx264 -preset ultrafast -pix_fmt yuv420p -crf 1 -threads 0 -bf 0 release-2.4.mp4
