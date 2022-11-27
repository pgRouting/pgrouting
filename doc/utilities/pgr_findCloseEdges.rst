..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_findCloseEdges.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_findCloseEdges.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_findCloseEdges.html>`__


``pgr_findCloseEdges``
===============================================================================

``pgr_findCloseEdges`` - Finds the close edges to a point geometry.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/dijkstra_shortest_paths.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** signatures:

    * ``pgr_findCloseEdges`` (`One point`_)
    * ``pgr_findCloseEdges`` (`Many points`_)

Description
-------------------------------------------------------------------------------

``pgr_findCloseEdges`` - An utility function that finds the closest edge to a
point geometry.

* The geometries must be in the same coordinate system (have the same SRID).
* The code to do the calculations can be obtained for further specific
  adjustments needed by the application.
* ``EMTPY SET`` is returned on dryrun executions

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **point**, **tolerance**, [**options**])
   | pgr_findCloseEdges(`Edges SQL`_, **points**, **tolerance**, [**options**])
   | **options:** ``[cap, partial, dryrun]``

   | RETURNS SET OF |result-find|
   | OR EMPTY SET

.. index::
    single: findCloseEdges(One point)

One point
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **point**, **tolerance**, [**options**])
   | **options:** ``[cap, partial, dryrun]``

   | RETURNS SET OF |result-find|
   | OR EMPTY SET

:Example: With default values

* Default: ``cap => 1``

  * Maximum one row answer.
* Default: ``partial => true``

  * With less calculations as possible.
* Default: ``dryrun => false``

  * Process query
* Returns

  * values on ``edge_id``, ``fraction``, ``side`` columns.
  * ``NULL`` on ``distance``, ``geom``, ``edge`` columns.

.. literalinclude:: findCloseEdges.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
   single: findCloseEdges(One point)

Many points
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **points**, **tolerance**, [**options**])
   | **options:** ``[cap, partial, dryrun]``

   | RETURNS SET OF |result-find|
   | OR EMPTY SET

:Example: Find at most :math:`2` edges close to all vertices on the points of
          interest table.

One answer per point, as small as possible.

.. literalinclude:: findCloseEdges.queries
   :start-after: -- q2
   :end-before: -- q3

Columns ``edge_id``, ``fraction``, ``side`` and ``geom`` are returned with
values.

``geom`` contains the original point geometry to assist on deterpartialing to which
point geometry the row belongs to.

.. index::
   single: findCloseEdges(Many points)

Parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below.
   * - **point**
     - ``POINT``
     - The point geometry
   * - **points**
     - ``POINT[]``
     - An array of point geometries
   * - **tolerance**
     - ``FLOAT``
     - Max distance between geometries


Optional parameters
...............................................................................

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``cap``
     - ``INTEGER``
     - :math:`1`
     - Limit output rows
   * - ``partial``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` only columns needed for :doc:`withPoints-category` are
         calculated.
       * When ``false`` all columns are calculated
   * - ``dryrun``
     - ``BOOLEAN``
     - ``false``
     - * When ``false`` calculations are performed.
       * When ``true`` calculations are not performed and the query to do the
         calculations is exposed in a PostgreSQL ``NOTICE``.

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - **ANY-INTEGER**
     - Identifier of the edge.
   * - ``geom``
     - ``geometry``
     - The ``LINESTRING`` geometry of the edge.

Result Columns
-------------------------------------------------------------------------------

Returns set of |result-find|

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``edge_id``
     - ``BIGINT``
     - Identifier of the edge.

       * When :math:`cap = 1`, it is the closest edge.
   * - ``fraction``
     - ``FLOAT``
     - Value in <0,1> that indicates the relative postition from the first
       end-point of the edge.
   * - ``side``
     - ``CHAR``
     - Value in ``[r, l]`` indicating if the point is:

       * In the right ``r``.
       * In the left ``l``.

         * When the point is on the line it is considered to be on the right.
   * - ``distance``
     - ``FLOAT``
     - Distance from point to edge.

       * ``NULL`` when ``cap = 1`` on the `One point`_ signature
   * - ``geom``
     - ``geometry``
     - ``POINT`` geometry

       * `One Point`_: Contains the point on the edge that is ``fraction`` away
         from the starting point of the edge.
       * `Many Points`_: Contains the corresponding **original point**
   * - ``edge``
     - ``geometry``
     - ``LINESTRING`` geometry from the **original point** to the closest point
       of the edge with identifier ``edge_id``


.. rubric:: One point results

* The green nodes is the **original point**
* The geometry ``geom`` is a point on the :math:`sp \rightarrow ep` edge.
* The geometry ``edge`` is a line that connects the **original point** with
  ``geom``

