.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_create_vert_table:

pgr_createVerticesTable 
===============================================================================

.. index:: 
	single: pgr_createVerticesTable(text,text,text,text,text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_createVerticesTable`` — Reconstructs the vertices table based on the source and target information.


Synopsis
-------------------------------------------------------------------------------

The function returns:

  - ``OK`` after the vertices table has been reconstructed.
  - ``FAIL`` when the vertices table was not recosntructed due to an error. 

.. code-block:: sql

	varchar pgr_createVerticesTable(text edge_table,  text the_geom:='the_geom'
                           text source:='source',text target:='target',text rows_where:='true')



Description
-------------------------------------------------------------------------------

.. Rubric:: Parameters

The reconstruction of the vertices table  function accepts the following parameters:

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:the_geom: ``text``  Geometry column name of the network table. Default value is ``the_geom``.  
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``. 
:rows_where: ``text``   Condition to select a subset or rows.  Default value is ``true`` to indicate all rows.

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

  - ``FAIL`` when the vertices table was not recosntructed due to an error. 

    * A requiered column of the Network table is not found or is not of the appropiate type.
    * The condition is not well formed.
    * The names of source, target are the same.
    * The SRID of the geometry could not be determined.

.. rubric:: The Vertices Table

The vertices table is a requierment of the :ref:`pgr_analyzeGraph <pgr_analyze_graph>` and the :ref:`pgr_analyzeOneway <pgr_analyze_oneway>` functions.

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`.
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing. See :ref:`pgr_analyzeOneway <pgr_analyze_oneway>`. 
:the_geom: ``geometry`` Point geometry of the vertex.

.. rubric:: History

* Renamed in version 2.0.0

Usage when the edge table's columns MATCH the default values:
-------------------------------------------------------------------------------
 
.. rubric:: The simplest way to use pgr_createVerticesTable is: 

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable')


.. rubric:: When the arguments are given in the order described in the parameters:

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable','the_geom','source','target')

We get the same result as the simplest way to use the function.

.. warning::  | An error would occur when the arguments are not given in the appropiate order: In this example, the column source column ``source`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``the_geom`` is passed to the function as the source column. 
 | ``select  pgr_createVerticesTable('mytable','source','the_geom','target')``


.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',the_geom:='the_geom',source:='source',target:='target')

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='source',target:='target',the_geom:='the_geom')

Parameters defined with a default value can be ommited, as long as the value matches the default:

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='source')

.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',rows_where:='id < 10')

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',rows_where:='the_geom && (select st_buffer(the_geom,0.05) from mytable where id=5)')

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',rows_where:='the_geom && (select st_buffer(othergeom,0.05) from otherTable where gid=100)')



Usage when the edge table's columns DO NOT MATCH the default values:
-------------------------------------------------------------------------------
 
For the following table

.. code-block:: sql

	  mytable (gid  bigint, mygeom geometry, src integer, tgt integer) 

.. rubric:: Using positional notation: 

The arguments need to be given in the order described in the parameters:

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable','mygeom','src',tgt')

.. warning::  | An error would occur when the arguments are not given in the appropiate order: In this example, the column ``src`` of the table ``mytable`` is passed to the function as the geometry column, and the geometry column ``mygeom`` is passed to the function as the source column.
 | ``select  pgr_createVerticesTable('mytable','src','mygeom','tgt')``


.. rubric:: When using the named notation

The order of the parameters do not matter:

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',the_geom:='mygeom',source:='src',target:='tgt')

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom')

In this scenario omitting a parameter would create an error because the default values for the column names do not match the column names of the table.


.. rubric:: Selecting rows using rows_where parameter

Selecting rows based on the id.

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable','mygeom','src','tgt',rows_where:='id < 10')

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',rows_where:='id < 10')

Selecting the rows where the geometry is near the geometry of row with ``id`` =5 .

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='the_geom && (select st_buffer(the_geom,0.05) from mytable where id=5)')

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='the_geom && (select st_buffer(the_geom,0.05) from mytable where id=5)')

Selecting the rows where the geometry is near the geometry of the row with ``gid`` =100 of the table ``othertable``.

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable','mygeom','src','tgt',
	                            rows_where:='the_geom && (select st_buffer(othergeom,0.05) from otherTable where gid=100)')

.. code-block:: sql

	 select  pgr_createVerticesTable('mytable',source:='src',target:='tgt',the_geom:='mygeom',
	                            rows_where:='the_geom && (select st_buffer(othergeom,0.05) from otherTable where gid=100)')



Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT pgr_createVerticesTable('edge_table');
	NOTICE:  PROCESSING:
	NOTICE:  pgr_createVerticesTable('edge_table','the_geom','source','target','true')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Populating public.edge_table_vertices_pgr, please wait...
	NOTICE:    -----> VERTICES TABLE CREATED WITH  17 vertices
	NOTICE:  Edges with NULL geometry,source or target: 0
	NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
	
	 pgr_createVerticesTable 
	--------------------
	 OK
	(1 row)


The example uses the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`topology`  for an overview of a topology for routing algorithms.
* :ref:`pgr_createTopology <pgr_create_topology>` to create a topology based on the geometry.
* :ref:`pgr_analyze_graph` to analyze the edges and vertices of the edge table.
* :ref:`pgr_analyze_oneway` to analyze directionality of the edges.
