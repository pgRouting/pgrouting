..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _maxFlowApplications:

Applications of Maximum Flow
============================

  - :ref:`pgr_maximumCardinalityMatching` - Calculates a maximum cardinality matching in a graph.
  - :ref:`pgr_edgeDisjointPaths` - Calculates edge disjoint paths between two groups of vertices.

Maximum flow algorithms provide solutions to other graph problems.
These

.. toctree::
        :hidden:

        ./pgr_maximumCardinalityMatching
        ./pgr_edgeDisjointPaths


Example problems
================

Maximum cardinality matching
----------------------------

A matching or independent edge set in a graph is a set of edges without common vertices.

A maximum matching is a matching that contains the largest possible number of edges.
There may be many maximum matchings.

The :ref:`pgr_maximumCardinalityMatching` function can be used to calculate one such maximum matching.
The graph can be directed or undirected.

Edge disjoint paths
-------------------

In a undirected/directed graph, two paths are edge-disjoint(or edge-independant) if they do not have any internal edge in common.

While the number of maximum edge disjoint paths is fixed, there may be several different routes.

The :ref:`pgr_edgeDisjointPaths` function returns the maximum number of paths and possible routes.



See Also
--------

* https://en.wikipedia.org/wiki/Maximum_flow_problem#Application
