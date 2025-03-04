..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Utilities ; pgr_findCloseEdges
   single: findCloseEdges

|

``pgr_findCloseEdges``
===============================================================================

``pgr_findCloseEdges`` - Finds the close edges to a point geometry.

.. rubric:: Availability

.. rubric:: Version 3.8.0

* Error messages adjustment.
* ``partial`` option is removed.
* Function promoted to official.

.. rubric:: Version 3.4.0

* New proposed function.

Description
-------------------------------------------------------------------------------

``pgr_findCloseEdges`` - An utility function that finds the closest edge to a
point geometry.

* The geometries must be in the same coordinate system (have the same SRID).
* The code to do the calculations can be obtained for further specific
  adjustments needed by the application.
* ``EMPTY SET`` is returned on dryrun executions

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **point**, **tolerance**, [**options**])
   | pgr_findCloseEdges(`Edges SQL`_, **points**, **tolerance**, [**options**])
   | **options:** ``[cap, dryrun]``

   | Returns set of |result-find|
   | OR EMPTY SET

.. index::
    single: findCloseEdges ; One point

One point
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **point**, **tolerance**, [**options**])
   | **options:** ``[cap, dryrun]``

   | Returns set of |result-find|
   | OR EMPTY SET

:Example: Get two close edges to points of interest with :math:`pid = 5`

* ``cap => 2``

.. literalinclude:: findCloseEdges.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
   single: findCloseEdges ; Many points

Many points
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_findCloseEdges(`Edges SQL`_, **points**, **tolerance**, [**options**])
   | **options:** ``[cap, dryrun]``

   | Returns set of |result-find|
   | OR EMPTY SET

:Example: For each points of interests, find the closest edge.

.. literalinclude:: findCloseEdges.queries
   :start-after: -- q2
   :end-before: -- q3

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

Result columns
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
     - Value in <0,1> that indicates the relative position from the first
       end-point of the edge.
   * - ``side``
     - ``CHAR``
     - Value in ``[r, l]`` indicating if the point is:

       * At the right ``r`` of the segment.

         * When the point is on the line it is considered to be on the right.
       * At the left ``l`` of the segment.
   * - ``distance``
     - ``FLOAT``
     - Distance from the point to the edge.
   * - ``geom``
     - ``geometry``
     - Original ``POINT`` geometry.
   * - ``edge``
     - ``geometry``
     - ``LINESTRING`` geometry that connects the original **point** to the
       closest point of the edge with identifier ``edge_id``


Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

One point in an edge
...............................................................................

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o0
   :end-before: -- o1

.. graphviz::

   digraph D {
     subgraph cluster0 {
       label="data";
       point [shape=circle;style=filled;color=green;fontsize=8;width=0.3;fixedsize=true];
       point [pos="0,1.5!"]

       sp, ep [shape=circle;fontsize=8;width=0.3;fixedsize=true];
       sp[pos="2,0!"]
       ep[pos="2,3!"]

       sp -> ep:s
     }
     subgraph cluster1 {
       label="results";
       geom [shape=circle;style=filled;color=green;fontsize=8;width=0.3;fixedsize=true];
       geom [pos="3,1.5!"]

       np11 [shape=point;color=black;size=0;fontsize=8;fixedsize=true];
       np11 [pos="5,1.5!";xlabel="fraction=0.5"];

       sp1, ep1 [shape=circle;fontsize=8;width=0.3;fixedsize=true];
       sp1[pos="5,0!"]
       ep1[pos="5,3!"]

       sp1:n ->  np11:s [dir=none]
       np11:n -> ep1:s

       geom -> np11 [color=red,label="edge"];
     }
   }

* The green node is the original point.
* ``geom`` has the value of the original point.
* The geometry ``edge`` is a line that connects the original point with the edge
  :math:`sp \rightarrow ep` edge.
* The point is located at the left of the edge.

One point dry run execution
...............................................................................

Using the query from the previous example:

* Returns ``EMPTY SET``.
* ``dryrun => true``

  * Generates a PostgreSQL ``NOTICE`` with the code used.
  * The generated code can be used as a starting base code for additional
    requirements, like taking into consideration the SRID.

.. literalinclude:: findCloseEdges.queries
   :start-after: -- o5
   :end-before: -- o6

Many points in an edge
...............................................................................

* The green nodes are the **original points**
* The geometry ``geom``, marked as **g1** and **g2** are the **original
  points**
* The geometry ``edge``, marked as **edge1** and **edge2** is a line that
  connects the **original point** with the closest point on the :math:`sp
  \rightarrow ep` edge.

.. literalinclude:: findCloseEdges.queries
   :start-after: -- m0
   :end-before: -- m1

.. graphviz::

   digraph G {
     subgraph cluster0 {
       p1,p2 [shape=circle;style=filled;color=green;fontsize=8;width=0.3;fixedsize=true];
       p1 [pos="0,2!"]
       p2 [pos="3,1!"]

       sp, ep [shape=circle;fontsize=8;width=0.3;fixedsize=true];
       sp[pos="1,0!"]
       ep[pos="1,3!"]

       sp -> ep:s
     }

     subgraph cluster1 {
       g1, g2 [shape=circle;style=filled;color=green;fontsize=8;width=0.3;fixedsize=true];
       g1 [pos="4,2!"]
       g2 [pos="7,1!"]

       np11,np21 [shape=point;color=black;size=0;fontsize=8;fixedsize=true];
       np11 [pos="5,2!";xlabel="fraction=0.63"];
       np21 [pos="5,1!";xlabel="fraction=0.33"];

       sp1, ep1 [shape=circle;fontsize=8;width=0.3;fixedsize=true];
       sp1[pos="5,0!"]
       ep1[pos="5,3!"]

       sp1:n ->  np21:s [dir=none]
       np21:n ->  np11:s [dir=none]
       np11:n -> ep1:s

       g1 -> np11 [color=red;label="edge"];
       g2 -> np21 [color=red;label="edge"]
     }
   }

Many points dry run execution
...............................................................................

* Returns ``EMPTY SET``.
* ``dryrun => true``

  * Do not process query
  * Generate a PostgreSQL ``NOTICE`` with the code used to calculate all columns

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

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
