.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _dependencies:

##########################
Dependencies Installation
##########################

This guide was made while making a fresh ubuntu desktop 14.04.02 installation.
Make the neceszry adjustments to fit your operative system.

Dependencies
===============================================================================


To be able to compile pgRouting make sure that the following dependencies are met:

* C and C++0x compilers
* Postgresql version >= 9.1 
* PostGIS version >= 2.0 
* The Boost Graph Library (BGL). Version >= 1.46
* CMake >= 2.8.8
* CGAL >=  4.2
* (optional, for Documentation) Sphinx >= 1.1 
* (optional, for Documentation as PDF) Latex >= [TBD]

Before starting, on a terminal window:

sudo apt-get update


CMake >= 2.8.8
---------------------

trusty provides: 2.8.8

.. code-block:: none

    sudo apt-get install cmake


C and (C++0x or c++11) compilers
----------------------------------

trusty provides: 4.8

.. code-block:: none

    sudo apt-get install g++


Postgresql version >= 9.1 
--------------------------

For example in trusty 9.3 is provided:

.. code-block:: none

    sudo apt-get install postgreSQL
    sudo apt-get install postgresql-server-dev-9.3


PostGIS version >= 2.0 
-----------------------

For example in trusty 2.1 is provided:

.. code-block:: none

    sudo apt-get install postgresql-9.3-postgis-2.1



The Boost Graph Library (BGL). Version >= 1.46
----------------------------------------------

trusty provides: 1.54.0

.. code-block:: none

    sudo apt-get install libboost-graph-dev


CGAL >=  4.2
---------------------

.. code-block:: none

    sudo apt-get install libcgal-dev

(optional, for Documentation) Sphinx >= 1.1 
--------------------------------------------

http://sphinx-doc.org/latest/install.html

trusty provides: 1.2.2

.. code-block:: none

    sudo apt-get install python-sphinx


(optional, for Documentation as PDF) Latex >= [TBD]
---------------------------------------------------

https://latex-project.org/ftp.html

trusty provides: 1.2.2

.. code-block:: none

    sudo apt-get install texlive


pgTap & pg_prove & perl for tests 
-----------------------------------

.. warning:: cmake does not test for this packages.

.. code-block:: none

    sudo apt-get install -y perl
    wget https://github.com/theory/pgtap/archive/master.zip
    unzip master.zip
    cd pgtap-master
    make
    sudo make install
    sudo ldconfig
    sudo apt-get install -y libtap-parser-sourcehandler-pgtap-perl

To run the tests:

.. code-block:: none

    tools/testers/algorithm-tester.pl
    createdb  -U <user> ___pgr___test___
    sh ./tools/testers/pg_prove_tests.sh <user>
    dropdb  -U <user> ___pgr___test___

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


