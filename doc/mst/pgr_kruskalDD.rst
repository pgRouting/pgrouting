..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_kruskalDD - Experimental
===============================================================================

``pgr_kruskalDD`` — Returns the catchament nodes using Kruskal algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental

  * v3.0.0

Description
-------------------------------------------------------------------------------

Using Kruskal algorithm, extracts the nodes that have aggregate costs less than
or equal to the value distance.

**The main Characteristics are:**

.. include:: kruskal-family.rst
   :start-after: kruskal-description-start
   :end-before: kruskal-description-end

- Returned tree nodes from a root vertex are on Depth First Search order.
- Depth First Search Running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vid, distance)
    pgr_kruskal(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: kruskalDD(Single vertex) - Experimental

Single vertex
...............................................................................

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vid, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertex :math:`2` with :math:`agg\_cost <= 3.5`

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: kruskalDD(Multiple vertices) - Experimental

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_kruskalDD(edges_sql, root_vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`agg\_cost <= 3.5`;

.. literalinclude:: doc-pgr_kruskalDD.queries
   :start-after: -- q2
   :end-before: -- q3

.. mst-information-start

Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**edges_sql**       ``TEXT``               SQL query described in `Inner query`_.
**root_vid**        ``BIGINT``             Identifier of the root vertex of the tree.

                                           - Used on `Single vertex`_
                                           - When :math:`0` gets the spanning forest
                                             starting in aleatory nodes for each tree.

**root_vids**       ``ARRAY[ANY-INTEGER]`` Array of identifiers of the root vertices.

                                           - Used on `Multiple vertices`_
                                           - :math:`0` values are ignored
                                           - For optimization purposes, any duplicated value is ignored.

**distance**        ``NUMERIC``            Upper limit for the inclusion of the node in the result.

                                           - When ``Negative`` **Throws error**
=================== ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. rubric::edges_sql

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result columns start

Returns SET OF ``(seq, depth, start_vid, node, edge, cost, agg_cost)``

===============  =========== ====================================================
Column           Type        Description
===============  =========== ====================================================
**seq**          ``BIGINT``  Sequential value starting from :math:`1`.
**depth**        ``BIGINT``  Depth of the ``node``.

                             - :math:`0`  when ``node`` = ``start_vid``.

**start_vid**    ``BIGINT``  Identifier of the root vertex.

                             - In `Multiple Vertices`_ results are in ascending order.

**node**         ``BIGINT``  Identifier of ``node`` reached using ``edge``.
**edge**         ``BIGINT``  Identifier of the ``edge`` used to arrive to ``node``.

                             - :math:`-1`  when ``node`` = ``start_vid``.

**cost**         ``FLOAT``   Cost to traverse ``edge``.
**agg_cost**     ``FLOAT``   Aggregate cost from ``start_vid`` to ``node``.
===============  =========== ====================================================

.. result columns end

.. mst-information-end

See Also
-------------------------------------------------------------------------------

- `Boost Kruskal documentation <https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/kruskal_min_spanning_tree.html>`__
- Kruskal on `wikipedia <https://en.wikipedia.org/wiki/Kruskal%27s_algorithm>`__
- The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

