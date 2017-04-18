..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_withPointsCostMatrix:

pgr_withPointsCostMatrix - proposed
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_withPointsCostMatrix`` - Calculates the shortest path and returns only the aggregate cost of the shortest path(s) found, for the combination of points given.


.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning



.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

.. rubric:: Availability: 2.2.0


Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vids)
    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vids, directed, driving_side)
    RETURNS SET OF (start_vid, end_vid, agg_cost)

.. note:: There is no **details** flag, unlike the other members of the withPoints family of functions.


Signatures
-------------------------------------------------------------------------------

.. index::
    single: withPointsCostMatrix(Minimal use) - Proposed

Minimal Signature
...............................................................................

The minimal signature:
    - Is for a **directed** graph.
    - The driving side is set as **b** both. So arriving/departing to/from the point(s) can be in any direction.

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vid)
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example:

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- withPoints q1
   :end-before: -- withPoints q2


.. index::
    single: withPointsCostMatrix(Complete Signature) - Proposed

Complete Signature
...............................................................................

.. code-block:: none

    pgr_withPointsCostMatrix(edges_sql, points_sql, start_vids,
        directed:=true, driving_side:='b')
    RETURNS SET OF (start_vid, end_vid, agg_cost)


:Example: returning a symmetrical cost matrix

* Using the default **side** value on the **points_sql** query
* Using an undirected graph
* Using the default **driving_side** value

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- withPoints q2
   :end-before: -- withPoints q3



Description of the Signatures
-------------------------------------------------------------------------------

..
    description of the sql queries


.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgRouting-concepts.rst
    :start-after: points_sql_start
    :end-before: points_sql_end


Description of the parameters of the signatures
...............................................................................


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


Examples
-------------------------------------------------------------------------------

:Example: Use with tsp

.. literalinclude:: doc-pgr_fooDmatrix.queries
   :start-after: -- withPoints q3
   :end-before: -- withPoints q4



See Also
-------------------------------------------------------------------------------

* :ref:`withPoints`
* :ref:`costMatrix`
* :ref:`tsp`
* `sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

