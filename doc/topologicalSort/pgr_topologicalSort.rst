..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_topologicalSort - Experimental
===============================================================================

``pgr_topologicalSort`` — Returns the linear ordering of the vertices(s) for weighted directed acyclic graphs(DAG).
In particular, the topological sort algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_65_1/libs/graph/doc/topological_sort.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_topologicalSort.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_topologicalSort.html>`__

* **TBD**

Description
-------------------------------------------------------------------------------

The topological sort algorithm creates a linear ordering of the vertices such that if edge (u,v) appears
in the graph, then v comes before u in the ordering.

This implementation can only be used with a **directed** graph with no cycles i.e. directed acyclic graph.

The main characteristics are:
  - Process is valid for directed acyclic graphs only. otherwise it will throw warnings.

  - For optimization purposes, if there are more than one answer, the function will return one of them.

  - The returned values are ordered in topological order:

  * Running time: :math:`O( (V + E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_topologicalSort(edges_sql)

    RETURNS SET OF (seq, sorted_v)
    OR EMPTY SET

:Example: For a **directed** graph

.. literalinclude:: doc-topologicalSort.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

=================== ====================== ========= =================================================
Parameter           Type                   Default   Description
=================== ====================== ========= =================================================
**edges_sql**       ``TEXT``                         SQL query as described above.
=================== ====================== ========= =================================================

Inner query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, sorted_v)``

===============  =========== ============================================================
Column           Type        Description
===============  =========== ============================================================
**seq**          ``INT``     Sequential value starting from **1**.
**sorted_v**     ``BIGINT``  Linear ordering of the vertices(ordered in topological order)
===============  =========== ============================================================

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Topological_sorting
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

