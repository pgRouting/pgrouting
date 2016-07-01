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


Proposed functions: Proposed by Steve Woodbridge
------------------------------------------------

:ref:`convenience_functions`
  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.
  -  :ref:`pgr_flip_edges` - flip the edges in an array of geometries so the connect end to end.
  -  :ref:`pgr_text_to_points` - convert a string of ``x,y;x,y;...`` locations into point geometries.
  -  :ref:`pgr_points_to_vids` - convert an array of point geometries into vertex ids.
  -  :ref:`pgr_points_to_dmatrix` - Create a distance matrix from an array of points.
  -  :ref:`pgr_vids_to_dmatrix` - Create a distance matrix from an array of ``vertix_id``.
  -  :ref:`pgr_vids_to_dmatrix2` - Create a distance matrix from an array of ``vertix_id``.

Proposed functions: Proposed by Zia Mohammed
---------------------------------------------

  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

.. toctree::
  :hidden:

  ../src/label_graph/doc/pgr_labelGraph.rst

.. toctree::
   :hidden:

   ../src/convenience/doc/convenience

Proposed functions: Proposed by GSoc Student
---------------------------------------------

  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)

.. toctree::
    :hidden:

    ../src/vrppdtw/doc/pgr_gsoc_vrppdtw.rst
    ../src/pickDeliver/doc/pgr_pickDeliver.rst


Proposed functions: Proposed by Rohith Reddy
---------------------------------------------

  -  :ref:`contraction` - Reduce network size using contraction techniques

.. toctree::
  :hidden:

  ../src/contraction/doc/contraction.rst

.. toctree::
  :hidden:

  ../src/contraction/doc/contraction

Experimental functions: by GSoC Students
----------------------------------------

  -  :ref:`pgr_vrp_basic` - VRP One Depot

..
    The template
    ------------

    - :ref:`pgr_funnyDijkstra`

.. toctree::
    :hidden:

    ../src/vrp_basic/doc/pgr_vrpOneDepot

..
    ../src/funnyDijkstra/doc/pgr_funnyDijkstra.rst
    ../src/vrppdtw/doc/index.rst

