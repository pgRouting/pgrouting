..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_minCostMaxFlow - Experimental
============================================

``pgr_minCostMaxFlow`` — Calculates the flow on the graph edges that maximizes the flow and minimizes the cost from the sources to the targets.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

* New **experimental** function

.. rubric:: Support

**Supported versions:**
current(`3.0 <https://docs.pgrouting.org/dev/en/pgr_minCostMaxFlow.html>`

Description
-------------------------------------------------------------------------------

.. include::  costFlow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: :math:`O(U * (E + V * logV))`, where :math:`U` is the value of the max flow. :math:`U` is upper bound on number of iteration. In many real world cases number of iterations is much smaller than :math:`U`.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source,  target)
    pgr_minCostMaxFlow(edges_sql, sources, target)
    pgr_minCostMaxFlow(edges_sql, source,  targets)
    pgr_minCostMaxFlow(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

.. index::
    single: minCostMaxFlow(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source, target)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`2` to vertex :math:`3`

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: minCostMaxFlow(One to Many) - Experimental

One to Many
...............................................................................

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source, targets)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`13` to vertices :math:`\{7, 1, 4\}`

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: minCostMaxFlow(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, sources, target)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example: From vertices :math:`\{1, 7, 14\}` to vertex :math:`12`

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: minCostMaxFlow(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_maxFlow(edges_sql, sources, targets)
    RETURNS FLOAT

:Example: From vertices :math:`\{7, 13\}` to vertices :math:`\{3, 9\}`

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q4
   :end-before: -- q5

.. include::  ./pgr_minCostMaxFlow_Cost.rst
    :start-after: pgr_costFlow_parameters_start
    :end-before: pgr_costFlow_parameters_end

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: costFlow_edges_sql_start
    :end-before: costFlow_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: result_costFlow_start
    :end-before: result_costFlow_end

See Also
-------------------------------------------------------------------------------

* :doc:`costFlow-family`
* https://www.boost.org/libs/graph/doc/successive_shortest_path_nonnegative_weights.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

