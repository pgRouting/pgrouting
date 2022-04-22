..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgRouting-concepts.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgRouting-concepts.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgRouting-concepts.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgRouting-concepts.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgRouting-concepts.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgRouting-concepts.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgRouting-concepts.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgRouting-concepts.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/doc/src/tutorial/index.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/doc/src/tutorial/index.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/doc/src/tutorial/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/doc/src/tutorial/index.html>`__

|

pgRouting Concepts
===============================================================================

.. contents::

|

Getting Started
-------------------------------------------------------------------------------

This is a simple guide to walk you through the steps of getting started
with pgRouting. In this guide we will cover:

.. contents::
    :local:


|

Create a routing Database
...............................................................................

The first thing we need to do is create a database and load pgrouting in
the database. Typically you will create a database for each project. Once
you have a database to work in, your can load your data and build your
application in that database. This makes it easy to move your project
later if you want to to say a production server.

For Postgresql 9.2 and later versions

.. code-block:: bash

	createdb mydatabase
	psql mydatabase -c "create extension postgis"
	psql mydatabase -c "create extension pgrouting"

|

Load Data
...............................................................................

There are several ways to load your data into pgRouting. The most direct way
is to load an Open Street Maps (OSM) dataset using **osm2pgrouting**. This is a
tool, integrated in pgRouting project, that loads OSM data into postgresql
with pgRouting requirements, including data structure and routing topology.

If you have other requirements, you can try various OpenSource tools that can
help you, like:

:shp2pgsql: - this is the postgresql shapefile loader
:ogr2ogr: - this is a vector data conversion utility
:osm2pgsql: - this is a tool for loading OSM data into postgresql

Please note that these tools will not import the data in a structure compatible
with pgRouting and you might need to adapt it.

These tools and probably others will allow you to read vector data so that
you may then load that data into your database as a table of some kind. At this
point you need to know a little about your data structure and content. One easy
way to browse your new data table is with pgAdmin or phpPgAdmin.

|

Build a Routing Topology
...............................................................................

.. note:: this step is not needed if data is loaded with `osm2pgrouting`

Next we need to build a topology for our street data. What this means is that
for any given edge in your street data the ends of that edge will be connected
to a unique node and to other edges that are also connected to that same unique
node. Once all the edges are connected to nodes we have a graph that can be
used for routing with pgrouting. We provide a tool that will help with this:

.. code-block:: sql

    select pgr_createTopology('myroads', 0.000001);

where you should replace 'myroads' with the name of your table storing the edges.

* :doc:`pgr_createTopology`

Check the Routing Topology
...............................................................................

There are lots of possible sources for errors in a graph. The data that you
started with may not have been designed with routing in mind. A graph has some
very specific requirements. One is that it is *NODED*, this means that except
for some very specific use cases, each road segment starts and ends at a node
and that in general is does not cross another road segment that it should be
connected to.

There can be other errors like the direction of a one-way street being entered
in the wrong direction. We do not have tools to search for all possible errors
but we have some basic tools that might help.

.. code-block:: sql

    select pgr_analyzegraph('myroads', 0.000001);
    select pgr_analyzeoneway('myroads',  s_in_rules, s_out_rules,
                                         t_in_rules, t_out_rules
                                         direction)
    select pgr_nodeNetwork('myroads', 0.001);

where you should replace 'myroads' with the name of your table storing the edges
('ways', in case you used osm2pgrouting to import the data).

* :doc:`pgr_analyzeGraph`
* :doc:`pgr_analyzeOneWay`
* :doc:`pgr_nodeNetwork`

Compute a Path
...............................................................................

Once you have all the preparation work done above, computing a route is fairly easy.
We have a lot of different algorithms that can work with your prepared road
network. The general form of a route query using Dijkstra algorithm is:

.. code-block:: none

    select pgr_dijkstra(`SELECT * FROM myroads', <start>, <end>)


