..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_createVerticesTable
===============================================================================

``pgr_createVerticesTable`` — Reconstructs the vertices table based on the source and target information.

.. rubric:: Availability

* Renamed in version 2.0.0

Description
-------------------------------------------------------------------------------

The function returns:

- ``OK`` after the vertices table has been reconstructed.
- ``FAIL`` when the vertices table was not reconstructed due to an error.

Signatures
-------------------------------------------------------------------------------

.. code-block:: sql

	pgr_createVerticesTable(edge_table, the_geom, source, target, rows_where)
    RETURNS VARCHAR

Parameters
-------------------------------------------------------------------------------

The reconstruction of the vertices table  function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``.
:rows_where: ``text``   Condition to SELECT a subset or rows.  Default value is ``true`` to indicate all rows.

.. warning::

    The ``edge_table`` will be affected

    - An index will be created, if it doesn't exists, to speed up the process to the following columns:

      * ``the_geom``
      * ``source``
      * ``target``

The function returns:

- ``OK`` after the vertices table has been reconstructed.

  * Creates a vertices table: <edge_table>_vertices_pgr.
  * Fills ``id`` and ``the_geom`` columns of the vertices table based on the source and target columns of the edge table.

- ``FAIL`` when the vertices table was not reconstructed due to an error.

  * A required column of the Network table is not found or is not of the appropriate type.
  * The condition is not well formed.
  * The names of source, target are the same.
  * The SRID of the geometry could not be determined.

.. rubric:: The Vertices Table

The vertices table is a requierment of the :doc:`pgr_analyzeGraph <pgr_analyzeGraph>` and the :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>` functions.

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :doc:`pgr_analyzeGraph <pgr_analyzeGraph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :doc:`pgr_analyzeGraph <pgr_analyzeGraph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing. See :doc:`pgr_analyzeOneWay <pgr_analyzeOneWay>`.
:the_geom: ``geometry`` Point geometry of the vertex.

Usage when the edge table's columns MATCH the default values:
...............................................................................

.. rubric:: The simplest way to use pgr_createVerticesTable is:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q1
   :end-before: --q1.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table');


.. rubric:: When the arguments are given in the order described in the parameters:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q2
   :end-before: --q2.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table','the_geom','source','target');

We get the same result as the simplest way to use the function.

.. warning::  An error would occur when the arguments are not given in the appropriate order: In this example, the column source column ``source`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``the_geom`` is passed to the function as the source column.

   .. literalinclude:: doc-pgr_createVerticesTable.queries
      :start-after: --q3
      :end-before: --q3.1

   .. code-block:: sql

       SELECT  pgr_createVerticesTable('edge_table','source','the_geom','target');
       NOTICE:  pgr_createVerticesTable('edge_table','source','the_geom','target','true')
       NOTICE:  Performing checks, please wait .....
       NOTICE:  ----> PGR ERROR in pgr_createVerticesTable: Wrong type of Column source: the_geom
       HINT:    ----> Expected type of the_geom is integer,smallint or bigint but USER-DEFINED was found
       NOTICE:  Unexpected error raise_exception
       pgr_createverticestable
       -------------------------
         FAIL
      (1 row)

.. rubric:: When using the named notation

The order of the parameters do not matter:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q3.23
   :end-before: --q3.24

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table',the_geom:='the_geom',source:='source',target:='target');

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q4
   :end-before: --q4.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table',source:='source',target:='target',the_geom:='the_geom');

Parameters defined with a default value can be omitted, as long as the value matches the default:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q5
   :end-before: --q5.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table',source:='source');

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q6
   :end-before: --q6.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table',rows_where:='id < 10');

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q7
   :end-before: --q7.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('edge_table',rows_where:='the_geom && (select st_buffer(the_geom,0.5) FROM edge_table WHERE id=5)');

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q8
   :end-before: --q8.1

