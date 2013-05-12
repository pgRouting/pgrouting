.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _index:

pgRouting Manual
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
	:maxdepth: 2

	src/introduction/index
	src/installation/index


*******************************************************************************
Reference
*******************************************************************************

.. toctree::
	:maxdepth: 2

	../src/common/doc/index

.. toctree::
	:maxdepth: 1

	../src/dijkstra/doc/index
	../src/astar/doc/index
	../src/bd_dijkstra/doc/index
	../src/bd_astar/doc/index
	../src/trsp/doc/index
	../src/apsp_johnson/doc/index
	../src/apsp_warshall/doc/index
	../src/ksp/doc/index
	../src/tsp/doc/index
	../src/driving_distance/doc/index


*******************************************************************************
Developer
*******************************************************************************

.. toctree::
	:maxdepth: 2

	src/installation/build
	src/developer/index


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

