..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_primDD - Experimental
===============================================================================

``pgr_primDD`` — Returns catchament nodes using Prim algorithm.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Experimental

  * v3.0.0

Description
-------------------------------------------------------------------------------

Using Prim algorithm, extracts the nodes that have aggregate costs less than
or equal to `distance`.

**The main Characteristics are:**

.. include:: prim-family.rst
   :start-after: prim-description-start
   :end-before: prim-description-end

- Returned tree nodes from a root vertex are on Depth First Search order.
- Depth First Search running time: :math:`O(E + V)`

Signatures
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_prim(Edges SQL, root vid, distance)
    pgr_prim(Edges SQL, root vids, distance)

    RETURNS SET OF (seq, depth, start_vid, node, edge, cost, agg_cost)

.. index::
    single: primDD(Single vertex) - Experimental

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
    single: primDD(Multiple vertices) - Experimental

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


* `Boost: Prim's documentation <https://www.boost.org/libs/graph/doc/prim_minimum_spanning_tree.html>`__
* `Wikipedia: Prim's algorithm <https://en.wikipedia.org/wiki/Prim'_algorithm>`__
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

