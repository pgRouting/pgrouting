..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_maxCardinalityMatch
===============================================================================

``pgr_maxCardinalityMatch`` — Calculates a maximum cardinality matching in a graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/maximum_matching.html

   Boost Graph Inside

.. Rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.5.0

  * Renamed from ``pgr_maximumCardinalityMatching``
  * **Proposed** function

* Version 2.3.0

  * New **Experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_maxCardinalityMatch.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_maxCardinalityMatch.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_maxCardinalityMatch.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_maxCardinalityMatch.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_maximumCardinalityMatching.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/max_flow/doc/pgr_maximumCardinalityMatching.html#pgr-maximumcardinalitymatching>`__


Description
-------------------------------------------------------------------------------

**The main characteristics are:**

* A matching or independent edge set in a graph is a set of edges without common vertices.
* A maximum matching is a matching that contains the largest possible number of edges.

  * There may be many maximum matchings.
  * Calculates **one** possible maximum cardinality matching in a graph.

* The graph can be **directed** or **undirected**.

* Running time: :math:`O( E*V * \alpha(E,V))`

  * :math:`\alpha(E,V)` is the inverse of the `Ackermann function`_.

.. _Ackermann function: https://en.wikipedia.org/wiki/Ackermann_function

Signatures
-------------------------------------------------------------------------------

.. index::
    single: MaximumCardinalityMatch

.. code-block:: none

    pgr_maxCardinalityMatch(Edges SQL [, directed])

    RETURNS SET OF (seq, edge_id, source, target)
    OR EMPTY SET

:Example: For an **undirected** graph

.. literalinclude:: doc-pgr_maxCardinalityMatch.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

============== ================== ======== =========================================
Parameter         Type            Default       Description
============== ================== ======== =========================================
**edges_sql**  ``TEXT``                    SQL query as described above.
**directed**   ``BOOLEAN``        ``true`` Determines the type of the graph.
                                           - When ``true`` Graph is considered `Directed`
                                           - When ``false`` the graph is considered as `Undirected`.

============== ================== ======== =========================================

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query, which should return a set of rows with the following columns:

====================  ===================   =================================================
Column                Type                  Description
====================  ===================   =================================================
**id**                ``ANY-INTEGER``       Identifier of the edge.
**source**            ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**            ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**going**             ``ANY-NUMERIC``       A positive value represents the existence of the edge (``source``, ``target``).
**coming**            ``ANY-NUMERIC``       A positive value represents the existence of the edge (``target``, ``source``).
====================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL FLOAT

Result Columns
-------------------------------------------------------------------------------

=====================  ====================  =================================================
Column                 Type                  Description
=====================  ====================  =================================================
**seq**                ``INT``               Sequential value starting from **1**.
**edge**               ``BIGINT``            Identifier of the edge in the original query.
**source**             ``BIGINT``            Identifier of the first end point of the edge.
**target**             ``BIGINT``            Identifier of the second end point of the edge.
=====================  ====================  =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`flow-family`
* https://www.boost.org/libs/graph/doc/maximum_matching.html
* https://en.wikipedia.org/wiki/Matching_%28graph_theory%29
* https://en.wikipedia.org/wiki/Ackermann_function

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

