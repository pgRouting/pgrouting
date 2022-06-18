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
  (`3.4 <https://docs.pgrouting.org/3.4/en/sampledata.html>`__)
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
network.
To be able to execute the sample queries, run the following SQL commands to
create a table with a small network data set.

.. contents::
   :local:

Main graph
-------------------------------------------------------------------------------

Edges
...............................................................................

.. literalinclude:: sampledata.queries
   :start-after: --EDGE TABLE CREATE start
   :end-before: --EDGE TABLE CREATE end

Edges data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: --EDGE TABLE ADD DATA start
   :end-before: --EDGE TABLE ADD DATA end

Vertices
...............................................................................

.. literalinclude:: sampledata.queries
   :start-after: -- q1
   :end-before: -- q2

Vertices data
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- q2
   :end-before: -- q3

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

.. literalinclude:: sampledata.queries
   :start-after: -- p1
   :end-before: -- p2

Points of interest geometry
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: sampledata.queries
   :start-after: -- p2
   :end-before: -- p3

Points of interest fillup
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Using :doc:`pgr_findCloseEdges`

.. literalinclude:: sampledata.queries
   :start-after: -- p3
   :end-before: -- p4

A special case to arrive from both sides of the edge.

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

.. literalinclude:: sampledata.queries
   :start-after: -- c1
   :end-before: -- c2

.. literalinclude:: sampledata.queries
   :start-after: -- c2
   :end-before: -- c3

.. literalinclude:: sampledata.queries
   :start-after: -- c3
   :end-before: -- c4


Restrictions
...............................................................................

.. literalinclude:: sampledata.queries
   :start-after: -- r1
   :end-before: -- r2

.. literalinclude:: sampledata.queries
   :start-after: -- r2
   :end-before: -- r3

.. literalinclude:: sampledata.queries
   :start-after: -- r3
   :end-before: -- r4


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
