/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/
..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _convenience_functions:

Convenience Functions
===============================================================================

The following functions are general purpose convenience functions that might
be useful when building a larger application or handling input from say an
Ajax handler.

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_flip_edges` - flip the edges in an array of geometries so the connect end to end.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.

Distance Matrix Functions
===============================================================================

These function my be helpful when you need to create or manipulate distance matricies, like for TSP or VRP related problems.

 - :ref:`pgr_points_to_dmatrix` - Create a distance matrix from an array of points.
 - :ref:`pgr_vids_to_dmatrix` - Create a distance matrix from an array of ``vertix_id``.
 - :ref:`pgr_vids_to_dmatrix2` - Create a distance matrix from an array of ``vertix_id``.

.. toctree::
  :hidden:

  pgr_pointToEdgeNode - convert a point geometry to a ``vertex_id`` based on closest edge. <point_to_edgenode>
  pgr_flipEdges - flip the edges in an array of geometries so the connect end to end. <flip_edges>
  pgr_textToPoints - convert a string of ``x,y;x,y;...`` locations into point geometries. <text_to_points>
  pgr_pointsToVids - convert an array of point geometries into ``node_id``s. <points_to_vids>

  pgr_pointsToDMatrix - Create a distance matrix from an array of points. <points_to_dmatrix>
  pgr_vidsToDMatrix - Create a distance matrix from an array of ``vertix_id``. <vids_to_dmatrix>
  pgr_vidsToDMatrix - Create a distance matrix from an array of ``vertix_id``. <vids_to_dmatrix2>

