.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _proposed:

Experimental and Proposed functions
===============================================================================

:ref:`proposed`


..
   begin-warning

.. warning:: These are proposed functions

  - They are not officially of the current release.
  - They likely will not be officially be part of the next release:

    - The functions might not make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might change.
    - Signature might change.
    - Functionality might change.
    - pgTap tests might be missing.
    - Might need c/c++ coding.
    - May lack documentation.
    - Documentation if any might need to be rewritten.
    - Documentation examples might need to be automatically generated.
    - Might need a lot of feedback from the comunity.
    - Might depend on a proposed function of pgRouting
    - Might depend on a deprecated function of pgRouting

..
   end-warning


Proposed functions
------------------------------------------------

- :ref:`contraction` - Reduce network size using contraction techniques

  - :ref:`pgr_contractgraph` - Reduce network size using contraction techniques

- :ref:`maxFlow`

  - :ref:`pgr_maxFlowPushRelabel` - Maximum flow using push&relabel algorithm.
  - :ref:`pgr_maxFlowEdmondsKarp` - Maximum flow using Edmonds&Karp algorithm.
  - :ref:`pgr_maxFlowBoykovKolmogorov` - Maximum flow using Boykov&Kolmogorov algorithm.
  
- :ref:`maxFlowApplications`

  - :ref:`pgr_maximumCardinalityMatching` - Calculates a maximum cardinality matching.
  - :ref:`pgr_edgeDisjointPaths` - Calculates edge disjoint paths.

- convenience

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.

- graph analysis

  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

- Vehicle Routing Problems

  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
  -  :ref:`pgr_vrp_basic` - VRP One Depot


.. toctree::
  :hidden:

  ../src/contraction/doc/contraction
  ../src/contraction/doc/pgr_contractGraph
  ../src/max_flow/doc/maxFlow
  ../src/max_flow/doc/maxFlowApplications

  ../src/convenience/doc/pgr_pointToEdgeNode
  ../src/convenience/doc/pgr_pointsToVids
  ../src/label_graph/doc/pgr_labelGraph
  ../src/vrppdtw/doc/pgr_gsoc_vrppdtw
  ../src/vrp_basic/doc/pgr_vrpOneDepot

  ../src/pickDeliver/doc/pgr_pickDeliver







..  
  The template
   ------------
    - :ref:`pgr_funnyDijkstra`
    .. toctree::
    :hidden:
    ..
     ../src/funnyDijkstra/doc/pgr_funnyDijkstra.rst

