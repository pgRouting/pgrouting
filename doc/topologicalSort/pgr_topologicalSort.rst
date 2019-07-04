..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_topologicalSort - Experimental
===============================================================================

``pgr_topologicalSort`` — Returns the linear ordering of the vertices(s) for weighted directed acyclic graphs(DAG).
In particular, the topological sort algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_65_1/libs/graph/doc/topological_sort.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* **TBD**

Description
-------------------------------------------------------------------------------

The topological sort algorithm creates a linear ordering of the vertices such that if edge (u,v) appears 
in the graph, then v comes before u in the ordering.

This implementation can only be used with a **directed** graph with no cycles i.e. directed acyclic graph. 

The main characteristics are:
  - Process is valid for weighted directed acyclic graphs only. otherwise it will throw warnings.
  - Values are returned when there is a path.

  - For optimization purposes, if there are more than one answer, the function will return one of them.
  
  - The returned values are ordered in topological order:

  * Running time: :math:`O( (V + E))`


Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_topologicalSort(edges_sql)

    RETURNS SET OF (seq, sorted_v)
    OR EMPTY SET


.. index::
    single: topologicalSort(One to One) - Experimental

One to One
...............................................................................

.. code-block:: none

    pgr_dagShortestPath(edges_sql)
    RETURNS SET OF (seq, sorted_v) 
    OR EMPTY SET

:Example: From vertex :math:`1` to vertex :math:`6` 

.. literalinclude:: doc-pgr_topologicalSort.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. pgr_topologicalSort_parameters_start

.. rubric:: Description of the parameters of the signatures

============== ================== ======== =================================================
Parameter      Type               Default     Description
============== ================== ======== =================================================
**edges_sql**        ``TEXT``                    SQL query as described above.
============== ================== ======== =================================================

.. pgr_topologicalSort_parameters_end

Inner Query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Results Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_short_start
    :end-before: return_path_short_end

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Topological_sorting
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

