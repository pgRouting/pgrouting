..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_maxFlow:

pgr_maxFlow - Proposed
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_maxFlow`` — Calculates the maximum flow in a directed graph from the source(s) to the targets(s) using the Push Relabel algorithm.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability: 2.4.0

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


.. rubric:: Characteristics

- The graph is **directed**.
- When the maximum flow is 0 then there is no flow and **0** is returned.

  - There is no flow when a **source** is the same as a **target**.

- Any duplicated value in the source(s) or target(s) are ignored.
- Uses the :ref:`pgr_pushRelabel <pgr_pushRelabel>` algorithm.

* Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_maxFlow(edges_sql, source,  target)
    pgr_maxFlow(edges_sql, sources,  target)
    pgr_maxFlow(edges_sql, source,  targets)
    pgr_maxFlow(edges_sql, sources,  targets)
    RETURNS BIGINT


.. index::
    single: maxFlow(One to One) - Proposed

One to One
.....................................................................

Calculates the maximum flow from the `source` to the `target`.

.. code-block:: none

    pgr_maxFlow(edges_sql, source,  target)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: maxFlow(One to Many) - Proposed

One to Many
.....................................................................

Calculates the maximum flow from the `source` to all of the `targets`.

.. code-block:: none

    pgr_maxFlow(edges_sql, source,  targets)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: maxFlow(Many to One) - Proposed

Many to One
.....................................................................

Calculates the maximum flow from all the `sources` to the `target`.

.. code-block:: none

    pgr_maxFlow(edges_sql, sources,  target)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: maxFlow(Many to Many) - Proposed

Many to Many
.....................................................................

Calculates the maximum flow from all of the `sources` to all of the `targets`.

.. code-block:: none

    pgr_maxFlow(edges_sql, sources,  targets)
    RETURNS BIGINT

:Example:

.. literalinclude:: doc-pgr_maxFlow.queries
   :start-after: -- q4
   :end-before: -- q5

Description of the Signatures
--------------------------------------------------------


.. include:: pgRouting-concepts.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end


.. pgr_flow_parameters_start

Description of the Parameters of the Flow Signatures
...............................................................................

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    The edges SQL query as described above.
**source**     ``BIGINT``                  Identifier of the starting vertex of the flow.
**sources**    ``ARRAY[BIGINT]``           Array of identifiers of the starting vertices of the flow.
**target**     ``BIGINT``                  Identifier of the ending vertex of the flow.
**targets**    ``ARRAY[BIGINT]``           Array of identifiers of the ending vertices of the flow.
============== ================== ======== =================================================

.. pgr_flow_parameters_end



Description of the return value
.....................................................................

====================== =================================================
Type                   Description
====================== =================================================
``BIGINT``             Maximum flow possible from the source(s) to the target(s)
====================== =================================================

See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

