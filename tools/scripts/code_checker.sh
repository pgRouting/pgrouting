#!/bin/sh
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

if ! test -d code_linter; then
    # Get our fork of codespell that adds --words-white-list and full filename support for -S option
    mkdir code_linter
    cd code_linter
    git clone https://github.com/google/styleguide
    cd styleguide
    git checkout gh-pages
    cd ../..
fi

DIRECTORY=$1
if test -z "$DIRECTORY"; then
    echo --------------------
    echo ------   *.h  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py src/*/src/*.h
    echo --------------------
    echo ------ *.hpp  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --extensions=hpp --headers=hpp --filter=-runtime/references src/*/src/*.hpp
    echo --------------------
    echo ------   *.c  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --extensions=c src/*/src/*.c
    echo --------------------
    echo ------ *.cpp  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --filter=-runtime/references src/*/src/*.cpp
else
    echo --------------------
    echo ------   *.h  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py src/$DIRECTORY/src/*.h
    echo --------------------
    echo ------ *.hpp  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --extensions=hpp --headers=hpp --filter=-runtime/references src/$DIRECTORY/src/*.hpp
    echo --------------------
    echo ------   *.c  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --extensions=c --filter=-readability/casting src/$DIRECTORY/src/*.c
    echo --------------------
    echo ------ *.cpp  ------
    echo --------------------
    python code_linter/styleguide/cpplint/cpplint.py --filter=-runtime/references src/$DIRECTORY/src/*.cpp
fi


