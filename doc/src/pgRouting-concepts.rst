..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgrouting_concepts:

pgRouting Concepts
===============================================================================

.. contents::

.. _Getting_started:

Getting Started
-------------------------------------------------------------------------------

This is a simple guide to walk you through the steps of getting started
with pgRouting. In this guide we will cover:

.. contents::
    :local:


.. _create_database:

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

.. _load_data:

Load Data
...............................................................................

How you load your data will depend in what form it comes it. There are
various OpenSource tools that can help you, like:

:osm2pgrouting: - this is a tool for loading OSM data into postgresql with pgRouting requirements
:shp2pgsql: - this is the postgresql shapefile loader
:ogr2ogr: - this is a vector data conversion utility
:osm2pgsql: - this is a tool for loading OSM data into postgresql

So these tools and probably others will allow you to read vector data so that
you may then load that data into your database as a table of some kind. At this
point you need to know a little about your data structure and content. One easy
way to browse your new data table is with pgAdmin3 or phpPgAdmin.

.. _build_topology:

Build a Routing Topology
...............................................................................

Next we need to build a topology for our street data. What this means is that
for any given edge in your street data the ends of that edge will be connected
to a unique node and to other edges that are also connected to that same unique
node. Once all the edges are connected to nodes we have a graph that can be
used for routing with pgrouting. We provide a tool that will help with this:

.. note:: this step is not needed if data is loaded with `osm2pgrouting`

.. code-block:: sql

    select pgr_createTopology('myroads', 0.000001);

* :ref:`pgr_create_topology`

.. _check_graph:

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

* :ref:`pgr_analyze_graph`
* :ref:`pgr_analyze_oneway`
* :ref:`pgr_node_network`


.. _compute_path:

Compute a Path
...............................................................................

Once you have all the preparation work done above, computing a route is fairly easy.
We have a lot of different algorithms that can work with your prepared road
network. The general form of a route query is:

.. code-block:: none

    select pgr_dijkstra('SELECT * FROM myroads', 1, 2)

As you can see this is fairly straight forward and you can look and the
specific algorithms for the details of the signatures and how to use them.
These results have information like edge id and/or the
node id along with the cost or geometry for the step in the path from *start*
to *end*. Using the ids you can join these result back to your edge table
to get more information about each step in the path.

* :ref:`pgr_dijkstra`



.. _inner_queries:

Inner Queries
-------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of valid inner queries and also the columns returned are depending of the function.
Which kind of inner query will depend on the function(s) requirements.
To simplify variety of types, ``ANY-INTEGER`` and ``ANY-NUMERICAL`` is used.

.. where_definition_starts

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. where_definition_ends

.. basic_edges_sql_start

Description of the edges_sql query for dijkstra like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. basic_edges_sql_end

.. no_id_edges_sql_start

Description of the edges_sql query (id is not necessary)
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 * When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. no_id_edges_sql_end



.. pgr_dijkstra_via_parameters_start

Description of the parameters of the signatures
...............................................................................

=================== ====================== ========= =========================================
Parameter           Type                   Default   Description
=================== ====================== ========= =========================================
**edges_sql**       ``TEXT``                         SQL query as described above.
**via_vertices**    ``ARRAY[ANY-INTEGER]``           Array of ordered vertices identifiers that are going to be visited.
**directed**        ``BOOLEAN``            ``true``  - When ``true`` Graph is considered `Directed`
                                                     - When ``false`` the graph is considered as Undirected.
**strict**          ``BOOLEAN``            ``false`` - When ``false`` ignores missing paths returning all paths found
                                                     - When ``true`` if a path is missing stops and returns `EMPTY SET`
**U_turn_on_edge**  ``BOOLEAN``            ``true``  - When ``true`` departing from a visited vertex will not try to avoid using the edge used to reach it.  In other words, U turn using the edge with same `id` is allowed.
                                                     - When ``false`` when a departing from a visited vertex tries to avoid using the edge used to reach it.  In other words, U turn using the edge with same `id` is used when no other path is found.
=================== ====================== ========= =========================================

.. pgr_dijkstra_via_parameters_end


.. xy_edges_sql_start

