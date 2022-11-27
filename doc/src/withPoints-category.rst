..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
    ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/withPoints-category.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/withPoints-category.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/withPoints-category.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/withPoints-category.html>`__

withPoints - Category
===============================================================================

When points are added to the graph.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. proposed start

- :doc:`withPoints-family` - Functions based on Dijkstra algorithm.
- From the :doc:`TRSP-family`:

  - :doc:`pgr_trsp_withPoints` - Vertex/Point routing with restrictions.
  - :doc:`pgr_trspVia_withPoints` - Via Vertex/point routing with restrictions.

.. proposed end

Introduction
-------------------------------------------------------------------------------

The **with points** category modifies the graph on the fly by adding points on
edges as required by the `Points SQL`_ query.

The functions within this category give the ability to process between
arbitrary points located outside the original graph.

This category of functions was thought for routing vehicles, but might as well
work for some other application not involving vehicles.

When given a point identifier ``pid`` that its being mapped to an edge with an
identifier ``edge_id``, with a fraction from the source to the target along the
edge ``fraction`` and some additional information about which side of the edge
the point is on ``side``, then processing from arbitrary points can be done on
fixed networks.

All this functions consider as many traits from the "real world" as possible:

- Kind of graph:

  - **directed** graph
  - **undirected** graph

- Arriving at the point:

  - Compulsory arrival on the side of the segment where the point is located.
  - On either side of the segment.

- Countries with:

  - **Right** side driving
  - **Left** side driving

- Some points are:

  - **Permanent**: for example the set of points of clients stored in a table in
    the data base.

    - The graph has been modified to permanently have those points as vertices.
    - There is a table on the database that describes the points

  - **Temporal**: for example points given through a web application

    - Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.

- The numbering of the points are handled with negative sign.

  - This sign change is to avoid confusion when there is a vertex with the same
    identifier as the point identifier.
  - Original point identifiers are to be positive.
  - Transformation to negative is done internally.
  - Interpretation of the sign on the node information of the output

    - positive sign is a vertex of the original graph
    - negative sign is a point of the `Points SQL`_

Parameters
-------------------------------------------------------------------------------

.. withPoints_parameters_start

.. list-table::
   :width: 81
   :widths: 14 14 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below
   * - `Points SQL`_
     - ``TEXT``
     - `Points SQL`_ as described below
   * - `Combinations SQL`_
     - ``TEXT``
     - `Combinations SQL`_ as described below
   * - **start vid**
     - ``BIGINT``
     - Identifier of the starting vertex of the path. Negative value is for
       point’s identifier.
   * - **start vids**
     - ``ARRAY[BIGINT]``
     - Array of identifiers of starting vertices. Negative values are for
       point’s identifiers.
   * - **end vid**
     - ``BIGINT``
     - Identifier of the ending vertex of the path. Negative value is for
       point’s identifier.
   * - **end vids**
     - ``ARRAY[BIGINT]``
     - Array of identifiers of ending vertices. Negative values are for point’s
       identifiers.

.. withPoints_parameters_end

Optional parameters
...............................................................................

.. withPoints_optionals_start

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

.. withPoints_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. points_sql_start

.. list-table::
   :width: 81
   :widths: 12 14 7 44
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``pid``
     - **ANY-INTEGER**
     - **value**
     - Identifier of the point.

       * Use with positive value, as internally will be converted to negative
         value
       * If column is present, it can not be NULL.
       * If column is not present, a sequential negative **value** will be given
         automatically.
   * - ``edge_id``
     - **ANY-INTEGER**
     -
     - Identifier of the "closest" edge to the point.
   * - ``fraction``
     - **ANY-NUMERICAL**
     -
     - Value in <0,1> that indicates the relative postition from the first end
       point of the edge.
   * - ``side``
     - ``CHAR``
     - ``b``
     - Value in [``b``, ``r``, ``l``, ``NULL``] indicating if the point is:

       * In the right ``r``,
       * In the left ``l``,
       * In both sides ``b``, ``NULL``

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. points_sql_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Advanced documentation
-------------------------------------------------------------------------------

.. advanced_documentation_start

.. contents:: Contents
   :local:

About points
...............................................................................

For this section the following city (see :doc:`sampledata`) some interesing
points such as restaurant, supermarket, post office, etc. will be used as
example.

.. figure:: /images/Fig1-originalData.png
   :scale: 50%

- The graph is **directed**
- Red arrows show the ``(source, target)`` of the edge on the edge table
- Blue arrows show the ``(target, source)`` of the edge on the edge table
- Each point location shows where it is located with relation of the edge
  ``(source, target)``

  - On the right for points **2** and **4**.
  - On the left for points **1**, **3** and **5**.
  - On both sides for point **6**.

The representation on the data base follows the `Points SQL`_ description, and
for this example:

.. literalinclude:: withPoints-category.queries
   :start-after: --q1
   :end-before: --q2

Driving side
...............................................................................

In the the folowwing images:

- The squared vertices are the temporary vertices,
- The temporary vertices are added according to the driving side,
- visually showing the differences on how depending on the driving side the data
  is interpreted.

Right driving side
_______________________________________________________________________________

.. image:: images/rightDrivingSide.png
    :scale: 50%

- Point **1** located on edge ``(6, 5)``
- Point **2** located on edge ``(16, 17)``
- Point **3** located on edge ``(8, 12)``
- Point **4** located on edge ``(1, 3)``
- Point **5** located on edge ``(10, 11)``
- Point **6** located on edges ``(6, 7)`` and ``(7, 6)``

Left driving side
_______________________________________________________________________________

.. image:: images/leftDrivingSide.png
    :scale: 50%

- Point **1** located on edge ``(5, 6)``
- Point **2** located on edge ``(17, 16)``
- Point **3** located on edge ``(8, 12)``
- Point **4** located on edge ``(3, 1)``
- Point **5** located on edge ``(10, 11)``
- Point **6** located on edges ``(6, 7)`` and ``(7, 6)``

Driving side does not matter
_______________________________________________________________________________

- Like having all points to be considered in both sides ``b``
- Prefered usage on **undirected** graphs
- On the :doc:`TRSP-family` this option is not valid

.. image:: images/noMatterDrivingSide.png
    :scale: 50%

- Point **1** located on edge ``(5, 6)`` and ``(6, 5)``
- Point **2** located on edge ``(17, 16)``and ``16, 17``
- Point **3** located on edge ``(8, 12)``
- Point **4** located on edge ``(3, 1)`` and ``(1, 3)``
- Point **5** located on edge ``(10, 11)``
- Point **6** located on edges ``(6, 7)`` and ``(7, 6)``

Creating temporary vertices
...............................................................................

This section will demonstrate how a temporary vertex is created internally on
the graph.

.. rubric:: Problem

For edge:

.. literalinclude:: withPoints-category.queries
   :start-after: --q2
   :end-before: --q3

insert point:

.. literalinclude:: withPoints-category.queries
   :start-after: --q3
   :end-before: --q4

On a right hand side driving network
_______________________________________________________________________________

.. rubric:: Right driving side

.. image:: images/rightDrivingSide.png
    :scale: 50%

- Arrival to point ``-2`` can be achived only via vertex **16**.
- Does not affects edge ``(17, 16)``, therefore the edge is kept.
- It only affects the edge ``(16, 17)``, therefore the edge is removed.
- Create two new edges:

  - Edge ``(16, -2)`` with cost ``0.4`` (original cost * fraction == :math:`1 *
    0.4`)
  - Edge ``(-2, 17)`` with cost ``0.6`` (the remaing cost)

- The total cost of the additional edges is equal to the original cost.
- If more points are on the same edge, the process is repeated recursevly.

On a left hand side driving network
_______________________________________________________________________________

.. rubric:: Left driving side

.. image:: images/leftDrivingSide.png
    :scale: 50%

- Arrival to point ``-2`` can be achived only via vertex **17**.
- Does not affects edge ``(16, 17)``, therefore the edge is kept.
- It only affects the edge ``(17, 16)``, therefore the edge is removed.
- Create two new edges:

  - Work with the original edge ``(16, 17)`` as the fraction is a fraction of the
    original:

    - Edge ``(16, -2)`` with cost ``0.4`` (original cost * fraction == :math:`1 *
      0.4`)
    - Edge ``(-2, 17)`` with cost ``0.6`` (the remaing cost)
    - If more points are on the same edge, the process is repeated recursevly.

  - Flip the Edges and add them to the graph:

    - Edge ``(17, -2)`` becomes ``(-2, 16)`` with cost ``0.4`` and is added to the
      graph.
    - Edge ``(-2, 16)`` becomes ``(17, -2)`` with cost ``0.6`` and is added to
      the graph.

- The total cost of the additional edges is equal to the original cost.

When driving side does not matter
_______________________________________________________________________________

.. image:: images/noMatterDrivingSide.png
    :scale: 50%

- Arrival to point ``-2`` can be achived via vertices **16** or **17**.
- Affects the edges ``(16, 17)`` and ``(17, 16)``, therefore the edges are removed.
- Create four new edges:

  - Work with the original edge ``(16, 17)`` as the fraction is a fraction of the
    original:

    - Edge ``(16, -2)`` with cost ``0.4`` (original cost * fraction == :math:`1 *
      0.4`)
    - Edge ``(-2, 17)`` with cost ``0.6`` (the remaing cost)
    - If more points are on the same edge, the process is repeated recursevly.

  - Flip the Edges and add all the edges to the graph:

    - Edge ``(16, -2)`` is added to the graph.
    - Edge ``(-2, 17)`` is added to the graph.
    - Edge ``(16, -2)`` becomes ``(-2, 16)`` with cost ``0.4`` and is added to the
      graph.
    - Edge ``(-2, 17)`` becomes ``(17, -2)`` with cost ``0.6`` and is added to
      the graph.

.. advanced_documentation_end

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
