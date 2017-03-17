..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _Stable:

Stable proposed Functions
==================================

..  stable-begin-warning

.. warning::

    These are proposed functions for next mayor release.

      - They are not officially in the current release.
      - They will likely officially be part of the next mayor release:

        - The functions make use of ANY-INTEGER and ANY-NUMERICAL
        - Name might not change. (But still can)
        - Signature might not change. (But still can)
        - Functionality might not change. (But still can)
        - pgTap tests have being done. But might need more.
        - Documentation might need refinement.

..  stable-end-warning



- As part of the :ref:`dijkstra`

  - :ref:`pgr_dijkstraCostMatrix` Use pgr_dijkstra to calculate a cost matrix.
  - :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.

- A new :ref:`withPoints`

  - :ref:`pgr_withPoints` - Route from/to points anywhere on the graph.
  - :ref:`pgr_withPointsCost` - Costs of the shortest paths.
  - :ref:`pgr_withPointsCostMatrix` - Use pgr_withPoints to calculate a cost matrix.
  - :ref:`pgr_withPointsKSP` - K shortest paths with points.
  - :ref:`pgr_withPointsDD` - Driving distance.


.. rubric:: A new Section

:ref:`CostMatrix`

.. include:: costMatrix.rst
   :start-after: index from here
   :end-before: index to here


.. rubric:: A new Section

- :ref:`astar`
 
.. include:: aStar.rst
   :start-after: index from here
   :end-before: index to here

- :ref:`pgr_aStarCost` - Costs of the shortest paths.
- :ref:`pgr_aStarCostMatrix` - Use pgr_withPoints to calculate a cost matrix.


.. toctree::
    :hidden:

    pgr_dijkstraCostMatrix
    pgr_dijkstraVia

    withPoints

    costMatrix


.. _proposed:

Experimental and Proposed functions
===============================================================================


..  begin-warn-expr

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

..  end-warn-expr


- :ref:`contraction` - Reduce network size using contraction techniques

  - :ref:`pgr_contractgraph` - Reduce network size using contraction techniques

- :ref:`maxFlow`

  - :ref:`pgr_maxFlow` - Maximum flow using push&relabel algorithm.
  - :ref:`pgr_maxFlowPushRelabel` - Detailed Maximum flow using push&relabel algorithm.
  - :ref:`pgr_maxFlowEdmondsKarp` - Detailed Maximum flow using Edmonds&Karp algorithm.
  - :ref:`pgr_maxFlowBoykovKolmogorov` - Detailed Maximum flow using Boykov&Kolmogorov algorithm.

- :ref:`maxFlowApplications`

  - :ref:`pgr_maximumCardinalityMatching` - Calculates a maximum cardinality matching.
  - :ref:`pgr_edgeDisjointPaths` - Calculates edge disjoint paths.

- convenience

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.

- graph analysis

  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

- Vehicle Routing Problems

  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
  -  :ref:`pgr_vrp_basic` - VRP One Depot


.. toctree::
  :hidden:

  contraction
  pgr_contractGraph
  maxFlow
  maxFlowApplications
  pgr_pointToEdgeNode
  pgr_labelGraph
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot








..
  The template
   ------------
    - :ref:`pgr_funnyDijkstra`
    .. toctree::
    :hidden:
    ..
     ../src/funnyDijkstra/doc/pgr_funnyDijkstra.rst

