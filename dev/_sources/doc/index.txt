.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _index:

Table of Contents
===============================================================================

pgRouting extends the `PostGIS <http://postgis.net>`_/`PostgreSQL <http://postgresql.org>`_ geospatial database to provide geospatial routing and other network analysis functionality.

This is the manual for version |release|.

.. image:: static/images/ccbysa.png
	:align: left
	:alt: Creative Commons Attribution-Share Alike 3.0 License

This work is licensed under a `Creative Commons Attribution-Share Alike 3.0 License <http://creativecommons.org/licenses/by-sa/3.0/>`_. Feel free to use this material any way you like, but we ask that you attribute credit to the pgRouting Project and wherever possible, a link back to http://pgrouting.org.

*******************************************************************************
General
*******************************************************************************

.. toctree::
	:maxdepth: 1

	src/introduction/index
	src/installation/index

*******************************************************************************
Tutorial
*******************************************************************************

.. toctree::
	:maxdepth: 1

	src/tutorial/index
	src/tutorial/custom_query
	src/tutorial/custom_wrapper
	src/tutorial/recipes
	src/tutorial/performance

For a more complete introduction how to build a routing application read the `pgRouting Workshop <http://workshop.pgrouting.org>`_ (current version written for pgRouting 1.x).

*******************************************************************************
Reference
*******************************************************************************

.. toctree::
	:maxdepth: 1

	../src/common/doc/index
	../src/common/doc/types

pgRouting functions alphabetical order:

.. toctree::
	:maxdepth: 1

	../src/apsp_johnson/doc/index
	../src/apsp_warshall/doc/index
	../src/astar/doc/index
	../src/bd_dijkstra/doc/index
	../src/bd_astar/doc/index
	../src/dijkstra/doc/index
	../src/driving_distance/doc/index
	../src/ksp/doc/index
	../src/tsp/doc/index
	../src/trsp/doc/index


*******************************************************************************
Developer
*******************************************************************************

.. toctree::
	:maxdepth: 1

	src/installation/build
	src/developer/index


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