.. code-block:: sql

	DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
	SELECT  pgr_createVerticesTable('edge_table',rows_where:='the_geom && (select st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');

Usage when the edge table's columns DO NOT MATCH the default values:
...............................................................................

For the following table

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q9
   :end-before: --q9.1

.. code-block:: sql

	DROP TABLE IF EXISTS mytable;
	CREATE TABLE mytable AS (SELECT id AS gid, the_geom AS mygeom,source AS src ,target AS tgt FROM edge_table) ;

.. rubric:: Using positional notation:

The arguments need to be given in the order described in the parameters:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q10
   :end-before: --q10.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt');

.. warning::  | An error would occur when the arguments are not given in the appropriate order: In this example, the column ``src`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``mygeom`` is passed to the function as the source column.

    .. literalinclude:: doc-pgr_createVerticesTable.queries
       :start-after: --q11
       :end-before: --q11.1

    .. code-block:: sql

        SELECT  pgr_createVerticesTable('mytable','src','mygeom','tgt');
        NOTICE:  PROCESSING:
        NOTICE:  pgr_createVerticesTable('mytable','src','mygeom','tgt','true')
        NOTICE:  Performing checks, please wait .....
        NOTICE:  ----> PGR ERROR in pgr_createVerticesTable: Table mytable not found
        HINT:    ----> Check your table name
        NOTICE:  Unexpected error raise_exception
        pgr_createverticestable
        -------------------------
          FAIL
        (1 row)

.. rubric:: When using the named notation

The order of the parameters do not matter:

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q12
   :end-before: --q12.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable',the_geom:='mygeom',source:='src',target:='tgt');

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q13
   :end-before: --q13.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom');

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the gid.

.. literalinclude:: doc-pgr_createVerticesTable.queries
    :start-after: --q14
    :end-before: --q14.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',rows_where:='gid < 10');

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q15
   :end-before: --q15.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',rows_where:='gid < 10');

Selecting the rows where the geometry is near the geometry of row with ``gid`` =5.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q16
   :end-before: --q16.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='the_geom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE gid=5)');

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q17
   :end-before: --q17.1

.. code-block:: sql

	 SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='mygeom && (SELECT st_buffer(mygeom,0.5) FROM mytable WHERE id=5)');

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q18
   :end-before: --q18.1

.. code-block:: sql

	DROP TABLE IF EXISTS otherTable;
	CREATE TABLE otherTable AS  (SELECT 100 AS gid, st_point(2.5,2.5) AS other_geom) ;
	SELECT  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q19
   :end-before: --q19.1

.. code-block:: sql

	SELECT  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='the_geom && (SELECT st_buffer(othergeom,0.5) FROM otherTable WHERE gid=100)');

Additional Examples
-------------------------------------------------------------------------------

.. literalinclude:: doc-pgr_createVerticesTable.queries
   :start-after: --q20
   :end-before: --q20.1

.. code-block:: sql

	SELECT pgr_createVerticesTable('edge_table');
	NOTICE:  PROCESSING:
    NOTICE:  pgr_createVerticesTable('edge_table','the_geom','source','target','true')
    NOTICE:  Performing checks, pelase wait .....
    NOTICE:  Populating public.edge_table_vertices_pgr, please wait...
    NOTICE:    ----->   VERTICES TABLE CREATED WITH  17 VERTICES
    NOTICE:                                         FOR   18  EDGES
    NOTICE:    Edges with NULL geometry,source or target: 0
    NOTICE:                              Edges processed: 18
    NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
    NOTICE:  ----------------------------------------------

	 pgr_createVerticesTable
	--------------------
	 OK
	(1 row)

The example uses the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`  for an overview of a topology for routing algorithms.
* :doc:`pgr_createTopology` <pgr_create_topology>` to create a topology based on the geometry.
* :doc:`pgr_analyzeGraph` to analyze the edges and vertices of the edge table.
* :doc:`pgr_analyzeOneWay` to analyze directionality of the edges.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
