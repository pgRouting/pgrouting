..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Components - Family of functions
===============================================================================


.. index from here

* :doc:`pgr_connectedComponents` - Connected components of an undirected graph.
* :doc:`pgr_strongComponents` - Strongly connected components of a directed graph.
* :doc:`pgr_biconnectedComponents` - Biconnected components of an undirected graph.
* :doc:`pgr_articulationPoints` - Articulation points of an undirected graph.
* :doc:`pgr_bridges` - Bridges of an undirected graph.

.. index to here


.. toctree::
    :hidden:

    pgr_connectedComponents
    pgr_strongComponents
    pgr_biconnectedComponents
    pgr_articulationPoints
    pgr_bridges

.. rubric:: Previous versions of this page

* **Supported versions:**
  current(`3.0 <https://docs.pgrouting.org/3.0/en/components-family.html>`__)
  `2.6 <https://docs.pgrouting.org/2.6/en/components-family.html>`__

* **Unsupported versions:**
  `2.5 <https://docs.pgrouting.org/2.5/en/components-family.html>`__


Parameters
-------------------------------------------------------------------------------

.. components_parameters_start

=================== ====================== ========= =========================================
Parameter           Type                   Default   Description
=================== ====================== ========= =========================================
**Edges SQL**       ``TEXT``                         Inner query as described below.
=================== ====================== ========= =========================================

.. components_parameters_end

Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query which should return a set of rows with the following columns:

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

pgr_connectedComponents & pgr_strongComponents
...............................................................................

.. include:: pgr_connectedComponents.rst
    :start-after: return_componentsV_start
    :end-before: return_componentsV_end


pgr_biconnectedComponents
...............................................................................

.. include:: pgr_biconnectedComponents.rst
    :start-after: return_componentsE_start
    :end-before: return_componentsE_end

pgr_articulationPoints
...............................................................................

.. include:: pgr_articulationPoints.rst
    :start-after: return_articulation_start
    :end-before: return_articulation_end

pgr_bridges
...............................................................................

.. include:: pgr_bridges.rst
    :start-after: return_bridges_start
    :end-before: return_bridges_end


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
