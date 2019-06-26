..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_directedChPP_Cost - Experimental
============================================

``pgr_directedChPP_Cost`` — Calculates the minimum costs of a circuit path which contains every edge in a directed graph and starts and ends on the same vertex.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experiemntal on v3.0.0

**Supported versions**
current(`3.0 <https://docs.pgrouting.org/dev/en/pgr_directedChPP_Cost.html>`__)


Description
-------------------------------------------------------------------------------

The main characteristics are:
- Process is done only on edges with **positive** costs.  
- There is no return value when the graph is not connected.
- We will return **no path found** error if there is no path.
- It does not returns a path.
- Running time: :math:`O(E * (E + V * logV))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_directedChPP_Cost(edges_sql)
    RETURNS FLOAT

.. index::
    single: directedChPP_Cost - Experimental

:Example:

.. literalinclude:: doc-pgr_directedChPP_Cost.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: directedChPP_Cost - Experimental

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

.. pgr_directedChPP_parameters_start

Parameters
-------------------------------------------------------------------------------

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    The edges SQL query as described above.
============== ================== ======== =================================================

.. pgr_directedChPP_parameters_end

Result Columns
-------------------------------------------------------------------------------

====================== =================================================
Type                   Description
====================== =================================================
``FLOAT``              Minimum costs of a circuit path.
====================== =================================================

See Also
-------------------------------------------------------------------------------

* :doc:`chinesePostmanProblem-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

