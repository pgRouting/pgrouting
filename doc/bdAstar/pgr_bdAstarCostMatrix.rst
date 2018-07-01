..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_bdAstarCostMatrix:

pgr_bdAstarCostMatrix - proposed
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_bdAstarCostMatrix`` - Calculates the a cost matrix using :ref:`pgr_bdAstar`.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability: 2.5.0

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

Synopsis
-------------------------------------------------------------------------------

Using Dijkstra algorithm, calculate and return a cost matrix.

Signature Summary
-----------------

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, start_vids)
    pgr_bdAstarCostMatrix(edges_sql, start_vids, [, directed , heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET


Signatures
-------------------------------------------------------------------------------

.. index::
    single: bdAstarCostMatrix(Minimal Use) - Proposed

Minimal Signature
...............................................................................

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, start_vids)
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

This usage calculates the cost from the each ``start_vid`` in ``start_vids`` to  each ``start_vid`` in ``start_vids``
  -  on a **directed** graph
  -  with **heuristic**'s value 5
  -  with **factor**'s value 1
  -  with **epsilon**'s value 1


:Example: Cost matrix for vertices 1, 2, 3, and 4.


.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdAstar q1
   :end-before: -- bdAstar q2


.. index::
    single: bdAstarCostMatrix(Complete Signature) - Proposed

Complete Signature
...............................................................................

.. code-block:: none

    pgr_bdAstarCostMatrix(edges_sql, start_vids, [, directed , heuristic, factor, epsilon])
    RETURNS SET OF (start_vid, end_vid, agg_cost)
    OR EMPTY SET

This usage calculates the cost from the each ``start_vid`` in ``start_vids`` to  each ``start_vid`` in ``start_vids`` allowing the user to choose
    * if the graph is **directed** or **undirected**
    * **heuristic**,
    * and/or **factor**
    * and/or **epsilon**.


:Example: Cost matrix for an undirected graph for vertices 1, 2, 3, and 4.

This example returns a symmetric cost matrix.

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdAstar q2
   :end-before: -- bdAstar q3


Description of the Signatures
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: xy_edges_sql_start
    :end-before: xy_edges_sql_end

.. include:: bdAstar-family.rst
    :start-after: parameters_begin
    :end-before: parameters_end

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end


Examples
-------------------------------------------------------------------------------

:Example: Use with tsp

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- bdAstar q3
   :end-before: -- bdAstar q4


See Also
-------------------------------------------------------------------------------

* :ref:`bdAstar`
* :ref:`costMatrix`
* :ref:`tsp`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

