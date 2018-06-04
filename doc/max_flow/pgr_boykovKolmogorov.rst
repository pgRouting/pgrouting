..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_boykovKolmogorov:

pgr_boykovKolmogorov
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_boykovKolmogorov`` — Calculates the flow on the graph edges that maximizes the flow from the sources to the targets using Boykov Kolmogorov algorithm.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability:

* Renamed 2.5.0, Previous name pgr_maxFlowBoykovKolmogorov
* New in 2.3.0

.. include::  flow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: Polynomial

Signature Summary
-----------------

.. code-block:: none

    pgr_boykovKolmogorov(edges_sql, source,  target)
    pgr_boykovKolmogorov(edges_sql, sources, target)
    pgr_boykovKolmogorov(edges_sql, source,  targets)
    pgr_boykovKolmogorov(edges_sql, sources, targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET


.. index::
    single: boykovKolmogorov(One to One)

One to One
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from the `source` to the `target`.

.. code-block:: none

    pgr_boykovKolmogorov(edges_sql, source,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: boykovKolmogorov(One to Many)

One to Many
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from the `source` to all of the `targets`.

.. code-block:: none

    pgr_boykovKolmogorov(edges_sql, source,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q2
   :end-before: -- q3


.. index::
    single: boykovKolmogorov(Many to One)

Many to One
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from all of the `sources` to the `target`.

.. code-block:: none

    pgr_boykovKolmogorov(edges_sql, sources,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q3
   :end-before: -- q4


.. index::
    single: boykovKolmogorov(Many to Many)

Many to Many
.....................................................................

Calculates the flow on the graph edges that maximizes the flow from all of the `sources` to all of the `targets`.

.. code-block:: none

    pgr_boykovKolmogorov(edges_sql, sources,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
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

* :doc:`flow-family`, :doc:`pgr_pushRelabel`, :doc:`pgr_edmondsKarp`
* http://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

