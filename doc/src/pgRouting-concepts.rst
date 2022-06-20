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
  (`3.4 <https://docs.pgrouting.org/3.4/en/pgRouting-concepts.html>`__)
  `3.3 <https://docs.pgrouting.org/3.3/en/pgRouting-concepts.html>`__
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

pgRouting Concepts
===============================================================================

.. contents::


This is a simple guide to walk you through the steps of getting started
with pgRouting. In this guide we will cover:

.. contents::
    :local:

Graphs
-------------------------------------------------------------------------------

A graph is an ordered pair :math:`G = (V ,E)` where:

* :math:`V` is a set of vertices, also called nodes
* :math:`E \subseteq \{( u, v ) \mid u , v \in V \}`

There are different kinds of graphs.

* Undirected simple graph

  * :math:`E \subseteq \{( u, v ) \mid u , v \in V, u \neq v\}`

* Directed graph

  * :math:`E \subseteq \{( u, v ) \mid (u , v) \in (V X V) \}`

* Directed simple graph

  * :math:`E \subseteq \{( u, v ) \mid (u , v) \in (V X V), u \neq v\}`

Graphs:

* Do not have geometries
* Some graph theory problems requires graphs to have weights, called **cost** in
  pgRouting.

In pgRouting there are several ways to represent a graph on the database:

* With ``cost``

  * (``id``, ``source``, ``target``, ``cost``)

* With ``cost`` and ``reverse_cost``

  * (``id``, ``source``, ``target``, ``cost``, ``reverse_cost``)

Where:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Description
   * - ``id``
     - Identifier of the edge. Requirement to use the database in a consistent
       manner.
   * - ``source``
     - Identifier of a vertex
   * - ``target``
     - Identifier of a vertex
   * - ``cost``
     - Weight of the edge (``source``, ``target``)

       * When negative the edge (``source``, ``target``) do not exist on the
         graph
   * - ``reverse_cost``
     - Weight of the edge (``target``, ``source``)

       * When negative the edge (``target``, ``source``) do not exist on the
         graph

The decision of the graph to be **directed** or **undirected** is done when
running a pgRouting algorithm.


Graph with ``cost``
...............................................................................

The weighted directed graph, :math:`G_d(V,E)`, is interpreted as:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target`

* the set of edges :math:`E`

  - :math:`E = \{(source_i, target_i, cost_i) \text{ when } cost_i >=0 \}`

.. rubric:: Directed graph

In a directed graph the edge :math:`(source_i, target_i, cost_i)` has
directionality: :math:`source_i \rightarrow target_i`

For the following data:

.. literalinclude:: concepts.queries
   :start-after: -- g1
   :end-before: -- g2

.. graphviz::

   digraph G {
    1 -> 2 [label="  1(5)"];
    3;
   }

.. rubric:: Undirected graph

In an undirected graph the edge :math:`(source_i, target_i, cost_i)` does not
have directionality: :math:`source_i \; {\rule[0.5ex]{1em}{0.55pt}} \; target_i`

* In terms of a directed graph is like having two edges: :math:`source_i
  \leftrightarrow target_i`

For the following data:

.. literalinclude:: concepts.queries
   :start-after: -- g1
   :end-before: -- g2

.. graphviz::

   graph G {
    1 -- 2 [label="  1(5)"];
    3;
   }

Graph with ``cost`` and ``reverse_cost``
...............................................................................

The weighted directed graph, :math:`G_d(V,E)`, is defined by:

* the set of vertices  :math:`V`

  - :math:`V = source \cup target`

* the set of edges :math:`E`

  - :math:`E = \begin{split} \begin{align}
    & {\{(source_i, target_i, cost_i) \text{ when } cost_i >=0 \}} \\
    & \cup \\
    & {\{(target_i, source_i, reverse\_cost_i) \text{ when } reverse\_cost_i >=0 \}}
    \end{align} \end{split}`

.. rubric:: Directed graph

In a directed graph both edges have directionality

* edge :math:`(source_i, target_i, cost_i)` has directionality: :math:`source_i
  \rightarrow target_i`
* edge :math:`(target_i, source_i, reverse\_cost_i)` has directionality:
  :math:`target_i \rightarrow source_i`

For the following data:

.. literalinclude:: concepts.queries
   :start-after: -- g2
   :end-before: -- g3

.. graphviz::

   digraph G {
    1 -> 2 [label="  1(5)"];
    2 -> 1 [label="  1(2)"];
    3 -> 1 [label="  2(4)"];
    2 -> 3 [label="  3(7)"];
   }

.. rubric:: Undirected graph

In a directed graph both edges do not have directionality

* Edge :math:`(source_i, target_i, cost_i)` is :math:`source_i
  \; {\rule[0.5ex]{1em}{0.55pt}} \; target_i`
* Edge :math:`(target_i, source_i, reverse\_cost_i)` is :math:`target_i
  \; {\rule[0.5ex]{1em}{0.55pt}} \; source_i`
* In terms of a directed graph is like having four edges:

  * :math:`source_i \leftrightarrow target_i`
  * :math:`target_i \leftrightarrow source_i`

For the following data:

.. literalinclude:: concepts.queries
   :start-after: -- g2
   :end-before: -- g3

.. graphviz::

   graph G {
    1 -- 2 [label="  1(5)"];
    2 -- 1 [label="  1(2)"];
    3 -- 1 [label="  2(4)"];
    2 -- 3 [label="  3(7)"];
   }

Graphs without geometries
-------------------------------------------------------------------------------

Personal relationships, genealogy, file dependency problems can be solved
using pgRouting. Those problems, normally,  do not come with gemetries asociated
with the graph data.

Wiki example
...............................................................................

Solve the example problem taken from wikipedia (`see top
right graph <https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm>`__) which:

* Problem find the shortest path from :math:`1` to :math:`5`.
* Is an undirected graph.
* Although visually looks like to have geometries, the drawing is not to scale.

  * No geometries asociated to the vertices or edges

* Has 6 vertices :math`\{1,2,3,4,5,6\}'
* Has 9 edges:

