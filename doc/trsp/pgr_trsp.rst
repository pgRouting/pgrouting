..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_trsp.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_trsp.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_trsp.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_trsp.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_trsp.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_trsp.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_trsp.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_trsp.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/trsp/doc/pgr_trsp.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/trsp/doc/pgr_trsp.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/trsp/doc/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/src/trsp/doc/index.html>`__

pgr_trsp - Proposed
===============================================================================


``pgr_trsp`` - routing vertices with restrictions.

.. rubric:: Availability

* Version 3.4.0

  * New **Proposed** signatures

    * ``pgr_trsp`` (`One to One`_)
    * ``pgr_trsp`` (`One to Many`_)
    * ``pgr_trsp`` (`Many to One`_)
    * ``pgr_trsp`` (`Many to Many`_)
    * ``pgr_trsp`` (`Combinations`_)

  * Signature ``pgr_trsp(text,integer,integer,boolean,boolean,text)`` is deprecated
  * Signature ``pgr_trsp(text,integer,float,integer,float,boolean,boolean,text)`` is deprecated
  * Signature ``pgr_trspViaVertices(text,anyarray,boolean,boolean,text)`` is deprecated
  * Signature ``pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)`` is deprecated

* Version 2.1.0

  * New *Via* **prototypes**

    * ``pgr_trspViaVertices``
    * ``pgr_trspViaEdges``

* Version 2.0.0

  * **Official** function


.. contents:: Contents

Description
-------------------------------------------------------------------------------

Turn restricted shortest path (TRSP) is an algorithm that receives
turn restrictions in form of a query like those found in
real world navigable road networks.

The main characteristics are:

* Automatic detection of ``reverse_cost`` column
* Accepts **ANY-INTEGER** and **ANY-NUMERICAL** on input columns
* All variations give as result the same columns
* It does no guarantee the shortest path as it might contain restriction paths

The general algorithm is as follows:

* Execute a Dijkstra
* If the solution passes thru a restriction then

  * Execute the **TRSP** algorithm with restrictions




Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vid,  end vid  [, directed]) -- Proposed on v3.4
   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vid,  end vids [, directed]) -- Proposed on v3.4
   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vids, end vid  [, directed]) -- Proposed on v3.4
   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vids, end vids [, directed]) -- Proposed on v3.4
   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

.. index::
    single: trsp(One to One) -- Proposed on v3.4

One to One
...............................................................................

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vid,  end vid  [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From vertex :math:`2` to vertex  :math:`3` on an **undirected** graph

.. literalinclude:: doc-trsp.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: trsp(One to Many) -- Proposed on v3.4

One to many
...............................................................................

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vid,  end vids [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From vertex :math:`2` to vertices :math:`\{3, 7\}` on an **undirected** graph

.. literalinclude:: doc-trsp.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: trsp(Many to One) -- Proposed on v3.4

Many to One
...............................................................................

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vids, end vid  [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertex :math:`10` on a **directed** graph

.. literalinclude:: doc-trsp.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: trsp(Many to Many) -- Proposed on v3.4

Many to Many
...............................................................................

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, start vids, end vids [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: From vertices :math:`\{2, 7\}` to vertices :math:`\{3, 10\}` on an **undirected** graph

.. literalinclude:: doc-trsp.queries
   :start-after: -- q5
   :end-before: -- q6

.. index::
    single: trsp(Combinations) - Proposed on v3.4

Combinations
...............................................................................

.. parsed-literal::

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, [, directed]) -- Proposed on v3.4
   RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
   OR EMPTY SET

:Example: Using a combinations table on an **undirected** graph

.. literalinclude:: doc-trsp.queries
   :start-after: -- q6
   :end-before: -- q7


Parameters
-------------------------------------------------------------------------------

.. trsp_parameters_start

===================== ================== ========= ==========================================================
Parameter             Type               Default     Description
===================== ================== ========= ==========================================================
`Edges SQL`_          ``TEXT``                     `Edges SQL`_ as described below
`Restrictions SQL`_   ``TEXT``                     `Restrictions SQL`_ as described below
`Combinations SQL`_   ``TEXT``                     `Combinations SQL`_ as described below
**start vid**         ``BIGINT``                   Identifier of the starting vertex of the path.
**start vids**        ``ARRAY[BIGINT]``            Array of identifiers of starting vertices.
**end vid**           ``BIGINT``                   Identifier of the ending vertex of the path.
**end vids**          ``ARRAY[BIGINT]``            Array of identifiers of ending vertices.
``directed``          ``BOOLEAN``         ``true`` - When ``true`` Graph is considered `Directed`
                                                   - When ``false`` the graph is considered as `Undirected`.
===================== ================== ========= ==========================================================

.. pgr_trsp_parameters_end

Inner queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: TRSP-family.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Return Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_start
    :end-before: return_path_end


Additional Examples
-------------------------------------------------------------------------------

:Example: Without turn restrictions

.. literalinclude:: doc-trsp.queries
   :start-after: --q1
   :end-before: --q2

:Example: With turn restrictions

Then a query with turn restrictions is created as:

.. literalinclude:: doc-trsp.queries
   :start-after: --q3
   :end-before: --q4

An example query using vertex ids and via points:

.. literalinclude:: doc-trsp.queries
   :start-after: --q4
   :end-before: --q5

An example query using edge ids and via:

.. literalinclude:: doc-trsp.queries
   :start-after: --q5
   :end-before: --q6

The queries use the :doc:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* `Deprecated documentation <https://docs.pgrouting.org/3.3/en/pgr_trsp.html>`_
* :doc:`trsp_migration`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