This algorithm only requires *id*, *source*, *target* and *cost* as the minimal attributes, that by
default will be considered to be columns in your roads table. If the column names in your
roads table do not match exactly the names of these attributes, you can use aliases. For example,
if you imported OSM data using **osm2pgrouting**, your id column's name would be *gid* and your
roads table would be *ways*, so you would query a route from node id 1 to node id 2 by typing:

.. code-block:: none

    select pgr_dijkstra('SELECT gid AS id, source, target, cost FROM ways', 1, 2)

As you can see this is fairly straight forward and it also allows for great flexibility, both in terms
of database structure and in defining cost functions. You can test the previous query
using *length_m AS cost* to compute the shortest path in meters or *cost_s / 60 AS cost* to compute
the fastest path in minutes.

You can look and the specific algorithms for the details of the signatures and how
to use them. These results have information like edge id and/or the
node id along with the cost or geometry for the step in the path from *start*
to *end*. Using the ids you can join these result back to your edge table
to get more information about each step in the path.

* :doc:`pgr_dijkstra`

Group of Functions
-------------------------------------------------------------------------------

A function might have different overloads.
Across this documentation, to indicate which overload we use the following terms:

* `One to One`_
* `One to Many`_
* `Many to One`_
* `Many to Many`_
* `Combinations`_

Depending on the overload are the parameters used, keeping consistency across
all functions.

One to One
...............................................................................

When routing from:

* From **one** starting vertex
* to **one** ending vertex

One to Many
...............................................................................

When routing from:

* From **one** starting vertex
* to **many** ending vertices

Many to One
...............................................................................

When routing from:

* From **many** starting vertices
* to **one** ending vertex

Many to Many
...............................................................................

When routing from:

* From **many** starting vertices
* to **many** ending vertices

|

Combinations
...............................................................................

When routing from:

* From **many** different starting vertices
* to **many** different ending vertices
* Every tuple specifies a pair of a start vertex and an end vertex
* Users can define the combinations as desired.

|

Inner Queries
-------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of valid inner queries and also the columns returned are depending of the function.
Which kind of inner query will depend on the function(s) requirements.
To simplify variety of types, **ANY-INTEGER** and **ANY-NUMERICAL** is used.

.. where_definition_starts

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. where_definition_ends

|

Edges SQL
...............................................................................

.. rubric:: Edges SQL for

* :doc:`dijkstra-family`
* :doc:`withPoints-family`
* :doc:`bdDijkstra-family`
* :doc:`components-family`
* :doc:`kruskal-family`
* :doc:`prim-family`
* Some uncategorized functions

.. basic_edges_sql_start

.. list-table::
   :width: 81
   :widths: 14 14 7 44
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``id``
     - **ANY-INTEGER**
     -
     - Identifier of the edge.
   * - ``source``
     - **ANY-INTEGER**
     -
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - **ANY-INTEGER**
     -
     - Identifier of the second end point vertex of the edge.
   * - ``cost``
     - **ANY-NUMERICAL**
     -
     - Weight of the edge  (``source``, ``target``)
   * - ``reverse_cost``
     - **ANY-NUMERICAL**
     - -1
     - Weight of the edge (``target``, ``source``)

       - When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. basic_edges_sql_end

.. rubric:: Edges SQL for

* :doc:`allpairs-family`

.. no_id_edges_sql_start

.. list-table::
   :width: 81
   :widths: 14 14 7 44
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``source``
     - **ANY-INTEGER**
     -
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - **ANY-INTEGER**
     -
     - Identifier of the second end point vertex of the edge.
   * - ``cost``
     - **ANY-NUMERICAL**
     -
     - Weight of the edge  (``source``, ``target``)
   * - ``reverse_cost``
     - **ANY-NUMERICAL**
     - -1
     - Weight of the edge (``target``, ``source``)

       - When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. no_id_edges_sql_end

.. rubric:: Edges SQL for

* :doc:`aStar-family`
* :doc:`bdAstar-family`

.. xy_edges_sql_start