:math:`\begin{split} \begin{align}
E = & \{(1,2,7), (1,3,9), (1,6,14), \\
& (2,3,10), (2,4,13), \\
& (3,4,11), (3,6,2), \\
& (4,5,6), \\
& (5,6,9) \}
\end{align} \end{split}`


* The graph can be represented in many ways for example:

.. graphviz::

   graph G {
    rankdir="LR";
    1 [color="red"];
    5 [color="green"];
    1 -- 2 [label="  (7)"];
    5 -- 6 [label="  (9)"];
    1 -- 3 [label="  (9)"];
    1 -- 6 [label="  (14)"];
    2 -- 3 [label="  (10)"];
    2 -- 4 [label="  (13)"];
    3 -- 4 [label="  (11)"];
    3 -- 6 [label="  (2)"];
    4 -- 5 [label="  (6)"];
   }


Prepare the database
...............................................................................

Create a database for the example, access the database and install pgRouting: ::

   $ createdb wiki
   $ psql wiki
   wiki =# CREATE EXTENSION pgRouting CASCADE;

Create a table
...............................................................................

The basic elements needed to perform basic routing on an undirected graph are:

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
   * - ``source``
     - **ANY-INTEGER**
     - Identifier of the first end point vertex of the edge.
   * - ``target``
     - **ANY-INTEGER**
     - Identifier of the second end point vertex of the edge.
   * - ``cost``
     - **ANY-NUMERICAL**
     - Weight of the edge (``source``, ``target``)

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. literalinclude:: concepts.queries
   :start-after: -- q1
   :end-before: -- q2

Insert the data
...............................................................................

.. literalinclude:: concepts.queries
   :start-after: -- q2
   :end-before: -- q3

Find the shortest path
...............................................................................

To solve this example :doc:`pgr_dijkstra` is used:

.. literalinclude:: concepts.queries
   :start-after: -- q3
   :end-before: -- q4

To go from :math:`1` to :math:`5` the path goes thru the following vertices:
:math:`1 \rightarrow 3 \rightarrow 6 \rightarrow 5`

.. graphviz::

   graph G {
    rankdir="LR";
    1 [color="red"];
    5 [color="green"];
    1 -- 2 [label="  (7)"];
    5 -- 6 [label="  (9)", color="blue"];
    1 -- 3 [label="  (9)", color="blue"];
    1 -- 6 [label="  (14)"];
    2 -- 3 [label="  (10)"];
    2 -- 4 [label="  (13)"];
    3 -- 4 [label="  (11)"];
    3 -- 6 [label="  (2)", color="blue"];
    4 -- 5 [label="  (6)"];
   }

