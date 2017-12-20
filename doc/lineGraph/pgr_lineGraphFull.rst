..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_linegraphfull:

pgr_lineGraphFull
===============================================================================

.. index::
        single: pgr_lineGraphFull(text)

Name
------------------------------------------------------------------------------

''pgr_lineGraphFull'' - Utility function for transforming a given graph into a form that supports shortest path searches with turn penalties and restrictions.


Synopsis
-------------------------------------------------------------------------------

This graph transformation algorithm generates a new graph that has an edge for every edge in the original graph, and an additional edge for each possible turn that can be made by a path through the graph. Because of this, these new edges can then be removed from the graph in order to apply turn restrictions, or the cost of these edges can be modified to apply turn penalties.

Characteristics
-------------------------------------------------------------------------------

  - This function only works on directed graphs.

Signature Summary
-----------------

.. code-block:: none 

    pgr_lineGraphFull(edges_sql)

    RETURNS SET OF (seq, 
                    source, 
                    target, 
                    cost, 
                    original_source_vertex, 
                    original_source_edge, 
                    original_target_vertex,  
                    original_target_edge) OR EMPTY SET

Example Usage
-------------------------------------------------------------------------------

This example displays how this graph transformation works to create additional edges for each possible turn in a graph.

.. code-block:: none 

    SELECT id, source, target, cost FROM edge_table WHERE id IN (5, 8, 9, 11);

| |first|

.. |first| image:: images/original.png
   :align: middle

.. code-block:: none 

    SELECT * FROM pgr_lineGraphFull('SELECT id, source, target, cost FROM edge_table WHERE id IN (5, 8, 9, 11)');

| |second|

.. |second| image:: images/transformation.png
   :align: middle

In the transformed graph, all of the edges from the original graph are still present, but we now have additional edges for every turn that could be made across vertex 6. This graph can now be used to apply turn penalties and restrictions for routing problems by removing or adding costs to these new edges.

Sample Data Results
-------------------------------------------------------------------------------

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q1
   :end-before: -- q2


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