.. graphviz::

   digraph G {
     splines=false;
     subgraph cluster0 {
       point [shape=circle;style=filled;color=green];
       geom [shape=point;color=black;size=0];
       sp, ep;

       edge[weight=0];
       point -> geom [dir=none; penwidth=0, color=red];
       edge[weight=2];
       sp -> geom -> ep [dir=none;penwidth=3 ];

       {rank=same; point, geom}
     }

     subgraph cluster1 {
       point1 [shape=circle;style=filled;color=green;label=point];
       geom1 [shape=point;color=deepskyblue; xlabel="geom"; width=0.3];
       sp1 [label=sp]; ep1 [label=ep];

       edge[weight=0];
       point1 -> geom1 [weight=0, penwidth=3, color=red,
                      label="edge"];
       edge[weight=2];
       sp1 -> geom1 -> ep1 [dir=none;weight=1, penwidth=3 ];


       geom1 -> point1 [dir=none;weight=0, penwidth=0, color=red];
       {rank=same; point1, geom1}
     }
   }

.. rubric:: Many point results

* The green nodes are the **original points**
* The geometry ``geom``, marked as **g1** and **g2**  are the **original
  points**
* The geometry ``edge``, marked as **edge1** and **edge2** is a line that
  connects the **original point** with the closest point on the :math:`sp
  \rightarrow ep` edge.

.. graphviz::

   digraph G {
     splines = false;
     subgraph cluster0 {
        p1 [shape=circle;style=filled;color=green];
        g1 [shape=point;color=black;size=0];
        g2 [shape=point;color=black;size=0];
        sp, ep;
        p2 [shape=circle;style=filled;color=green];

        sp -> g1 [dir=none;weight=1, penwidth=3 ];
        g1 -> g2 [dir=none;weight=1, penwidth=3 ];
        g2 -> ep [weight=1, penwidth=3 ];

        g2 -> p2 [dir=none;weight=0, penwidth=0, color=red, partiallen=3];
        p1 -> g1 [dir=none;weight=0, penwidth=0, color=red, partiallen=3];
        p1 -> {g1, g2} [dir=none;weight=0, penwidth=0, color=red;]

        {rank=same; p1; g1}
        {rank=same; p2; g2}
     }
     subgraph cluster1 {
        p3 [shape=circle;style=filled;color=deepskyblue;label=g1];
        g3 [shape=point;color=black;size=0];
        g4 [shape=point;color=black;size=0];
        sp1 [label=sp]; ep1 [label=ep];
        p4 [shape=circle;style=filled;color=deepskyblue;label=g2];

        sp1 -> g3 [dir=none;weight=1, penwidth=3 ];
        g3 -> g4 [dir=none;weight=1, penwidth=3,len=10];
        g4 -> ep1 [weight=1, penwidth=3, len=10];

        g4 -> p4 [dir=back;weight=0, penwidth=3, color=red, partiallen=3,
                       label="edge2"];
        p3 -> g3 [weight=0, penwidth=3, color=red, partiallen=3,
                       label="edge1"];
        p3 -> {g3, g4} [dir=none;weight=0, penwidth=0, color=red];

        {rank=same; p3; g3}
        {rank=same; p4; g4}
     }
   }


Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

One point examples
...............................................................................

At most two answers
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* ``cap => 2``

  * Maximum two row answer.
* Default: ``partial => true``

  * With less calculations as possible.
* Default: ``dryrun => false``

  * Process query

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o1
   :end-before: -- o2

.. rubric:: Understanding the result

* ``NULL`` on ``geom``, ``edge``
* ``edge_id`` identifier of the edge close to the **original point**

  * Two edges are withing :math:`0.5` distance units from the **original
    point**: :math:`{5, 8}`
* For edge :math:`5`:

  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.8` fraction of the edge :math:`5`.
  * ``side``: The **original point** is located to the left side of edge
    :math:`5`.
  * ``distance``: The **original point** is located :math:`0.1` length units
    from edge :math:`5`.
* For edge :math:`8`:

  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.89..` fraction of the edge :math:`8`.
  * ``side``: The **original point** is located to the right side of edge
    :math:`8`.
  * ``distance``: The **original point** is located :math:`0.19..` length units
    from edge :math:`8`.

One answer, all columns
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Default: ``cap => 1``

  * Maximum one row answer.
* ``partial => false``

  * Calculate all columns
* Default: ``dryrun => false``

  * Process query

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o2
   :end-before: -- o3

.. rubric:: Understanding the result

* ``edge_id`` identifier of the edge **closest** to the **original point**

  * From all edges within :math:`0.5` distance units from the **original
    point**: :math:`{5}` is the closest one.
* For edge :math:`5`:

  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.8` fraction of the edge :math:`5`.
  * ``side``: The **original point** is located to the left side of edge
    :math:`5`.
  * ``distance``: The **original point** is located :math:`0.1` length units
    from edge :math:`5`.
  * ``geom``: Contains the geometry of the closest point on edge :math:`5` from
    the **original point**.
  * ``edge``: Contains the ``LINESTRING`` geometry of the **original point** to
    the closest point on on edge :math:`5` ``geom``

At most two answers with all columns
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* ``cap => 2``

  * Maximum two row answer.
* ``partial => false``

  * Calculate all columns
* Default: ``dryrun => false``

  * Process query

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o3
   :end-before: -- o4

.. rubric:: Understanding the result:

* ``edge_id`` identifier of the edge close to the **original point**

  * Two edges are withing :math:`0.5` distance units from the **original
    point**: :math:`{5, 8}`
* For edge :math:`5`:

  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.8` fraction of the edge :math:`5`.
  * ``side``: The **original point** is located to the left side of edge
    :math:`5`.
  * ``distance``: The **original point** is located :math:`0.1` length units
    from edge :math:`5`.
  * ``geom``: Contains the geometry of the closest point on edge :math:`5` from
    the **original point**.
  * ``edge``: Contains the ``LINESTRING`` geometry of the **original point** to
    the closest point on on edge :math:`5` ``geom``