Create a topology
...............................................................................

For more complex functions a complete topology might be needed.

To create the vertices table, use :doc:`pgr_extractVertices`

.. literalinclude:: concepts.queries
   :start-after: -- q4
   :end-before: -- q5

Graphs with geometries
-------------------------------------------------------------------------------

Create a routing Database
...............................................................................

The first step is to create a database and load pgRouting in the database.

Typically create a database for each project.

Once having the database to work in, load your data and build the routing
application in that database.

.. parsed-literal::

	createdb sampledata
	psql sampledata -c "CREATE EXTENSION postgis CASCADE"

Load Data
...............................................................................

There are several ways to load your data into pgRouting.

* Manually creating a database.

  * `Graphs without geometries`_
  * :doc:`sampledata`: a small graph used on the documentation examples

* Using `osm2pgrouting
  <https://workshop.pgrouting.org/latest/en/basic/data.html>`__

There are various open source tools that can help, like:

:shp2pgsql: - postgresql shapefile loader
:ogr2ogr: - vector data conversion utility
:osm2pgsql: - loadg OSM data into postgresql

Please note that these tools will **not** import the data in a structure
compatible with pgRouting and when this happens the topology needs to be
adajusted.

* Breakup a segments on each segment-segment instersection
* When missing, add columns and assign values to ``source``, ``target``,
  ``cost``, ``reverse_cost``.
* Connect a disconnected graph.
* Create the complete graph topology
* Create one or more graphs based on the application to be developed.

  * Create a contracted graph for the high speed roads
  * Create graphs per state/country

In few words:

   Prepare the graph

What and how to prepare the graph, will depend on the application and/or on the
quality of the data and/or on how close the information is to have a topology
usable by pgRouting and/or some other factors not mentioned.

The steps to prepare the graph involve geometry operations using `PostGIS
<https://postgis.net/>`__ and some others involve graph operations like
:doc:`pgr_contraction` to contract a graph.

The `workshop <https://workshop.pgrouting.org/latest>`__ has a step by step on
how to prepare a graph using Open Street Map data, for a small application.

The use of indexes on the database design in general:

* Have the geometries indexed.
* Have the identifiers columns indexed.

Please consult the `PostgreSQL <https://www.postgresql.org/docs/>`__ documentation
and the `PostGIS <https://postgis.net/>`__ documentation.


Build a routing topology
...............................................................................

The basic information to use the majority of the pgRouting functions ``id,
source, target, cost, [reverse_cost]`` is what in pgRouting is called the
routing topology.

``reverse_cost`` is optional but strongly recomended to have in order to reduce
the size of the database due to the size of the geometry columns.
Having said that, in this documentation ``reverse_cost`` is used in this
documentation.

When the data comes with geometries and there is no routing topology, then this
step is needed.

All the start and end vertices of the geometries need an identifier that is to
be stored in a ``source`` and ``target`` columns of the table of the data.
Likewise, ``cost`` and ``reverse_cost`` need to have the value of traversing the
edge in both directions.

