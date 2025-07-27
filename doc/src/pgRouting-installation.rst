..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Installation
===============================================================================

.. rubric:: Table of Contents

Instructions for downloading and installing binaries for different operating
systems, additional notes and corrections not included in this documentation can
be found in `Installation wiki
<https://github.com/pgRouting/pgrouting/wiki/Notes-on-Download%2C-Installation-and-building-pgRouting>`__

To use pgRouting, PostGIS needs to be installed, please read the information
about installation in this `Install Guide
<https://www.postgis.us/presentations/postgis_install_guide_22.html>`__

.. _install-short:

Short Version
-------------------------------------------------------------------------------


Extracting the tar ball

.. code-block:: shell

    tar xvfz pgrouting-${PROJECT_VERSION}.tar.gz
    cd pgrouting-${PROJECT_VERSION}

To compile assuming you have all the dependencies in your search path:

.. code-block:: shell

    mkdir build
    cd build
    cmake  ..
    make
    sudo make install

Once pgRouting is installed, it needs to be enabled in each individual
database you want to use it in.

.. code-block:: shell

    createdb routing
    psql routing -c 'CREATE EXTENSION pgRouting CASCADE'


Get the sources
-------------------------------------------------------------------------------

The pgRouting latest release can be found in
https://github.com/pgRouting/pgrouting/releases/latest

To download this release:

.. code-block:: shell

    wget -O pgrouting-${PROJECT_VERSION}.tar.gz \
       https://github.com/pgRouting/pgrouting/archive/v${PROJECT_VERSION}.tar.gz

Go to :ref:`install-short` for more instructions on extracting tar ball and
compiling pgRouting.

.. rubric:: git

To download the repository

.. code-block:: shell

    git clone git://github.com/pgRouting/pgrouting.git
    cd pgrouting
    git checkout v${PROJECT_VERSION}

Go to :ref:`install-short` for more instructions on compiling pgRouting
(there is no tar ball involved while downloading pgRouting repository from
GitHub).


Enabling and upgrading in the database
-------------------------------------------------------------------------------

.. rubric:: Enabling the database

pgRouting is a PostgreSQL extension and depends on PostGIS to provide
functionalities to end user. Below given code demonstrates enabling PostGIS and
pgRouting in the database.

.. code-block:: sql

    CREATE EXTENSION postgis;
    CREATE EXTENSION pgrouting;

Checking PostGIS and pgRouting version after enabling them in the database.

.. code-block:: sql

    SELECT PostGIS_full_version();
    SELECT * FROM pgr_version();

.. rubric:: Upgrading the database

To upgrade pgRouting in the database to version ${PROJECT_VERSION} use the
following command:

.. parsed-literal::

   ALTER EXTENSION pgrouting UPDATE TO "${PROJECT_VERSION}";

More information can be found in
https://www.postgresql.org/docs/current/sql-createextension.html


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

* Postgresql version >= ${POSTGRESQL_MINIMUM_VERSION}
* The Boost Graph Library (BGL) >= ${BOOST_MINIMUM_VERSION}
* CMake >= ${CMAKE_MINIMUM_REQUIRED_VERSION}

For user's documentation

* Sphinx >= ${SPHINX_MINIMUM_VERSION}

For developer's documentation

* Doxygen >= ${DOXYGEN_MINIMUM_VERSION}

For testing

* pgtap
* pg_prove

For using:

* PostGIS version >= ${POSTGIS_MINIMUM_VERSION}

.. rubric:: Example: Installing dependencies on linux

Installing the compilation dependencies

.. rubric:: Build dependencies

.. code-block:: bash

   sudo apt install \
      build-essential \
      libboost-graph-dev \
      postgresql-${POSTGRESQL_MINIMUM_VERSION} \
      postgresql-server-dev-${POSTGRESQL_MINIMUM_VERSION} \
      postgresql-${POSTGRESQL_MINIMUM_VERSION}-postgis

.. rubric:: Optional dependencies

For documentation

.. code-block:: bash

   sudo apt-get install -y  \
      python3-sphinx \
      python3-sphinx-bootstrap-theme \
      texlive \
      doxygen

For testing

.. code-block:: bash

    sudo apt install \
       libtap-parser-sourcehandler-pgtap-perl
       postgresql-${POSTGRESQL_MINIMUM_VERSION}-pgtap

Configuring
-------------------------------------------------------------------------------

pgRouting uses the `cmake` system to do the configuration.

Configurable variables
...............................................................................

.. rubric:: To see the variables that can be configured


.. code-block:: bash

   mkdir build
   cd build
   cmake -L ..

The build directory is different from the source directory

.. rubric:: Configuring The Documentation

User and developers documentation are not build if prerequisites are not found.

Most of the effort of the documentation has been on the HTML files.
Some variables for building documentation:

================== ========= ============================
Variable            Default     Comment
================== ========= ============================
BUILD_HTML         BOOL=ON   If ON, turn on/off building HTML for user's
                             documentation
BUILD_DOXY         BOOL=ON   If ON, turn on/off building HTML for developer's
                             documentation
BUILD_LATEX        BOOL=OFF  If ON, turn on/off building PDF
BUILD_MAN          BOOL=OFF  If ON, turn on/off building MAN pages
DOC_USE_BOOTSTRAP  BOOL=OFF  If ON, use sphinx-bootstrap for HTML pages of the
                             users documentation
EN                 BOOL=ON   if OFF the English documentation will no be built
ES                 BOOL=ON   if OFF the Spaish documentation will no be built
ZH_HANS            BOOL=ON   if OFF the Chinese simplified documentation will no
                             be built
================== ========= ============================

Building
-------------------------------------------------------------------------------

Using ``make`` to build the code and the documentation

The following instructions start from ``path/to/pgrouting/build``

.. parsed-literal::

    $ make          # default build
    $ make doc      # build only the user's documentation
    $ make doxy     # build only the developer's documentation
    $ make all      # build both the code and the user's documentation


We have tested on several platforms, For installing or reinstalling all the
steps are needed.

.. rubric:: Linux

The following instructions start from ``path/to/pgrouting``

.. parsed-literal::

    mkdir build
    cd build
    cmake  ..
    make
    sudo make install

Testing
-------------------------------------------------------------------------------

Currently there is no :code:`make test` and testing is done as follows

The following instructions start from ``path/to/pgrouting``

.. code-block:: bash

    tools/testers/doc_queries_generator.pl
    createdb -U <user> ___pgr___test___
    sh ./tools/testers/pg_prove_tests.sh <user>
    dropdb -U <user> ___pgr___test___

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
