.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _build:

Build Guide
===============================================================================

To be able to compile pgRouting make sure that the following dependencies are met:

* C and C++0x compilers
* Postgresql version >= 9.1 
* PostGIS version >= 2.0 
* The Boost Graph Library (BGL). Version >= 1.58
* CMake >= 2.8.8
* CGAL >=  4.2
* (optional, for Documentation) Sphinx >= 1.1 
* (optional, for Documentation as PDF) Latex >= [TBD]

The cmake system has variables the can be configured via the command line options by setting them with -D<varable>=<value>. You can get a listing of these via:

.. code-block:: bash

    mkdir build
    cd build
    cmake -L ..

Currently the cached variables are:

  -  Boost_DIR:PATH
  -  CMAKE_BUILD_TYPE:STRING
  -  CMAKE_INSTALL_PREFIX:PATH
  -  POSTGRESQL_EXECUTABLE:FILEPATH
  -  POSTGRESQL_PG_CONFIG:FILEPATH


These also show the current or default values based on our development system. So your values my be different. In general the ones that are of most interest are:

  -  WITH_DOC:BOOL=OFF     -- Turn on/off building the documentation
  -  BUILD_HTML:BOOL=OFF    -- If WITH_DOC=ON, turn on/off building HTML
  -  BUILD_LATEX:BOOL=OFF  -- If WITH_DOC=ON, turn on/off building PDF
  -  BUILD_MAN:BOOL=OFF     -- If WITH_DOC=ON, turn on/off building MAN pages

To change any of these add ``-D<variable>=<value>`` to the cmake lines below. For example to turn on documentation, your cmake command might look like:

.. code-block:: bash

    cmake -DWITH_DOC=OFF ..   # Turn off the doc with default settings
    cmake -DWITH_DOC=ON -DBUILD_LATEX ..  # Turn on doc and pdf

If you turn on the documentation, you also need to add the ``doc`` target to the make command.

.. code-block:: bash

    make          # build the code but not the doc
    make doc      # build only the doc
    make all doc  # build both the code and the doc


For MinGW on Windows
********************************************************************************

.. code-block:: bash

	mkdir build
	cd build
	cmake -G"MSYS Makefiles" ..
	make
	make install


For Linux
********************************************************************************

.. code-block:: bash

	mkdir build
	cd build
	cmake  ..
	make
	sudo make install


With Documentation
********************************************************************************

Build with documentation (requires `Sphinx <http://sphinx-doc.org/>`_):

.. code-block:: bash

	cmake -DWITH_DOC=ON ..
    make all doc


Rebuild modified documentation only:

.. code-block:: bash

	sphinx-build -b html -c build/doc/_build -d build/doc/_doctrees . build/html

