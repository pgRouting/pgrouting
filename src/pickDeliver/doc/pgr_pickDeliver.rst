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

.. _pgr_pickDeliver:

pgr_pickDeliver
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_pickDeliver`` — [medium description of
function1 it can span many
lines]

.. warning::  This is a proposed function.

     - Is not officially in the current release


..
   keep if uses boost (this is a comment)

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Very long description of function

..
   SPHINX manual
   http://www.sphinx-doc.org/en/stable/rest.html


Characteristics:
----------------

..
   Here is a sample of the kind of information in this section:

The main Characteristics are:
  - Process is done only on edges with positive costs.
  - Values are returned when there is a path.

    - When the starting vertex and ending vertex are the same, there is no path.

      - The `agg_cost` the non included values `(v, v)` is `0`

    - When the starting vertex and ending vertex are the different and there is no path:

      - The `agg_cost` the non included values `(u, v)` is :math:`\infty`

  - For optimization purposes, any duplicated value in the `start_vids` or `end_vids` are ignored.

  - The returned values are ordered:

    - `start_vid` ascending
    - `end_vid` ascending

  - Runing time: :math:`O(| start\_vids | * (V \log V + E))`

Signature Summary
-----------------

..
   If the function has more than one signature
   Remove the unneseary parts of the signature, just leving the name of the parameters
   Like in these examples

.. code-block:: none

    pgr_pickDeliver(edges_sql, start_vid,  end_vids)
    pgr_pickDeliver(edges_sql, start_vids, end_vids, directed:=true)
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
      OR EMPTY SET


..
  This is a reminder of how your query looks like
  pgr_pickDeliver(
    orders_sql TEXT,
    max_vehicles INTEGER,
    capacity FLOAT,
    max_cycles INTEGER, 
    OUT seq INTEGER,
    OUT vehicle_id INTEGER,
    OUT order_id BIGINT,
    OUT cost FLOAT,
    OUT agg_cost FLOAT)


Signatures
===============================================================================

.. index:: 
    single: pgr_pickDeliver(edges_sql, start_vid,  end_vids) - proposed

Minimal signature
-----------------


..
   Small description, example:

The minimal signature is for a **directed** graph from one ``start_vid`` to many ``end_vids``:

.. code-block:: none

    pgr_pickDeliver(edges_sql, start_vid,  end_vids)
    RETURNS SET OF (seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)
      OR EMPTY SET

:Example:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q1
   :end-before: --q2


Complete signature
------------------

This signature performs a .....

  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

.. index:: 
    single: pgr_pickDeliver(edges_sql, start_vids, end_vids, directed:=true) - proposed

.. code-block:: none

    pgr_pickDeliver(  [parameters],
        boolean directed:=true);
    RETURNS SET OF ( [output] ) or EMPTY SET


:Example:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q2
   :end-before: --q3

Description of the Signatures
=============================

..
   DELETE / ADD DEPENDING ON YOUR REQUIREMENTS

Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the edge.
**source**        ``ANY-INTEGER``       Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``       Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``     Weight of the edge `(source, target)`, If negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``     (optional) Weight of the edge `(target, source)`, If negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   =================================================

Description of the Points SQL query
-------------------------------------------------------------------------------

:points_sql: an SQL query, which should return a set of rows with the following columns:

============ ================= =================================================
Column            Type              Description
============ ================= =================================================
**pid**      ``ANY-INTEGER``   (optional) Identifier of the point. Can not be NULL. If column not present, a sequential identifier will be given automatically.
**eid**      ``ANY-INTEGER``   Identifier of the "closest" edge to the point.
**fraction** ``ANY-NUMERICAL`` Value in [0,1] that indicates the relative postition from the first end point of the edge.
**side**     ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the point is:
                                 - In the right, left of the edge or
                                 - If it doesn't matter with 'b' or NULL.
                                 - If column not present 'b' is considered.

                               Can be in upper or lower case.
============ ================= =================================================


Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


Description of the parameters of the signatures
-------------------------------------------------------------------------------

============== ====================== =================================================
Column         Type                   Description
============== ====================== =================================================
**edges_sql**  ``TEXT``               SQL query as decribed above.
**points_sql** ``TEXT``               Points SQL query as decribed above.
**start_vid**  ``BIGINT``             Identifier of the starting vertex of the path.
**start_vids** ``ARRAY[ANY-INTEGER]`` Array of identifiers of starting vertices.
**end_vid**    ``BIGINT``             Identifier of the ending vertex of the path.
**end_vids**   ``ARRAY[ANY-INTEGER]`` Array of identifiers of ending vertices.
**directed**   ``BOOLEAN``            (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
============== ====================== =================================================


Examples
========

The examples of this section are based on the :ref:`sampledata` network.



[put as many examples as needed and use the documentation data for the examples]

:Example:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q1
   :end-before: --q2

..
   If needed here are some subtitles  

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

:Example: This example is in a subtitle

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q1
   :end-before: --q2


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Official in version X.X
* Proposed in version Y.Y 


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

