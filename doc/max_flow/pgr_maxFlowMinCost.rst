..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Flow Family ; pgr_maxFlowMinCost - Experimental
   single: maxFlowMinCost - Experimental

|

``pgr_maxFlowMinCost`` - Experimental
===============================================================================

``pgr_maxFlowMinCost`` — Calculates the edges that minimizes the total cost of
the maximum flow on a graph

.. rubric:: Availability

* Version 3.2.0

  * New experimental signature:

    * pgr_maxFlowMinCost(Combinations)

* Version 3.0.0

  * New experimental function.

Description
-------------------------------------------------------------------------------

|boost| graph inside.

.. include::  flow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* **TODO** check which statement is true:

  * The cost value of all input edges must be nonnegative.
  * Process is done when the cost value of all input edges is nonnegative.
  * Process is done on edges with nonnegative cost.

* Running time: :math:`O(U * (E + V * logV))`

  * where :math:`U` is the value of the max flow.
  * :math:`U` is upper bound on number of iterations. In many real world cases
    number of iterations is much smaller than :math:`U`.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, **start vid**, **end vid**)
   | pgr_maxFlowMinCost(`Edges SQL`_, **start vid**, **end vids**)
   | pgr_maxFlowMinCost(`Edges SQL`_, **start vids**, **end vid**)
   | pgr_maxFlowMinCost(`Edges SQL`_, **start vids**, **end vids**)
   | pgr_maxFlowMinCost(`Edges SQL`_, `Combinations SQL`_)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

.. index::
    single: maxFlowMinCost - Experimental ; One to One - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, **start vid**, **end vid**)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

:Example: From vertex :math:`11` to vertex :math:`12`

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlowMinCost - Experimental ; One to Many - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, **start vid**, **end vids**)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

:Example: From vertex :math:`11` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlowMinCost - Experimental ; Many to One - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, **start vids**, **end vid**)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

:Example: From vertices :math:`\{11, 3, 17\}` to vertex :math:`12`

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlowMinCost - Experimental ; Many to Many - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, **start vids**, **end vids**)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

:Example: From vertices :math:`\{11, 3, 17\}` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: maxFlowMinCost - Experimental ; Combinations -- Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost(`Edges SQL`_, `Combinations SQL`_)

   | Returns set of |result-flow-mincost|
   | OR EMPTY SET

:Example: Using a combinations table, equivalent to calculating result from
          vertices :math:`\{5, 6\}` to vertices :math:`\{10, 15, 14\}`.

The combinations table:

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q51
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: dijkstra-family.rst
    :start-after: dijkstra_parameters_start
    :end-before: dijkstra_parameters_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: costFlow_edges_sql_start
    :end-before: costFlow_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: result_costFlow_start
    :end-before: result_costFlow_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Manually assigned vertex combinations.

.. literalinclude:: maxFlowMinCost.queries
   :start-after: -- q6
   :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* `Boost: push relabel max flow
  <https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

