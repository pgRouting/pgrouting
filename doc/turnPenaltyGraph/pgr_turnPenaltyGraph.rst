..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_turnPenaltyGraph:

pgr_turnPenaltyGraph - 
===============================================================================

``pgr_turnPenaltyGraph`` — Returns a transformed version of a given graph where each vertex is split up into multiple vertices in order to add a new edge to the graph for every possible turn that can be made. This makes a shortest path search with turn penalties and restrictions possible.


Synopsis
-------------------------------------------------------------------------------

This graph transformation algorithm generates a new graph that has an edge for every edge in the original graph, and an additional edge for each possible turn that can be made by a path through the graph. Because of this, these new edges can then be removed from the graph in order to apply turn restrictions, or the cost of these edges can be modified to apply turn penalties.

Example
-------------------------------------------------------------------------------

| |first|

.. |first| image:: images/original.png
   :align: middle

.. code-block:: none 

    INSER INTO turn_table SELECT * FROM pgr_turnPenaltyGraph('SELECT id, source, target, cost FROM original_graph);

| |second|

.. |second| image:: images/transformation.png
   :align: middle

.. code-block:: none 

   DELETE FROM turn_table WHERE original_source_edge = e6 AND original_target_edge = e7;
   DELETE FROM turn_table WHERE original_source_edge = e1 AND original_target_edge = e2;

| |third|

.. |third| image:: images/restrictions.png
   :align: middle

.. code-block:: none 

   UPDATE turn_table SET cost = 5 WHERE original_source_edge = e3 AND original_target_edge = e4;
   UPDATE turn_table SET cost = 1 WHERE original_source_edge = e6 AND original_target_edge = e5; 

| |fourth|

.. |fourth| image:: images/penalties.png
   :align: middle

.. code-block:: none 

   source_arr := array_to_string(array(select source from turn_penalty_table where original_source_vertex = 2), ',');
   target_arr := array_to_string(array(select target from turn_penalty_table where original_target_vertex = 13),',');

   SELECT * FROM pgr_dijkstraCost('SELECT seq as id, source, target, cost from turn_table', 
                                  ARRAY[' source_arr '], 
                                  ARRAY[' target_arr ']);

| |fifth|

.. |fifth| image:: images/dijkstra.png
   :align: middle

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

  - This only works on directed graphs and does not use reverse edge costs.

Signature Summary
-----------------

.. code-block:: none 

    pgr_turnPenaltyGraph(edges_sql)

    RETURNS SET OF (seq, source, vertex, cost, original_source_vertex, original_source_edge, original_target_vertex, original_target_edge) OR EMPTY SET

Signatures
-------------------------------------------------------------------------------

.. index::
    single: turnPenaltyGraph(Minimal Use)

Complete Signature
.......................................

.. code-block:: none

    pgr_turnPenaltyGraph(edges_sql)
    RETURNS SET OF (seq, source, vertex, cost, original_source_vertex, original_source_edge, original_target_vertex, original_target_edge) OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_turnPenaltyGraph.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: turnPenaltyGraph(Complete signature)

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

