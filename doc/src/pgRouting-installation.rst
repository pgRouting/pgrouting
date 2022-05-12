..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgRouting-installation.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgRouting-installation.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgRouting-installation.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgRouting-installation.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgRouting-installation.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgRouting-installation.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgRouting-installation.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgRouting-installation.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/doc/src/installation/installation.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/doc/src/installation/installation.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/doc/src/installation/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/doc/src/installation/index.html>`__

Installation
===============================================================================

.. rubric:: Table of Contents

* :ref:`install-short`
* :ref:`install_get_sources`
* :ref:`install_enable_db`
* :ref:`install_dependencies`
* :ref:`install_configuring`
* :ref:`install_build`
* :ref:`install_testing`

Instructions for downloading and installing binaries for different operating 
systems, additional notes and corrections not included in this documentation can
be found in `Installation wiki <https://github.com/pgRouting/pgrouting/wiki/Notes-on-Download%2C-Installation-and-building-pgRouting>`__

To use pgRouting PostGIS needs to be installed, please read the information 
about installation in this `Install Guide <https://www.postgis.us/presentations/postgis_install_guide_22.html>`__

.. _install-short:

Short Version
-------------------------------------------------------------------------------


Extracting the tar ball

.. code-block:: bash

    tar xvfz pgrouting-${PROJECT_VERSION}.tar.gz
    cd pgrouting-${PROJECT_VERSION}

To compile assuming you have all the dependencies in your search path:

.. code-block:: bash

    mkdir build
    cd build
    cmake  ..
    make
    sudo make install

Once pgRouting is installed, it needs to be enabled in each individual 
database you want to use it in.

.. code-block:: bash

    createdb routing
    psql routing -c 'CREATE EXTENSION PostGIS'
    psql routing -c 'CREATE EXTENSION pgRouting'


.. _install_get_sources:

Get the sources
-------------------------------------------------------------------------------


The pgRouting latest release can be found in 
https://github.com/pgRouting/pgrouting/releases/latest

.. rubric:: wget

To download this release:

.. code-block:: bash

    wget -O pgrouting-${PROJECT_VERSION}.tar.gz https://github.com/pgRouting/pgrouting/archive/v${PROJECT_VERSION}.tar.gz

Go to :ref:`install-short` for more instructions on extracting tar ball and compiling pgRouting.

.. rubric:: git

To download the repository

.. code-block:: bash

    git clone git://github.com/pgRouting/pgrouting.git
    cd pgrouting
    git checkout v${PROJECT_VERSION}

Go to :ref:`install-short` for more instructions on compiling pgRouting
(there is no tar ball involved while downloading pgRouting repository from GitHub).



.. _install_enable_db:

Enabling and upgrading in the database
----------------------------------------------

.. rubric:: Enabling the database

pgRouting is a PostgreSQL extension and depends on PostGIS to provide functionalities 
to end user. Below given code demonstrates enabling PostGIS and pgRouting in the 
database.

.. code-block:: sql

    CREATE EXTENSION postgis;
    CREATE EXTENSION pgrouting;

Checking PostGIS and pgRouting version after enabling them in the database.

.. code-block:: sql

    SELECT PostGIS_full_version();
    SELECT * FROM pgr_version();

.. rubric:: Upgrading the database

To upgrade pgRouting in the database to version ${PROJECT_VERSION} use the following command:

.. TODO: pumpup release must change this value

.. code-block:: sql

   ALTER EXTENSION pgrouting UPDATE TO "${PROJECT_VERSION}";

More information can be found in https://www.postgresql.org/docs/current/sql-createextension.html


.. _install_dependencies:

Dependencies
-------------------------------------------------------------------------------

.. rubric:: Compilation Dependencies

To be able to compile pgRouting, make sure that the following dependencies are 
met:

* C and C++0x compilers

    * Compiling with Boost 1.56 up to Boost 1.74 requires C++ Compiler with 
      C++03 or C++11 standard support
    * Compiling with Boost 1.75 requires C++ Compiler with C++14 standard 
      support
      
* Postgresql version = Supported versions by PostgreSQL
* The Boost Graph Library (BGL). Version >= 1.56
* CMake >= 3.2


.. rubric:: optional dependencies

For user's documentation

* Sphinx >= 1.1
* Latex

For developer's documentation

* Doxygen >= 1.7

For testing

* pgtap
* pg_prove

For using:

* PostGIS version >= 2.2

.. rubric:: Example: Installing dependencies on linux

Installing the compilation dependencies

.. rubric:: Database dependencies

.. code-block:: bash

    sudo apt install postgresql-14
    sudo apt install postgresql-server-dev-14
    sudo apt install postgresql-14-postgis

.. rubric:: Configuring PostgreSQL

