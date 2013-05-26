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

Analyzes the "edge_tab" and "vertices_tmp" tables and flags if nodes are deadends, ie. ``vertices_tmp.cnt=1`` and identifies nodes that might be disconnected because of gaps < ``tolerance`` or because of zlevel errors in the data. 

.. code-block:: sql

  character varying pgr_analyzeGraph(tab text, geom text, tol double precision);


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

	[TBD]


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_analyze_oneway`
* :ref:`analytics`
