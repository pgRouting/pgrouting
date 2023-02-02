..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/via-category.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/via-category.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/via-category.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/via-category.html>`__

Via - Category
===============================================================================

.. rubric:: proposed

.. include:: proposed.rst
    :start-after: begin-warning
    :end-before: end-warning

.. proposed start

* :doc:`pgr_dijkstraVia`
* :doc:`pgr_withPointsVia`
* :doc:`pgr_trspVia`
* :doc:`pgr_trspVia_withPoints`

.. proposed end

General Information
-------------------------------------------------------------------------------

This category intends to solve the general problem:

   Given a graph and a list of vertices, find the shortest path between
   :math:`vertex_i` and :math:`vertex_{i+1}` for all vertices

In other words, find a continuos route that visits all the vertices in the order
given.

:path: represents a section of a **route**.
:route: is a sequence of **paths**

Parameters
-------------------------------------------------------------------------------

**Used on:**

* :doc:`pgr_dijkstraVia`
* :doc:`pgr_trspVia`

.. via_parameters_start

.. list-table::
   :width: 81
   :widths: 14 20 7 40
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     -
     - SQL query as described.
   * - **via vertices**
     - ``ARRAY`` [ **ANY-INTEGER** ]
     -
     - Array of ordered vertices identifiers that are going to be visited.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT

.. via_parameters_end

**Used on:**

* :doc:`pgr_withPointsVia`
* :doc:`pgr_trspVia_withPoints`

.. via_withPoints_parameters_start

.. list-table::
   :width: 81
   :widths: 14 20 7 40
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     -
     - SQL query as described.
   * - `Points SQL`_
     - ``TEXT``
     -
     - SQL query as described.
   * - **via vertices**
     - ``ARRAY`` [ **ANY-INTEGER** ]
     -
     - Array of ordered vertices identifiers that are going to be visited.

       * When positive it is considered a vertex identifier
       * When negative it is considered a point identifier

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. via_withPoints_parameters_end

Besides the compulsory parameters each function has, there are optional
parameters that exist due to the kind of function.

Via optional parameters
...............................................................................

.. rubric:: Used on all Via functions

.. via_optionals_start

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``strict``
     - ``BOOLEAN``
     - ``false``
     - * When ``true`` if a path is missing stops and returns **EMPTY SET**
       * When ``false`` ignores missing paths returning all paths found
   * - ``U_turn_on_edge``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` departing from a visited vertex will not try to avoid

.. via_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Depending on the function one or more inner queries are needed.

Edges SQL
...............................................................................

.. rubric:: Used on all Via functions

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

Used on

* :doc:`pgr_trspVia`

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

Points SQL
...............................................................................

Used on

* :doc:`pgr_withPointsVia`

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result_via_start

.. list-table::
   :width: 81
   :widths: 12 14 60
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from **1**.
   * - ``path_id``
     - ``INTEGER``
     - Identifier of a path. Has value **1** for the first path.
   * - ``path_seq``
     - ``INTEGER``
     - Relative position in the path. Has value **1** for the beginning of a
       path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex of the path.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex of the path.
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_vid`` to ``end_vid``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence.

       * -1 for the last node of the path.
       * -2 for the last node of the route.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.
   * - ``route_agg_cost``
     - ``FLOAT``
     - Total cost from ``start_vid`` of ``seq = 1`` to ``end_vid`` of the
       current ``seq``.

.. result_via_end

.. result_via_withPoints_start

.. Note::
   When ``start_vid``, ``end_vid`` and ``node`` columns have negative values,
   the identifier is for a Point.

.. result_via_withPoints_end

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_dijkstraVia`
* :doc:`pgr_trspVia`
* :doc:`pgr_withPointsVia`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
