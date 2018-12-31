..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_dijkstraCostMatrix
===============================================================================

``pgr_dijkstraCostMatrix`` - Calculates the a cost matrix using pgr_dijktras.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability

* Official on v3.0.0
* Proposed on v2.3.0

Description
-------------------------------------------------------------------------------

Using Dijkstra algorithm, calculate and return a cost matrix.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. rubric:: Using defaults

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for vertices :math:`\{1, 2, 3, 4\}` on a **directed** graph

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q1
   :end-before: -- dijkstra q2


.. index::
    single: dijkstraCostMatrix

Complete Signature
...............................................................................

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Symmetric cost matrix for vertices :math:`\{1, 2, 3, 4\}` on an **undirected** graph

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q2
   :end-before: -- dijkstra q3

Parameters
-------------------------------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of the vertices.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
================ ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with tsp

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q3
   :end-before: -- dijkstra q4

See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

