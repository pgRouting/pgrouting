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

These are proposed functions

  - They are not officially of the current release.
  - They likely will not be officialy be part of the next release:

    - The functions might not make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might change.
    - Signature might change.
    - Functionality might change.
    - pgTap tests might be missing.
    - Might need c/c++ coding.
    - May lack documentationr.
    - Documentation if any might need to be rewritten.
    - Documentation examples might need to be automatically generated.
    - Might need a lot of feedback from the comunity.
    - Might depend on a proposed function of pgRouting
    - Might depend on a deprecated function of pgRouting


Proposed functions
------------------------------------------------

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network
  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
  -  :ref:`pgr_vrp_basic` - VRP One Depot

.. toctree::
  :hidden:

  pgr_pointToEdgeNode
  pgr_textToPoints
  pgr_pointsToVids
  pgr_labelGraph
  pgr_gsoc_vrppdtw
  pgr_pickDeliver
  pgr_vrpOneDepot


Documentation only proposed functions:
---------------------------------------------

  -  :ref:`contraction` - Reduce network size using contraction techniques

.. toctree::
  :hidden:

  contraction


..
    The template
    ------------

    - :ref:`pgr_funnyDijkstra`

.. toctree::
    :hidden:


..
    ../src/funnyDijkstra/doc/pgr_funnyDijkstra.rst
    ../src/vrppdtw/doc/index.rst

