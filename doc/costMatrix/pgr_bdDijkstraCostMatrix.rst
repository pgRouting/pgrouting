..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bdDijkstraCostMatrix
===============================================================================

``pgr_bdDijkstraCostMatrix`` - Calculates the a cost matrix using :doc:`pgr_bdDijkstra`.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability:

* Official on v3.0.0
* Proposed on v2.5.0

**Supported versions:**
current(`3.0 <https://docs.pgrouting.org/dev/en/pgr_bdDijkstraCostMatrix.html>`__)
`2.6 <https://docs.pgrouting.org/2.6/en/pgr_bdDijkstraCostMatrix.html>`__
`2.5 <https://docs.pgrouting.org/2.5/en/pgr_bdDijkstraCostMatrix.html>`__

Description
-------------------------------------------------------------------------------

Using Dijkstra algorithm, calculate and return a cost matrix.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_bdDijkstraCostMatrix(edges_sql, start_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. index::
    single: bdDijkstraCostMatrix(Minimal Use)

.. rubric:: Using default

.. code-block:: none

    pgr_bdDijkstraCostMatrix(edges_sql, start_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for vertices :math:`\{1, 2, 3, 4\}` on a **directed** graph

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdDijkstra q1
   :end-before: -- bdDijkstra q2

Complete Signature
...............................................................................

.. code-block:: none

    pgr_bdDijkstraCostMatrix(edges_sql, start_vids [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Symmetric cost matrix for vertices :math:`\{1, 2, 3, 4\}` on an **undirected** graph

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdDijkstra q2
   :end-before: -- bdDijkstra q3

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

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with tsp

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdDijkstra q3
   :end-before: -- bdDijkstra q4

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_bdDijkstra`
* :doc:`costMatrix-category`
* :doc:`pgr_TSP`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

