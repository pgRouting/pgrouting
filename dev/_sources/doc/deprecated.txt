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

  - :ref:`pgr_apspJohnson<pgr_apsp_johnson>` - Replaced with :ref:`pgr_johnson` 
  - :ref:`pgr_apspWarshall<pgr_apsp_warshall>` - Replaced with :ref:`pgr_floydWarshall`
  - :ref:`pgr_kDijkstra<pgr_kdijkstra>` - Replaced with :ref:`pgr_dijkstraCost`
  - :ref:`pgr_drivingDistance<pgr_driving_distance_v2>` - Driving Distance (V2.0 signature). See new signature :ref:`pgr_drivingDistance`

.. toctree::
   :hidden: 

   ../src/apsp_johnson/doc/pgr_apspJohnson
   ../src/apsp_warshall/doc/pgr_apspWarshall
   ../src/kdijkstra/doc/pgr_kDijkstra
   ../src/driving_distance/doc/pgr_drivingDistance_v2

Deprecated on version 2.1

 - :ref:`pgr_dijkstra<pgr_dijkstra_v2>` - Mutliple destination Shortest Path Dijkstra (V2.0 signature)
 - :ref:`pgr_ksp<pgr_ksp_v2>` - K shortest paths (V2.0 signature)

.. toctree::
   :hidden: 

   ../src/dijkstra/doc/pgr_dijkstra_v2
   ../src/ksp/doc/pgr_ksp_v2

Deprecated Developer's Auxiliary Functions
------------------------------------------


Deprecated on version 2.1

  -  :ref:`pgr_get_column_name` - to get the name of the column as is stored in the postgres administration tables.
  -  :ref:`pgr_get_table_name` - to retrieve the name of the table as is stored in the postgres administration tables.
  -  :ref:`pgr_is_column_indexed` - to check if the column is indexed.
  -  :ref:`pgr_is_column_in_table` - to check only for the existance of the column.
  -  :ref:`pgr_quote_ident` - to quotes the input text to be used as an identifier in an SQL statement string.
  -  :ref:`pgr_versionless` - to compare two version numbers.
  -  :ref:`pgr_start_point` - to get the start point of a (multi)linestring.
  -  :ref:`pgr_end_point` - to get the end point of a (multi)linestring.



.. toctree::
   :hidden: 

   ../src/common/doc/utilities/index

