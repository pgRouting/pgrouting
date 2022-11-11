#!/bin/bash
# /*PGR-GNU*****************************************************************
# File: translate.sh
# Copyright (c) 2017 pgRouting developers
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
set -e


cd build/
rm -rf ./*
cmake -DHTML=ON -DDOC_USE_BOOTSTRAP=ON ..
make doc
cd ..


#tools/testers/doc_queries_generator.pl -alg pickDeliver

#tools/testers/doc_queries_generator.pl -documentation
cd build
#make doc
cd ..



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
