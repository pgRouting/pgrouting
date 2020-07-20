..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_maxFlowMinCost_Cost - Experimental
============================================

``pgr_maxFlowMinCost_Cost`` — Calculates the minmum cost maximum flow in a directed graph from the source(s) to the targets(s).

.. TODO check which functions is used from boost

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxFlowMinCost_Cost.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxFlowMinCost_Cost.html>`__

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

- The graph is **directed**.
- **The cost value of all input edges must be nonnegative.**
- When the maximum flow is 0 then there is no flow and **0** is returned.

  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Uses the :doc:`pgr_maxFlowMinCost <pgr_maxFlowMinCost>` algorithm.

* Running time: :math:`O(U * (E + V * logV))`, where :math:`U` is the value of the max flow. :math:`U` is upper bound on number of iteration. In many real world cases number of iterations is much smaller than :math:`U`.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_maxFlowMinCost_Cost(Edges SQL, source, target)
    pgr_maxFlowMinCost_Cost(Edges SQL, sources, target)
    pgr_maxFlowMinCost_Cost(Edges SQL, source, targets)
    pgr_maxFlowMinCost_Cost(Edges SQL, sources, targets)
    RETURNS FLOAT

.. index::
    single: maxFlowMinCost_Cost(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_maxFlowMinCost_Cost(Edges SQL, source, target)
    RETURNS FLOAT

:Example: From vertex :math:`2` to vertex :math:`3`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlowMinCost_Cost(One to Many) - Experimental

One to Many
...............................................................................

.. code-block:: none

    pgr_maxFlowMinCost_Cost(Edges SQL, source, targets)
    RETURNS FLOAT

:Example: From vertex :math:`13` to vertices :math:`\{7, 1, 4\}`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlowMinCost_Cost(Many to One) - Experimental

Many to One
...............................................................................

.. code-block:: none

    pgr_maxFlowMinCost_Cost(Edges SQL, sources, target)
    RETURNS FLOAT

:Example: From vertices :math:`\{1, 7, 14\}` to vertex :math:`12`

.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlowMinCost_Cost(Many to Many) - Experimental

Many to Many
...............................................................................

.. code-block:: none

    pgr_maxFlowMinCost_Cost(Edges SQL, sources, targets)
    RETURNS FLOAT

:Example: From vertices :math:`\{7, 13\}` to vertices :math:`\{3, 9\}`


.. literalinclude:: doc-pgr_maxFlowMinCost_Cost.queries
   :start-after: -- q4
   :end-before: -- q5


Parameters
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end

Inner query
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: costFlow_edges_sql_start
    :end-before: costFlow_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``FLOAT``              Minimum Cost Maximum Flow possible from the source(s) to the target(s)
====================== =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* https://www.boost.org/libs/graph/doc/successive_shortest_path_nonnegative_weights.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

