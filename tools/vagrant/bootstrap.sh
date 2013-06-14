#!/bin/bash
# ------------------------------------------------------------------------------
# Vagrant - Virtualized Development 
# Copyright(c) pgRouting Contributors
#
# Virtual environment bootstrap script
# ------------------------------------------------------------------------------

# Enable PPA support
# ------------------------------------------------------------------------------
apt-get update -qq
apt-get install -y -qq python-software-properties

# Add PPA's'
# ------------------------------------------------------------------------------
apt-add-repository -y ppa:georepublic/pgrouting-travis
#apt-add-repository -y ppa:ubuntugis/ubuntugis-unstable

# Add PostgreSQL Apt repository
# ------------------------------------------------------------------------------
#echo "deb http://apt.postgresql.org/pub/repos/apt/ $(lsb_release -cs)-pgdg main" > pgdg.list
#mv pgdg.list /etc/apt/sources.list.d/
#wget --quiet -O - http://apt.postgresql.org/pub/repos/apt/ACCC4CF8.asc | sudo apt-key add -

apt-get update -qq

# Install packages
# ------------------------------------------------------------------------------
apt-get install -y -qq cmake git


