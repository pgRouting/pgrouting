.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _build:

###########
Build Guide
###########

Dependencies
===============================================================================

To be able to compile pgRouting make sure that the following dependencies are met:

* C and C++0x compilers
* Postgresql version >= 9.1 
* PostGIS version >= 2.0 
* The Boost Graph Library (BGL). Version >= 1.55
* CMake >= 2.8.8
* CGAL >=  4.2
* (optional, for Documentation) Sphinx >= 1.1 
* (optional, for Documentation as PDF) Latex >= [TBD]

Configuration
===============================================================================

PgRouting uses the `cmake` system to do the configuration.

The following instructions start from *path/to/pgrouting/*

Ccreate the build directory

.. code-block:: bash

    $ mkdir build


To configure:

.. code-block:: bash

    $ cd build
    $ cmake -L ..

Configurable variables
----------------------

The documentation configurable variables are:

:WITH_DOC:    BOOL=OFF     -- Turn on/off building the documentation
:BUILD_HTML:  BOOL=ON     -- If WITH_DOC=ON, turn on/off building HTML
:BUILD_LATEX: BOOL=OFF    -- If WITH_DOC=ON, turn on/off building PDF
:BUILD_MAN:   BOOL=OFF    -- If WITH_DOC=ON, turn on/off building MAN pages

Configuring with documentation

.. code-block:: bash

    $ cmake -DWITH_DOC=ON ..

.. note:: Most of the effort of the documentation has being on the html files.


Building
===============================================================================

Using ``make`` to build the code and the docuemtnation

The following instructions start from *path/to/pgrouting/build*

.. code-block:: bash

    $ make          # build the code but not the documentation
    $ make doc      # build only the documentation
    $ make all doc  # build both the code and the documentation

Installation and reinstallation
===============================================================================

We have tested on several plataforms, For installing or reinstalling all the steps are needed.

.. warning:: The sql signatures are configured and build in the ``cmake`` command.

For MinGW on Windows
----------------------------------------------------------------

.. code-block:: bash

	$ mkdir build
	$ cd build
	$ cmake -G"MSYS Makefiles" ..
	$ make
	$ make install


For Linux
----------------------------------------------------------------

The following instructions start from *path/to/pgrouting*

.. code-block:: bash

	$ mkdir build
	$ cd build
	$ cmake  ..
	$ make
	$ sudo make install



