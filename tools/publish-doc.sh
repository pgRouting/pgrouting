#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Publish Documentation
# ------------------------------------------------------------------------------

if test a$1 = a ; then
    echo "Usage: publish-doc.sh dir"
    echo "       where dir might be dev or v2.0"
    exit 1
fi

VERSION=`git describe --tags --long`
BRANCH=`git branch | grep '*' | awk '{print $2}'`

if test ! -d build ; then
    echo "Error 'build' directory does not exist."
    exit 1
fi

# Switch branch
git checkout gh-pages && rm -Rf "$1"

# Copy files and directories
mkdir "$1"
cp -R build/doc/html/en "$1"/en
cp build/doc/html/index.html .
cp build/doc/latex/en/pgRoutingDocumentation.pdf "$1"/en/
gzip -c build/doc/man/en/pgrouting.7 > "$1"/en/pgrouting.7.gz

# Commit changes
git add "$1"
git add index.html
git commit -m "Updated to commit $VERSION" . && git push origin gh-pages

# Switch branch
git checkout $BRANCH
