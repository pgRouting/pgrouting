..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Driving Distance - Category
===============================================================================

.. index from here

* :doc:`pgr_drivingDistance` - Driving Distance based on Dijkstra's algorithm
* :doc:`pgr_primDD` - Driving Distance based on Prim's algorithm
* :doc:`pgr_kruskalDD` - Driving Distance based on Kruskal's algorithm
* Post pocessing

  * :doc:`pgr_alphaShape` - Alpha shape computation

.. index to here

.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. index proposed from here

* :doc:`pgr_withPointsDD` - Driving Distance based on pgr_withPoints

.. index proposed to here

.. toctree::
    :hidden:

    pgr_alphaShape.rst


Calculate nodes that are within a distance.

.. dd_traits_start

* Extracts all the nodes that have costs less than or equal to the value
  distance.
* The edges extracted will conform to the corresponding spanning tree.
* Edge :math:`(u, v)` will not be included when:

  * The distance from the **root** to :math:`u` > limit distance.
  * The distance from the **root** to :math:`v` > limit distance.
  * No new nodes are created on the graph, so when is within the limit and is
    not within the limit, the edge is not included.

.. dd_traits_end

Parameters
-------------------------------------------------------------------------------

.. mst-dd-params_start

.. list-table::
   :width: 81
   :widths: 14 20 46
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - Edges SQL as described below.
   * - **Root vid**
     - ``BIGINT``
     - Identifier of the root vertex of the tree.
   * - **Root vids**
     - ``ARRAY[ANY-INTEGER]``
     - Array of identifiers of the root vertices.

       - :math:`0` values are ignored
       - For optimization purposes, any duplicated value is ignored.
   * - **distance**
     - ``FLOAT``
     - Upper limit for the inclusion of a node in the result.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERIC: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. mst-dd-params_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