* For edge :math:`8`:

  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.89..` fraction of the edge :math:`8`.
  * ``side``: The **original point** is located to the right side of edge
    :math:`8`.
  * ``distance``: The **original point** is located :math:`0.19..` length units
    from edge :math:`8`.
  * ``geom``: Contains the geometry of the closest point on edge :math:`8` from
    the **original point**.
  * ``edge``: Contains the ``LINESTRING`` geometry of the **original point** to
    the closest point on on edge :math:`8` ``geom``

One point dry run execution
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Returns ``EMPTY SET``.


* ``partial => true``

  * Is ignored
  * Because it is a **dry run** excecution, the code for all calculations are
    shown on the PostgreSQL ``NOTICE``.
* ``dryrun => true``

  * Do not process query
  * Generate a PostgreSQL ``NOTICE`` with the code used to calculate all columns

    * ``cap`` and **original point** are used in the code

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o5
   :end-before: -- o6

Many points examples
...............................................................................

At most two answers per point
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* ``cap => 2``

  * Maximum two row answer.
* Default: ``partial => true``

  * With less calculations as possible.
* Default: ``dryrun => false``

  * Process query

.. literalinclude:: findCloseEdges.queries
   :start-after: -- m1
   :end-before: -- m2

.. rubric:: Understanding the result

* ``NULL`` on ``edge``
* ``edge_id`` identifier of the edge close to a **original point** (``geom``)

  * Two edges at most withing :math:`0.5` distance units from each of the
    **original points**:

    * For ``POINT(1.8 0.4)`` and ``POINT(0.3 1.8)`` only one edge was found.
    * For the rest of the points two edges were found.
* For point ``POINT(2.9 1.8)``

  * Edge :math:`5` is before :math:`8` therefore edge :math:`5` has the shortest
    distance to ``POINT(2.9 1.8)``.
  * For edge :math:`5`:

    * ``fraction``: The closest point from the **original point** is at the
      :math:`0.8` fraction of the edge :math:`5`.
    * ``side``: The **original point** is located to the left side of edge
      :math:`5`.
    * ``distance``: The **original point** is located :math:`0.1` length units
      from edge :math:`5`.
  * For edge :math:`8`:

    * ``fraction``: The closest point from the **original point** is at the
      :math:`0.89..` fraction of the edge :math:`8`.
    * ``side``: The **original point** is located to the right side of edge
      :math:`8`.
    * ``distance``: The **original point** is located :math:`0.19..` length
      units from edge :math:`8`.

One answer per point, all columns
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Default: ``cap => 1``

  * Maximum one row answer.
* ``partial => false``

  * Calculate all columns
* Default: ``dryrun => false``

  * Process query

.. literalinclude:: findCloseEdges.queries
   :start-after: -- m2
   :end-before: -- m3

.. rubric:: Understanding the result

* ``edge_id`` identifier of the edge **closest** to the **original point**

  * From all edges within :math:`0.5` distance units from the **original
    point**: :math:`{5}` is the closest one.
* For the **original point** ``POINT(2.9 1.8)``

  * Edge :math:`5` is the closest edge to the **original point**
  * ``fraction``: The closest point from the **original point** is at the
    :math:`0.8` fraction of the edge :math:`5`.
  * ``side``: The **original point** is located to the left side of edge
    :math:`5`.
  * ``distance``: The **original point** is located :math:`0.1` length units
    from edge :math:`5`.
  * ``geom``: Contains the geometry of the **original point** ``POINT(2.9 1.8)``
  * ``edge``: Contains the ``LINESTRING`` geometry of the **original point**
    (``geom``) to the closest point on on edge.

Many points dry run execution
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

* Returns ``EMPTY SET``.


* ``partial => true``

  * Is ignored
  * Because it is a **dry run** excecution, the code for all calculations are
    shown on the PostgreSQL ``NOTICE``.
* ``dryrun => true``

  * Do not process query
  * Generate a PostgreSQL ``NOTICE`` with the code used to calculate all columns

    * ``cap`` and **original point** are used in the code

.. literalinclude:: findCloseEdges.queries
   :start-after: -- m4
   :end-before: -- m5

Find at most two routes to a given point
...............................................................................

Using :doc:`pgr_withPoints`

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o4
   :end-before: -- o5

A point of interest table
...............................................................................

Handling points outside the graph.

.. include:: sampledata.rst
   :start-after: pois_start
   :end-before: pois_end

Connecting disconnected components
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: connecting_graph_start
    :end-before: connecting_graph_end


See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
