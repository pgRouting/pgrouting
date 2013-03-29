.. _common-matching:

=========================
pgRouting Matching
=========================
:Author: Stephen Woodbridge <woodbri@swoodbridge.com>
:Date: $Date: 2013-03-22 20:14:00 -5000 (Fri, 22 Mar 2013) $
:Revision: $Revision: 0000 $
:Description: This is a collection of functions for matching points and vectors.
:Copyright: Stephen Woodbridge. This is released under the MIT-X license.


Data Type Definitions
=====================

    CREATE TYPE link_point AS (id integer, name varchar);

Matching Functions
==================

.. function:: pgr_find_nearest_link_within_distance(point, distance, tbl)
   :module: pgrouting_matching.sql

   This function finds nearest link to a given node
   * point - text representation of point
   * distance - function will search for a link within this distance
   * tbl - table name
   * returns integer


.. function:: pgr_find_nearest_node_within_distance(point, distance, tbl)

   This function finds nearest node to a given node
   * point - text representation of point
   * distance - function will search for a link within this distance
   * tbl - table name
   * returns integer

.. function:: pgr_find_node_by_nearest_link_within_distance(point, distance, tbl)

   This function finds nearest node as a source or target of the
   * nearest link
   * point - text representation of point
   * distance - function will search for a link within this distance
   * tbl - table name
   * returns link_point



.. function:: pgr_match_line_as_geometry(tbl, line, distance, distance2, dir, rc)

   This function matches given line to the existing network.
   * tbl - table name
   * line - line to match
   * distance - distance for nearest node search
   * distance2 - distance for shortest path search
   * dir - true if your network graph is directed
   * rc - true if you have a reverse_cost column
   * Returns set of edges as geometry (GEOMS).


.. function:: pgr_match_line(tbl, line, distance, distance2, dir, rc)

   This function matches given line to the existing network.
   * tbl - table name
   * line - line to match
   * distance - distance for nearest node search
   * distance2 - distance for shortest path search
   * dir - true if your network graph is directed
   * rc - true if you have a reverse_cost column
   * Returns set of edges (PATH_RESULT).


.. function:: pgr_match_line_as_linestring(tbl, line, distance, distance2, dir, rc)

   This function matches given line to the existing network.
   * tbl - table name
   * line - line to match
   * distance - distance for nearest node search
   * distance2 - distance for shortest path search
   * dir - true if your network graph is directed
   * rc - true if you have a reverse_cost column
   * Returns single (multi)linestring (GEOMETRY).

