.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _man:

pgRouting Reference
===============================================================================

pgRouting extends the `PostGIS <http://postgis.net>`_/`PostgreSQL <http://postgresql.org>`_ geospatial database to provide geospatial routing and other network analysis functionality.

.. toctree::
	:maxdepth: 1

	src/introduction/index

pgRouting provides several :ref:`common functions <common>`:

.. toctree::
	:maxdepth: 1
	:hidden:

	../src/common/doc/index

.. toctree::
	:maxdepth: 1
	:glob: 

	../src/common/doc/functions/*

pgRouting defines a few :ref:`custom data types <data_types>`:

.. toctree::
	:maxdepth: 1
	:hidden:

	../src/common/doc/types/index

.. toctree::
	:maxdepth: 1
	:glob: 

	../src/common/doc/types/*

pgRouting functions in alphabetical order:

.. toctree::
	:maxdepth: 1

	../src/apsp_johnson/doc/index
	../src/apsp_warshall/doc/index
	../src/astar/doc/index
	../src/bd_astar/doc/index
	../src/bd_dijkstra/doc/index
	../src/dijkstra/doc/index
	../src/kdijkstra/doc/index
	../src/ksp/doc/index
	../src/tsp/doc/index
	../src/trsp/doc/index

If pgRouting is compiled with "Driving Distance" enabled:

.. toctree::
	:maxdepth: 1

	../src/driving_distance/doc/dd_alphashape
	../src/driving_distance/doc/index
	../src/driving_distance/doc/dd_points_as_polygon

Some functions from previous releases may have been removed. 

.. toctree::
	:maxdepth: 1

	src/developer/discontinued

.. toctree::
	:maxdepth: 1

	src/introduction/index
	src/introduction/support


