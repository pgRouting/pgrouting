..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: withPoints Family ; pgr_withPointsCostMatrix
   single: With Points Category ; pgr_withPointsCostMatrix
   single: Cost Matrix Category ; pgr_withPointsCostMatrix
   single: withPointsCostMatrix

|

``pgr_withPointsCostMatrix``
===============================================================================

``pgr_withPointsCostMatrix`` - Calculates a cost matrix using
:doc:`pgr_withPoints`.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.
* Output columns standardized to |matrix-result|
* Signature change: ``driving_side`` parameter changed from named optional to
  unnamed positional.

  - Directed graph valid values: ``l`` or ``L`` and ``r``, ``R``
  - Undirected graph valid values: ``b`` or ``B``

.. rubric:: Version 2.2.0

* New proposed function.

Description
-------------------------------------------------------------------------------

Modify the graph to include points defined by points_sql.
Using Dijkstra algorithm, return calculate and return a cost matrix.

The main characteristics are:

- Process is done only on edges with positive costs.
- It does not return a path.
- Returns the sum of the costs of the shortest path for pair combination of
  vertices in the modified graph.

  - The returned values are in the form of a set of |matrix-result|.

- Vertices of the graph are:

  - **positive** when it belongs to the edges sql
  - **negative** when it belongs to the points sql

- Values are returned when there is a path.

  - When the starting vertex and ending vertex are the same, there is no path.

    - The `agg_cost` in the non included values `(v, v)` is `0`

  - When the starting vertex and ending vertex are the different and there is no
    path:

    - The `agg_cost` in the non included values `(u, v)` is :math:`\infty`

  - If the values returned are stored in a table, the unique index would be the
    pair: `(start_vid, end_vid)`.

  - For **undirected** graphs, the results are **symmetric**.

    - The `agg_cost` of `(u, v)` is the same as for `(v, u)`.

- For optimization purposes, any duplicated value in the input arrays of **start vids** or
  **end vids** or are ignored.

- The returned values are ordered:

  - `start_vid` ascending
  - `end_vid` ascending

- Running time: :math:`O(|start\_vids|\times(V \log V + E))`

.. include:: dijkstra-family.rst
    :start-after: dijkstra_description_start
    :end-before: dijkstra_description_end

.. include:: costMatrix-category.rst
    :start-after: costMatrix_details_start
    :end-before: costMatrix_details_end

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_withPointsCostMatrix(`Edges SQL`_, `Points SQL`_, **start vids**, **driving side**  [**options**])
   | **options:** ``[directed]``

   | Returns set of |matrix-result|
   | OR EMPTY SET

.. note:: There is no **details** flag, unlike the other members of the
   withPoints family of functions.

:Example: Cost matrix for points :math:`\{1, 6\}` and vertices :math:`\{10,
          11\}` on an **undirected** graph

* Returning a **symmetrical** cost matrix
* Using the default ``side`` value on the **points_sql** query
* Using the default ``driving_side`` value

.. literalinclude:: withPointsCostMatrix.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: costMatrix-category.rst
    :start-after: costMatrix_withPoints_parameters_start
    :end-before: costMatrix_withPoints_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

With points optional parameters
...............................................................................

.. include:: withPoints-family.rst
    :start-after: withPoints_optionals_start
    :end-before: * - ``details

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Points SQL
...............................................................................

.. include:: withPoints-category.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_withPoints_end

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Use :doc:`pgr_findCloseEdges` in the `Points SQL`_.
...............................................................................

Find the matrix cost of the routes from vertex :math:`1` and the two closest
locations on the graph of point `(2.9, 1.8)`.

.. literalinclude:: withPointsCostMatrix.queries
    :start-after: -- q3
    :end-before: -- q4

* Point :math:`-1` corresponds to the closest edge from point `(2.9, 1.8)`.
* Point :math:`-2` corresponds to the next close edge from point `(2.9, 1.8)`.

Use with :doc:`pgr_TSP`.
...............................................................................

.. literalinclude:: withPointsCostMatrix.queries
   :start-after: -- q2
   :end-before: -- q3

See Also
-------------------------------------------------------------------------------

* :doc:`withPoints-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
