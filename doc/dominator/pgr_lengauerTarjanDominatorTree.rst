..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


pgr_lengauerTarjanDominatorTree -Experimental
===============================================================================

``pgr_lengauerTarjanDominatorTree`` — Returns the immediate dominator of all
vertices.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/lengauer_tarjan_dominator.htm

   Boost Graph Inside

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.2.0

  * New **experimental** function


Description
-------------------------------------------------------------------------------
The algorithm calculates the *immidiate dominator* of each vertex called
**idom**, once **idom** of each vertex is calculated then by making every
**idom** of each vertex as its parent, the dominator tree can be built.

**The main Characteristics are:**

- The algorithm works in directed graph only.
- The returned values are not ordered.
- The algorithm returns *idom* of each vertex.
- If the *root vertex* not present in the graph then it returns empty set.
- Running time: :math:`O((V+E)log(V+E))`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_lengauerTarjanDominatorTree(`Edges SQL`_, **root vertex**)

   | RETURNS SET OF |result-idom|
   | OR EMPTY SET


.. index::
    single: lengauerTarjanDominatorTree - Experimental on v3.2


:Example: The dominator tree with root vertex :math:`5`

.. literalinclude:: doc-lengauerTarjanDominatorTree.queries
   :start-after: --q1
   :end-before: --q2

Parameters
-------------------------------------------------------------------------------

=============== ============ =================================================
Column          Type           Description
=============== ============ =================================================
`Edges SQL`_     ``TEXT``    SQL query as described above.
**root vertex**  ``BIGINT``  Identifier of the starting vertex.
=============== ============ =================================================


Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, vertex_id, idom)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
``seq``        ``INTEGER`` Sequential value starting from **1**.
``vertex_id``  ``BIGINT``  Identifier of vertex .
``idom``       ``BIGINT``  Immediate dominator of vertex.
============== =========== =================================================

Additional Examples
-------------------------------------------------------------------------------

:Example: Dominator tree of another component.

.. literalinclude:: doc-lengauerTarjanDominatorTree.queries
    :start-after: --q2
    :end-before: --q3

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Lengauer-Tarjan dominator tree algorithm
  <https://www.boost.org/libs/graph/doc/lengauer_tarjan_dominator.htm>`__
* `Wikipedia: dominator tree
  <https://en.wikipedia.org/wiki/Dominator_(graph_theory)>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
