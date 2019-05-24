..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_bridges - Proposed
===============================================================================

``pgr_bridges`` - Return the bridges of an undirected graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/connected_components.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* On v3.0.0

  * Set as `proposed`
  * Return columns change: ``seq`` is removed

* New as experimental on v2.5.0

Description
-------------------------------------------------------------------------------

A bridge is an edge of an undirected graph whose deletion increases its number
of connected components.
This implementation can only be used with an undirected graph.

**The main characteristics are:**

- The signature is for an **undirected** graph.
- The returned values are ordered:

  - `edge` ascending

- Running time: :math:`O(E * (V + E))`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: bridges -- Proposed

.. code-block:: none

    pgr_bridges(Edges SQL)

    RETURNS SET OF (edge)
    OR EMPTY SET

:Example: The bridges of the graph

.. literalinclude:: doc-pgr_bridges.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: components-family.rst
    :start-after: components_parameters_start
    :end-before: components_parameters_end

Inner query
-------------------------------------------------------------------------------

:edges SQL: an SQL query of an **undirected** graph, which should return a set of rows with the following columns:

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------
.. return_bridges_start

Returns set of ``(edge)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**edge**       ``BIGINT`` Identifier of the edge that is a bridge.
============== ========== =================================================

.. return_bridges_end

See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Bridge_%28graph_theory%29
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

