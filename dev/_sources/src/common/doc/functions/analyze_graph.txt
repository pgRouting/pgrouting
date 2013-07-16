.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_analyze_graph:

pgr_analyzeGraph - Analyze a Graph
===============================================================================

.. index:: 
	single: pgr_analyzeGraph(text,text,double precision)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_analyzeGraph`` — Analyzes the "network" and "vertices_tmp" tables.


Synopsis
-------------------------------------------------------------------------------

Analyzes the edge table and "vertices_tmp" tables and flags if nodes are deadends, ie. ``vertices_tmp.cnt=1`` and identifies nodes that might be disconnected because of gaps < ``tolerance`` or because of zlevel errors in the data. 

.. code-block:: sql

  character varying pgr_analyzeGraph(geom_table text, geo_cname text,
                                     tolerance double precision);


Description
-------------------------------------------------------------------------------

:geom_table: ``text`` network table name (may contain the schema name as well)
:geo_cname: ``text`` geometry column name of the network table
:tolerance: ``float8`` snapping tolerance of disconnected edges (in projection unit)


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_analyzeGraph('edge_table','the_geom',0.0001);

    NOTICE:  Adding "cnt" and "chk" columns to vertices_tmp
    NOTICE:  Adding unique index "vertices_tmp_id_idx".
    NOTICE:  Adding index on "source" for "edge_table".
    NOTICE:  Adding index on "target" for "edge_table".
    NOTICE:  Adding index on "edge_table" for "the_geom".
    NOTICE:  Populating vertices_tmp.cnt
    NOTICE:  Analyzing graph for gaps and zlev errors.
    NOTICE:  Found 1 potential problems at 'SELECT * FROM vertices_tmp WHERE chk=1'
    
     pgr_analyzegraph 
    ------------------
     OK
    (1 row)

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_analyze_oneway`
* :ref:`analytics`
