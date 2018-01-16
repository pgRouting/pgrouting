..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_linegraphfull:

pgr_lineGraphFull - Experimental
===============================================================================

``pgr_lineGraphFull`` — Transforms a given graph into a new graph where all of the vertices from the original graph are converted to line graphs.

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Synopsis
-------------------------------------------------------------------------------

This graph transformation algorithm generates a new graph that contains all of the same edges of the original graph except every vertex has been converted to a line graph. 

One application for this type of graph transformation is to allow for the addition of a cost to turning in shortest path calcuations for routing traffic. This can be done because each of the intersections (vertices) in the original graph are now line graphs that have a new edge for each possible turn across that intersection.

Characteristics
-------------------------------------------------------------------------------

The main characteristics are:
  - This function is intended for directed graphs.
  - This function will currently give incorrect results if negative vertex ids are used in the input.
  - This function will currently give incorrect results if duplicated edge ids are used in the input.

Signature Summary
-----------------

.. code-block:: none

    pgr_lineGraphFull(edges_sql)
    RETURNS SET OF (seq, source, target, cost, edge) 
        OR EMPTY SET

Signatures
------------------------------------------------------------------------------

.. index::
    single: lineGraphFull(Only signature)

Minimal signature
-----------------------------------------------

.. code-block:: none

    pgr_lineGraphFull(TEXT edges_sql)
    RETURNS SET OF (seq, source, target, cost, edge) OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q1
   :end-before: -- q2

Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. pgr_lineGraphFull_parameters_start

Description of the parameters of the signatures
...............................................................................

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**sql**        ``TEXT``                    SQL query as described above.
============== ================== ======== =================================================

.. pgr_lineGraphFull_parameters_end


Additional Examples
-------------------------------------------------------------------------------

The examples of this section are based on the :doc:`sampledata` network.

The examples include the subgraph including edges 5, 8, 9 and 11 with reverse_cost.

Example for generating the LineGraphFull
-------------------------------------------------------------------------------

This example displays how this graph transformation works to create additional edges for each possible turn in a graph.

.. code-block:: none

    SELECT id, source, target, cost, reverse_cost FROM edge_table WHERE id IN (5, 8, 9, 11);

| |first|

.. |first| image:: images/original.png
   :align: middle

.. code-block:: none

    SELECT * FROM pgr_lineGraphFull('SELECT id, 
                                            source, 
                                            target, 
                                            cost, 
                                            reverse_cost 
                                       FROM edge_table 
                                         WHERE id IN (5, 8, 9, 11)');

| |second|

.. |second| image:: images/transformation.png
   :align: middle

In the transformed graph, all of the edges from the original graph are still present (yellow), but we now have additional edges for every turn that could be made across vertex 6 (orange).

Example for creating table that identifies transformed vertices
-----------------------------------------------------------------------------
 
The vertices in the transformed graph are each created by splitting up the vertices in the original graph. Unless a vertex in the original graph is a leaf vertex, it will generate more than one vertex in the transformed graph. One of the newly created vertices in the transformed graph will have the same vertex-id as the vertex that it was created from in the original graph, but the rest of the newly created vertices will have negative vertex ids. Following is an example of how to generate a table that maps the ids of the newly created vertices with the original vertex that they were created from.

.. literalinclude:: doc-pgr_lineGraphFull.queries
   :start-after: -- q2
   :end-before: -- q3

Example for running a dijkstra's shortest path with turn penalties
-----------------------------------------------------------------------------


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

