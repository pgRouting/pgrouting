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
  - They likely will not be officialy be part of the version 2.3 release:

    - The functions might not make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might change.
    - Signature might change.
    - Functionality might change.
    - pgTap tests might not being done.
    - Might need c/c++ coding.
    - May lack documentation,
    - Documentation if any might need to be rewritten.
    - Documentation examples might need to be automatically generated.
    - Might need a lot of feedback from the comunity.
    - Might need a non official function of pgRouting
    - Might need a deprecated function of pgRouting


Proposed functions
------------------------------------------------

:ref:`convenience_functions`
  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network
  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
  -  :ref:`pgr_vrp_basic` - VRP One Depot

.. toctree::
  :hidden:

  ../src/convenience/doc/convenience
  ../src/label_graph/doc/pgr_labelGraph.rst
  ../src/vrppdtw/doc/pgr_gsoc_vrppdtw.rst
  ../src/pickDeliver/doc/pgr_pickDeliver.rst
  ../src/vrp_basic/doc/pgr_vrpOneDepot


Documentation only proposed functions:
---------------------------------------------

  -  :ref:`contraction` - Reduce network size using contraction techniques

.. toctree::
  :hidden:

  ../src/contraction/doc/contraction


..
    The template
    ------------

    - :ref:`pgr_funnyDijkstra`

.. toctree::
    :hidden:


..
    ../src/funnyDijkstra/doc/pgr_funnyDijkstra.rst
    ../src/vrppdtw/doc/index.rst