.. list-table::
   :width: 81
   :widths: 12 14 7 44
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``id``
     - **ANY-INTEGER**
     -
     - Identifier of the edge.
   * - ``source``
     - **ANY-INTEGER**
     -
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - **ANY-INTEGER**
     -
     - Identifier of the second end point vertex of the edge.
   * - ``cost``
     - **ANY-NUMERICAL**
     -
     - Weight of the edge  (``source``, ``target``)

       * When negative: edge (``source``, ``target``) does not exist, therefore
         it's not part of the graph.
   * - ``reverse_cost``
     - **ANY-NUMERICAL**
     - -1
     - Weight of the edge (``target``, ``source``),

       * When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.
   * - ``x1``
     - **ANY-NUMERICAL**
     -
     - X coordinate of ``source`` vertex.
   * - ``y1``
     - **ANY-NUMERICAL**
     -
     - Y coordinate of ``source`` vertex.
   * - ``x2``
     - **ANY-NUMERICAL**
     -
     - X coordinate of ``target`` vertex.
   * - ``y2``
     - **ANY-NUMERICAL**
     -
     - Y coordinate of ``target`` vertex.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. xy_edges_sql_end


.. rubric:: Edges SQL for

* :doc:`flow-family`

.. include:: flow-family.rst
   :start-after: flow_edges_sql_start
   :end-before: flow_edges_sql_end

.. rubric:: Edges SQL for the following functions of :doc:`flow-family`

* :doc:`pgr_maxFlowMinCost`
* :doc:`pgr_maxFlowMinCost_Cost`

.. include:: flow-family.rst
   :start-after: costFlow_edges_sql_start
   :end-before: costFlow_edges_sql_end

|

Combinations SQL
...............................................................................

.. rubric:: Used on combination signatures

.. basic_combinations_sql_start

.. list-table::
   :width: 81
   :widths: 12 14 60
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - ``source``
     - **ANY-INTEGER**
     - Identifier of the departure vertex.
   * - ``target``
     - **ANY-INTEGER**
     - Identifier of the arrival vertex.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. basic_combinations_sql_end


|

Points SQL
...............................................................................

.. rubric:: Points SQL for

* :doc:`withPoints-family`

.. include:: withPoints-category.rst
   :start-after: points_sql_start
   :end-before: points_sql_end

Parameters
-------------------------------------------------------------------------------

The main parameter of the majority of the pgRouting functions is a query that
selects the edges of the graph.

.. only_edge_param_start

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

.. only_edge_param_end

Depending on the family or category of a function it will have additional
parameters, some of them are compulsory and some are optional.

The compulsory parameters are nameless and must be given in the required order.
The optional parameters are named parameters and will have a default value.

.. rubric:: Parameters for the Via functions

* :doc:`pgr_dijkstraVia`

