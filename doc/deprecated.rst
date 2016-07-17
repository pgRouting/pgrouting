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

.. warning:: These functions are deprecated!!!

    * That means they have been replaced by new functions or are no longer supported, and may be removed from future versions.
    * All code that uses the functions should be converted to use its replacement if one exists. 


Deprecated on version 2.3
----------------------------

Routing functions
...................

- :ref:`pgr_astar-V2.0` - See new signatures of :ref:`pgr_aStar`
- :ref:`pgr_tsp-V2.0` - See new signatures of :ref:`tsp`


Auxiliary functions
...................

- :ref:`pgr_flip_edges`
- :ref:`pgr_vids_to_dmatrix`
- :ref:`pgr_vids_to_dmatrix2`
- :ref:`pgr_points_to_dmatrix`
- :ref:`pgr_text_to_points`


.. toctree::
   :hidden:

   ../src/astar/doc/pgr_astar-v2
   ../src/tsp/doc/pgr_tsp-V2.0
   ../src/convenience/doc/pgr_flipEdges
   ../src/convenience/doc/pgr_vidsToDMatrix
   ../src/convenience/doc/pgr_vidsToDMatrix2
   ../src/convenience/doc/pgr_pointsToDMatrix
   ../src/convenience/doc/pgr_textToPoints


Deprecated on version 2.2
----------------------------

Routing functions
...................

- :ref:`pgr_apsp_johnson` - Replaced with :ref:`pgr_johnson` 
- :ref:`pgr_apsp_warshall` - Replaced with :ref:`pgr_floydWarshall`
- :ref:`pgr_kdijkstra` - Replaced with :ref:`pgr_dijkstraCost` and :ref:`pgr_dijkstra` (one to many)

.. toctree::
   :hidden: 

   ../src/apsp_johnson/doc/pgr_apspJohnson
   ../src/apsp_warshall/doc/pgr_apspWarshall
   ../src/kdijkstra/doc/pgr_kDijkstra


Deprecated on version 2.1
------------------------------------------

Routing functions
...................

- :ref:`pgr_dijkstra_v2` - See new signature in :ref:`pgr_dijkstra` (one to one)
- :ref:`pgr_ksp_v2` - See new signature in :ref:`pgr_ksp`
- :ref:`pgr_driving_distance_v2` - See new signature in :ref:`pgr_drivingDistance`


Auxiliary functions
...................

-  :ref:`pgr_get_column_name`
-  :ref:`pgr_get_table_name`
-  :ref:`pgr_is_column_indexed`
-  :ref:`pgr_is_column_in_table`
-  :ref:`pgr_quote_ident`
-  :ref:`pgr_versionless`
-  :ref:`pgr_start_point`
-  :ref:`pgr_end_point`



.. toctree::
   :hidden: 

   ../src/dijkstra/doc/pgr_dijkstra_v2
   ../src/ksp/doc/pgr_ksp_v2
   ../src/driving_distance/doc/pgr_drivingDistance_v2
   ../src/common/doc/utilities/get_column_name
   ../src/common/doc/utilities/get_table_name
   ../src/common/doc/utilities/is_column_indexed
   ../src/common/doc/utilities/is_column_in_table
   ../src/common/doc/utilities/point_to_id
   ../src/common/doc/utilities/quote_ident
   ../src/common/doc/utilities/versionless
   ../src/common/doc/utilities/start_point
   ../src/common/doc/utilities/end_point


