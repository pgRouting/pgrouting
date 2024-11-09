#!/bin/bash
# -*- coding: utf-8 -*-
###############################################################################
#
#  Project:  pgRouting
#  Purpose:  (Interactive) script to lint C and C++ code
#  Author:   Vicky Vergara <vicky_vergara@hotmail.com>
#
###############################################################################
#  Copyright (c) 2016, Vicky Vergara <vicky_vergara@hotmail.com>
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
###############################################################################
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
INCLUDE_ORDER="-build/include_order:src/cpp_common/pgdata_getters.cpp,-build/include_order:include/cpp_common/get_data.hpp"

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

