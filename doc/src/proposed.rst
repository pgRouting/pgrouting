..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _Stable:

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



As part of the :ref:`dijkstra`

- :ref:`pgr_dijkstraCostMatrix` Use pgr_dijkstra to calculate a cost matrix.
- :ref:`pgr_dijkstraVia` - Use pgr_dijkstra to make a route via vertices.

.. rubric:: Families

:ref:`astar`

.. include:: aStar-family.rst
   :start-after: index from here
   :end-before: index to here

:ref:`bdAstar`

.. include:: bdAstar-family.rst
   :start-after: index from here
   :end-before: index to here

:ref:`bdDijkstra`

.. include:: bdDijkstra-family.rst
   :start-after: index from here
   :end-before: index to here

:ref:`maxFlow`

.. include:: flow-family.rst
   :start-after: index from here
   :end-before: index to here

:ref:`withPoints`

.. include:: withPoints-family.rst
   :start-after: index from here
   :end-before: index to here

.. rubric:: categories

:ref:`Cost`

.. include:: cost-category.rst
   :start-after: index from here
   :end-before: index to here

:ref:`CostMatrix`

.. include:: costMatrix-category.rst
   :start-after: index from here
   :end-before: index to here

:ref:`KSP`

.. include:: KSP-category.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
    :hidden:

    aStar-family
    bdAstar-family
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


:ref:`contraction` - Reduce network size using contraction techniques

- :ref:`pgr_contractgraph` - Reduce network size using contraction techniques


.. rubric:: Graph Analysis

-  :ref:`pgr_labelGraph` - Analyze / label  subgraphs within a network

:ref:`components` - Analyze components within a graph

-  :ref:`pgr_connectedComponents` - Return the connected components of an undirected graph
-  :ref:`pgr_strongComponents` - Return the strongly connected components of a directed graph
-  :ref:`pgr_biconnectedComponents` - Return the biconnected components of an undirected graph
-  :ref:`pgr_articulationPoints` - Return the articulation points of an undirected graph
-  :ref:`pgr_bridges` - Return the bridges of an undirected graph

.. rubric:: VRP

- :ref:`pgr_gsocvrppdtw`
- :ref:`pgr_vrp_basic`


.. toctree::
  :hidden:

  contraction-family
  flow-family
  pgr_labelGraph
  components-family
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot

.. rubric:: Graph Operations

:ref:`transformation` - Maps a given graph to a new form

.. include:: transformation-family.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
  :hidden:

  pgr_lineGraph
  pgr_lineGraphFull
  transformation-family

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
