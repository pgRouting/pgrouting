.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_create_topology:

pgr_createTopology
===============================================================================

.. index:: 
	single: pgr_createTopology(text,double precision,text,text,text,text,text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_createTopology`` — Builds a network topology based on the geometry information.


Synopsis
-------------------------------------------------------------------------------

The function returns:

  - ``OK`` after the network topology has been built and the vertices table created.
  - ``FAIL`` when the network topology was not built due to an error. 

.. code-block:: sql

	varchar pgr_createTopology(text edge_table, double precision tolerance, 
                           text the_geom:='the_geom', text id:='id',
                           text source:='source',text target:='target',
                           text rows_where:='true', boolean clean:=false)



Description
-------------------------------------------------------------------------------

.. rubric:: Parameters

The topology creation function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name AS well)
:tolerance: ``float8`` Snapping tolerance of disconnected edges. (in projection unit)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.  
:id: ``text``  Primary key column name of the network table. Default value is ``id``. 
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``. 
:rows_where: ``text``   Condition to SELECT a subset or rows.  Default value is ``true`` to indicate
 all rows that where ``source`` or ``target`` have a null value, otherwise the condition is used.
:clean ``boolean`` Clean any previous topology.  Default value is ``false``.

.. warning::

    The ``edge_table`` will be affected

	- The ``source`` column values will change.
	- The ``target`` column values will change.
        - An index will be created, if it doesn't exists, to speed up the process to the following columns:

	   * ``id``
	   * ``the_geom``
	   * ``source``
	   * ``target``

The function returns:

  - ``OK`` after the network topology has been built.

    * Creates a vertices table: <edge_table>_vertices_pgr.
    * Fills ``id`` and ``the_geom`` columns of the vertices table.
    * Fills the source and target columns of the edge table referencing the ``id`` of the vertices table.


  - ``FAIL`` when the network topology was not built due to an error:

    * A required column of the Network table is not found or is not of the appropriate type.
    * The condition is not well formed.
    * The names of source , target or id are the same.
    * The SRID of the geometry could not be determined.


.. rubric:: The Vertices Table

The vertices table is a requirment of the :ref:`pgr_analyzeGraph <pgr_analyze_graph>` and the :ref:`pgr_analyzeOneway <pgr_analyze_oneway>` functions.

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex AS incoming. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex AS outgoing. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`. 
:the_geom: ``geometry`` Point geometry of the vertex.

.. rubric:: History

* Renamed in version 2.0.0

Usage when the edge table's columns MATCH the default values:
-------------------------------------------------------------------------------
 
.. rubric:: The simplest way to use pgr_createtopology is: 

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001);



.. rubric:: When the arguments are given in the order described in the parameters:

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,'the_geom','id','source','target');

We get the same result AS the simplest way to use the function.

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``id`` of the table ``ege_table`` is passed to the function AS the geometry column, and the geometry column ``the_geom`` is passed to the function AS the id column. 
 | ``SELECT  pgr_createTopology('edge_table',0.001,'id','the_geom','source','target');``
 | ERROR: Can not determine the srid of the geometry "id" in table public.edge_table

.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,the_geom:='the_geom',id:='id',source:='source',target:='target');

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,source:='source',id:='id',target:='target',the_geom:='the_geom');

Parameters defined with a default value can be omited, AS long AS the value matches the default:

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,source:='source');

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,rows_where:='id < 10');

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 SELECT  pgr_createTopology('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(the_geom,0.05) FROM edge_table WHERE id=5)');

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. code-block:: sql

	DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom);
	SELECT  pgr_createTopology('edge_table',0.001,rows_where:='the_geom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');



Usage when the edge table's columns DO NOT MATCH the default values:
-------------------------------------------------------------------------------
 
For the following table

.. code-block:: sql

	DROP TABLE IF EXISTS mytable;
	CREATE TABLE mytable AS (SELECT id AS gid, the_geom AS mygeom,source AS src ,target AS tgt FROM edge_table) ;

.. rubric:: Using positional notation: 

The arguments need to be given in the order described in the parameters:

.. code-block:: sql

	SELECT  pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt');

.. warning::  | An error would occur when the arguments are not given in the appropiriate order: In this example, the column ``gid`` of the table ``mytable`` is passed to the function AS the geometry column, and the geometry column ``mygeom`` is passed to the function AS the id column.
 | ``SELECT  pgr_createTopology('mytable',0.001,'gid','mygeom','src','tgt');``
 | ERROR: Can not determine the srid of the geometry "gid" in table public.mytable


.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,the_geom:='mygeom',id:='gid',source:='src',target:='tgt');

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom');

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.


.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt',rows_where:='gid < 10');

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,1) FROM mytable WHERE gid=5)');

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. code-block:: sql

	DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ; 
	SELECT  pgr_createTopology('mytable',0.001,'mygeom','gid','src','tgt',
                           rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');

.. code-block:: sql

	 SELECT  pgr_createTopology('mytable',0.001,source:='src',id:='gid',target:='tgt',the_geom:='mygeom',
                           rows_where:='mygeom && (SELECT st_buffer(other_geom,1) FROM otherTable WHERE gid=100)');



Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT pgr_createTopology('edge_table', 0.001,rows_where:='id<10');
	NOTICE:  PROCESSING:
	NOTICE:  pgr_createTopology('edge_table',0.0001,'the_geom','id','source','target','id<10')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Creating Topology, Please wait...
	NOTICE:  -------------> TOPOLOGY CREATED FOR  9 edges
	NOTICE:  Rows with NULL geometry or NULL id: 0
	NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
	
	 pgr_createtopology 
	--------------------
	 OK
	(1 row)

	SELECT pgr_createTopology('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_createTopology('edge_table',0.0001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Creating Topology, Please wait...
	NOTICE:  -------------> TOPOLOGY CREATED FOR  18 edges
	NOTICE:  Rows with NULL geometry or NULL id: 0
	NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr

	 pgr_createtopology 
	--------------------
	 OK
	(1 row)

The example uses the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`topology` for an overview of a topology for routing algorithms.
* :ref:`pgr_create_vert_table` to reconstruct the vertices table based on the source and target information.
* :ref:`pgr_analyze_graph` to analyze the edges and vertices of the edge table.