Description of the edges_sql query for astar like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`

                                                 - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,

                                                 - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

**x1**            ``ANY-NUMERICAL``              X coordinate of `source` vertex.
**y1**            ``ANY-NUMERICAL``              Y coordinate of `source` vertex.
**x2**            ``ANY-NUMERICAL``              X coordinate of `target` vertex.
**y2**            ``ANY-NUMERICAL``              Y coordinate of `target` vertex.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. xy_edges_sql_end

.. flow_edges_sql_start

Description of the edges_sql query for Max-flow like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

==================== =================== ======== =================================================
Column               Type                Default  Description
==================== =================== ======== =================================================
**id**               ``ANY-INTEGER``              Identifier of the edge.
**source**           ``ANY-INTEGER``              Identifier of the first end point vertex of the edge.
**target**           ``ANY-INTEGER``              Identifier of the second end point vertex of the edge.
**capacity**         ``ANY-INTEGER``              Weight of the edge  `(source, target)`

                                                  - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.

**reverse_capacity** ``ANY-INTEGER``       -1     Weight of the edge `(target, source)`,

                                                  - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

==================== =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT


.. flow_edges_sql_end



.. points_sql_start

Description of the Points SQL query
...............................................................................

:points_sql: an SQL query, which should return a set of rows with the following columns:

============ ================= =================================================
Column            Type              Description
============ ================= =================================================
**pid**      ``ANY-INTEGER``   (optional) Identifier of the point.

                               - If column present, it can not be NULL.
                               - If column not present, a sequential identifier will be given automatically.

**edge_id**  ``ANY-INTEGER``   Identifier of the "closest" edge to the point.
**fraction** ``ANY-NUMERICAL`` Value in <0,1> that indicates the relative postition from the first end point of the edge.
**side**     ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the point is:

                               - In the right, left of the edge or
                               - If it doesn't matter with 'b' or NULL.
                               - If column not present 'b' is considered.

============ ================= =================================================

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

.. points_sql_end


.. _return_values:

Return columns & values
--------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of columns returned are depending of the function.

.. return_path_start

Description of the return values for a path
...............................................................................

Returns set of ``(seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**path_id**    ``INT``    Path identifier. Has value **1** for the first of a path. Used when there are multiple paths for the same ``start_vid`` to ``end_vid`` combination.
**path_seq**   ``INT``    Relative position in the path. Has value **1** for the beginning of a path.
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**node**       ``BIGINT`` Identifier of the node in the path from ``start_vid`` to ``end_vid``.
**edge**       ``BIGINT`` Identifier of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path.
**cost**       ``FLOAT``  Cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_v`` to ``node``.
============== ========== =================================================

.. return_path_end

.. return_cost_start

Description of the return values for a Cost function
...............................................................................

Returns set of ``(start_vid, end_vid, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_vid`` to ``end_vid``.
============== ========== =================================================

.. return_cost_end



.. result_flow_start

Description of the Return Values
.....................................................................

=====================  ====================  =================================================
Column                 Type                  Description
=====================  ====================  =================================================
**seq**                ``INT``               Sequential value starting from **1**.
**edge_id**            ``BIGINT``            Identifier of the edge in the original query(edges_sql).
**source**             ``BIGINT``            Identifier of the first end point vertex of the edge.
**target**             ``BIGINT``            Identifier of the second end point vertex of the edge.
**flow**               ``BIGINT``            Flow through the edge in the direction (source, target).
**residual_capacity**  ``BIGINT``            Residual capacity of the edge in the direction (source, target).
=====================  ====================  =================================================

.. result_flow_end

.. _advanced_topics:

Advanced Topics
-------------------------------------------------------------------------------

.. contents::
    :local:

.. _topology:

Routing Topology
...............................................................................


.. rubric:: Overview

Typically when GIS files are loaded into the data database for use with pgRouting they do not have topology information associated with them. To create a useful topology the data needs to be "noded". This means that where two or more roads form an intersection there it needs to be a node at the intersection and all the road segments need to be broken at the intersection, assuming that you can navigate from any of these segments to any other segment via that intersection.

You can use the :ref:`graph analysis functions <analytics>` to help you see where you might have topology problems in your data. If you need to node your data, we also have a function :ref:`pgr_nodeNetwork() <pgr_node_network>` that might work for you. This function splits ALL crossing segments and nodes them. There are some cases where this might NOT be the right thing to do.

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

The function :ref:`pgr_createTopology() <pgr_create_topology>` will create the ``vertices_tmp`` table and populate the ``source`` and ``target`` columns. The following example populated the remaining columns. In this example, the ``fcc`` column contains feature class code and the ``CASE`` statements converts it to an average speed.

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

With :ref:`pgr_analyze_graph` the graph can be checked for errors. For example for table "mytab" that has "mytab_vertices_pgr" as the vertices table:

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

:ref:`pgr_analyze_oneway` analyzes one way streets in a graph and identifies any flipped segments. Basically if you count the edges coming into a node and the edges exiting a node the number has to be greater than one.

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


Consult the `developer's documentation <http://docs.pgrouting.org/doxy/2.4/index.html>`_



.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

