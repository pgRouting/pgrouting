..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropiate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_maximumCardinalityMatching:

pgr_maximumCardinalityMatching
==============================


Name
----

``pgr_maximumCardinalityMatching`` — Calculates a maximum cardinality matching in a graph. Implemented by Boost Graph Library.

.. warning::  This is a proposed function.

     - Is not officially in the current release

..
   keep if uses boost (this is a comment)

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/maximum_matching.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Calculates a maximum cardinality matching in a directed/undirected graph.
Implementation details_.

.. _details: http://www.boost.org/libs/graph/doc/maximum_matching.html

Characteristics:
----------------

The main characterics are:
  - Calculates one possible maximum cardinality matching in a graph.
  - The graph can be directed or undirected.
  - Running time: :math:`O( E*V * \alpha(E,V))`
  - :math:`\alpha(E,V)` is the inverse of the `Ackermann function`_.

  .. _Ackermann function: https://en.wikipedia.org/wiki/Ackermann_function

Signature Summary
-----------------

..
   If the function has more than one signature
   Remove the unnecessary parts of the signature, just leaving the name of the parameters
   Like in these examples

.. code-block:: none

    pgr_maximumcardinalitymatching(edges_sql)
    pgr_maximumcardinalitymatching(edges_sql, directed:=true)

    RETURNS SET OF (id, source, target)
        OR EMPTY SET


..
  This is a reminder of how your query looks like
        pgr_maximumcardinalitymatching(
            edges_sql TEXT,
            OUT id BIGINT,
            OUT source BIGINT,
            OUT target BIGINT
        )

Signatures
----------


Minimal signature
.................

.. code-block:: none

    pgr_maximumcardinalitymatching(edges_sql)
    RETURNS SET OF (id, source, target) OR EMPTY SET

The minimal signature calculates one possible maximum cardinality matching.
If the directed parameter is not specified, it is assumed that the graph is directed.

:Example:

.. literalinclude:: doc-maximumCardinalityMatching.queries
   :start-after: -- q1
   :end-before: -- q2

Description of the Signatures
=============================

..
   DELETE / ADD DEPENDING ON YOUR REQUIREMENTS

Description of the SQL query
----------------------------

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
-----------------------------------------------

================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
**directed**      ``BOOLEAN``            (optional) Determines the type of the graph. Default TRUE.
================= ====================== =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

:Example:

.. literalinclude:: doc-maximumCardinalityMatching.queries
   :start-after: -- q2
   :end-before: -- q3

.. literalinclude:: doc-maximumCardinalityMatching.queries
   :start-after: -- q3
   :end-before: -- q4


See Also
--------

* https://en.wikipedia.org/wiki/Matching_%28graph_theory%29

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

