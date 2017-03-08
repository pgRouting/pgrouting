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

- convenience

  -  :ref:`pgr_point_to_edgenode` - convert a point geometry to a ``vertex_id`` based on closest edge.

- graph analysis

  -  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

- Vehicle Routing Problems

  -  :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
  -  :ref:`pgr_vrp_basic` - VRP One Depot


.. toctree::

  contraction
  pgr_contractGraph
  pgr_textToPoints
  pgr_pointsToVids
  pgr_labelGraph
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot


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

