.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _topology:

Routing Topology
===============================================================================

:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Copyright: Stephen Woodbridge. This document is released under the MIT-X license.


Overview
-------------------------------------------------------------------------------

Typically when GIS files are loaded into the data database for use with pgRouting they do not have topology information assocatired with. For data to create a useful topology it needs to be "noded". This means that where two or more roads form an intersection there it needs to be a node at the intersection and all the road segments need to be broken at the intersection.

You can use the :ref:`graph analysis functions <common-analytics>` to help you see where you might have problems in your data. If you need to node your data, we also have a function :ref:`pgr_nodeNetwork() <pgr_node_network>` that might work for you. This function splits ALL crossing segments and nodes them. There are some cases where this might NOT be the right thing to do.

For example, when you have an overpass and underpass intersection, you do not want these noded, but pgr_nodeNetwork does not know that is the cases and will node them which is not good because then the router will be able to turn off the overpass onto the underpass like it was a flat 2D intersection.

For those cases where topology needs to be added the following functions may be useful. One way to prep the data for pgRouting is to add the following columns to your table and then populate them as appropriate.

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

    UPDATE edge_table SET x1 = x(st_startpoint(the_geom)),
                          y1 = y(st_startpoint(the_geom)),
                          x2 = x(st_endpoint(the_geom)),
                          y2 = y(st_endpoint(the_geom)),
      cost_len  = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      rcost_len = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      len_km = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')/1000.0,
      len_miles = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')/1000.0*0.6213712,
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

    -- UPDATE the cost infomation based on oneway streets

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


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_create_topology`
* :ref:`pgr_node_network`
* :ref:`pgr_point_to_id`

