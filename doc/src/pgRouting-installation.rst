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
  (`3.2 <https://docs.pgrouting.org/3.2/en/pgRouting-installation.html>`__)
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

Instructions for downloading and installing binaries for different Operative systems instructions and additional notes and corrections not included in this documentation can be found in `Installation wiki <https://github.com/pgRouting/pgrouting/wiki/Notes-on-Download%2C-Installation-and-building-pgRouting>`__

To use pgRouting postGIS needs to be installed, please read the information about installation in this `Install Guide <https://www.postgis.us/presentations/postgis_install_guide_22.html>`__

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

Once pgRouting is installed, it needs to be enabled in each individual database you want to use it in.

.. code-block:: bash

    createdb routing
    psql routing -c 'CREATE EXTENSION PostGIS'
    psql routing -c 'CREATE EXTENSION pgRouting'


.. _install_get_sources:

Get the sources
-------------------------------------------------------------------------------


The pgRouting latest release can be found in https://github.com/pgRouting/pgrouting/releases/latest

.. rubric:: wget

To download this release:

.. code-block:: bash

    wget -O pgrouting-${PROJECT_VERSION}.tar.gz https://github.com/pgRouting/pgrouting/archive/v${PROJECT_VERSION}.tar.gz

Goto :ref:`install-short` to the extract and compile instructions.

.. rubric:: git

To download the repository

.. code-block:: bash

    git clone git://github.com/pgRouting/pgrouting.git
    cd pgrouting
    git checkout v${PROJECT_VERSION}

Goto :ref:`install-short` to the compile instructions (there is no tar ball).



.. _install_enable_db:

Enabling and upgrading in the database
----------------------------------------------

.. rubric:: Enabling the database

pgRouting is an extension and depends on postGIS. Enabling postGIS before enabling pgRouting in the database

.. code-block:: sql

  CREATE EXTENSION postgis;
  CREATE EXTENSION pgrouting;


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

To be able to compile pgRouting, make sure that the following dependencies are met:

* C and C++0x compilers
  * Compiling with Boost 1.56 up to Boost 1.74 requires C++ Compiler with C++03 or C++11 standard support
  * Compiling with Boost 1.75 requires C++ Compiler with C++14 standard support
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

.. code-block:: none

    sudo apt-get install
        postgresql-10 \
        postgresql-server-dev-10 \
        postgresql-10-postgis


.. rubric:: Build dependencies

.. code-block:: none

    sudo apt-get install
        cmake \
        g++ \
        libboost-graph-dev

.. rubric:: Optional dependencies

For documentation and testing

.. code-block:: none

    sudo apt-get install -y python-sphinx \
        texlive \
        doxygen \
        libtap-parser-sourcehandler-pgtap-perl \
        postgresql-10-pgtap


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

Most of the effort of the documentation has being on the HTML files.
Some variables for the documentation:

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

Configuring with documentation

.. code-block:: bash

    $ cmake -DWITH_DOC=ON ..

.. note:: Most of the effort of the documentation has being on the html files.


.. _install_build:

Building
----------------------------------------------------------------

Using ``make`` to build the code and the documentation

The following instructions start from *path/to/pgrouting/build*

.. code-block:: bash

    $ make          # build the code but not the documentation
    $ make doc      # build only the documentation
    $ make all doc  # build both the code and the documentation


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

When the configuration changes:

.. code-block:: bash

    rm -rf build

and start the build process as mentioned above.

.. _install_testing:

Testing
-------------------------

Currently there is no :code:`make test` and testing is done as follows

The following instructions start from *path/to/pgrouting/*

.. code-block:: none

    tools/testers/doc_queries_generator.pl
    createdb  -U <user> ___pgr___test___
    sh ./tools/testers/pg_prove_tests.sh <user>
    dropdb  -U <user> ___pgr___test___

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`


