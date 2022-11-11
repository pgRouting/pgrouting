#!/bin/bash
# ------------------------------------------------------------------------------
# /*PGR-GNU*****************************************************************
# File: remove_obsolete_entries.sh
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
# ------------------------------------------------------------------------------

# For all the chapter files
find locale -type f -name '*.po' -exec sh -c '
    if grep -q "#~" "$1"; then
        perl -pi -0777 -e "s/#~.*//s" "$1"
        git add "$1"
    fi
  ' sh {} \;
