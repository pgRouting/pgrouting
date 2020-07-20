..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_contraction
===============================================================================

``pgr_contraction`` — Performs graph contraction and returns the contracted vertices and edges.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * Return columns change: ``seq`` is removed
  * Name change from ``pgr_contractGraph``
  * Bug fixes
  * **Official** function

* Version 2.3.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_contraction.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_contraction.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_contractGraph.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_contractGraph.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_contractGraph.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/contraction/doc/pgr_contractGraph.html>`__

Description
-------------------------------------------------------------------------------

Contraction reduces the size of the graph by removing some of the vertices and
edges and, for example, might add edges that represent a sequence of original
edges decreasing the total time and space used in graph algorithms.

The main Characteristics are:
  - Process is done only on edges with positive costs.
  - Does not return the full contracted graph

    - Only changes on the graph are returned

  - Currnetly there are two types of contraction methods

    - Dead End Contraction
    - Linear Contraction

  - The returned values include

    -  the added edges by linear contraction.
    -  the modified vertices by dead end contraction.

  - The returned values are ordered as follows:

    - column `id` ascending when type = `v`
    - column `id` descending when type = `e`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

The pgr_contraction function has the following signature:

.. index::
   single: contraction

.. code-block:: none

    pgr_contraction(Edges SQL, Contraction order [, max_cycles] [, forbidden_vertices] [, directed])
    RETURNS SETOF (type, id, contracted_vertices, source, target, cost)

:Example: Making a dead end contraction and a linear contraction with vertex 2 forbidden from being contracted

.. literalinclude:: doc-pgr_contraction.queries
   :start-after: -- q2
   :end-before: -- q3

Parameters
-------------------------------------------------------------------------------

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**Edges SQL**           ``TEXT``               SQL query as described in `Inner query`_
**Ccontraction Order**  ``ARRAY[ANY-INTEGER]`` Ordered contraction operations.
                                                -  1 = Dead end contraction
                                                -  2 = Linear contraction
======================= ====================== =================================================

Optional Parameters
...............................................................................

======================= ====================== ============ =====================================
Column                  Type                   Default      Description
======================= ====================== ============ =====================================
**forbidden_vertices**  ``ARRAY[ANY-INTEGER]`` Empty        Identifiers of vertices forbidden from contraction.
**max_cycles**          ``INTEGER``            :math:`1`    Number of times the contraction operations on `contraction_order` will be performed.
**directed**            ``BOOLEAN``            ``true``     * When ``true`` the graph is considered as `Directed`.
                                                            * When ``false`` the graph is considered as `Undirected`.
======================= ====================== ============ =====================================

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SETOF  (type, id, contracted_vertices, source, target, cost)

The function returns a single row. The columns of the row are:

============================ =================   ===================================================================
Column                       Type                Description
============================ =================   ===================================================================
**type**                     ``TEXT``            Type of the `id`.
                                                  - **'v'** when the row is a vertex.
                                                  - **'e'** when the row is an edge.
**id**                       ``BIGINT``          All numbers on this column are ``DISTINCT``
                                                  * When ``type`` = **'v'**.

                                                    * Identifier of the modified vertex.
                                                  * When ``type`` = **'e'**.

                                                    - Decreasing sequence starting from **-1**.

                                                    - Representing a pseudo `id` as is not incorporated in the set of original edges.
**contracted_vertices**      ``ARRAY[BIGINT]``   Array of contracted vertex identifiers.
**source**                   ``BIGINT``          * When ``type`` = **'v'**: :math:`-1`
                                                 * When ``type`` = **'e'**: Identifier of the source vertex of the current edge (``source``, ``target``).
**target**                   ``BIGINT``          * When ``type`` = **'v'**: :math:`-1`
                                                 * When ``type`` = **'e'**: Identifier of the target vertex of the current edge (``source``, ``target``).
**cost**                     ``FLOAT``           * When ``type`` = **'v'**: :math:`-1`
                                                 * When ``type`` = **'e'**: Weight of the current edge (``source``, ``target``).
============================ =================   ===================================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Only dead end contraction

.. literalinclude:: doc-pgr_contraction.queries
   :start-after: -- q3
   :end-before: -- q4

:Example: Only linear contraction

.. literalinclude:: doc-pgr_contraction.queries
   :start-after: -- q4
   :end-before: -- q5

See Also
-------------------------------------------------------------------------------

* :doc:`contraction-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

