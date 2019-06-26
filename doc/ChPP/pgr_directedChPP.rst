..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_directedChPP - Experimental
============================================

``pgr_directedChPP`` — Calculates the shortest circuit path which contains every edge in a directed graph and starts and ends on the same vertex.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experiemntal on v3.0.0

**Supported versions**
current(`3.0 <https://docs.pgrouting.org/dev/en/pgr_directedChPP.html

Description
-------------------------------------------------------------------------------

**The main characteristics are:**

- Process is done only on edges with **positive** costs.
- There is no path when the graph is not connected.
- We will return **no path found** error if there is no path.
- Running time: :math:`O(E * (E + V * logV))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary
.. code-block:: none

    pgr_directedChPP(edges_sql)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example:

.. literalinclude:: doc-pgr_directedChPP.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: directedChPP - Experimental

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. include:: pgr_directedChPP_Cost.rst
    :start-after: pgr_directedChPP_parameters_start
    :end-before: pgr_directedChPP_parameters_end

Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end

See Also
-------------------------------------------------------------------------------

* :doc:`chinesePostmanProblem-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

