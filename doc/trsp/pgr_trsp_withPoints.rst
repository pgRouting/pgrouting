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

``pgr_trsp_withPoints`` Vertex/Point - Vertex/Point routing with restrictions.

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
- Vertices of the graph are:

  - **positive** when it belongs to the `Edges SQL`_
  - **negative** when it belongs to the `Points SQL`_

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.

    - The agg_cost the non included values (v, v) is 0

  - When the starting vertex and ending vertex are the different and there is no path:

    - The agg_cost the non included values (u, v) is ∞

- For optimization purposes, any duplicated value in the start_vids or end_vids are ignored.
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

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid,  end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` to vertex :math:`3` with details of passing points

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e2
   :end-before: --e3

.. index::
    single: trsp_withPoints(One to Many) - Proposed on v3.4

One to Many
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vid, end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` to point :math:`3` and vertex :math:`5`

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e3
   :end-before: --e4

.. index::
    single: trsp_withPoints(Many to One) - Proposed on v3.4

Many to One
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vid
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3`

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e4
   :end-before: --e5

.. index::
    single: trsp_withPoints(Many to Many) - Proposed on v3.4

Many to Many
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Points SQL`_, start vids, end vids
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From point :math:`1` and vertex :math:`2`  to point :math:`3` and vertex :math:`7`

.. literalinclude:: trsp_withPoints.queries
   :start-after: --e5
   :end-before: --q2

.. index::
    single: trsp_withPoints(Combinations) - Proposed on v3.4

Combinations
...............................................................................

.. parsed-literal::

   pgr_trsp_withPoints(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, `Points SQL`_,
                             [, directed], [, driving_side] [, details]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)

:Example: Two (source, target) combinations: (from point :math:`1` to vertex :math:`3`), and (from vertex :math:`2` to
          point :math:`3`) with **right** side driving topology.


.. literalinclude:: trsp_withPoints.queries
   :start-after: --q5
   :end-before: --q6

Parameters
-------------------------------------------------------------------------------

====================== ================================== ========= ==========================================
Parameter              Type                               Default   Description
====================== ================================== ========= ==========================================
`Edges SQL`_           ``TEXT``                                     `Edges SQL`_ as described below.
`Points SQL`_          ``TEXT``                                     `Points SQL`_ as described below.
`Combinations SQL`_    ``TEXT``                                     `Combinations SQL`_ as described below.
**start vid**          **ANY-INTEGER**                              Starting vertex identifier. When negative: is a point's identifier.
**end vid**            **ANY-INTEGER**                              Ending vertex identifier. When negative: is a point's identifier.
**start vids**         ``ARRAY[`` **ANY-INTEGER** ``]``             Array of identifiers of starting vertices. When negative: is a point's identifier.
**end vids**           ``ARRAY[`` **ANY-INTEGER]** ``]``            Array of identifiers of ending vertices. When negative: is a point's identifier.
``directed``           ``BOOLEAN``                        ``true``  - When ``true`` the graph is considered as directed.
                                                                    - When ``false`` the graph is considred as undirected.

``driving_side``       ``CHAR``                           ``b``     Value in :math:`{b,r,l}` indicating if the driving side is:
                                                                    - ``r`` for right driving side
                                                                    - ``l`` for left driving side
                                                                    - ``b`` for both, or ignore driving side

``details``            ``BOOLEAN``                        ``false`` - When ``true`` the results will include the points in `Points SQL`_ that are in the path.
                                                                    - When ``false`` ignores other points of the `Points SQL`_.
====================== ================================== ========= ==========================================

Inner query
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: TRSP-family.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

Points SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result Columns
-------------------------------------------------------------------------------

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
``seq``       ``INTEGER`` Row sequence.
``path_seq``  ``INTEGER`` Path sequence that indicates the relative position on the path.
``start_vid`` ``BIGINT``  Identifier of the starting vertex. When negative: is a point's identifier.
``end_vid``   ``BIGINT``  Identifier of the ending vertex. When negative: is a point's identifier.
``node``      ``BIGINT``  Identifier of the node:
                            - A positive value indicates the node is a vertex from `Edges SQL`_.
                            - A negative value indicates the node is a point from `Points SQL`_.

``edge``      ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                            - ``-1`` for the last row in the path sequence.

``cost``      ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                            - ``0`` for the last row in the path sequence.

``agg_cost``  ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Which path (if any) passes in front of point :math:`6` or vertex :math:`6` with **right** side driving
          topology.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --q2
   :end-before: --q3

:Example: Which path (if any) passes in front of point :math:`6` or vertex :math:`6` with **left** side driving
          topology.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --q3
   :end-before: --q4

:Example: From point :math:`1` and vertex :math:`2` to point :math:`3` to vertex :math:`7` on an **undirected** graph,
          with details.

.. literalinclude:: trsp_withPoints.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