Entering psql console

.. code-block:: bash

    sudo systemctl start postgresql.service
    sudo -i -u postgres
    psql

To exit psql console

.. code-block:: psql

    \q 

Entering psql console directly without switching roles can be done by the 
following commands

.. code-block:: bash

    sudo -u postgres psql

Then use the above given method to exit out of the psql console

Checking PostgreSQL version

.. code-block:: bash

    psql --version

or

Enter the psql console using above given method and then enter

.. code-block:: sql

    SELECT VERSION();

Creating PostgreSQL role

.. code-block:: bash

    sudo -i -u postgres
    createuser --interactive

or

.. code-block:: bash

    sudo -u postgres createuser --interactive

Default role provided by PostgreSQL is postgres. To create new roles you
can use the above provided commands. The prompt will ask the user to type 
name of the role and then provide affirmation. Proceed with the steps and 
you will succeed in creating PostgreSQL role successfully.

To add password to the role or change previously created password of the
role use the following commands

.. code-block:: sql

    ALTER USER <role name> PASSWORD <password>

To get additional details on the flags associated with ``createuser`` below 
given command can be used

.. code-block:: none

    man createuser

Creating Database in PostgreSQL

.. code-block:: bash

    sudo -i -u postgres
    createdb <database name>

or 

.. code-block:: bash

    sudo -u postgres createdb <database name>
    
Connecting to a PostgreSQL Database

Enter the psql console and type the following commands

.. code-block:: psql

    \connect <database name>

.. rubric:: Build dependencies

.. code-block:: bash

    sudo apt install cmake
    sudo apt install g++
    sudo apt install libboost-graph-dev

.. rubric:: Optional dependencies

For documentation and testing

.. code-block:: bash

    pip install sphinx
    pip install sphinx-bootstrap-theme
    sudo apt install texlive 
    sudo apt install doxygen
    sudo apt install libtap-parser-sourcehandler-pgtap-perl
    sudo apt install postgresql-14-pgtap


.. _install_configuring:

Configuring
-------------------------

pgRouting uses the `cmake` system to do the configuration.

The build directory is different from the source directory

Create the build directory

.. code-block:: bash

    $ mkdir build

Configurable variables
.......................

.. rubric:: To see the variables that can be configured


.. code-block:: bash

    $ cd build
    $ cmake -L ..


.. rubric:: Configuring The Documentation

Most of the effort of the documentation has been on the HTML files.
Some variables for building documentation:

================== ========= ============================
Variable            Default     Comment
================== ========= ============================
WITH_DOC           BOOL=OFF  Turn on/off building the documentation
BUILD_HTML         BOOL=ON   If ON, turn on/off building HTML for user's documentation
BUILD_DOXY         BOOL=ON   If ON, turn on/off building HTML for developer's documentation
BUILD_LATEX        BOOL=OFF  If ON, turn on/off building PDF
BUILD_MAN          BOOL=OFF  If ON, turn on/off building MAN pages
DOC_USE_BOOTSTRAP  BOOL=OFF  If ON, use sphinx-bootstrap for HTML pages of the users documentation
================== ========= ============================

Configuring cmake to create documentation before building
pgRouting

.. code-block:: bash

    $ cmake -DWITH_DOC=ON -DDOC_USE_BOOTSTRAP=ON ..

.. note:: Most of the effort of the documentation has been on the html files.


.. _install_build:

Building
----------------------------------------------------------------

Using ``make`` to build the code and the documentation

The following instructions start from *path/to/pgrouting/build*

.. code-block:: bash

    $ make          # build the code but not the documentation
    $ make doc      # build only the user's documentation
    $ make all doc  # build both the code and the user's documentation
    $ make doxy     # build only the developer's documentation


We have tested on several platforms, For installing or reinstalling all the steps are needed.

.. warning:: The sql signatures are configured and build in the ``cmake`` command.

.. rubric:: MinGW on Windows

.. code-block:: bash

    $ mkdir build
    $ cd build
    $ cmake -G"MSYS Makefiles" ..
    $ make
    $ make install


.. rubric:: Linux

The following instructions start from *path/to/pgrouting*

.. code-block:: bash

    mkdir build
    cd build
    cmake  ..
    make
    sudo make install

To remove the build when the configuration changes, use the following
code:

.. code-block:: bash

    rm -rf build

and start the build process as mentioned previously.

.. _install_testing:

Testing
-------------------------

Currently there is no :code:`make test` and testing is done as follows

The following instructions start from *path/to/pgrouting/*

.. code-block:: none

    tools/testers/doc_queries_generator.pl
    createdb -U <user> ___pgr___test___
    sh ./tools/testers/pg_prove_tests.sh <user>
    dropdb -U <user> ___pgr___test___

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


