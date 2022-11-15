#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: addNewVersionLink.sh
# Copyright (c) 2021 pgRouting developers
# Mail: project@pgrouting.org
# ------
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# ********************************************************************PGR-GNU*/
OLDVER=$1
NEWVER=$2

if [ -z "${OLDVER}" ] ; then echo 'USE: tools/developer/addNewVersionLink.sh "3\.2" "3\.3"'; exit 1; fi;
if [ -z "${NEWVER}" ] ; then echo 'USE: tools/developer/addNewVersionLink.sh "3\.2" "3\.3"'; exit 1; fi;

OLDSTR='^  \(`'"${OLDVER}"' (.*)\/'"${OLDVER}"'(.*)\)$'
NEWSTR='  \(`'"${NEWVER}"' $1\/'"${NEWVER}"'$2\)\n  `'"${OLDVER}"' $1\/'"${OLDVER}"'$2'
perl -pi -e 's/'"$OLDSTR"'/'"${NEWSTR}"'/' $(git ls-files './*.rst')
