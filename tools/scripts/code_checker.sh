#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2016-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

set -e

if ! test -d code_linter; then
    # Get our fork of codespell that adds --words-white-list and full filename support for -S option
    mkdir code_linter
    pushd code_linter || exit 1
    git clone --branch develop https://github.com/cpplint/cpplint
    # cd styleguide || exit 1
    # git checkout gh-pages
    popd || exit 1
    ls code_linter
    ls code_linter/cpplint
    echo code_linter installed
fi

DIRECTORY="$1"
INCLUDE_ORDER="
-build/include_order:src/cpp_common/pgdata_getters.cpp,
-build/include_order:include/cpp_common/get_data.hpp,
-build/include_order:src/cpp_common/report_messages.cpp,
-build/include_order:src/dijkstra/shortestPath_process.cpp,
-build/include_order:src/metrics/metrics_process.cpp,
-build/include_order:src/ordering/ordering_process.cpp,
-build/include_order:src/astar/astar_process.cpp,
-build/include_order:src/allpairs/allpairs_process.cpp"

if test -z "$DIRECTORY"; then
    echo "--------------------"
    echo "------   *.c  ------"
    echo "--------------------"
    code_linter/cpplint/cpplint.py --extensions=c  --linelength=120 --filter=-readability/casting src/*/*.c
    echo "--------------------"
    echo "------ *.cpp  ------"
    echo "--------------------"
    code_linter/cpplint/cpplint.py --linelength=120 \
        --filter=-readability/nolint,-runtime/references,-whitespace/indent_namespace,"${INCLUDE_ORDER}"  \
        --linelength=120 src/*/*.cpp

    echo "--------------------"
    echo "------ HEADERS  ------"
    echo "--------------------"
    code_linter/cpplint/cpplint.py --extensions=hpp,h --headers=hpp,h  --linelength=120 \
        --filter=-runtime/references,-whitespace/indent_namespace,"${INCLUDE_ORDER}" \
        include/*/*.h* \
        include/*/*/*.h*

else
    if [ "$DIRECTORY" = "h" ]; then

    echo "--------------------"
    echo "------ IN PLACE HEADERS  ------"
    echo "--------------------"
    code_linter/cpplint/cpplint.py --extensions=hpp,h --headers=hpp,h  --linelength=120 \
        --filter=-runtime/references,-whitespace/indent_namespace,"${INCLUDE_ORDER}" \
        include/*/*.h* \
        include/*/*/*.h*

    else
        echo "--------------------"
        echo "------   *.c  ------"
        echo "--------------------"
        code_linter/cpplint/cpplint.py --extensions=c  --linelength=120 \
            --filter=-readability/casting \
            src/"$DIRECTORY"/*.c

        echo "--------------------"
        echo "------ *.cpp  ------"
        echo "--------------------"
        code_linter/cpplint/cpplint.py  --linelength=120 \
            --filter=-runtime/references,-whitespace/indent_namespace,"${INCLUDE_ORDER}" \
            src/"$DIRECTORY"/*.cpp

        echo "--------------------"
        echo "------   C HEADER  ------"
        echo "--------------------"
        code_linter/cpplint/cpplint.py \
            include/drivers/"$DIRECTORY"/*.h \
            include/c_types/"$DIRECTORY"/*.h

        echo "--------------------"
        echo "------ C++ HEADER  ------"
        echo "--------------------"
        code_linter/cpplint/cpplint.py  --extensions=hpp,h --headers=hpp,h  --linelength=120 \
            --filter=-runtime/references,-whitespace/indent_namespace,"${INCLUDE_ORDER}" \
            include/"$DIRECTORY"/*.h*
    fi
fi

