.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _deprecated:

Deprecated Functions
====================

.. warning:: This functions have being deprecated



Deprecated Routing Functions
----------------------------

Deprecated on version 2.2

  - :ref:`pgr_apspJohnson<pgr_apsp_johnson>` - All Pairs Shortest Path, Johnson’s Algorithm
  - :ref:`pgr_apspWarshall<pgr_apsp_warshall>` - All Pairs Shortest Path, Floyd-Warshall Algorithm
  - :ref:`pgr_kDijkstra<pgr_kdijkstra>` - Mutliple destination Shortest Path Dijkstra
  - :ref:`pgr_drivingDistance<pgr_driving_distance_v2>` - Driving Distance (V2.0 signature)

.. toctree::
   :hidden: 

   ../src/apsp_johnson/doc/index
   ../src/apsp_warshall/doc/index
   ../src/kdijkstra/doc/index
   ../src/driving_distance/doc/dd_driving_distance_v2

Deprecated on version 2.1

 - :ref:`pgr_dijkstra<pgr_dijkstra_v2>` - Mutliple destination Shortest Path Dijkstra (V2.0 signature)
 - :ref:`pgr_ksp<pgr_ksp_v2>` - K shortest paths (V2.0 signature)

.. toctree::
   :hidden: 

   ../src/dijkstra/doc/dijkstra_v2
   ../src/ksp/doc/ksp_v2

Deprecated Developer's Auxiliary Functions
------------------------------------------


Deprecated on version 2.1

  -  :ref:`pgr_get_column_name` - to get the name of the column as is stored in the postgres administration tables.
  -  :ref:`pgr_get_table_name` - to retrieve the name of the table as is stored in the postgres administration tables.
  -  :ref:`pgr_is_column_indexed` - to check if the column is indexed.
  -  :ref:`pgr_is_column_in_table` - to check only for the existance of the column.
  -  :ref:`pgr_point_to_id` -to insert/get the id of the inserted point in a vertices table.
  -  :ref:`pgr_quote_ident` - to quotes the input text to be used as an identifier in an SQL statement string.
  -  :ref:`pgr_versionless` - to compare two version numbers.
  -  :ref:`pgr_start_point` - to get the start point of a (multi)linestring.
  -  :ref:`pgr_end_point` - to get the end point of a (multi)linestring.



.. toctree::
   :hidden: 

   ../src/common/doc/utilities/index

