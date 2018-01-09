..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_analyze_graph:

pgr_analyzeGraph
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_analyzeGraph`` — Analyzes the network topology.


Synopsis
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

Description
-------------------------------------------------------------------------------

.. rubric:: Prerequisites

The  edge table to be analyzed must contain a source column and a target column filled with id's of the vertices of the segments and the corresponding vertices table <edge_table>_vertices_pgr that stores the vertices information.

- Use :ref:`pgr_createVerticesTable <pgr_create_vert_table>` to create the vertices table.
- Use :ref:`pgr_createTopology <pgr_create_topology>` to create the topology and the vertices table.

.. rubric:: Parameters

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

The vertices table can be created with :ref:`pgr_createVerticesTable <pgr_create_vert_table>` or :ref:`pgr_createTopology <pgr_create_topology>`

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex.
:chk: ``integer``  Indicator that the vertex might have a problem.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:the_geom: ``geometry`` Point geometry of the vertex.

.. rubric:: History

* New in version 2.0.0

Usage when the edge table's columns MATCH the default values:
-------------------------------------------------------------------------------

.. rubric:: The simplest way to use pgr_analyzeGraph is:

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001);
	 SELECT  pgr_analyzeGraph('edge_table',0.001);

.. rubric:: When the arguments are given in the order described in the parameters:

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target');

We get the same result as the simplest way to use the function.

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``id`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``the_geom`` is passed to the function as the id column.

.. code-block:: sql

    SELECT  pgr_analyzeGraph('edge_table',0.001,'id','the_geom','source','target');
    NOTICE:  PROCESSING:
    NOTICE:  pgr_analyzeGraph('edge_table',0.001,'id','the_geom','source','target','true')
    NOTICE:  Performing checks, please wait ...
    NOTICE:  Got function st_srid(bigint) does not exist
    NOTICE:  ERROR: something went wrong when checking for SRID of id in table public.edge_table
    pgr_analyzegraph
    ------------------
      FAIL
    (1 row)


.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');

Parameters defined with a default value can be omitted, as long as the value matches the default:

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,source:='source');

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id. Displays the analysis a the section of the network.

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='id < 10');

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. code-block:: sql

        DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
	SELECT  pgr_analyzeGraph('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');



Usage when the edge table's columns DO NOT MATCH the default values:
-------------------------------------------------------------------------------

For the following table

.. code-block:: sql

	DROP TABLE IF EXISTS mytable;
	CREATE TABLE mytable AS (SELECT id AS gid, source AS src ,target AS tgt , the_geom AS mygeom FROM edge_table);
	SELECT pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt');

.. rubric:: Using positional notation:

The arguments need to be given in the order described in the parameters:

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt');

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``gid`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``mygeom`` is passed to the function as the id column.

.. code-block:: sql

    SELECT  pgr_analyzeGraph('mytable',0.001,'gid','mygeom','src','tgt');
    NOTICE:  PROCESSING:
    NOTICE:  pgr_analyzeGraph('mytable',0.001,'gid','mygeom','src','tgt','true')
    NOTICE:  Performing checks, please wait ...
    NOTICE:  Got function st_srid(bigint) does not exist
    NOTICE:  ERROR: something went wrong when checking for SRID of gid in table public.mytable
    pgr_analyzegraph
    ------------------
      FAIL
    (1 row)



.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.


.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');

.. code-block:: sql

	 SELECT  pgr_analyzeGraph('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');

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



Examples
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

* :ref:`topology`  for an overview of a topology for routing algorithms.
* :ref:`pgr_analyze_oneway` to analyze directionality of the edges.
* :ref:`pgr_createVerticesTable <pgr_create_vert_table>` to reconstruct the vertices table based on the source and target information.
* :ref:`pgr_nodeNetwork <pgr_node_network>` to create nodes to a not noded edge table.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
