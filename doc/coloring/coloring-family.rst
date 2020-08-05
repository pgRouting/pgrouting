..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Coloring - Family of functions (Experimental)
===============================================================================

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_sequentialVertexColoring` - Vertex coloring algorithm using greedy approach.

.. index to here


.. toctree::
    :hidden:

    pgr_sequentialVertexColoring

.. rubric:: Versions of this page

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/coloring-family.html>`__)


Parameters
-------------------------------------------------------------------------------

=================== ====================== =================================================
Parameter           Type                   Description
=================== ====================== =================================================
**Edges SQL**       ``TEXT``               Inner query as described below.
=================== ====================== =================================================


Inner query
-------------------------------------------------------------------------------

:Edges SQL: an SQL query of an **undirected** graph, which should return
            a set of rows with the following columns:

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end


Result Columns
-------------------------------------------------------------------------------

pgr_sequentialVertexColoring
...............................................................................

.. include:: pgr_sequentialVertexColoring.rst
    :start-after: result columns start
    :end-before: result columns end


See Also
-------------------------------------------------------------------------------

.. include:: pgr_sequentialVertexColoring.rst
    :start-after: see also start
    :end-before: see also end


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
