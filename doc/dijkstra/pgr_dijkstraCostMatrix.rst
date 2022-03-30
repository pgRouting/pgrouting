..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_dijkstraCostMatrix.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_dijkstraCostMatrix.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_dijkstraCostMatrix.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_dijkstraCostMatrix.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_dijkstraCostMatrix.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_dijkstraCostMatrix.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_dijkstraCostMatrix.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_dijkstraCostMatrix.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/costMatrix/doc/pgr_dijkstraCostMatrix.html>`__

pgr_dijkstraCostMatrix
===============================================================================

``pgr_dijkstraCostMatrix`` - Calculates the a cost matrix using
:doc:`pgr_dijkstra`.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

* Version 3.0.0

  * **Official** function

* Version 2.3.0

  * New **proposed** function


Description
-------------------------------------------------------------------------------

Using Dijkstra algorithm, calculate and return a cost matrix.

.. include:: dijkstra-family.rst
    :start-after: dijkstra_description_start
    :end-before: dijkstra_description_end

.. include:: costMatrix-category.rst
    :start-after: costMatrix_details_start
    :end-before: costMatrix_details_end

Signatures
-------------------------------------------------------------------------------

.. index::
    single: dijkstraCostMatrix

.. rubric:: Summary

.. parsed-literal::

    pgr_dijkstraCost(`Edges SQL`_, **start vids** [, directed])
    RETURNS SET OF (start_vid, end_vid, agg_cost)

:Example: Symmetric cost matrix for vertices :math:`\{1, 2, 3, 4\}` on an
          **undirected** graph

.. literalinclude:: doc-pgr_dijkstraCostMatrix.queries
   :start-after: -- dijkstra q2
   :end-before: -- dijkstra q3

Parameters
-------------------------------------------------------------------------------

.. include:: costMatrix-category.rst
    :start-after: costMatrix_parameters_start
    :end-before: costMatrix_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Inner query
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Additional Examples
-------------------------------------------------------------------------------

:Example: Use with :doc:`pgr_TSP`.

.. literalinclude:: doc-pgr_dijkstraCostMatrix.queries
   :start-after: -- dijkstra q3
   :end-before: -- dijkstra q4

See Also
-------------------------------------------------------------------------------

* :doc:`dijkstra-family`
* :doc:`costMatrix-category`
* :doc:`TSP-family`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

