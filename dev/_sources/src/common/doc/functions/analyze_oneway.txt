.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_analyze_oneway:

pgr_analyzeOneway - Analyze One Way Streets
===============================================================================

.. index:: 
	single: pgr_analyzeOneway(text,text,text[],text[],text[],text[],boolean)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_analyzeOneway`` — Analyzes oneway Sstreets and identifies flipped segments.


Synopsis
-------------------------------------------------------------------------------

This function analyzes oneway streets in a graph and identifies any flipped segments. 

.. code-block:: sql

	text pgr_analyzeOneway(geom_table text, 1way_cname text, s_in_rules text[], s_out_rules text[], 
                               t_in_rules text[], t_out_rules text[], 2way_if_null boolean);


Description
-------------------------------------------------------------------------------

:geom_table: ``text`` network table name (may contain the schema name as well)
:1way_cname: ``text`` oneway column name name of the network table
:s_in_rules: ``text[]`` source node **in** rules
:s_out_rules: ``text[]`` source node **out** rules
:t_in_rules: ``text[]`` target node **in** rules
:t_out_rules: ``text[]`` target node **out** rules
:2way_if_null: ``boolean`` flag to treat oneway NULL values as bi-directional

This query will add two columns to the vertices_tmp table ``ein int`` and ``eout int`` and populate it with the appropriate counts.

The rules are defined as an array of text strings that if match the ``1way_cname`` value would be counted as ``true`` for the source or target **in** or **out** condition.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT pgr_analyzeOneway('edge_table', 'dir',
        ARRAY['', 'B', 'TF'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'FT'],
        ARRAY['', 'B', 'TF'],
        true);

    NOTICE:  Adding "ein" and "eout" columns to vertices_tmp
    NOTICE:  Zeroing columns "ein" and "eout" on "vertices_tmp".
    NOTICE:  Analyzing graph for one way street errors.
    NOTICE:  Analysis 25% complete ...
    NOTICE:  Analysis 50% complete ...
    NOTICE:  Analysis 75% complete ...
    NOTICE:  Analysis 100% complete ...
    NOTICE:  Found 0 potential problems at 'SELECT * FROM vertices_tmp WHERE ein=0 or eout=0'

     pgr_analyzeoneway 
    -------------------
     OK
    (1 row)

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_analyze_graph`
* :ref:`analytics`
