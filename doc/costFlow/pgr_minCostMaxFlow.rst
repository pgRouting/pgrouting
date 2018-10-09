..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_minCostMaxFlow - Experimental 
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_minCostMaxFlow`` — Calculates the flow on the graph edges that maximizes the flow and minimizes the cost from the sources to the targets.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. include::  costFlow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: :math:`O(U * (E + V * logV))`, where :math:`U` is the value of the max flow. :math:`U` is upper bound on number of iteration. In many real world cases number of iterations is much smaller than :math:`U`.

Signature Summary
-----------------

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source, target)
    pgr_minCostMaxFlow(edges_sql, sources, target)
    pgr_minCostMaxFlow(edges_sql, source, targets)
    pgr_minCostMaxFlow(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET


.. index::
    single: minCostMaxFlow(One to One) - Experimental 

One to One
.....................................................................

Calculates the minimum cost maximum flow from the `source` to the `target`.

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source, target)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: minCostMaxFlow(Many to One) - Experimental 

Many to One 
.....................................................................

Calculates the minimum cost maximum flow from the `sources` to all of the `target`.

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, sources, target)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: minCostMaxFlow(One to Many) - Experimental 

One to Many 
.....................................................................

Calculates the minimum cost maximum flow from the `source` to all of the `targets`.

.. code-block:: none

    pgr_minCostMaxFlow(edges_sql, source, targets)
    RETURNS SET OF (seq, edge, source, target, flow, residual_capacity, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: minCostMaxFlow(Many to Many) - Experimental 

Many to Many
.....................................................................

Calculates the minimum cost maximum flow from the `sources` to all of the `targets`.

.. code-block:: none

    pgr_maxFlow(edges_sql, sources, targets)
    RETURNS FLOAT 

:Example:

.. literalinclude:: doc-pgr_minCostMaxFlow.queries
   :start-after: -- q4
   :end-before: -- q5


Description of the Signatures
--------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: costFlow_edges_sql_start
    :end-before: costFlow_edges_sql_end


.. include::  ./pgr_minCostMaxFlow_Cost.rst
    :start-after: pgr_costFlow_parameters_start
    :end-before: pgr_costFlow_parameters_end


.. include:: pgRouting-concepts.rst
    :start-after: result_costFlow_start
    :end-before: result_costFlow_end


See Also
--------

* :doc:`costFlow-family`
* https://www.boost.org/libs/graph/doc/successive_shortest_path_nonnegative_weights.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

