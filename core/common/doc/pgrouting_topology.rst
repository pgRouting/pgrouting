==================
pgRouting Topology
==================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: This is a collection of tools for building topology needed
for graphs. This is primarily used when loading GIS data like shapefile.
:Copyright: Stephen Woodbridge. This document is released under the MIT-X license.

Topology Overview
=================

Typically when GIS files are loaded into the data database for use with
pgRouting they do not have topology information assocatired with. For 
those cases where topology needs to be added the following functions may
be useful. One way to prep the data for pgRouting is to add the following
columns to your table and then populate them as appropriate.

    ::

    alter table st
        add column source integer,
        add column target integer,
        add column cost_len double precision,
        add column cost_time double precision,
        add column rcost_len double precision,
        add column rcost_time double precision,
        add column x1 double precision,
        add column y1 double precision,
        add column x2 double precision,
        add column y2 double precision,
        add column to_cost double precision,
        add column rule text,
        add column isolated integer;

    select pgr_assign_vertex_id('st', 0.000001, 'the_geom', 'gid');

The function pgr_assign_vertex_id() will create the "vertices_tmp" table
and populate the "source" and "target" columns. The following example
populated the remaining columns. In this example, the "fcc" column contains
feature class code and the CASE statements converts it to an average speed.

    ::

    update st set x1 = x(st_startpoint(the_geom)),
                  y1 = y(st_startpoint(the_geom)),
                  x2 = x(st_endpoint(the_geom)),
                  y2 = y(st_endpoint(the_geom)),
      cost_len  = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      rcost_len = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]'),
      len_km = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')/1000.0,
      len_miles = st_length_spheroid(the_geom, 'SPHEROID["WGS84",6378137,298.25728]')/1000.0*0.6213712,
      speed_mph = case when fcc='A10' then 65
                       when fcc='A15' then 65
                       when fcc='A20' then 55
                       when fcc='A25' then 55
                       when fcc='A30' then 45
                       when fcc='A35' then 45
                       when fcc='A40' then 35
                       when fcc='A45' then 35
                       when fcc='A50' then 25
                       when fcc='A60' then 25
                       when fcc='A61' then 25
                       when fcc='A62' then 25
                       when fcc='A64' then 25
                       when fcc='A70' then 15
                       when fcc='A69' then 10
                       else null end,
      speed_kmh = case when fcc='A10' then 104
                       when fcc='A15' then 104
                       when fcc='A20' then 88
                       when fcc='A25' then 88
                       when fcc='A30' then 72
                       when fcc='A35' then 72
                       when fcc='A40' then 56
                       when fcc='A45' then 56
                       when fcc='A50' then 40
                       when fcc='A60' then 50
                       when fcc='A61' then 40
                       when fcc='A62' then 40
                       when fcc='A64' then 40
                       when fcc='A70' then 25
                       when fcc='A69' then 15
                       else null end;

    -- Update the cost infomation based on oneway streets

    update st set
      cost_time = case
        when one_way='TF' then 10000.0
        else cost_len/1000.0/speed_kmh::numeric*3600.0
        end,
      rcost_time = case
        when one_way='FT' then 10000.0
        else cost_len/1000.0/speed_kmh::numeric*3600.0
        end;

    -- clean up the database because we have updated a lot of records

    vacuum analyze verbose st;

Now your database should be ready to use any (most?) of the pgRouting
 algorithms.

Functions
=========

.. function:: pgr_assign_vertex_id(geom_table varchar, tolerance double precision, geo_cname varchar, gid_cname varchar)

   Fill the source and target_id column for all lines. All line ends
   with a distance less than tolerance, are assigned the same id.
   This function assumes the "source" and "target" columns exist on
   table "geom_table" and are of type integer or bigint.
   * geom_table - name of the edge table
   * tolerance  - tolerance distance used to matching nodes
   * geo_cname  - the geometry column name
   * gid_cname  - the edge unique identifier (INTEGER or BIGINT)



.. function:: pgr_point_to_id(p, tolerance)

   *This function should not be used directly. Use assign_vertex_id instead.*

   Inserts a point into a temporary vertices table, and return an id
   of a new point or an existing point. Tolerance is the minimal distance
   between existing points and the new point to create a new point.
   * Returns BIGINT


