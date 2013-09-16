#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Prepare packaging (sample)
# ------------------------------------------------------------------------------

echo "DEBEMAIL=daniel@georepublic.de" > ~/.bash_aliases
echo "DEBFULLNAME='Daniel Kastl (Georepublic)'" >> ~/.bash_aliases

git config --global user.name "Daniel Kastl"
git config --global user.email "daniel@georepublic.de"
git config --global core.editor vim
git config --global color.ui true
