..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_withPointsCostMatrix - proposed
===============================================================================

``pgr_withPointsCostMatrix`` - Calculates the shortest path and returns only the aggregate cost of the shortest path(s) found, for the combination of points given.

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 2.2.0

  * New **proposed** function


* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_withPointsCostMatrix.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_withPointsCostMatrix.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_withPointsCostMatrix.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_withPointsCostMatrix.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_withPointsCostMatrix.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/costMatrix/doc/pgr_withPointsCostMatrix.html>`__

Description
-------------------------------------------------------------------------------

* **TBD**

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vids [, directed] [, driving_side])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the withPoints family of functions.

.. rubric:: Using default

The minimal signature:
    - Is for a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for points :math:`\{1, 6\}` and vertices :math:`\{3, 6\}` on a **directed** graph


.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- withPoints q1
   :end-before: -- withPoints q2

.. index::
    single: withPointsCostMatrix - Proposed

Complete Signature
...............................................................................

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vids,
        directed:=true, driving_side:='b')
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Cost matrix for points :math:`\{1, 6\}` and vertices :math:`\{3, 6\}` on an **undirected** graph

* Returning a **symmetrical** cost matrix
* Using the default **side** value on the **points_sql** query
* Using the default **driving_side** value

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- withPoints q2
   :end-before: -- withPoints q3

Parameters
-------------------------------------------------------------------------------

================ ====================== =================================================
Parameter        Type                   Description
================ ====================== =================================================
**edges_sql**    ``TEXT``               Edges SQL query as described above.
**points_sql**   ``TEXT``               Points SQL query as described above.
**start_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of starting vertices. When negative: is a point's pid.
**directed**     ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**driving_side** ``CHAR``               (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                          - In the right or left or
                                          - If it doesn't matter with 'b' or NULL.
                                          - If column not present 'b' is considered.

================ ====================== =================================================

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Inner query
-------------------------------------------------------------------------------

..
    description of the sql queries

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end

Additional Examples
-------------------------------------------------------------------------------

:Example: :doc:`pgr_TSP` using ``pgr_withPointsCostMatrix`` for points :math:`\{1, 6\}` and vertices :math:`\{3, 6\}` on an **undirected** graph

.. literalinclude:: doc-pgr_withPointsCostMatrix.queries
   :start-after: -- withPoints q3
   :end-before: -- withPoints q4

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_withPoints`
* :doc:`costMatrix-category`
* :doc:`pgr_TSP`
* `sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

