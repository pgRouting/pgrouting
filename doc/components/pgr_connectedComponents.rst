..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_connectedComponents:

pgr_connectedComponents - Experimental
===============================================================================


``pgr_connectedComponents`` — Return the connected components of an undirected graph using a DFS-based approach.
In particular, the algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/connected_components.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Synopsis
-------------------------------------------------------------------------------

A connected component of an undirected graph is a set of vertices that are all reachable
from each other.
This implementation can only be used with an undirected graph.

Characteristics
-------------------------------------------------------------------------------

The main Characteristics are:

  - Components are described by vertices

  - The returned values are ordered:

    - `component` ascending
    - `node` ascending

  - Running time: :math:`O(V + E)`

Signatures
-------------------------------------------------------------------------------

.. index::
    single: connectedComponents

.. code-block:: none

    pgr_connectedComponents(edges_sql)

    RETURNS SET OF (seq, component, n_seq, node)
        OR EMPTY SET

The signature is for a **undirected** graph.

:Example:

.. literalinclude:: doc-pgr_connectedComponents.queries
   :start-after: -- q1
   :end-before: -- q2

.. image:: images/cc_sampledata.png
   :width: 685px
   :height: 387px

Description of the Signatures
-------------------------------------------------------------------------------

.. include:: components-family.rst
    :start-after: components_edges_sql_start
    :end-before: components_edges_sql_end

.. include:: components-family.rst
    :start-after: components_parameters_start
    :end-before: components_parameters_end

.. include:: components-family.rst
    :start-after: return_componentsV_start
    :end-before: return_componentsV_end


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Connected_component_%28graph_theory%29
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

