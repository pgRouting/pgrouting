..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_transitiveClosure - Experimental
===============================================================================

``pgr_transitiveClosure`` — Returns the transitive closure graph of the input graph.
In particular, the transitive closure algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_70_0/libs/graph/doc/transitive_closure.html

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_transitiveClosure.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_transitiveClosure.html>`__

Description
-------------------------------------------------------------------------------

The transitive_closure() function transforms the input graph g into the transitive closure graph tc.

This implementation can only be used with a **directed** graph with no cycles i.e. directed acyclic graph.

The main characteristics are:
  - Process is valid for directed acyclic graphs only. otherwise it will throw warnings.

  - The returned values are not ordered:

  * Running time: :math:`O(|V||E|)`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

The pgr_transitiveClosure function has the following signature:

.. index::
   single: transitiveClosure

.. code-block:: none

    pgr_transitiveClosure(Edges SQL)
    RETURNS SETOF (id, vid, target_array)

:Example: Complete Graph of 3 vertexs

.. literalinclude:: doc-transitiveClosure.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

======================= ====================== =================================================
Column                  Type                   Description
======================= ====================== =================================================
**Edges SQL**           ``TEXT``               SQL query as described in `Inner query`_
======================= ====================== =================================================

Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

RETURNS SETOF  (seq, vid, target_array)

The function returns a single row. The columns of the row are:

============================ =================   ===================================================================
Column                       Type                Description
============================ =================   ===================================================================
**seq**                      ``INTEGER``         Sequential value starting from **1**.
**vid**                      ``BIGINT``          Identifier of the vertex.
**target_array**             ``ARRAY[BIGINT]``   Array of identifiers of the vertices that are reachable from vertex v.
============================ =================   ===================================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Some sub graphs of the sample data

.. literalinclude:: doc-transitiveClosure.queries
   :start-after: -- q2
   :end-before: -- q4


See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Transitive_closure
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
