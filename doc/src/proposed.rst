..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/

.. _stable:

Stable Proposed Functions
==================================

..  stable-begin-warning

.. warning:: Proposed functions for next mayor release.

  - They are not officially in the current release.
  - They will likely officially be part of the next mayor release:

    - The functions make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might not change. (But still can)
    - Signature might not change. (But still can)
    - Functionality might not change. (But still can)
    - pgTap tests have being done. But might need more.
    - Documentation might need refinement.

..  stable-end-warning



As part of the :doc:`dijkstra-family`

- :doc:`pgr_dijkstraCostMatrix` Use pgr_dijkstra to calculate a cost matrix.
- :doc:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.

.. rubric:: Families

:doc:`bdDijkstra-family`

.. include:: bdDijkstra-family.rst
   :start-after: index from here
   :end-before: index to here


:doc:`withPoints-family`

.. include:: withPoints-family.rst
   :start-after: index from here
   :end-before: index to here

.. rubric:: categories

:doc:`cost-category`

.. include:: cost-category.rst
   :start-after: proposed index start
   :end-before: proposed index end

:doc:`costMatrix-category`

.. include:: costMatrix-category.rst
   :start-after: proposed index start
   :end-before: proposed index end

:doc:`KSP-category`

.. include:: KSP-category.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
    :hidden:

    bdDijkstra-family
    withPoints-family
    cost-category
    costMatrix-category
    KSP-category


.. _proposed:

Experimental Functions
===============================================================================


..  begin-warn-expr

.. warning:: Experimental functions

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

:doc:`costFlow-family`

.. include:: costFlow-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`ChPP-family`

.. include:: ChPP-family.rst
   :start-after: index from here
   :end-before: index to here


:doc:`contraction-family` - Reduce network size using contraction techniques

- :doc:`pgr_contractGraph` - Reduce network size using contraction techniques



.. rubric:: Graph Analysis

:doc:`components-family` - Analyze components within a graph

-  :doc:`pgr_connectedComponents` - Return the connected components of an undirected graph
-  :doc:`pgr_strongComponents` - Return the strongly connected components of a directed graph
-  :doc:`pgr_biconnectedComponents` - Return the biconnected components of an undirected graph
-  :doc:`pgr_articulationPoints` - Return the articulation points of an undirected graph
-  :doc:`pgr_bridges` - Return the bridges of an undirected graph

.. rubric:: VRP

- :doc:`pgr_gsoc_vrppdtw`
- :doc:`pgr_vrpOneDepot`

.. rubric:: Shortest Path Algorithms

- :doc:`pgr_bellmanFord` - Return shortest path for a undirected or directed  negative weighted graph.
- :doc:`pgr_dagShortestPath` - Return shortest path for directed acyclic graph
- :doc:`pgr_kruskal` - TBD
- :doc:`pgr_prim` - TBD
- :doc:`pgr_stoerWagner` - TBD

.. toctree::
  :hidden:

  contraction-family
  components-family
  costFlow-family
  ChPP-family
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot
  pgr_bellmanFord
  pgr_dagShortestPath
  pgr_kruskal
  pgr_prim
  pgr_stoerWagner


.. rubric:: Graph Operations

:doc:`transformation-family` - Maps a given graph to a new form

.. include:: transformation-family.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
  :hidden:

  transformation-family
  pgr_lineGraph
  pgr_lineGraphFull

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
