..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_pushRelabel:

pgr_pushRelabel - Proposed
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_pushRelabel`` — Calculates the flow on the graph edges that maximizes the flow from the sources to the targets using Push Relabel Algorithm.


.. include:: ../../proposedNext.rst
   :start-after: begin-warning
   :end-before: end-warning


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 

   Boost Graph Inside


.. include::  ./maxFlow.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: :math:`O( V ^ 3)`

Signature Summary
-----------------

.. code-block:: none

    pgr_pushRelabel(edges_sql, source,  target) - Proposed
    pgr_pushRelabel(edges_sql, sources, target) - Proposed
    pgr_pushRelabel(edges_sql, source,  targets) - Proposed
    pgr_pushRelabel(edges_sql, sources, targets) - Proposed
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET


.. index::
    single: pushRelabel(One to One) - Proposed

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
    single: pushRelabel(One to Many) - Proposed

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
    single: pushRelabel(Many to One) - Proposed

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
    single: pushRelabel(Many to Many) - Proposed

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

Description of the Signatures
--------------------------------------------------------

.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end


.. include::  ./pgr_maxFlow.rst
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end


.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: result_flow_start
    :end-before: result_flow_end


See Also
--------

* :ref:`maxFlow`, :ref:`pgr_maxFlowBoykovKolmogorov <pgr_maxFlowBoykovKolmogorov>`, :ref:`pgr_maxFlowEdmondsKarp <pgr_maxFlowEdmondsKarp>`
* http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html 
* https://en.wikipedia.org/wiki/Push%E2%80%93relabel_maximum_flow_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

