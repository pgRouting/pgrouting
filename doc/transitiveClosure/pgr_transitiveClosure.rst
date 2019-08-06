..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_transitiveClosure - Experimental
===============================================================================

``pgr_transitiveClosure`` — Returns the transitive closure graph of the input graph.
In particular, the transitive closure algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_70_0/libs/graph/doc/transitive_closure.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* **TBD**

Description
-------------------------------------------------------------------------------

The transitive_closure() function transforms the input graph g into the transitive closure graph tc.

This implementation can only be used with a **directed** graph with no cycles i.e. directed acyclic graph. 

The main characteristics are:
  - Process is valid for directed acyclic graphs only. otherwise it will throw warnings.
  
  - The returned values are not ordered:

  * Running time: :math:`O( (V + E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_transitiveClosure(edges_sql)

    RETURNS SET OF (seq, vid, )
    OR EMPTY SET


.. index::
    single: transitiveClosure(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_dagShortestPath(edges_sql)
    RETURNS SET OF (seq, sorted_v) 
    OR EMPTY SET

:Example: From vertex :math:`1` to vertex :math:`6` 

.. literalinclude:: doc-pgr_transitiveClosure.queries

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

