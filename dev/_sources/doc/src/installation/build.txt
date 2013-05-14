.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _build:

Build Guide
===============================================================================

To be able to compile pgRouting make sure that the following dependencies are met:

* C and C++ compilers
* Postgresql version >= [TBD]
* PostGIS version >= [TBD]
* The Boost Graph Library (BGL). Version >= [TBD]
* CMake >= 2.8.8
* (optional, for Driving Distance) CGAL >= [TBD]
* (optional, for Documentation) Sphinx >= [TBD] 


For MinGW on Windows
********************************************************************************

.. code-block:: bash

	mkdir build
	cd build
	cmake -G"MSYS Makefiles" -DWITH_DD=ON ..
	make
	make install


For Linux
********************************************************************************

.. code-block:: bash

	mkdir build
	cd build
	cmake -DWITH_DD=ON ..
	make
	sudo make install

Alternative syntax (undocumented CMake parameters):

.. code-block:: bash

	cmake -H. -Bbuild -DWITH_DOC=ON -DWITH_DD=ON 
	cd build && make && cd ..


With Documentation
********************************************************************************

Build with documentation (requires `Sphinx <http://sphinx-doc.org/>`_):

.. code-block:: bash

	cmake -DWITH_DOC=ON -DWITH_DD=ON ..


Rebuild modified documentation only:

.. code-block:: bash

	sphinx-build -b html -c build/doc/_build -d build/doc/_doctrees . build/html

