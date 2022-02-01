..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_turnRestrictedPath.html>`__
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_turnRestrictedPath.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_turnRestrictedPath.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_turnRestrictedPath.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_turnRestrictedPath.html>`__

pgr_turnRestrictedPath - Experimental
===============================================================================

``pgr_turnRestrictedPath`` — Using Yen's algorithm returns the shortest path with turn restrictions.

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **Experimental** function

.. contents:: Contents

Description
-------------------------------------------------------------------------------

Using Yen's algorithm to obtain K shortest paths and analyze the paths to select the paths that do not use the
restrictions


Signatures
-------------------------------------------------------------------------------

.. index::
   single: turnRestrictedPath - Experimental on v3.0

.. parsed-literal:: none

    pgr_turnRestrictedPath(`Edges SQL`_, `Restrictions SQL`_, Start vid, End vid, K cycles,
      [, directed] [,heap_paths] [, stop_on_first] [,strict])
    RETURNS SETOF (seq, path_id, path_seq, node, edge, cost, agg_cost)



One to One
...............................................................................

.. code-block:: none

    pgr_turnRestrictedPath(`Edges SQL`_, `Restrictions SQL`_, start vid,  end vid  [, directed])
    RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost)
    OR EMPTY SET

:Example: From vertex :math:`8` to vertex  :math:`10` on a **directed** graph

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q1
   :end-before: -- q2

.. index::
    single: turnRestrictedPath(One to Many) -- Proposed on v3.4


Parameters
-------------------------------------------------------------------------------

.. turnRestrictedPath_parameters_start

===================== ================== ========== ==========================================================
Parameter             Type               Default     Description
===================== ================== ========== ==========================================================
**Edges SQL**         ``TEXT``                      `Edges SQL`_ as described below
**Restrictions SQL**  ``TEXT``                      `Restrictions SQL`_ as described below
**start vid**         ``BIGINT``                    Identifier of the starting vertex of the path.
**end vid**           ``BIGINT``                    Identifier of the ending vertex of the path.
**directed**          ``BOOLEAN``         ``true``  - When ``true`` Graph is considered `Directed`
                                                    - When ``false`` the graph is considered as `Undirected`.

**heap_paths**        ``BOOLEAN``         ``false`` - When ``true`` returns all the paths stored in the process heap.
                                                    - When ``false`` only returns ``k`` paths.

**stop on first**     ``BOOLEAN``         ``true``  - When ``true`` stops on first path found that dos not violate restrictions
                                                    - When ``false`` continues work

**strict**            ``BOOLEAN``         ``false`` - When ``true`` returns only paths that do not violate restrictions
                                                    - When ``false`` returns all paths found
===================== ================== ========== ==========================================================

.. pgr_turnRestrictedPath_parameters_end

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


Result Columns
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
   :start-after: ksp results start
   :end-before: ksp results end

Additional Examples
-------------------------------------------------------------------------------

:Example: From vertex :math:`8` to :math:`10` with ``strict`` flag on.

No results show because the only path available follows a restriction.

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: From vertex :math:`8` to vertex  :math:`10` on an **undirected** graph

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q3
   :end-before: -- q4

:Example: From vertex :math:`8` to vertex  :math:`10` with more alternatives

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q4
   :end-before: -- q5


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
