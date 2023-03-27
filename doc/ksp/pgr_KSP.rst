..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_KSP
===============================================================================

``pgr_KSP`` — Yen's algorithm for K shortest paths using Dijkstra.


.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.1.0

  * Signature change

    * Old signature no longer supported

* Version 2.0.0

  * **Official** function


Description
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the
number of shortest paths desired.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_KSP(`Edges SQL`_, **start vid**, **end vid**, **K**, [**options**])
   | **options:** ``[directed, heap_paths]``

   | RETURNS SET OF |ksp-result|
   | OR EMPTY SET

:Example: Get 2 paths from :math:`6` to :math:`17` on a directed graph.

.. literalinclude:: doc-ksp.queries
    :start-after: --q1
    :end-before: --q2

Parameters
-------------------------------------------------------------------------------

.. ksp_parameters_start

.. list-table::
   :width: 81
   :widths: 17 22 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - SQL query as described.
   * - **start vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.
   * - **end vid**
     - **ANY-INTEGER**
     - Identifier of the arrival vertex.
   * - **K**
     - **ANY-INTEGER**
     - Number of required paths.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. ksp_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

KSP Optional parameters
-------------------------------------------------------------------------------

.. ksp_optionals_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``heap_paths``
     - ``BOOLEAN``
     - ``false``
     - * When ``false`` Returns at most K paths.
       * When ``true`` all the calculated paths while processing are returned.
       * Roughly, when the shortest path has ``N`` edges, the heap will contain
         about than ``N * K`` paths for small value of ``K`` and ``K > 5``.

.. ksp_optionals_end


Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. ksp_returns_start

Returns set of ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost,
agg_cost)``

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from **1**.
   * - ``path_id``
     - ``INTEGER``
     - Path identifier.

       * Has value **1** for the first of a path from **start vid** to
         **end_vid**
   * - ``path_seq``
     - ``INTEGER``
     - Relative position in the path. Has value **1** for the beginning of a
       path.
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from **start vid** to **end vid**
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence. **-1** for the last node of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.

       * :math:`0` for the last ``node`` of the path.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from **start vid** to ``node``.

.. ksp_returns_end


Additional Examples
-------------------------------------------------------------------------------

:Example: Get 2 paths from :math:`6` to :math:`17` on an undirected graph

Also get the paths in the heap.

.. literalinclude:: doc-ksp.queries
    :start-after: --q2
    :end-before: --q3

See Also
-------------------------------------------------------------------------------

* :doc:`KSP-category`
* :doc:`sampledata`
* https://en.wikipedia.org/wiki/K_shortest_path_routing

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
