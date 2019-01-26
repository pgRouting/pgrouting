..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_analyzeGraph
===============================================================================

``pgr_analyzeGraph`` — Analyzes the network topology.

.. rubric:: Availability

* New in version 2.0.0

Description
-------------------------------------------------------------------------------

The function returns:

- ``OK`` after the analysis has finished.
- ``FAIL`` when the analysis was not completed due to an error.

.. index::
	single: analyzeGraph(Complete Signature)

.. code-block:: sql

	varchar pgr_analyzeGraph(text edge_table, double precision tolerance,
                           text the_geom:='the_geom', text id:='id',
                           text source:='source',text target:='target',text rows_where:='true')

.. rubric:: Prerequisites

The  edge table to be analyzed must contain a source column and a target column filled with id's of the vertices of the segments and the corresponding vertices table <edge_table>_vertices_pgr that stores the vertices information.

- Use :doc:`pgr_createVerticesTable <pgr_createVerticesTable>` to create the vertices table.
- Use :doc:`pgr_createTopology <pgr_createTopology>` to create the topology and the vertices table.

Parameters
-------------------------------------------------------------------------------

The analyze graph function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:tolerance: ``float8`` Snapping tolerance of disconnected edges. (in projection unit)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.
:id: ``text``  Primary key column name of the network table. Default value is ``id``.
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``.
:rows_where: ``text``   Condition to select  a subset or rows.  Default value is ``true`` to indicate all rows.

The function returns:

- ``OK`` after the analysis has finished.

  * Uses the vertices table: <edge_table>_vertices_pgr.
  * Fills completely the ``cnt`` and ``chk`` columns of the vertices table.
  * Returns the analysis of the section of the network defined by  ``rows_where``

- ``FAIL`` when the analysis was not completed due to an error.

  * The vertices table is not found.
  * A required column of the Network table is not found or is not of the appropriate type.
  * The condition is not well formed.
  * The names of source , target or id are the same.
  * The SRID of the geometry could not be determined.


.. rubric:: The Vertices Table

The vertices table can be created with :doc:`pgr_createVerticesTable <pgr_createVerticesTable>` or :doc:`pgr_createTopology <pgr_createTopology>`

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex.
:chk: ``integer``  Indicator that the vertex might have a problem.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:the_geom: ``geometry`` Point geometry of the vertex.

Usage when the edge table's columns MATCH the default values:
...............................................................................

.. rubric:: The simplest way to use pgr_analyzeGraph is:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q1
   :end-before: -- q3

.. rubric:: When the arguments are given in the order described in the parameters:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q3
   :end-before: -- q3.1

We get the same result as the simplest way to use the function.

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``id`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``the_geom`` is passed to the function as the id column.

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q4
   :end-before: -- q4.1


.. rubric:: When using the named notation

The order of the parameters do not matter:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q5
   :end-before: -- q5.1

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q6.1
   :end-before: -- q6.2

Parameters defined with a default value can be omitted, as long as the value matches the default:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q6.3
   :end-before: -- q6.4

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id. Displays the analysis a the section of the network.

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q7
   :end-before: -- q7.1

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q8
   :end-before: -- q8.1

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q9
   :end-before: -- q10

Usage when the edge table's columns DO NOT MATCH the default values:
...............................................................................

For the following table

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q11
   :end-before: -- q12

.. rubric:: Using positional notation:

The arguments need to be given in the order described in the parameters:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q13
   :end-before: -- q13.1

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``gid`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``mygeom`` is passed to the function as the id column.

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q14
   :end-before: -- q14.1

.. rubric:: When using the named notation

The order of the parameters do not matter:

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q15
   :end-before: -- q15.1

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q16
   :end-before: -- q16.1

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.


.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q17
   :end-before: -- q17.1

.. literalinclude:: pgr_analyzeGraph.queries
   :start-after: -- q18
   :end-before: -- q18.1

Selecting the rows WHERE the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');

Selecting the rows WHERE the geometry is near the place='myhouse' of the table ``othertable``. (note the use of quote_literal)

.. code-block:: sql

        DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 'myhouse'::text AS place, st_point(2.5,2.5) AS other_geom) ;
	SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',
                 rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
                 rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE place='||quote_literal('myhouse')||')');



Additional Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT  pgr_createTopology('edge_table',0.001);
	SELECT pgr_analyzeGraph('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 2
	NOTICE:                            Dead ends: 7
	NOTICE:  Potential gaps found near dead ends: 1
	NOTICE:               Intersections detected: 1
	NOTICE:                      Ring geometries: 0

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)

	SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','id < 10')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 0
	NOTICE:                            Dead ends: 4
	NOTICE:  Potential gaps found near dead ends: 0
	NOTICE:               Intersections detected: 0
	NOTICE:                      Ring geometries: 0

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)

	SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id >= 10');
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','id >= 10')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 2
	NOTICE:                            Dead ends: 8
	NOTICE:  Potential gaps found near dead ends: 1
	NOTICE:               Intersections detected: 1
	NOTICE:                      Ring geometries: 0

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)

	-- Simulate removal of edges
	SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17');
	SELECT pgr_analyzeGraph('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 0
	NOTICE:                            Dead ends: 3
	NOTICE:  Potential gaps found near dead ends: 0
	NOTICE:               Intersections detected: 0
	NOTICE:                      Ring geometries: 0

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)
    SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id <17');
    NOTICE:  PROCESSING:
    NOTICE:  pgr_createTopology('edge_table',0.001,'the_geom','id','source','target','id <17')
    NOTICE:  Performing checks, pelase wait .....
    NOTICE:  Creating Topology, Please wait...
    NOTICE:  -------------> TOPOLOGY CREATED FOR  16 edges
    NOTICE:  Rows with NULL geometry or NULL id: 0
    NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
    NOTICE:  ----------------------------------------------

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)

    SELECT pgr_analyzeGraph('edge_table', 0.001);
    NOTICE:  PROCESSING:
    NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','true')
    NOTICE:  Performing checks, pelase wait...
    NOTICE:  Analyzing for dead ends. Please wait...
    NOTICE:  Analyzing for gaps. Please wait...
    NOTICE:  Analyzing for isolated edges. Please wait...
    NOTICE:  Analyzing for ring geometries. Please wait...
    NOTICE:  Analyzing for intersections. Please wait...
    NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
    NOTICE:                    Isolated segments: 0
    NOTICE:                            Dead ends: 3
    NOTICE:  Potential gaps found near dead ends: 0
    NOTICE:               Intersections detected: 0
    NOTICE:                      Ring geometries: 0

	 pgr_analyzeGraph
	--------------------
	 OK
	(1 row)


The examples use the :doc:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`  for an overview of a topology for routing algorithms.
* :doc:`pgr_analyzeOneWay` to analyze directionality of the edges.
* :doc:`pgr_createVerticesTable <pgr_createVerticesTable>` to reconstruct the vertices table based on the source and target information.
* :doc:`pgr_nodeNetwork <pgr_nodeNetwork>` to create nodes to a not noded edge table.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
