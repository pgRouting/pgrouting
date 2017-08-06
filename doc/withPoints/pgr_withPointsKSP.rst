..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPointsKSP:

pgr_withPointsKSP - Proposed
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_withPointsKSP`` - Find the K shortest paths using Yen's algorithm.


.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability: 2.2.0

Synopsis
-------------------------------------------------------------------------------

Modifies the graph to include the points defined in the ``points_sql`` and
using Yen algorithm, finds the K shortest paths.


Signature Summary
-----------------

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K)
    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K, directed, heap_paths, driving_side, details)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)

Signatures
-----------

.. index::
    single: withPointsKSP(Minimal Signature) - Proposed

Minimal Usage
....................................

The minimal usage:
    - Is for a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.
    - No **details** are given about distance of other points of the query.
    - No **heap paths** are returned.

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)


:Example:

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q1
   :end-before: --q2

.. index::
    single: withPointsKSP(Complete Signature) - Proposed

Complete Signature
....................................

Finds the K shortest paths depending on the optional parameters setup.

.. code-block:: none

    pgr_withPointsKSP(edges_sql, points_sql, start_pid, end_pid, K,
        directed:=true, heap_paths:=false, driving_side:='b', details:=false)
    RETURNS SET OF (seq, path_id, path_seq, node, edge, cost, agg_cost)


:Example: With details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q2
   :end-before: --q3

Description of the Signatures
-------------------------------


..
    description of the sql queries

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end



Description of the parameters of the signatures
..............................................................


================ ================= =================================================
Parameter        Type              Description
================ ================= =================================================
**edges_sql**    ``TEXT``          Edges SQL query as described above.
**points_sql**   ``TEXT``          Points SQL query as described above.
**start_pid**    ``ANY-INTEGER``   Starting point id.
**end_pid**      ``ANY-INTEGER``   Ending point id.
**K**            ``INTEGER``       Number of shortest paths.
**directed**     ``BOOLEAN``       (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
**heap_paths**   ``BOOLEAN``       (optional). When ``true`` the paths calculated to get the shortests paths will be returned also. Default is ``false`` only the K shortest paths are returned.
**driving_side** ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the driving side is:
                                     - In the right or left or
                                     - If it doesn't matter with 'b' or NULL.
                                     - If column not present 'b' is considered.

**details**      ``BOOLEAN``       (optional). When ``true`` the results will include the driving distance to the points with in the ``distance``.
                                   Default is ``false`` which ignores other points of the points_sql.
================ ================= =================================================


Description of the return values
..............................................................

Returns set of ``(seq, path_id, path_seq, node, edge, cost, agg_cost)``

============ =========== =================================================
Column           Type              Description
============ =========== =================================================
**seq**      ``INTEGER`` Row sequence.
**path_seq** ``INTEGER`` Relative position in the path of node and edge. Has value 1 for the beginning of a path.
**path_id**  ``INTEGER``  Path identifier. The ordering of the paths: For two paths i, j if i < j then agg_cost(i) <= agg_cost(j).
**node**     ``BIGINT``  Identifier of the node in the path. Negative values are the identifiers of a point.
**edge**     ``BIGINT``  Identifier of the edge used to go from ``node`` to the next node in the path sequence.
                           - ``-1`` for the last row in the path sequence.

**cost**     ``FLOAT``   Cost to traverse from ``node`` using ``edge`` to the next ``node`` in the path sequence.
                           - ``0`` for the last row in the path sequence.

**agg_cost** ``FLOAT``   Aggregate cost from ``start_pid`` to ``node``.
                           - ``0`` for the first row in the path sequence.

============ =========== =================================================



Examples
--------------------------------------------------------------------------------------

:Example: Left side driving topology with details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q3
   :end-before: --q4

:Example: Right side driving topology with heap paths and details.

.. literalinclude:: doc-pgr_withPointsKSP.queries
   :start-after: --q4
   :end-before: --q5

The queries use the :doc:`sampledata` network.

.. rubric:: History

* Proposed in version 2.2


See Also
-------------------------------------------------------------------------------

* :ref:`withPoints`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

