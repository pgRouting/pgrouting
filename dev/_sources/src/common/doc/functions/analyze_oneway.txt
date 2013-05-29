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

	text pgr_analyzeOneway(tab text, col text, s_in_rules text[], s_out_rules text[], 
                               t_in_rules text[], t_out_rules text[], 2way_if_null boolean);


Description
-------------------------------------------------------------------------------

:tab: ``text`` edge table name
:col: ``text`` oneway column name
:s_in_rules: ``text[]`` source node in rules
:s_out_rules: ``text[]`` source node out rules
:t_in_tules: ``text[]`` target node in rules
:t_out_rules: ``text[]`` target node out rules
:2way_if_null: ``boolean`` flag to treat oneway nNULL values as by directional

This query will add two columns to the vertices_tmp table ``ein int`` and ``eout int`` and populate it with the appropriate counts.

The rules are defined as an array of text strings that if match the ``col`` value would be counted as ``true`` for the source or target in or out condition.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	[TBD]


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_analyze_graph`
* :ref:`analytics`
