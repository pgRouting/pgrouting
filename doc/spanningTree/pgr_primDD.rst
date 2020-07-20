..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_primDD
===============================================================================

``pgr_primDD`` — Catchament nodes using Prim's algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * New **Official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_primDD.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_primDD.html>`__

Description
-------------------------------------------------------------------------------

Using Prim algorithm, extracts the nodes that have aggregate costs less than
or equal to the value ``Distance`` within the calculated minimum spanning tree.

**The main Characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- Returned tree nodes from a root vertex are on Depth First Search order.
- Depth First Search running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: none

    pgr_prim(Edges SQL, root vid, distance)
    pgr_prim(Edges SQL, root vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: primDD(Single vertex)

Single vertex
...............................................................................

.. code-block:: none

    pgr_primDD(Edges SQL, root vid, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertex :math:`2` with :math:`agg\_cost <= 3.5`

.. literalinclude:: doc-pgr_primDD.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: primDD(Multiple vertices)

Multiple vertices
...............................................................................

.. code-block:: none

    pgr_primDD(Edges SQL, root vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

:Example: The Minimum Spanning Tree starting on vertices :math:`\{13, 2\}` with :math:`agg\_cost <= 3.5`;

.. literalinclude:: doc-pgr_primDD.queries
   :start-after: -- q2
   :end-before: -- q3


.. Parameters, Inner query & result columns

.. include:: pgr_kruskalDD.rst
   :start-after: mstDD-information-start
   :end-before: mstDD-information-end


See Also
-------------------------------------------------------------------------------

* :doc:`spanningTree-family`
* :doc:`prim-family`
* The queries use the :doc:`sampledata` network.
* `Boost: Prim's algorithm documentation <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm <https://en.wikipedia.org/wiki/Prim%27s_algorithm>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

