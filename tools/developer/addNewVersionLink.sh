#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

OLDVER=$1
NEWVER=$2

if [ -z "${OLDVER}" ] ; then echo 'USE: tools/developer/addNewVersionLink.sh "3\.2" "3\.3"'; exit 1; fi;
if [ -z "${NEWVER}" ] ; then echo 'USE: tools/developer/addNewVersionLink.sh "3\.2" "3\.3"'; exit 1; fi;

OLDSTR='^  \(`'"${OLDVER}"' (.*)\/'"${OLDVER}"'(.*)\)$'
NEWSTR='  \(`'"${NEWVER}"' $1\/'"${NEWVER}"'$2\)\n  `'"${OLDVER}"' $1\/'"${OLDVER}"'$2'
perl -pi -e 's/'"$OLDSTR"'/'"${NEWSTR}"'/' $(git ls-files './*.rst')
