..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index:: Driving Distance Category

|

Driving Distance - Category
===============================================================================

.. official-start

* :doc:`pgr_drivingDistance` - Driving Distance based on Dijkstra's algorithm
* :doc:`pgr_primDD` - Driving Distance based on Prim's algorithm
* :doc:`pgr_kruskalDD` - Driving Distance based on Kruskal's algorithm
* Post processing

  * :doc:`pgr_alphaShape` - Alpha shape computation

.. official-end

.. include:: proposed.rst
   :start-after: warning-begin
   :end-before: end-warning

.. proposed-start

* :doc:`pgr_withPointsDD` - Driving Distance based on pgr_withPoints

.. proposed-end

.. toctree::
    :hidden:

    pgr_alphaShape.rst


Calculate nodes that are within a distance.

.. dd_traits_start

* Extracts all the nodes that have costs less than or equal to the value
  distance.
* The edges extracted will conform to the corresponding spanning tree.
* Edge :math:`(u, v)` will not be included when:

  * The distance from the **root** to :math:`u` > limit distance.
  * The distance from the **root** to :math:`v` > limit distance.
  * No new nodes are created on the graph, so when is within the limit and is
    not within the limit, the edge is not included.

.. dd_traits_end

Parameters
-------------------------------------------------------------------------------

.. spantree-params_start

.. list-table::
   :width: 81
   :widths: 14 20 46
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - Edges SQL as described below.
   * - **Root vid**
     - ``BIGINT``
     - Identifier of the root vertex of the tree.
   * - **Root vids**
     - ``ARRAY[ANY-INTEGER]``
     - Array of identifiers of the root vertices.

       - :math:`0` values are ignored
       - For optimization purposes, any duplicated value is ignored.
   * - **distance**
     - ``FLOAT``
     - Upper limit for the inclusion of a node in the result.

Where:

:ANY-NUMERIC: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. spantree-params_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result columns
...............................................................................

.. spantree-result-columns-start

Returns set of |result-spantree|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - ``seq``
     - ``BIGINT``
     - Sequential value starting from :math:`1`.
   * - ``depth``
     - ``BIGINT``
     - Depth of the ``node``.

       - :math:`0` when ``node`` = ``start_vid``.
       - :math:`depth-1` is the depth of ``pred``

   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the root vertex.
   * - ``pred``
     - ``BIGINT``
     - Predecessor of ``node``.

       - When ``node`` = ``start_vid`` then has the value ``node``.
   * - ``node``
     - ``BIGINT``
     - Identifier of ``node`` reached using ``edge``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the ``edge`` used to arrive from ``pred`` to ``node``.

       - :math:`-1` when ``node`` = ``start_vid``.

   * - ``cost``
     - ``FLOAT``
     - Cost to traverse ``edge``.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

.. spantree-result-columns-end

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
