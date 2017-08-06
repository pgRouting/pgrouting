..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraCostMatrix:

pgr_dijkstraCostMatrix - proposed
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_dijkstraCostMatrix`` - Calculates the a cost matrix using pgr_dijktras.


.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability: 2.3.0

Synopsis
-------------------------------------------------------------------------------

Using Dijkstra algorithm, calculate and return a cost matrix.

Signature Summary
-----------------

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vids)
    pgr_dijkstraCostMatrix(edges_sql, start_vids, directed)
    RETURNS SET OF (start_vid, end_vid, agg_cost)



Signatures
-------------------------------------------------------------------------------

.. index::
    single: dijkstraCostMatrix(Minimal Use) - Proposed

Minimal Signature
...............................................................................

The minimal signature:
    - Is for a **directed** graph.

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Cost matrix for vertices 1, 2, 3, and 4.


.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q1
   :end-before: -- dijkstra q2


.. index::
    single: dijkstraCostMatrix(Complete Signature) - Proposed

Complete Signature
...............................................................................

.. code-block:: none

    pgr_dijkstraCostMatrix(edges_sql, start_vids, directed:=true)
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: Cost matrix for an undirected graph for vertices 1, 2, 3, and 4.

This example returns a symmetric cost matrix.

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q2
   :end-before: -- dijkstra q3


Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end



Description of the parameters of the signatures
...............................................................................

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of the vertices.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
================ ====================== =================================================


.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end


Examples
-------------------------------------------------------------------------------

:Example: Use with tsp

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- dijkstra q3
   :end-before: -- dijkstra q4


See Also
-------------------------------------------------------------------------------

* :ref:`dijkstra`
* :ref:`costMatrix`
* :ref:`tsp`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

