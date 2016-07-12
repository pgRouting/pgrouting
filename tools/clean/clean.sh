#!/bin/sh
# ------------------------------------------------------------------------------
#
# Basic common coding style errors cleaning
# Copyright(c) pgRouting Contributors
#
# -------- USE
#
# ./path/to/here/clean <extension>
#
# Example
# ./path/to/here/clean cpp
# ------------------------------------------------------------------------------


EXTENSION=$1

echo $EXTENSION
# remove trailing spaces
perl -pi -e 's/\s\+$//' *.$EXTENSION

# " )" -> ")"
perl -pi -e 's/\s\+\)/)/g' *.$EXTENSION
# " (" -> "("
perl -pi -e 's/(\S)\s\+\(/$1(/g' *.$EXTENSION

# add a space before & after =
perl -pi -e 's/(\S)=/$1 = /g' *.$EXTENSION

#fix >= == <= != += -= *=
perl -pi -e 's/(\*|!|\+|-|<|=|>) =/$1=/g' *.$EXTENSION

# add a space after: , << 
perl -pi -e 's/(,|<<)(\S)/$1 $2/g' *.$EXTENSION
perl -pi -e 's/(,|<<|=)[ ]+$/$1/' *.$EXTENSION
perl -pi -e 's/(,|<<|=) +/$1 /g' *.$EXTENSION

# add a space around: && ||
perl -pi -e 's/(&&|\|\|)/ $1 /g' *.$EXTENSION

# "( " -> "("
perl -pi -e 's/\(\s\+/(/g' *.$EXTENSION

# Missing space before {
perl -pi -e 's/(\S){/$1 {/g' *.$EXTENSION


# add a space before: <<, ==, <=, >=
perl -pi -e 's/(\S)(<<|<=|==|>=)/$1 $2/g' *.$EXTENSION

# 2 spaces -> 1 space
perl -pi -e 's/(\S+)  (\S+)/$1 $2/g' *.$EXTENSION

# At least two spaces is best between code and comments
perl -pi -e 's/(\S+) \/\//$1  \/\/ /g' *.$EXTENSION

# if( for(
perl -pi -e 's/\s(if|for|while)\(/ $1 \(/g' *.$EXTENSION

# "( " -> "("
perl -pi -e 's/\(\s\+/(/g' *.$EXTENSION

# remove trailing spaces
perl -pi -e 's/\s\+$//' *.$EXTENSION
perl -pi -e 's/ +$//' *.$EXTENSION

