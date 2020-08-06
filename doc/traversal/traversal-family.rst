..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Traversal - Family of functions (Experimental)
===============================================================================

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_depthFirstSearch` - Depth first search traversal of the graph.

.. index to here


.. toctree::
    :hidden:

    pgr_depthFirstSearch

.. rubric:: Versions of this page

* **Supported versions:**
  current(`3.2 <https://docs.pgrouting.org/3.2/en/traversal-family.html>`__)


Inner query
-------------------------------------------------------------------------------

.. rubric:: Edges SQL

.. edges_sql_start

================= =================== ======== =================================================
Column            Type                 Default  Description
================= =================== ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              - When positive: edge `(source, target)` exist on the graph.
                                                 - When negative: edge `(source, target)` does not exist on the graph.

**reverse_cost**  ``ANY-NUMERICAL``       -1     - When positive: edge `(target, source)` exist on the graph.
                                                 - When negative: edge `(target, source)` does not exist on the graph.

================= =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. edges_sql_end


See Also
-------------------------------------------------------------------------------

.. include:: pgr_depthFirstSearch.rst
    :start-after: see also start
    :end-before: see also end


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
