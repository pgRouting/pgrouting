..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_connectedComponentsV:

pgr_connectedComponentsV
===============================================================================


``pgr_connectedComponentsV`` — Return the connected components of an undirected graph using a DFS-based approach.
In particular, the algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/connected_components.html

   Boost Graph Inside


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

.. code-block:: none

    pgr_connectedComponentsV(edges_sql)

    RETURNS SET OF (seq, component, n_seq, node)
        OR EMPTY SET

The signature is for a **undirected** graph. 

:Example:

.. literalinclude:: doc-pgr_connectedComponentsV.queries
   :start-after: -- q1
   :end-before: -- q2

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

* http://en.wikipedia.org/wiki/Connected_component_(graph_theory)
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

