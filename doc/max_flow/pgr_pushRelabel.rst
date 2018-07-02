..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_pushRelabel:

pgr_pushRelabel
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_pushRelabel`` — Calculates the flow on the graph edges that maximizes the flow from the sources to the targets using Push Relabel Algorithm.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability:

* Renamed 2.5.0, Previous name pgr_maxFlowPushRelabel
* New in 2.3.0

.. include::  flow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_pushRelabel(edges_sql, source,  target)
    pgr_pushRelabel(edges_sql, sources, target)
    pgr_pushRelabel(edges_sql, source,  targets)
    pgr_pushRelabel(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET


.. index::
    single: pushRelabel(One to One)

One to One
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from the `source` to the `target`.

.. code-block:: none

    pgr_pushRelabel(edges_sql, source,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_pushRelabel.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: pushRelabel(One to Many)

One to Many
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from the `source` to all of the `targets`.

.. code-block:: none

    pgr_pushRelabel(edges_sql, source,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_pushRelabel.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: pushRelabel(Many to One)

Many to One
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from all of the `sources` to the `target`.

.. code-block:: none

    pgr_pushRelabel(edges_sql, sources,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_pushRelabel.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: pushRelabel(Many to Many)

Many to Many
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from all of the `sources` to all of the `targets`.

.. code-block:: none

    pgr_pushRelabel(edges_sql, sources,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_pushRelabel.queries
   :start-after: -- q4
   :end-before: -- q5

Parameters
--------------------------------------------------------

.. include::  ./pgr_maxFlow.rst
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end

Inner query
--------------------------------------------------------

edges_sql
...........................................................

.. include:: pgRouting-concepts.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

Result Columns
--------------------------------------------------------

.. include:: pgRouting-concepts.rst
   :start-after: result_flow_start
   :end-before: result_flow_end


See Also
--------

* :doc:`flow-family`, :doc:`pgr_boykovKolmogorov`, :doc:`pgr_edmondsKarp`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

