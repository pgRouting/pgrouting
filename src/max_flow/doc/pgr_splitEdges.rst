..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropiate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_splitEdges:

pgr_splitEdges
==============


Name
----

``pgr_splitEdges`` — Converts bidirectional/undirected edge representation
like the one in the sample data to the directional format, with one edge per row.

.. warning::  This is a proposed function.

     - Is not officially in the current release

Synopsis
-------------------------------------------------------------------------------

Helper function. Converts bidirectional/undirected edge representation
like the one in the sample data to the directional format, with one edge per row.
Utilized in some algorithms like :ref:`pgr_maximumCardinalityMatching` and :ref:`pgr_edgeDisjointPaths`.
These algorithms have no cost or reverse_cost parameters in their edges, thus losing the semantics
of directionality unless preprocessing is done.

Function Definition
-------------------
Given the following query:
:math:`pgr\_splitedges(edges\_sql)`

where :math:`edges\_sql = \{(id_i, source_i, target_i, cost_i, reverse\_cost_i)\}`

Then:

:math:`pgr\_splitEdges(edges\_sql) = \begin{cases} &\{(source_i, target_i, cost_i)\} &\quad  \text{ if } cost_i > 0 \\\bigcup &\{(target_i, source_i, reverse\_cost_i)\} &\quad \text{ if } reverse\_cost_i > 0 \end{cases}`


Signature
---------

.. code-block:: none

    pgr_splitedges(edges_sql)
    RETURNS SET OF (id, source, target, cost)
        OR EMPTY SET

Return value
............




:Example:

.. literalinclude:: doc-splitEdges.queries
   :start-after: -- q1
   :end-before: -- q2

Description of the Signatures
=============================

Description of the SQL query
----------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

====================  ===================   =================================================
Column                Type                  Description
====================  ===================   =================================================
**id**                ``ANY-INTEGER``       Identifier of the edge.
**source**            ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**            ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**              ``ANY-NUMERIC``       Cost of the edge from source to target.
**reverse_cost**      ``ANY-NUMERIC``       Cost of the edge from target to source.
====================  ===================   =================================================

Where:
 - :ANY-INTEGER: SMALLINT, INTEGER, BIGINT
 - :ANY-NUMERIC: SMALLINT, INTEGER, BIGINT, REAL, DOUBLE PRECISION

Description of the parameters of the signatures
-----------------------------------------------
================= ====================== =================================================
Column            Type                   Description
================= ====================== =================================================
**edges_sql**     ``TEXT``               SQL query as described above.
================= ====================== =================================================


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

