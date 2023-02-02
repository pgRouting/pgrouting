..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_maxFlowMinCost_Cost.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_maxFlowMinCost_Cost.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_maxFlowMinCost_Cost.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_maxFlowMinCost_Cost.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_maxFlowMinCost_Cost.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxFlowMinCost_Cost.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxFlowMinCost_Cost.html>`__

``pgr_maxFlowMinCost_Cost`` - Experimental
===============================================================================

``pgr_maxFlowMinCost_Cost`` — Calculates the minimum total cost of the maximum
flow on a graph

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function:

    * ``pgr_maxFlowMinCost_Cost`` (`Combinations`_)

* Version 3.0.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------

.. include::  flow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

**The main characteristics are:**

- The graph is **directed**.
- **The cost value of all input edges must be nonnegative.**
- When the maximum flow is 0 then there is no flow and **0** is returned.

  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Uses :doc:`pgr_maxFlowMinCost`.

* Running time: :math:`O(U * (E + V * logV))`

  * where :math:`U` is the value of the max flow.
  * :math:`U` is upper bound on number of iterations.
    In many real world cases number of iterations is much smaller than :math:`U`.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vid**, **end vid**)
   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vid**, **end vids**)
   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vids**, **end vid**)
   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vids**, **end vids**)
   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, `Combinations SQL`_)

   | RETURNS FLOAT

.. index::
    single: maxFlowMinCost_Cost(One to One) - Experimental on v3.0

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vid**, **end vid**)

   | RETURNS FLOAT

:Example: From vertex :math:`11` to vertex :math:`12`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlowMinCost_Cost(One to Many) - Experimental on v3.0

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vid**, **end vids**)

   | RETURNS FLOAT

:Example: From vertex :math:`11` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlowMinCost_Cost(Many to One) - Experimental on v3.0

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vids**, **end vid**)

   | RETURNS FLOAT

:Example: From vertices :math:`\{11, 3, 17\}` to vertex :math:`12`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlowMinCost_Cost(Many to Many) - Experimental on v3.0

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, **start vids**, **end vids**)

   | RETURNS FLOAT

:Example: From vertices :math:`\{11, 3, 17\}` to vertices :math:`\{5, 10, 12\}`


.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: maxFlowMinCost_Cost(Combinations) - Experimental on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlowMinCost_Cost(`Edges SQL`_, `Combinations SQL`_)

   | RETURNS FLOAT

:Example: Using a combinations table, equivalent to calculating result from
          vertices :math:`\{5, 6\}` to vertices :math:`\{10, 15, 14\}`.

The combinations table:

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
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

Resturn Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``FLOAT``              Minimum Cost Maximum Flow possible from the source(s) to the target(s)
====================== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Manually assigned vertex combinations.

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q6
   :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* https://www.boost.org/libs/graph/doc/successive_shortest_path_nonnegative_weights.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

