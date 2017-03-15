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

.. deprecated_warning_start

.. warning:: These function(s) are deprecated!!!

    * Are no longer supported,
    * May be removed from future versions.
    * May have been replaced by new functions.
    * All code that uses the functions should be converted to use its replacement if one exists. 

.. deprecated_warning_end

Deprecated on version 2.5
----------------------------

Routing functions
...................

- pgr_bdastar - See new signatures of :ref:`pgr_bdAstar`
- :ref:`pgr_maxFlowBoykovKolmogorov` - See new signatures of :ref:`pgr_boykovKolmogorov`
- :ref:`pgr_maxFlowEdmondsKarp` - See new signatures of :ref:`pgr_edmondsKarp`
- :ref:`pgr_maxFlowPushRelabel` - See new signatures of :ref:`pgr_pushRelabel`
- :ref:`pgr_maximumCardinalityMatching` - See new signatures of :ref:`pgr_maxCardinalityMatch`

..
   ../src/max_flow/doc/pgr_maxFlowBoykovKolmogorov.rst
   ../src/max_flow/doc/pgr_maxFlowEdmondsKarp.rst
   ../src/max_flow/doc/pgr_maxFlowPushRelabel.rst
   ../src/max_flow/doc/pgr_maximumCardinalityMatching.rst

Deprecated on version 2.4
----------------------------

Routing functions
...................

- :ref:`bd_dijkstra_v2` - See new signatures of :ref:`pgr_bdDijkstra`

Auxiliary functions
...................

- :ref:`pgr_points_to_vids`

..
   ../src/bd_dijkstra/doc/pgr_bdDijkstra-v2
   ../src/convenience/doc/pgr_pointsToVids

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


..
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


..
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

-  :ref:`pgr_getColumnName`
-  :ref:`pgr_getTableName`
-  :ref:`pgr_isColumnCndexed`
-  :ref:`pgr_isColumnInTable`
-  :ref:`pgr_quote_ident`
-  :ref:`pgr_versionless`
-  :ref:`pgr_startPoint`
-  :ref:`pgr_endPoint`
-  :ref:`pgr_pointToId`


..
   ../src/dijkstra/doc/pgr_dijkstra_v2
   ../src/ksp/doc/pgr_ksp_v2
   ../src/driving_distance/doc/pgr_drivingDistance_v2
   ../src/common/doc/utilities/get_column_name
   ../src/common/doc/utilities/get_table_name
   ../src/common/doc/utilities/is_column_indexed
   ../src/common/doc/utilities/is_column_in_table
   ../src/common/doc/utilities/quote_ident
   ../src/common/doc/utilities/versionless
   ../src/common/doc/utilities/start_point
   ../src/common/doc/utilities/end_point
   ../src/common/doc/utilities/point_to_id