If the columns do not exist they need to be added to the table in question. (see
`ALTER TABLE <https://www.postgresql.org/docs/current/sql-altertable.html>`__

The function :doc:`pgr_extractVertices` is used to create a vertices table
based on the edge identifier and the geometry of the edge of the graph.

Finnaly using the data stored on the vertices tables the ``source`` and
``target`` are filled up.

See :doc:`sampledata` for an example for building the topology.

Data coming from OSM and using `osm2pgrouting
<https://github.com/pgRouting/osm2pgrouting>`__ as an import tool, comes with
the routing topology. See an example of using ``osm2pgrouting`` on the `workshop
<https://workshop.pgrouting.org/latest/en/basic/data.html>`__.

Update to length of geometry
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The cost and reverse cost values can be in terms of the length of the geometry.
Assume that the ``cost`` and ``reverse_cost`` columns in the sample data
represent:

* :math:`1` when the edge exists in the graph
* :math:`-1` when the edge does not exist in the graph

Using that information updating to the length of the geometries:

.. literalinclude:: concepts.queries
   :start-after: -- topo1
   :end-before: -- topo2

Which gives the following results

.. literalinclude:: concepts.queries
   :start-after: -- topo2
   :end-before: -- topo3

Note that to be able to follow the documentation examples, everything is based
on the original graph.

So fixing to what there was before:

.. literalinclude:: concepts.queries
   :start-after: -- topo3
   :end-before: -- topo4


Check the Routing Topology
...............................................................................

There are lots of possible problems in a graph.

* The data used may not have been designed with routing in mind.
* A graph has some very specific requirements.
* The graph is disconnected.
* There are unwanted intersections.
* The graph is too large and needs to be contracted.
* A subgraph is needed for the application.
* and many other problems that the pgRouting user, that is the application
  developer might encounter.

Crossing edges
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. cross_edges_start

To get the crossing edges:

.. literalinclude:: concepts.queries
   :start-after: -- cross1
   :end-before: -- cross2

.. figure:: images/crossing_edges.png

That information is correct, for example, when in terms of vehicles, is it a
tunnel or bride crossing over another road.

It might be incorrect, for example:

1. When it is actually an intersection of roads, where vehicles can make turns.
2. When in terms of electrical lines, the electrical line is able to swith roads
   even on a tunnel or bridge.

When it is incorrect, it needs fixing:

1. For vehicles and pedestrians

   * If the data comes from OSM and was imported to the database using
     ``osm2pgrouting``, the fix needs to be done in the `OSM portal
     <https://www.openstreetmap.org>`__ and the data imported again.
   * In general when the data comes from a supplier that has the data prepared
     for routing vehicles, and there is a problem, the data is to be fixed from
     the supplier

2. For very specific applications

   * The data is correct when from the point of view of routing vehicles or
     pedestrians.
   * The data needs a local fix for the specific application.

Once analized one by one the crossings, for the ones that need a local fix,
the edges need to be `split <https://postgis.net/docs/ST_Split.html>`__.

.. literalinclude:: concepts.queries
   :start-after: -- cross2
   :end-before: -- cross3

The new edges need to be added to the edges table, the rest of the attributes
need to be updated in the new edges, the old edges need to be
removed and the routing topology needs to be updated.

Adding split edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For each pair of crossing edges a process similar to this one must be performed.

The columns inserted and the way are calculated are based on the application.
For example, if the edges have a trait **name**, then that column is to be
copied.

For pgRouting calculations

* **factor** based on the position of the intersection of the edges can be used
  to adjust the ``cost`` and ``reverse_cost`` columns.
* Capacity information, used on the :doc:`flow-family` functions does not need
  to change when spliting edges.

.. literalinclude:: concepts.queries
   :start-after: -- cross3
   :end-before: -- cross4

Adding new vertices
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

After adding all the split edges requiered by the application, the newly created
vertices need to be added to the vertices table.

.. literalinclude:: concepts.queries
   :start-after: -- cross4
   :end-before: -- cross5

Updating edges topology
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: concepts.queries
   :start-after: -- cross5
   :end-before: -- cross6

Removing the surplus edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Once all significant information needed by the application has been transported
to the new edges, then the crossing edges can be deleted.

.. literalinclude:: concepts.queries
   :start-after: -- cross6
   :end-before: -- cross7

There are other options to do this task, like creating a view, or a materialized
view.

Updating vertices topology
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

To keep the graph consistent, the vertices topology needs to be updated

.. literalinclude:: concepts.queries
   :start-after: -- cross7
   :end-before: -- cross8

Checking for crossing edges
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

There are no crossing edges on the graph.

.. literalinclude:: concepts.queries
   :start-after: -- cross8
   :end-before: -- cross9

.. cross_edges_end

Dead ends
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To get the dead ends:

.. literalinclude:: concepts.queries
   :start-after: -- deadend1
   :end-before: -- deadend2

That information is correct, for example, when the dead end is on the limit of
the imported graph.

Visually node :math:`4` looks to be as start/ending of 3 edges, but it is not.

Is that correct?

* Is there such a small curb:

  * that does not allow a vehicle to use that visual intersection?
  * Is my applicaction for pedestrians and therefore the pedestrican can easily
    walk on the small curb?
  * Is my application for the electicicity and the electrical lines than can
    easily be extended on top of the small curb?

* Is there a big cliff and from eagles view look like the dead end is close to
  the segment?

.. TODO checked up to here

Compute a Path
...............................................................................

Once you have all the preparation work done above, computing a route is fairly
easy.
We have a lot of different algorithms that can work with your prepared road
network.
The general form of a route query using Dijkstra algorithm is:

.. parsed-literal::

    select pgr_dijkstra('SELECT * FROM myroads', <start>, <end>)


This algorithm only requires *id*, *source*, *target* and *cost* as the minimal
attributes, that by default will be considered to be columns in your roads
table.
If the column names in your roads table do not match exactly the names of these
attributes, you can use aliases.
For example, if you imported OSM data using **osm2pgrouting**, your id column's
name would be *gid* and your roads table would be *ways*, so you would query a
route from node id 1 to node id 2 by typing:

.. parsed-literal::

    select pgr_dijkstra('SELECT gid AS id, source, target, cost FROM ways', 1, 2)

As you can see this is fairly straight forward and it also allows for great
flexibility, both in terms of database structure and in defining cost functions.
You can test the previous query using *length_m AS cost* to compute the shortest
path in meters or *cost_s / 60 AS cost* to compute the fastest path in minutes.

You can look and the specific algorithms for the details of the signatures and
how to use them.
These results have information like edge id and/or the node id along with the
cost or geometry for the step in the path from *start* to *end*.
Using the ids you can join these result back to your edge table to get more
information about each step in the path.

* :doc:`pgr_dijkstra`

Group of Functions
-------------------------------------------------------------------------------

A function might have different overloads.
Across this documentation, to indicate which overload we use the following
terms:

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

Combinations
...............................................................................

When routing from:

* From **many** different starting vertices
* to **many** different ending vertices
* Every tuple specifies a pair of a start vertex and an end vertex
* Users can define the combinations as desired.

Inner Queries
-------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of valid inner queries and also the columns returned are
depending of the function.
Which kind of inner query will depend on the function(s) requirements.
To simplify variety of types, **ANY-INTEGER** and **ANY-NUMERICAL** is used.

.. where_definition_starts

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. where_definition_ends

Edges SQL
...............................................................................

General
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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
     - Weight of the edge (``source``, ``target``)
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

General without ``id``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

General with (X,Y)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

Flow
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. rubric:: Edges SQL for :doc:`flow-family`

.. rubric:: Edges SQL for

* :doc:`pgr_pushRelabel`
* :doc:`pgr_edmondsKarp`
* :doc:`pgr_boykovKolmogorov`

.. flow_edges_sql_start

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
   * - ``capacity``
     - **ANY-INTEGER**
     -
     - Weight of the edge  (``source``, ``target``)
   * - ``reverse_capacity``
     - **ANY-INTEGER**
     - -1
     - Weight of the edge (``target``, ``source``)

       - When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. flow_edges_sql_end

.. rubric:: Edges SQL for the following functions of :doc:`flow-family`

* :doc:`pgr_maxFlowMinCost`
* :doc:`pgr_maxFlowMinCost_Cost`

.. costFlow_edges_sql_start

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
   * - ``capacity``
     - **ANY-INTEGER**
     -
     - Capacity of the edge  (``source``, ``target``)

       - When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.
   * - ``reverse_capacity``
     - **ANY-INTEGER**
     - -1
     - Capacity of the edge (``target``, ``source``)

       - When negative: edge (``target``, ``source``) does not exist, therefore
         it's not part of the graph.
   * - ``cost``
     - **ANY-NUMERICAL**
     -
     - Weight of the edge  (``source``, ``target``) if it exist
   * - ``reverse_cost``
     - **ANY-NUMERICAL**
     - :math:`-1`
     - Weight of the edge (``target``, ``source``) if it exist

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. costFlow_edges_sql_end

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

Restrictions SQL
...............................................................................

.. restrictions_columns_start

.. list-table::
   :width: 81
   :widths: 7 17 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``path``
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - Sequence of edge identifiers that form a path that is not allowed to be
       taken.
       - Empty arrays or ``NULL`` arrays are ignored.
       - Arrays that have a ``NULL`` element will raise an exception.
   * - ``Cost``
     - **ANY-NUMERICAL**
     - Cost of taking the forbidden path.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. restrictions_columns_end


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

Parameters for the Via functions
...............................................................................

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

For the TRSP functions
...............................................................................

* :doc:`pgr_trsp`

.. restriction_parameters_start

.. list-table::
   :width: 81
   :widths: 17 22 44
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - SQL query as described.
   * - `Restrictions SQL`_
     - ``TEXT``
     - SQL query as described.
   * - `Combinations SQL`_
     - ``TEXT``
     - `Combinations SQL`_ as described below
   * - **start vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.
   * - **start vids**
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - Array of identifiers of destination vertices.
   * - **end vid**
     - **ANY-INTEGER**
     - Identifier of the departure vertex.
   * - **end vids**
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - Array of identifiers of destination vertices.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. restriction_parameters_end

Return columns
-------------------------------------------------------------------------------

.. contents::
    :local:

There are several kinds of columns returned are depending of the function.

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
     - Relative position in the path. Has value **1** for the beginning of a
       path.
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

Returns ``(seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)``

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

Multiple paths
...............................................................................

Selective for multiple paths.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The columns depend on the function call.

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

       * Has value **1** for the first of a path from ``start_vid`` to
         ``end_vid``.
   * - ``path_seq``
     - ``INTEGER``
     - Relative position in the path. Has value **1** for the beginning of a
       path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex.
       Returned when multiple starting vetrices are in the query.

       * `Many to One`_
       * `Many to Many`_
       * `Combinations`_
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex.
       Returned when multiple ending vertices are in the query.

       * `One to Many`_
       * `Many to Many`_
       * `Combinations`_
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

Non selective for multiple paths
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Regardless of the call, al the columns are returned.

* :doc:`pgr_trsp`

.. return_path_all_columns_start

Returns set of ``(seq, path_id, path_seq, start_vid, end_vid, node, edge, cost,
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

       * Has value **1** for the first of a path from ``start_vid`` to
         ``end_vid``.
   * - ``path_seq``
     - ``INTEGER``
     - Relative position in the path. Has value **1** for the beginning of a
       path.
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex.
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

.. return_path_all_columns_end

Return columns for cost functions
...............................................................................

.. rubric:: Used in the following

* :doc:`cost-category`
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

.. return_cost_withPoints_start

.. Note::
   When start_vid or end_vid columns have negative values, the identifier is for
   a Point.

.. return_cost_withPoints_end

Return columns for flow functions
...............................................................................

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
...............................................................................

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

.. TODO this will change, nothing is been generated automatically

.. contents::
    :local:

.. _topology:

Routing Topology
...............................................................................

.. rubric:: Overview

Typically when GIS files are loaded into the data database for use with
pgRouting they do not have topology information associated with them. To create
a useful topology the data needs to be "noded". This means that where two or
more roads form an intersection there it needs to be a node at the intersection
and all the road segments need to be broken at the intersection, assuming that
you can navigate from any of these segments to any other segment via that
intersection.

You can use the :ref:`graph analysis functions <analytics>` to help you see
where you might have topology problems in your data. If you need to node your
data, we also have a function :doc:`pgr_nodeNetwork() <pgr_nodeNetwork>` that
might work for you. This function splits ALL crossing segments and nodes them.
There are some cases where this might NOT be the right thing to do.

For example, when you have an overpass and underpass intersection, you do not
want these noded, but pgr_nodeNetwork does not know that is the case and will
node them which is not good because then the router will be able to turn off the
overpass onto the underpass like it was a flat 2D intersection. To deal with
this problem some data sets use z-levels at these types of intersections and
other data might not node these intersection which would be ok.

For those cases where topology needs to be added the following functions may be
useful. One way to prep the data for pgRouting is to add the following columns
to your table and then populate them as appropriate. This example makes a lot of
assumption like that you original data tables already has certain columns in it
like ``one_way``, ``fcc``, and possibly others and that they contain specific
data values. This is only to give you an idea of what you can do with your data.

.. parsed-literal::

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

The function :doc:`pgr_createTopology <pgr_createTopology>` will create the
``vertices_tmp`` table and populate the ``source`` and ``target`` columns. The
following example populated the remaining columns. In this example, the ``fcc``
column contains feature class code and the ``CASE`` statements converts it to an
average speed.

.. parsed-literal::

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

It is common to find problems with graphs that have not been constructed fully
noded or in graphs with z-levels at intersection that have been entered
incorrectly. An other problem is one way streets that have been entered in the
wrong direction. We can not detect errors with respect to "ground" truth, but we
can look for inconsistencies and some anomalies in a graph and report them for
additional inspections.

We do not current have any visualization tools for these problems, but I have
used mapserver to render the graph and highlight potential problem areas.
Someone familiar with graphviz might contribute tools for generating images with
that.


Analyze a Graph
...............................................................................

With :doc:`pgr_analyzeGraph` the graph can be checked for errors. For example
for table "mytab" that has "mytab_vertices_pgr" as the vertices table:

.. parsed-literal::

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

.. parsed-literal::

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



For isolated road segments, for example, a segment where both ends are deadends.
you can find these with the following query:

.. parsed-literal::

    SELECT *
        FROM mytab a, mytab_vertices_pgr b, mytab_vertices_pgr c
        WHERE a.source=b.id AND b.cnt=1 AND a.target=c.id AND c.cnt=1;


If you want to visualize these on a graphic image, then you can use something
like mapserver to render the edges and the vertices and style based on ``cnt``
or if they are isolated, etc. You can also do this with a tool like graphviz, or
geoserver or other similar tools.


Analyze One Way Streets
...............................................................................

:doc:`pgr_analyzeOneWay` analyzes one way streets in a graph and identifies any
flipped segments. Basically if you count the edges coming into a node and the
edges exiting a node the number has to be greater than one.

This query will add two columns to the vertices_tmp table ``ein int`` and ``eout
int`` and populate it with the appropriate counts. After running this on a graph
you can identify nodes with potential problems with the following query.


The rules are defined as an array of text strings that if match the ``col``
value would be counted as true for the source or target in or out condition.


Example
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Lets assume we have a table "st" of edges and a column "one_way" that might have
values like:

* 'FT'    - oneway from the source to the target node.
* 'TF'    - oneway from the target to the source node.
* 'B'     - two way street.
* ''      - empty field, assume twoway.
* <NULL>  - NULL field, use two_way_if_null flag.

Then we could form the following query to analyze the oneway streets for errors.

.. parsed-literal::

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

Typically these problems are generated by a break in the network, the one way
direction set wrong, maybe an error related to z-levels or a network that is not
properly noded.

The above tools do not detect all network issues, but they will identify some
common problems. There are other problems that are hard to detect because they
are more global in nature like multiple disconnected networks. Think of an
island with a road network that is not connected to the mainland network because
the bridge or ferry routes are missing.




.. _performance:

Performance Tips
-------------------------------------------------------------------------------

.. contents::
    :local:


For the Routing functions
...............................................................................

To get faster results bound your queries to the area of interest of routing to
have, for example, no more than one million rows.

Use an inner query SQL that does not include some edges in the routing function

.. parsed-literal::

	SELECT id, source, target from edge_table WHERE
        	id < 17 and
        	the_geom  && (select st_buffer(the_geom,1) as myarea FROM  edge_table where id = 5)

Integrating the inner query to the pgRouting function:

.. parsed-literal::

    SELECT * FROM pgr_dijkstra(
	    'SELECT id, source, target from edge_table WHERE
        	id < 17 and
        	the_geom  && (select st_buffer(the_geom,1) as myarea FROM  edge_table where id = 5)',
        1, 2)




For the topology functions:
...............................................................................

When "you know" that you are going to remove a set of edges from the edges
table, and without those edges you are going to use a routing function you can
do the following:

Analize the new topology based on the actual topology:

.. parsed-literal::

	pgr_analyzegraph('edge_table',rows_where:='id < 17');

Or create a new topology if the change is permanent:

.. parsed-literal::

	pgr_createTopology('edge_table',rows_where:='id < 17');
	pgr_analyzegraph('edge_table',rows_where:='id < 17');


.. _how_contribute:

How to contribute
-------------------------------------------------------------------------------

.. rubric:: Wiki

* Edit an existing  `pgRouting Wiki
  <https://github.com/pgRouting/pgrouting/wiki>`__ page.
* Or create a new Wiki page

  * Create a page on the `pgRouting Wiki
    <https://github.com/pgRouting/pgrouting/wiki>`__
  * Give the title an appropriate name


* `Example
  <https://github.com/pgRouting/pgrouting/wiki/How-to:-Handle-parallel-edges-(KSP)>`__

.. rubric:: Adding Functionaity to pgRouting


Consult the `developer's documentation
<https://docs.pgrouting.org/doxy/2.4/index.html>`__



.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

