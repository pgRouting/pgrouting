..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Metrics Family ; pgr_bandwidth - Experimental
   single: bandwidth - Experimental on v4.0

|

``pgr_bandwidth`` - Experimental
===============================================================================

``pgr_bandwidth`` - Calculates the bandwidth of the graph

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. rubric:: Availability

* Version 4.0.0

  * New experimental function.

Description
-------------------------------------------------------------------------------

TBD

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   pgr_bandwidth(`Edges SQL`_,)

   | Returns ``BIGINT``

:Example: For an undirected graph with edges.

.. literalinclude:: bandwidth.queries
   :start-after: -- q1
   :end-before: -- q2

.. rubric:: Explanation

TBD

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: edges_start
    :end-before: edges_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

Returns a bigint ``(pgr_bandwidth)``

=================  =========== ==========================================
Column             Type        Description
=================  =========== ==========================================
``pgr_bandwidth``   ``BIGINT`` - gives the bandwidth of the graph.
=================  =========== ==========================================

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: bandwidth
  <https://www.boost.org/libs/graph/doc/bandwidth.html>`_

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
