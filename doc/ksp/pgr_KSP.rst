..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_KSP
===============================================================================

``pgr_KSP`` — Returns the "K" shortest paths.


.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.1.0

  * Signature change

    * Old signature no longer supported

* Version 2.0.0

  * **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_KSP.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_KSP.html>`__
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_KSP.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_KSP.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_KSP.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/ksp/doc/pgr_ksp.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/ksp/doc/pgr_ksp.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/ksp/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/ksp/doc/index.html>`__


Description
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the number of shortest paths desired.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: sql

    pgr_KSP(edges_sql, start_vid, end_vid, K [, directed] [, heap_paths])
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost) or EMPTY SET

.. rubric:: Using defaults

.. code-block:: sql

    pgr_ksp(edges_sql, start_vid, end_vid, K);
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost) or EMPTY SET

:Example: **TBD**

.. index::
    single: KSP

Complete Signature
...............................................................................

.. code-block:: sql

    pgr_KSP(edges_sql, start_vid, end_vid, K [, directed] [, heap_paths])
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost) or EMPTY SET

:Example: **TBD**

Parameters
-------------------------------------------------------------------------------

============== ============ =================================================
Column         Type           Description
============== ============ =================================================
**edges_sql**   ``TEXT``    SQL query as described above.
**start_vid**   ``BIGINT``  Identifier of the starting vertex.
**end_vid**     ``BIGINT``  Identifier of the ending vertex.
**k**           ``INTEGER`` The desiered number of paths.
**directed**    ``BOOLEAN`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**heap_paths**  ``BOOLEAN`` (optional). When ``true`` returns all the paths stored in the process heap. Default is ``false`` which only returns ``k`` paths.
============== ============ =================================================

Roughly, if the shortest path has ``N`` edges, the heap will contain about than ``N * k`` paths for small value of ``k`` and ``k > 1``.

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, path_seq, path_id, node, edge, cost, agg_cost)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
**seq**        ``INTEGER`` Sequential value starting from **1**.
**path_seq**   ``INTEGER`` Relative position in the path of ``node`` and ``edge``. Has value **1** for the beginning of a path.
**path_id**    ``BIGINT``  Path identifier. The ordering of the paths For two paths i, j if i < j then agg_cost(i) <= agg_cost(j).
**node**       ``BIGINT``  Identifier of the node in the path.
**edge**       ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the route.
**cost**       ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
**agg_cost**   ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
============== =========== =================================================

Additional Examples
------------------------------------------------------------------------------------------

:Example: To handle the one flag to choose signatures

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-ksp.queries
    :start-after: --q1
    :end-before: --q2

:Example: For queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-ksp.queries
    :start-after: --q2
    :end-before: --q3

:Examples: For queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns

The examples in this section use the following :ref:`fig2`

.. literalinclude:: doc-ksp.queries
    :start-after: --q3
    :end-before: --q4

:Example: For queries marked as ``directed`` with ``cost`` column

The examples in this section use the following :ref:`fig3`

.. literalinclude:: doc-ksp.queries
    :start-after: --q4
    :end-before: --q5

:Example: For queries marked as ``undirected`` with ``cost`` column

The examples in this section use the following :ref:`fig4`

.. literalinclude:: doc-ksp.queries
    :start-after: --q5
    :end-before: --q6

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/K_shortest_path_routing
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
