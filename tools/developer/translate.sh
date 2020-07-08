#!/bin/bash

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
