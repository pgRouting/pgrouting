#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Publish Documentation
# ------------------------------------------------------------------------------

VERSION=`git describe --tags --long`
BRANCH=`git branch | grep '*' | awk '{print $2}'`

if test ! -d build ; then
    echo "Error 'build' directory does not exist."
    exit 1
fi

# Switch branch
git checkout gh-pages && rm -Rf dev 

# Copy files and directories
mkdir dev
cp -R build/doc/html/en dev/en
cp build/doc/html/index.html .
cp build/doc/latex/en/pgRoutingDocumentation.pdf dev/en/
gzip -c build/doc/man/en/pgrouting.7 > dev/en/pgrouting.7.gz

# Commit changes
git add dev 
git commit -m "updated to commit $VERSION" . && git push origin gh-pages

# Switch branch
git checkout $BRANCH
