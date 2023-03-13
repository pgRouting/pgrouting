..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_maxFlow.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_maxFlow.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_maxFlow.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_maxFlow.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_maxFlow.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_maxFlow.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxFlow.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxFlow.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_maxFlow.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_maxFlow.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_maxFlow.html>`__

``pgr_maxFlow``
===============================================================================

``pgr_maxFlow`` — Calculates the maximum flow in a directed graph from the
source(s) to the targets(s) using the Push Relabel algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.2.0

  * New **proposed** signature

    * ``pgr_maxFlow`` (`Combinations`_)

* Version 3.0.0

  * **Official** function

* Version 2.4.0

  * New **Proposed** function


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

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, **start vid**, **end vid**)
   | pgr_maxFlow(`Edges SQL`_, **start vid**, **end vids**)
   | pgr_maxFlow(`Edges SQL`_, **start vids**, **end vid**)
   | pgr_maxFlow(`Edges SQL`_, **start vids**, **end vids**)
   | pgr_maxFlow(`Edges SQL`_, `Combinations SQL`_)

   | RETURNS ``BIGINT``

.. index::
    single: maxFlow(One to One)

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, **start vid**, **end vid**)

   | RETURNS ``BIGINT``

:Example: From vertex :math:`11` to vertex :math:`12`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: maxFlow(One to Many)

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, **start vid**, **end vids**)

   | RETURNS ``BIGINT``

:Example: From vertex :math:`11` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: maxFlow(Many to One)

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, **start vids**, **end vid**)

   | RETURNS ``BIGINT``

:Example: From vertices :math:`\{11, 3, 17\}` to vertex :math:`12`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: maxFlow(Many to Many)

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, **start vids**, **end vids**)

   | RETURNS ``BIGINT``

:Example: From vertices :math:`\{11, 3, 17\}` to vertices :math:`\{5, 10, 12\}`

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: maxFlow(Combinations) - Proposed on v3.2

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_maxFlow(`Edges SQL`_, `Combinations SQL`_)

   | RETURNS ``BIGINT``

:Example: Using a combinations table, equivalent to calculating result from
          vertices :math:`\{5, 6\}` to vertices :math:`\{10, 15, 14\}`.

The combinations table:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q5
   :end-before: -- q51

The query:

.. literalinclude:: doc-pgr_maxFlow.queries
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
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``BIGINT``             Maximum flow possible from the source(s) to the target(s)
====================== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Manually assigned vertex combinations.

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q6
   :end-before: -- q7

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`

  * :doc:`pgr_pushRelabel`

* https://www.boost.org/libs/graph/doc/push_relabel_max_flow.html
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