.. pgr_dijkstra_via_parameters_start

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
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     -
     - Array of ordered vertices identifiers that are going to be visited.
   * - ``directed``
     - ``BOOLEAN``
     - ``true``
     - - When ``true`` Graph is considered `Directed`
       - When ``false`` the graph is considered as Undirected.
   * - ``strict``
     - ``BOOLEAN``
     - ``false``
     - * When ``true`` if a path is missing stops and returns **EMPTY SET**
       * When ``false`` ignores missing paths returning all paths found
   * - ``U_turn_on_edge``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` departing from a visited vertex will not try to avoid
         using the edge used to reach it.  In other words, U turn using the edge
         with same identifier is allowed.
       * When ``false`` when a departing from a visited vertex tries to avoid
         using the edge used to reach it.  In other words, U turn using the edge
         with same identifier is used when no other path is found.


.. pgr_dijkstra_via_parameters_end

Return columns
--------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of columns returned are depending of the function.

|

Return columns for a path
...............................................................................

.. rubric:: Used on functions that return one path solution

.. return_path_short_start

Returns set of ``(seq, path_seq [, start_vid] [, end_vid], node, edge, cost,
agg_cost)``

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
     - Relative position in the path. Has value **1** for the beginning of a path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex.
       Returned when multiple starting vetrices are in the query.

       * `Many to One`_
       * `Many to Many`_
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex.
       Returned when multiple ending vertices are in the query.

       * `One to Many`_
       * `Many to Many`_
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_vid`` to ``end_vid``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence. **-1** for the last node of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

.. return_path_short_end

.. rubric:: Used on functions the following:

* :doc:`pgr_withPoints`


.. return_withpoint_path_short_start

Returns set of ``(seq, path_seq [, start_pid] [, end_pid], node, edge, cost,
agg_cost)``

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
     - Relative position in the path.

       * **1** For the first row of the path.
   * - ``start_pid``
     - ``BIGINT``
     - Identifier of a starting vertex/point of the path.

       * When positive is the identifier of the starting vertex.
       * When negative is the identifier of the starting point.
       * Returned on `Many to One`_  and `Many to Many`_
   * - ``end_pid``
     - ``BIGINT``
     - Identifier of an ending vertex/point of the path.

       * When positive is the identifier of the ending vertex.
       * When negative is the identifier of the ending point.
       * Returned on `One to Many`_ and `Many to Many`_
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_pid`` to ``end_pid``.

       * When positive is the identifier of the a vertex.
       * When negative is the identifier of the a point.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence.

       * **-1** for the last row of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.

       * **0** For the first row of the path.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

       * **0** For the first row of the path.

.. return_withpoint_path_short_end


.. rubric:: Used on functions the following:

* :doc:`pgr_dijkstraNear`

.. return_path_complete_start

Returns set of ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

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
     - Relative position in the path. Has value **1** for the beginning of a path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex of the current path.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex of the current path.
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_vid`` to ``end_vid``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence. **-1** for the last node of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

.. return_path_complete_end

|

Return columns for multiple paths
...............................................................................

.. rubric:: Used on functions that return many paths solutions

.. return_path_start

Set of ``(seq, path_id, path_seq [, start_vid] [, end_vid], node, edge, cost,
agg_cost)``

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
     - Path identifier.

       * Has value **1** for the first of a path from ``start_vid`` to ``end_vid``.
   * - ``path_seq``
     - ``INTEGER``
     - Relative position in the path. Has value **1** for the beginning of a path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex.
       Returned when multiple starting vetrices are in the query.

       * `Many to One`_
       * `Many to Many`_
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex.
       Returned when multiple ending vertices are in the query.

       * `One to Many`_
       * `Many to Many`_
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from ``start_vid`` to ``end_vid``.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to go from ``node`` to the next node in the
       path sequence. **-1** for the last node of the path.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``node`` using ``edge`` to the next node in the
       path sequence.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

.. return_path_end

|

Return columns for cost functions
...............................................................................

.. rubric:: Used in the following

* Cost functions
* :doc:`costMatrix-category`
* :doc:`allpairs-family`

.. return_cost_start

Set of ``(start_vid, end_vid, agg_cost)``

.. list-table::
   :width: 81
   :widths: 12 14 60
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``end_vid``.

.. return_cost_end

.. rubric:: Used in the following

* :doc:`pgr_withPointsCost`

.. return_cost_withPoints_start

Set of ``(start_vid, end_vid, agg_cost)``

.. list-table::
   :width: 81
   :widths: 12 14 60
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex or point.

       * When positive: is a vertex’s identifier.
       * When negative: is a point’s identifier.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex or point.

       * When positive: is a vertex’s identifier.
       * When negative: is a point’s identifier.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``end_vid``.

.. return_cost_withPoints_end

|

Return columns for flow functions
.....................................................................

.. rubric:: Edges SQL for the following

* :doc:`flow-family`

.. include:: flow-family.rst
    :start-after: result_flow_start
    :end-before: result_flow_end

.. rubric:: Edges SQL for the following functions of :doc:`flow-family`

* :doc:`pgr_maxFlowMinCost`

.. include:: flow-family.rst
    :start-after: result_costFlow_start
    :end-before: result_costFlow_end

Return columns for spanning tree functions
.....................................................................


.. rubric:: Edges SQL for the following

* :doc:`pgr_prim`
* :doc:`pgr_kruskal`

.. r-edge-cost-start

Returns SET OF ``(edge, cost)``

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse the edge.

.. r-edge-cost-end

Advanced Topics
-------------------------------------------------------------------------------

.. contents::
    :local:

.. _topology:

Routing Topology
...............................................................................


.. rubric:: Overview

Typically when GIS files are loaded into the data database for use with pgRouting they do not have topology information associated with them. To create a useful topology the data needs to be "noded". This means that where two or more roads form an intersection there it needs to be a node at the intersection and all the road segments need to be broken at the intersection, assuming that you can navigate from any of these segments to any other segment via that intersection.

You can use the :ref:`graph analysis functions <analytics>` to help you see where you might have topology problems in your data. If you need to node your data, we also have a function :doc:`pgr_nodeNetwork() <pgr_nodeNetwork>` that might work for you. This function splits ALL crossing segments and nodes them. There are some cases where this might NOT be the right thing to do.

For example, when you have an overpass and underpass intersection, you do not want these noded, but pgr_nodeNetwork does not know that is the case and will node them which is not good because then the router will be able to turn off the overpass onto the underpass like it was a flat 2D intersection. To deal with this problem some data sets use z-levels at these types of intersections and other data might not node these intersection which would be ok.

For those cases where topology needs to be added the following functions may be useful. One way to prep the data for pgRouting is to add the following columns to your table and then populate them as appropriate. This example makes a lot of assumption like that you original data tables already has certain columns in it like ``one_way``, ``fcc``, and possibly others and that they contain specific data values. This is only to give you an idea of what you can do with your data.

.. code-block:: sql

    ALTER TABLE edge_table
        ADD COLUMN source integer,
        ADD COLUMN target integer,
        ADD COLUMN cost_len double precision,
        ADD COLUMN cost_time double precision,
        ADD COLUMN rcost_len double precision,
        ADD COLUMN rcost_time double precision,
        ADD COLUMN x1 double precision,
        ADD COLUMN y1 double precision,
        ADD COLUMN x2 double precision,
        ADD COLUMN y2 double precision,
        ADD COLUMN to_cost double precision,
        ADD COLUMN rule text,
        ADD COLUMN isolated integer;

    SELECT pgr_createTopology('edge_table', 0.000001, 'the_geom', 'id');

The function :doc:`pgr_createTopology <pgr_createTopology>` will create the ``vertices_tmp`` table and populate the ``source`` and ``target`` columns. The following example populated the remaining columns. In this example, the ``fcc`` column contains feature class code and the ``CASE`` statements converts it to an average speed.

.. code-block:: sql

    UPDATE edge_table SET x1 = st_x(st_startpoint(the_geom)),
                          y1 = st_y(st_startpoint(the_geom)),
                          x2 = st_x(st_endpoint(the_geom)),
                          y2 = st_y(st_endpoint(the_geom)),
      cost_len  = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      rcost_len = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      len_km = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')/1000.0,
      len_miles = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')
                  / 1000.0 * 0.6213712,
      speed_mph = CASE WHEN fcc='A10' THEN 65
                       WHEN fcc='A15' THEN 65
                       WHEN fcc='A20' THEN 55
                       WHEN fcc='A25' THEN 55
                       WHEN fcc='A30' THEN 45
                       WHEN fcc='A35' THEN 45
                       WHEN fcc='A40' THEN 35
                       WHEN fcc='A45' THEN 35
                       WHEN fcc='A50' THEN 25
                       WHEN fcc='A60' THEN 25
                       WHEN fcc='A61' THEN 25
                       WHEN fcc='A62' THEN 25
                       WHEN fcc='A64' THEN 25
                       WHEN fcc='A70' THEN 15
                       WHEN fcc='A69' THEN 10
                       ELSE null END,
      speed_kmh = CASE WHEN fcc='A10' THEN 104
                       WHEN fcc='A15' THEN 104
                       WHEN fcc='A20' THEN 88
                       WHEN fcc='A25' THEN 88
                       WHEN fcc='A30' THEN 72
                       WHEN fcc='A35' THEN 72
                       WHEN fcc='A40' THEN 56
                       WHEN fcc='A45' THEN 56
                       WHEN fcc='A50' THEN 40
                       WHEN fcc='A60' THEN 50
                       WHEN fcc='A61' THEN 40
                       WHEN fcc='A62' THEN 40
                       WHEN fcc='A64' THEN 40
                       WHEN fcc='A70' THEN 25
                       WHEN fcc='A69' THEN 15
                       ELSE null END;

    -- UPDATE the cost information based on oneway streets

    UPDATE edge_table SET
        cost_time = CASE
            WHEN one_way='TF' THEN 10000.0
            ELSE cost_len/1000.0/speed_kmh::numeric*3600.0
            END,
        rcost_time = CASE
            WHEN one_way='FT' THEN 10000.0
            ELSE cost_len/1000.0/speed_kmh::numeric*3600.0
            END;

    -- clean up the database because we have updated a lot of records

    VACUUM ANALYZE VERBOSE edge_table;


Now your database should be ready to use any (most?) of the pgRouting algorithms.


.. _analytics:

Graph Analytics
...............................................................................


.. rubric:: Overview

It is common to find problems with graphs that have not been constructed fully noded or in graphs with z-levels at intersection that have been entered incorrectly. An other problem is one way streets that have been entered in the wrong direction. We can not detect errors with respect to "ground" truth, but we can look for inconsistencies and some anomalies in a graph and report them for additional inspections.

We do not current have any visualization tools for these problems, but I have used mapserver to render the graph and highlight potential problem areas. Someone familiar with graphviz might contribute tools for generating images with that.


Analyze a Graph
...............................................................................

With :doc:`pgr_analyzeGraph` the graph can be checked for errors. For example for table "mytab" that has "mytab_vertices_pgr" as the vertices table:

.. code-block:: sql

    SELECT pgr_analyzeGraph('mytab', 0.000002);
    NOTICE:  Performing checks, pelase wait...
    NOTICE:  Analyzing for dead ends. Please wait...
    NOTICE:  Analyzing for gaps. Please wait...
    NOTICE:  Analyzing for isolated edges. Please wait...
    NOTICE:  Analyzing for ring geometries. Please wait...
    NOTICE:  Analyzing for intersections. Please wait...
    NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
    NOTICE:                    Isolated segments: 158
    NOTICE:                            Dead ends: 20028
    NOTICE:  Potential gaps found near dead ends: 527
    NOTICE:               Intersections detected: 2560
    NOTICE:                      Ring geometries: 0
    pgr_analyzeGraph
    ----------
       OK
    (1 row)


In the vertices table "mytab_vertices_pgr":

- Deadends are identified by ``cnt=1``
- Potencial gap problems are identified with ``chk=1``.

.. code-block:: sql

    SELECT count(*) as deadends  FROM mytab_vertices_pgr WHERE cnt = 1;
    deadends
    ----------
        20028
     (1 row)

    SELECT count(*) as gaps  FROM mytab_vertices_pgr WHERE chk = 1;
     gaps
     -----
       527
     (1 row)



For isolated road segments, for example, a segment where both ends are deadends. you can find these with the following query:

.. code-block:: sql

    SELECT *
        FROM mytab a, mytab_vertices_pgr b, mytab_vertices_pgr c
        WHERE a.source=b.id AND b.cnt=1 AND a.target=c.id AND c.cnt=1;


If you want to visualize these on a graphic image, then you can use something like mapserver to render the edges and the vertices and style based on ``cnt`` or if they are isolated, etc. You can also do this with a tool like graphviz, or geoserver or other similar tools.


Analyze One Way Streets
...............................................................................

:doc:`pgr_analyzeOneWay` analyzes one way streets in a graph and identifies any flipped segments. Basically if you count the edges coming into a node and the edges exiting a node the number has to be greater than one.

This query will add two columns to the vertices_tmp table ``ein int`` and ``eout int`` and populate it with the appropriate counts. After running this on a graph you can identify nodes with potential problems with the following query.


The rules are defined as an array of text strings that if match the ``col`` value would be counted as true for the source or target in or out condition.


Example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Lets assume we have a table "st" of edges and a column "one_way" that might have values like:

* 'FT'    - oneway from the source to the target node.
* 'TF'    - oneway from the target to the source node.
* 'B'     - two way street.
* ''      - empty field, assume twoway.
* <NULL>  - NULL field, use two_way_if_null flag.

Then we could form the following query to analyze the oneway streets for errors.

.. code-block:: sql

    SELECT pgr_analyzeOneway('mytab',
                ARRAY['', 'B', 'TF'],
                ARRAY['', 'B', 'FT'],
                ARRAY['', 'B', 'FT'],
                ARRAY['', 'B', 'TF'],
                );

    -- now we can see the problem nodes
    SELECT * FROM mytab_vertices_pgr WHERE ein=0 OR eout=0;

    -- and the problem edges connected to those nodes
    SELECT gid FROM mytab a, mytab_vertices_pgr b WHERE a.source=b.id AND ein=0 OR eout=0
    UNION
    SELECT gid FROM mytab a, mytab_vertices_pgr b WHERE a.target=b.id AND ein=0 OR eout=0;

Typically these problems are generated by a break in the network, the one way direction set wrong, maybe an error related to z-levels or a network that is not properly noded.

The above tools do not detect all network issues, but they will identify some common problems. There are other problems that are hard to detect because they are more global in nature like multiple disconnected networks. Think of an island with a road network that is not connected to the mainland network because the bridge or ferry routes are missing.




.. _performance:

Performance Tips
-------------------------------------------------------------------------------

.. contents::
    :local:


For the Routing functions
...............................................................................

To get faster results bound your queries to the area of interest of routing to have, for example, no more than one million rows.

Use an inner query SQL that does not include some edges in the routing function

.. code-block:: sql

	SELECT id, source, target from edge_table WHERE
        	id < 17 and
        	the_geom  && (select st_buffer(the_geom,1) as myarea FROM  edge_table where id = 5)

Integrating the inner query to the pgRouting function:

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
	    'SELECT id, source, target from edge_table WHERE
        	id < 17 and
        	the_geom  && (select st_buffer(the_geom,1) as myarea FROM  edge_table where id = 5)',
        1, 2)




For the topology functions:
...............................................................................

When "you know" that you are going to remove a set of edges from the edges table, and without those edges you are going to use a routing function you can do the following:

Analize the new topology based on the actual topology:

.. code-block:: sql

	pgr_analyzegraph('edge_table',rows_where:='id < 17');

Or create a new topology if the change is permanent:

.. code-block:: sql

	pgr_createTopology('edge_table',rows_where:='id < 17');
	pgr_analyzegraph('edge_table',rows_where:='id < 17');


.. _how_contribute:

How to contribute
-------------------------------------------------------------------------------

.. rubric:: Wiki

* Edit an existing  `pgRouting Wiki <https://github.com/pgRouting/pgrouting/wiki>`_ page.
* Or create a new Wiki page

  * Create a page on the `pgRouting Wiki <https://github.com/pgRouting/pgrouting/wiki>`_
  * Give the title an appropriate name


* `Example <https://github.com/pgRouting/pgrouting/wiki/How-to:-Handle-parallel-edges-(KSP)>`_

.. rubric:: Adding Functionaity to pgRouting


Consult the `developer's documentation <https://docs.pgrouting.org/doxy/2.4/index.html>`_



.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

