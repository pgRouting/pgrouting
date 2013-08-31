.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_analyze_oneway:

pgr_analyzeOneway
===============================================================================

.. index:: 
	single: pgr_analyzeOneway(text,text[],text[],text[],text[],text,text,text,boolean)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_analyzeOneway`` — Analyzes oneway Sstreets and identifies flipped segments.


Synopsis
-------------------------------------------------------------------------------

This function analyzes oneway streets in a graph and identifies any flipped segments. 

.. code-block:: sql

	text pgr_analyzeOneway(geom_table text, 
			       text[] s_in_rules, text[] s_out_rules, 
                               text[] t_in_rules, text[] t_out_rules, 
			       text oneway='oneway', text source='source', text target='target',
			       boolean two_way_if_null=true);


Description
-------------------------------------------------------------------------------


.. rubric:: Prerequisites

The  edge table to be analyzed must contain a source column and a target column filled with id's of the vertices of the segments and the corre
sponding vertices table <edge_table>_vertices_pgr that stores the vertices information.

  - Use :ref:`pgr_createVerticesTable <pgr_create_vert_table>` to create the vertices table.
  - Use :ref:`pgr_createTopology <pgr_create_topology>` to create the topology and the vertices table. 

.. rubric:: Parameters

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:s_in_rules: ``text[]`` source node **in** rules
:s_out_rules: ``text[]`` source node **out** rules
:t_in_rules: ``text[]`` target node **in** rules
:t_out_rules: ``text[]`` target node **out** rules
:oneway: ``text`` oneway column name name of the network table. Default value is ``oneway``.
:source: ``text`` Source column name of the network table. Default value is ``source``.
:target: ``text``  Target column name of the network table.  Default value is ``target``. 
:two_way_if_null: ``boolean`` flag to treat oneway NULL values as bi-directional.  Default value is ``true``.

.. note:: It is strongly recomended to use the named notation. See :ref:`pgr_createVerticesTable <pgr_create_vert_table>` or :ref:`pgr_createTopology <pgr_create_topology>` for examples.


The function returns:

  - ``OK`` after the analysis has finished.

    * Uses the vertices table: <edge_table>_vertices_pgr.
    * Fills completly the ``ein`` and ``eout`` columns of the vertices table.

  - ``FAIL`` when the analysis was not completed due to an error. 

    * The vertices table is not found.
    * A requiered column of the Network table is not found or is not of the appropiate type.
    * The names of source , target or oneway are the same.

The rules are defined as an array of text strings that if match the ``oneway`` value would be counted as ``true`` for the source or target **in** or **out** condition.

.. rubric:: The Vertices Table

The vertices table can be created with :ref:`pgr_createVerticesTable <pgr_create_vert_table>` or :ref:`pgr_createTopology <pgr_create_topology>`

The structure of the vertices table is:

:id: ``bigint`` Identifier of the vertex.
:cnt: ``integer`` Number of vertices in the edge_table that reference this vertex. See :ref:`pgr_analyzeGgraph <pgr_analyze_graph>`.
:chk: ``integer``  Indicator that the vertex might have a problem. See :ref:`pgr_analyzeGraph <pgr_analyze_graph>`.
:ein: ``integer`` Number of vertices in the edge_table that reference this vertex as incoming. 
:eout: ``integer`` Number of vertices in the edge_table that reference this vertex as outgoing.
:the_geom: ``geometry`` Point geometry of the vertex.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT pgr_analyzeOneway('edge_table', 
        ARRAY['', 'B', 'TF'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'TF'],
        oneway:='dir');
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analizeGraph('edge_table','{"",B,TF}','{"",B,FT}','{"",B,FT}','{"",B,TF}','dir','source','target',t)
	NOTICE:  Analyzing graph for one way street errors.
	NOTICE:  Analysis 25% complete ...
	NOTICE:  Analysis 50% complete ...
	NOTICE:  Analysis 75% complete ...
	NOTICE:  Analysis 100% complete ...
	NOTICE:  Found 0 potential problems in directionality

	pgr_analyzeoneway 
	-------------------
	OK
	(1 row)

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`topology`  for an overview of a topology for routing algorithms.
* :ref:`analytics` for an overview of the analysis of a graph.
* :ref:`pgr_analyze_graph` to analyze the edges and vertices of the edge table.
* :ref:`pgr_create_vert_table` to reconstruct the vertices table based on the source and target information.

