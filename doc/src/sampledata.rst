..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/sampledata.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/sampledata.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/sampledata.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/sampledata.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/sampledata.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/sampledata.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/sampledata.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/sampledata.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/sampledata.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/sampledata.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/doc/src/developer/sampledata.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/doc/src/developer/sampledata.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/doc/src/developer/sampledata.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/doc/src/developer/sampledata.html>`__

Sample Data
===============================================================================

The documentation provides very simple example queries based on a small sample
network that resembles a city.
To be able to execute the mayority of the examples queries, follow the
instructions bellow.

.. contents::
   :local:

Main graph
-------------------------------------------------------------------------------

A graph consists of a set of edges and a set of vertices.

The following city is to be inserted into the database:

.. figure:: /images/Fig1-originalData.png

Information known at this point is the geometry of the edges, cost values,
cpacity values, category values and some locations that are not in the graph.

The process to have working topology starts by inserting the edges.
After that everything else is calculated.

Edges
...............................................................................

The database design for the documentation of pgRouting, keeps in the same row 2
segments, one in the direction of the geometry and the second in the oposite
direction. Therfore some information has the ``reverse_`` prefix which
corresponds to the segment on the oposite direction of the geometry.

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Description
   * - ``id``
     - A unique identifier.
   * - ``source``
     - Identifier of the starting vertex of the geometry ``geom``.
   * - ``target``
     - Identifier of the ending vertex of the geometry ``geom``
   * - ``cost``
     - Cost to traverse from `source` to ``target``.
   * - ``reverse_cost``
     - Cost to traverse from `target` to ``source``.
   * - ``capacity``
     - Flow capacity from `source` to ``target``.
   * - ``reverse_capacity``
     - Flow capacity from `target` to ``source``.
   * - ``category``
     - Flow capacity from `target` to ``source``.
   * - ``reverse_category``
     - Flow capacity from `target` to ``source``.
   * - ``x1``
     - :math:`x` coordinate of the starting vertex of the geometry.

       - For convinience it is saved on the table but can be calculated as
         ``ST_X(ST_StartPoint(geom))``.
   * - ``y2``
     - :math:`y` coordinate of the ending vertex of the geometry.

       - For convinience it is saved on the table but can be calculated as
         ``ST_Y(ST_EndPoint(geom))``.
   * - ``geom``
     - The geometry of the segments.

.. literalinclude:: sampledata.queries
   :start-after: --EDGE TABLE CREATE start
   :end-before: --EDGE TABLE CREATE end

Starting on  PostgreSQL 12::

   ...
   x1 FLOAT GENERATED ALWAYS AS (ST_X(ST_StartPoint(geom))) STORED,
   y1 FLOAT GENERATED ALWAYS AS (ST_Y(ST_StartPoint(geom))) STORED,
   x1 FLOAT GENERATED ALWAYS AS (ST_X(ST_EndPoint(geom))) STORED,
   y1 FLOAT GENERATED ALWAYS AS (ST_Y(ST_EndPoint(geom))) STORED,
   ...

Optionally indexes on different columns can be created.
The recomendation is to have

* ``id`` indexed.
* ``source`` and ``target`` columns indexed to speed up pgRouting queries.
* ``geom`` indexed to speed up gemetry processes that might be needed in the
  front end.

For this small example the indexes are skipped, except for ``id``

Edges data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Inserting into the database the information of the edges:

.. literalinclude:: sampledata.queries
   :start-after: --EDGE TABLE ADD DATA start
   :end-before: --EDGE TABLE ADD DATA end

Negative values on the cost, capacity and category means that the edge do not
exist.

Vertices
...............................................................................

The vertex information is calculated based on the identifier of the edge and the
geometry and saved on a table.
Saving all the information provided by :doc:`pgr_extractVertices`:

.. literalinclude:: sampledata.queries
   :start-after: -- q1
   :end-before: -- q1-1

In this case the because the ``CREATE`` statement was not used, the definition
of an index on the table is needed.

.. literalinclude:: sampledata.queries
   :start-after: -- q1-1
   :end-before: Table "public

The structure of the table is:

.. literalinclude:: sampledata.queries
   :start-after: (1 row)
   :end-before: -- q1-2

Vertices data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The saved information of the vertices is:

.. literalinclude:: sampledata.queries
   :start-after: -- q2
   :end-before: -- q3

Here is where adding more columns to the vertices table can be done.
Additional columns names and types will depend on the application.

The topology
...............................................................................

This queries based on the vertices data create a topology by filling the
``source`` and ``target`` columns in the edges table.

.. literalinclude:: sampledata.queries
   :start-after: -- q3
   :end-before: -- q4

Topology data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- q4
   :end-before: -- q5

Points outside the graph
...............................................................................

.. pois_start

Points of interest
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Some times the applications work "on the fly" starting from a location that is
not a vertex in the graph.
Those locations, in pgRrouting are called points of interest.

The information needed in the points of interest is ``pid``, ``edge_id``,
``side``, ``fraction``.

On this documentation there will be some 6 fixed points of interest and they
will be stored on a table.

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Description
   * - ``pid``
     - A unique identifier.
   * - ``edge_id``
     - Identifier of the edge nearest edge that allows an arrival to the point.
   * - ``side``
     - Is it on the left, right or both sides of the segment ``edge_id``
   * - ``fraction``
     - Where in the segment is the point located.
   * - ``geom``
     - The geometry of the points.
   * - ``newPoint``
     - The geometry of the points moved on top of the segment.

.. literalinclude:: sampledata.queries
   :start-after: -- p1
   :end-before: -- p2

Points of interest geometry
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Inserting the data of the points of interest:

.. literalinclude:: sampledata.queries
   :start-after: -- p2
   :end-before: -- p3

Points of interest fillup
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Using :doc:`pgr_findCloseEdges`
Calculating for visual purposes the points over the graph.

.. literalinclude:: sampledata.queries
   :start-after: -- p3
   :end-before: -- p4

A special case to arrive from both sides of the edge.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- p4
   :end-before: -- p5

Points of interest data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- p5
   :end-before: -- p6

.. pois_end

Support tables
-------------------------------------------------------------------------------

Combinations
...............................................................................

Many functions can be used with a combinations of ``(source, target)`` pairs
when wanting a route from ``source`` to ``target``.

For convinence of this documentations, some combinations will be stored on a
table:

.. literalinclude:: sampledata.queries
   :start-after: -- c1
   :end-before: -- c2

Inserting the data:

.. literalinclude:: sampledata.queries
   :start-after: -- c2
   :end-before: -- c3

Combinations data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- c3
   :end-before: -- c4


Restrictions
...............................................................................

Some functions accept soft restrictions about the segments.

.. the_restrictions_start

The creation of the restrictions table

.. literalinclude:: sampledata.queries
   :start-after: -- r1
   :end-before: -- r2

Adding the restrictions

.. literalinclude:: sampledata.queries
   :start-after: -- r2
   :end-before: -- r3

Restrictions data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- r3
   :end-before: -- r4

.. the_restrictions_end

Images
-------------------------------------------------------------------------------

* Red arrows correspond when ``cost`` > 0 in the edge table.
* Blue arrows correspond when ``reverse_cost`` > 0 in the edge table.
* Points are outside the graph.
* Click on the graph to enlarge.

.. _fig1:

Directed graph with ``cost`` and ``reverse_cost``
...............................................................................

When working with city networks, this is recommended for point of view of
vehicles.

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

   Directed, with cost and reverse_cost

.. _fig2:

Undirected graph with ``cost`` and ``reverse_cost``
...............................................................................

When working with city networks, this is recommended for point of view of
pedestrians.

.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   Undirected, with cost and reverse cost

.. _fig3:

Directed graph with ``cost``
...............................................................................

.. figure:: /images/Fig2-cost.png
   :scale: 20%

   Directed, with cost

.. _fig4:

Undirected graph with ``cost``
...............................................................................

.. figure:: /images/Fig4-costUndirected.png
   :scale: 20%

   Undirected, with cost


.. _pickdeliverdata:


Pick & Deliver Data
-------------------------------------------------------------------------------

.. pd_data_start

This data example **lc101** is from data published at
https://www.sintef.no/projectweb/top/pdptw/li-lim-benchmark/

The vehicles
...............................................................................

There are 25 vehciles in the problem all with the same characteristics.

.. literalinclude:: lc101.queries
   :start-after: -- q1
   :end-before: -- q2

The original orders
...............................................................................

The data comes in different rows for the pickup and the delivery of the same
order.

.. literalinclude:: lc101.queries
   :start-after: -- q2
   :end-before: -- q3

The orders
...............................................................................

The original data needs to be converted to an appropiate table:

.. literalinclude:: lc101.queries
   :start-after: -- q3
   :end-before: -- q4

.. pd_data_end
