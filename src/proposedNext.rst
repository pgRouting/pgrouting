.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _proposedNext:

Proposed Functions for version 2.3
==================================

:ref:`proposedNext`

This are proposed functions for version 2.3.

  - They are not officially in the version 2.2 release.
  - They will likely officialy be part of the version 2.3 release:

    - The functions make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might not change.
    - Signature might not  change.
    - Functionality might not change.
    - pgTap tests have being done.
    - Needs feedback from the comunity.


Proposed Routing Functions
--------------------------

:author: Vicky Vergara

  - As part of Dijkstra Family of functions

    - :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.

  - :ref:`pgr_withPoints<withPoints>` - withPoints family functions

    - :ref:`pgr_withPoints` - Route from/to points anywhere on the graph.
    - :ref:`pgr_withPointsCost` - Costs of the shortest paths.
    - :ref:`pgr_withPointsKSP<pgr_withPointsKSP>` - K shortest paths with points.
    - :ref:`pgr_withPointsDD<pgr_withPointsDD>` - Driving distance.


.. toctree::
   :hidden: 

   ../src/dijkstra/doc/pgr_dijkstraVia
   ../src/withPoints/doc/withPoints

