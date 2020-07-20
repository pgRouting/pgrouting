..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_maxFlow
===============================================================================

``pgr_maxFlow`` — Calculates the maximum flow in a directed graph from the source(s) to the targets(s) using the Push Relabel algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.4.0

  * New **Proposed** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxFlow.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxFlow.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_maxFlow.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_maxFlow.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_maxFlow.html>`__

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

- The graph is **directed**.
- Calculates the maximum flow from the `source(s)` to the `target(s)`.

  - When the maximum flow is **0** then there is no flow and **0** is returned.
  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Uses the :doc:`pgr_pushRelabel <pgr_pushRelabel>` algorithm.

* Running time: :math:`O( V ^ 3)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_maxFlow(Edges SQL, source,  target)
    pgr_maxFlow(Edges SQL, sources,  target)
    pgr_maxFlow(Edges SQL, source,  targets)
    pgr_maxFlow(Edges SQL, sources,  targets)
    RETURNS BIGINT

.. index::
    single: maxFlow(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_maxFlow(Edges SQL, source,  target)
    RETURNS BIGINT

:Example: From vertex :math:`6` to vertex :math:`11`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlow(One to Many)

One to Many
...............................................................................

.. code-block:: none

    pgr_maxFlow(Edges SQL, source,  targets)
    RETURNS BIGINT

:Example: From vertex :math:`6` to vertices :math:`\{11, 1, 13\}`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlow(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_maxFlow(Edges SQL, sources,  target)
    RETURNS BIGINT

:Example: From vertices :math:`\{6, 8, 12\}` to vertex :math:`11`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlow(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_maxFlow(Edges SQL, sources,  targets)
    RETURNS BIGINT

:Example: From vertices :math:`\{6, 8, 12\}` to vertices :math:`\{1, 3, 11\}`

.. literalinclude:: doc-pgr_maxFlow.queries
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
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``BIGINT``             Maximum flow possible from the source(s) to the target(s)
====================== =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

