.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _tutorial:

Getting Started
===============================================================================

This is a simple guide to walk you through the steps of getting started
with pgRouting. In this guide we will cover:

    * How to create a database to use for our project
    * How to load some data
    * How to build a topology
    * How to check your graph for errors
    * How to compute a route
    * How to use other tools to view your graph and route
    * How to create a web app

How to create a database to use for our project
-------------------------------------------------------------------------------

The first thing we need to do is create a database and load pgrouting in 
the database. Typically you will create a database for each project. Once 
you have a database to work in, your can load your data and build your
application in that database. This makes it easy to move your project
later if you want to to say a production server.

For Postgresql 9.1 and later versions

.. code-block:: bash

	createdb mydatabase
	psql mydatabase -c "create extension postgis"
	psql mydatabase -c "create extension pgrouting"


For older versions of postgresql

.. code-block:: bash

	createdb -T template1 template_postgis
	psql template_postgis -c "create language plpgsql"
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis-1.5/postgis.sql
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis-1.5/spatial_ref_sys.sql
	psql template_postgis -f /usr/share/postgresql/8.3/contrib/postgis_comments.sql

	createdb -T template_postgis template_pgrouting
	psql template_pgrouting -f /usr/share/pgrouting/pgrouting--1.0.7.sql

	createdb -T template_pgrouting mydatabase


How to load some data
-------------------------------------------------------------------------------

How you load your data will depend in what form it comes it. There are
various OpenSource tools that can help you, like:

:shp2pgsql: - this is the postgresql shapefile loader
:ogr2ogr: - this is a vector data conversion utility
:osm2pgsql: - this is a tool for loading OSM data into postgresql

So these tools and probably others will allow you to read vector data and
and can load that data into your database as a table of some kind. At this
point you need to know a little about your data structure and content. One easy
way to browse you data table is with pgAdmin3 or phpPgAdmin.


How to build a topology
-------------------------------------------------------------------------------

Next we need to build a topology for our street data. What this means is that
for any given edge in your street data the ends of that edge will be connected
to a unique node and to other edges that are also connected to that same unique
node. Once all the edges are connected to nodes we have a graph that can be
used for routing with pgrouting. We provide a tools the will help with this:

.. code-block:: sql

    select pgr_createTopology('myroads', 0.000001, 'the_geom', 'gid');

See :ref:`pgr_create_topology` for more information.


How to check your graph for errors
-------------------------------------------------------------------------------

There are lots of possible sources for errors in a graph. The data that you
started with may not have been designed with routing in mind. A graph as some
very specific requirments. One it that it is *NODED*, this means that except
for some very specific use cases, each road segments starts and ends at a node
and that in general is does not cross another road segment that it should be
connected to.

There can be other errors like the direction of a one-way street being entered
in the wrong direction. We do not have tools to search for all possible errors
but we have some basic tools that might help.

.. code-block:: sql

    select pgr_analyzegraph('myroads', 'the_geom', 0.000001);
    select pgr_analyzeoneway('myroads', 'direction', s_in_rules, s_out_rules, t_in_rules, t_out_rules)

See :ref:`common-analytics` for more information.

If your data needs to be *NODED*, we have a tool that can help for that also.

See :ref:`pgr_node_network` for more information.


How to compute a route
-------------------------------------------------------------------------------

Once you have all the prep work done above, computing a route is fairly easy.
We have a lot of different algorithms but they can work with your prepared
road network. The general form of a route query is:

.. code-block:: sql

    select pgr_<algorithm>(<SQL for edges>, start, end, <additonal options>)

As you can see this is fairly straight forward and you can look and the 
specific algorithms for the details on how to use them. What you get as a
result from these queries will be a set of record of type ``pgr_costResult``
or ``pgr_geomResult``. These results have information like edge id and/or the
node id along with the cost or geometry for the step in the path from *start*
to *end*. Using the ids you can join these result back to your edge table
to get more information about each step in the path.

 * See also :ref:`type_cost_result` and :ref:`type_geom_result`.

How to use other tools to view your graph and route
-------------------------------------------------------------------------------

TBD

How to create a web app
-------------------------------------------------------------------------------

TBD


