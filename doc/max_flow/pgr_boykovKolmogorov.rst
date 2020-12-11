..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_boykovKolmogorov
============================================

``pgr_boykovKolmogorov`` — Calculates the flow on the graph edges that maximizes the flow from the sources to the targets using Boykov Kolmogorov algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

   Boost Graph Inside

.. Rubric:: Availability:

* Version 3.2.0

  * New **proposed** function:

    * pgr_boykovKolmogorov(Combinations)

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * Renamed from ``pgr_maxFlowBoykovKolmogorov``
  * **Proposed** function

* Version 2.3.0

  * New **Experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/pgr_boykovKolmogorov.html>`__)
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_boykovKolmogorov.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_boykovKolmogorov.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_boykovKolmogorov.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_boykovKolmogorov.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_maxFlowBoykovKolmogorov.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/max_flow/doc/pgr_maxFlowBoykovKolmogorov.html>`__


Description
-------------------------------------------------------------------------------

.. include::  flow-family.rst
    :start-after: characteristics_start
    :end-before: characteristics_end

* Running time: Polynomial

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, source,  target)
    pgr_boykovKolmogorov(Edges SQL, sources, target)
    pgr_boykovKolmogorov(Edges SQL, source,  targets)
    pgr_boykovKolmogorov(Edges SQL, sources, targets)
    pgr_boykovKolmogorov(Edges SQL, Combinations SQL) -- Proposed on v3.2
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

.. index::
    single: boykovKolmogorov(One to One)

One to One
...............................................................................

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, source,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example:  From vertex :math:`6` to vertex :math:`11`

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: boykovKolmogorov(One to Many)

One to Many
...............................................................................

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, source,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{1, 3, 11\}`

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: boykovKolmogorov(Many to One)

Many to One
...............................................................................

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, sources,  target)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example: From vertices :math:`\{6, 8, 12\}` to vertex :math:`11`

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: boykovKolmogorov(Many to Many)

Many to Many
...............................................................................

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, sources,  targets)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example: From vertices :math:`\{6, 8, 12\}` to vertices :math:`\{1, 3, 11\}`

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: boykovKolmogorov(Combinations) -- Proposed on v3.2

Combinations
...............................................................................

.. code-block:: none

    pgr_boykovKolmogorov(Edges SQL, Combinations SQL)
    RETURNS SET OF (seq, edge, start_vid, end_vid, flow, residual_capacity)
    OR EMPTY SET

:Example: Using a combinations table, equivalent to calculating result from vertices :math:`\{6, 8, 12\}` to vertices :math:`\{1, 3, 11\}`.

.. literalinclude:: doc-pgr_boykovKolmogorov.queries
   :start-after: -- q5
   :end-before: -- q6

Parameters
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: pgr_flow_parameters_start
    :end-before: pgr_flow_parameters_end

Inner queries
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: flow_edges_sql_start
    :end-before: flow_edges_sql_end

.. include:: flow-family.rst
    :start-after: flow_combinations_sql_start
    :end-before: flow_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

.. include:: flow-family.rst
    :start-after: result_flow_start
    :end-before: result_flow_end

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`, :doc:`pgr_pushRelabel`, :doc:`pgr_edmondsKarp`
* https://www.boost.org/libs/graph/doc/boykov_kolmogorov_max_flow.html

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

