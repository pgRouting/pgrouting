..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_maxCardinalityMatch:

pgr_maxCardinalityMatch - Proposed
============================================================


Synopsis
------------------------------------------------------------

``pgr_maxCardinalityMatch`` — Calculates a maximum cardinality matching in a graph.

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/maximum_matching.html

   Boost Graph Inside

.. Rubric:: Availability:

* Renamed 2.5.0, Previous name pgr_maximumCardinalityMatching
* New in 2.3.0


.. rubric:: Characteristics

* A matching or independent edge set in a graph is a set of edges without common vertices.
* A maximum matching is a matching that contains the largest possible number of edges.

  * There may be many maximum matchings.
  * Calculates **one** possible maximum cardinality matching in a graph.

* The graph can be **directed** or **undirected**.
* Running time: :math:`O( E*V * \alpha(E,V))`

    * :math:`\alpha(E,V)` is the inverse of the `Ackermann function`_.


.. _Ackermann function: https://en.wikipedia.org/wiki/Ackermann_function

Signature Summary
------------------------------------------------------------

.. code-block:: none

    pgr_MaximumCardinalityMatching(edges_sql) - Proposed
    pgr_MaximumCardinalityMatching(edges_sql, directed) - Proposed

    RETURNS SET OF (seq, edge_id, source, target)
        OR EMPTY SET


.. index::
    single: MaximumCardinalityMatching(Minimal Use) - Proposed



Minimal Use
.............................................

.. code-block:: none

    pgr_MaximumCardinalityMatching(edges_sql)
    RETURNS SET OF (seq, edge_id, source, target) OR EMPTY SET

The minimal use calculates one possible maximum cardinality matching on a **directed** graph.

:Example:

.. literalinclude:: doc-pgr_maxCardinalityMatch.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: MaximumCardinalityMatching(Complete Signature) - Proposed

Complete signature
.............................................

.. code-block:: none

    pgr_MaximumCardinalityMatching(edges_sql, directed)
    RETURNS SET OF (seq, edge_id, source, target) OR EMPTY SET


The complete signature calculates one possible maximum cardinality matching.

:Example:

.. literalinclude:: doc-pgr_maxCardinalityMatch.queries
   :start-after: -- q2
   :end-before: -- q3



Description of the Signatures
--------------------------------------------------------


Description of the SQL query
...........................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

====================  ===================   =================================================
Column                Type                  Description
====================  ===================   =================================================
**id**                ``ANY-INTEGER``       Identifier of the edge.
**source**            ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**            ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**going**             ``ANY-NUMERIC``       A positive value represents the existence of the edge (source, target).
**coming**            ``ANY-NUMERIC``       A positive value represents the existence of the edge (target, source).
====================  ===================   =================================================

Where:

  - :ANY-INTEGER: SMALLINT, INTEGER, BIGINT
  - :ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, DOUBLE PRECISION

Description of the parameters of the signatures
...........................................................

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**directed**      ``BOOLEAN``            (optional) Determines the type of the graph. Default TRUE.
================= ====================== =================================================

Description of the Result
...........................................................

=====================  ====================  =================================================
Column                 Type                  Description
=====================  ====================  =================================================
**seq**                ``INT``               Sequential value starting from **1**.
**edge**               ``BIGINT``            Identifier of the edge in the original query(edges_sql).
**source**             ``BIGINT``            Identifier of the first end point of the edge.
**target**             ``BIGINT``            Identifier of the second end point of the edge.
=====================  ====================  =================================================

See Also
--------

* :ref:`maxFlow`
* http://www.boost.org/libs/graph/doc/maximum_matching.html
* https://en.wikipedia.org/wiki/Matching_%28graph_theory%29
* https://en.wikipedia.org/wiki/Ackermann_function

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

