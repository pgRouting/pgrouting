.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _proposedNext:

Stable proposed Functions
==================================

..
    from-here
    :ref:`proposedNext`

..
    begin-warning

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

..
    end-warning



- As part of the :ref:`dijkstra`

  - :ref:`pgr_dijkstraCostMatrix` Use pgr_dijkstra to calculate a cost matrix.
  - :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.

- A new :ref:`withPoints`

  - :ref:`pgr_withPoints` - Route from/to points anywhere on the graph.
  - :ref:`pgr_withPointsCost` - Costs of the shortest paths.
  - :ref:`pgr_withPointsCostMatrix` - Use pgr_withPoints to calculate a cost matrix.
  - :ref:`pgr_withPointsKSP` - K shortest paths with points.
  - :ref:`pgr_withPointsDD` - Driving distance.


- A new Section

  - :ref:`CostMatrix`

- A new Section

  - ref:`astar`

..
    to-here

.. toctree::
    :hidden:

    ../src/costMatrix/doc/pgr_dijkstraCostMatrix
    ../src/dijkstra/doc/pgr_dijkstraVia

    ../src/withPoints/doc/withPoints

    ../src/costMatrix/doc/costMatrix
    ../src/astar/doc/astar

