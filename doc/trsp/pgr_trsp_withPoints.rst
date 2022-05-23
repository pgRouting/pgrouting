..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_trsp_withPoints.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgr_trsp_withPoints.html>`__)

pgr_trsp_withPoints - Proposed
===============================================================================

``pgr_trsp_withPoints`` Routing Vertex/Point with restrictions.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **Proposed** signatures

    * ``pgr_trsp_withPoints`` (`One to One`_)
    * ``pgr_trsp_withPoints`` (`One to Many`_)
    * ``pgr_trsp_withPoints`` (`Many to One`_)
    * ``pgr_trsp_withPoints`` (`Many to Many`_)
    * ``pgr_trsp_withPoints`` (`Combinations`_)

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, find the shortest path(s)

**The main characteristics are:**

- Process is done only on edges with positive costs.
- Driving side can not be `b` (**both**)
- Vertices of the graph are:

  - **positive** when it belongs to the `Edges SQL`_
  - **negative** when it belongs to the `Points SQL`_

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.

    - The agg_cost the non included values (v, v) is 0

  - When the starting vertex and ending vertex are the different and there is no
    path:

    - The agg_cost the non included values (u, v) is ∞

- For optimization purposes, any duplicated value in the start_vids or end_vids
  are ignored.
- The returned values are ordered:
  - start_vid ascending
  - end_vid ascending

* Running time: :math:`O(|start\_vids|\times(V \log V + E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid,  end vid
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid,  end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vid
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, `Points SQL`_,
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

.. index::
    single: trsp_withPoints(One to One) - Proposed on v3.4

One to One
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid,  end vid
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` to vertex :math:`10` with details on a **left**
          driving side configuration on a **directed** graph with **details**.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e1
   :end-before: --e2

.. index::
    single: trsp_withPoints(One to Many) - Proposed on v3.4

One to Many
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid, end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` to point :math:`3` and vertex :math:`7`.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: trsp_withPoints(Many to One) - Proposed on v3.4

Many to One
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vid
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6` to point :math:`3`.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: trsp_withPoints(Many to Many) - Proposed on v3.4

Many to Many
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`6`  to point :math:`3` and
          vertex :math:`1`.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: trsp_withPoints(Combinations) - Proposed on v3.4

Combinations
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, `Points SQL`_,
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)

:Example: From point :math:`1` to vertex :math:`10` and from vertex :math:`6` to
          point :math:`3` with **right** side driving configuration.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e5
   :end-before: --q1

Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: restriction_parameters_start
    :end-before: restriction_parameters_end


Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. withPoints_parameters_start

.. list-table::
   :width: 81
   :widths: 14 7 7 60
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``driving_side``
     - ``CHAR``
     - ``r``
     - Value in [``r``, ``l``] indicating if the driving side is:

       - ``r`` for right driving side
       - ``l`` for left driving side
       - Any other value will be considered as ``r``
   * - ``details``
     - ``BOOLEAN``
     - ``false``
     - - When ``true`` the results will include the points that are in the path.
       - When ``false`` the results will not include the points that are in the
         path.

.. withPoints_parameters_end

|

Inner query
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Return Columns
-------------------------------------------------------------------------------

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
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.

       - ``0`` for the last row in the path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

       - ``0`` for the first row in the path sequence.

.. result columns end

|

Additional Examples
-------------------------------------------------------------------------------

:Example: Which path (if any) passes in front of point :math:`6` or vertex
          :math:`11` with **right** side driving topology.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --q1
   :end-before: --q2


:Example: From point :math:`1` and vertex :math:`6` to point :math:`3` to vertex
          :math:`1` on an **undirected** graph, with details.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --q2
   :end-before: --q3

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`withPoints-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
