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
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_turnRestrictedPath.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_turnRestrictedPath.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_turnRestrictedPath.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_turnRestrictedPath.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_turnRestrictedPath.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_turnRestrictedPath.html>`__

pgr_turnRestrictedPath - Experimental
===============================================================================

``pgr_turnRestrictedPath`` Using Yen's algorithm Vertex -Vertex routing with
restrictions

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New experimental function

Description
-------------------------------------------------------------------------------

Using Yen's algorithm to obtain K shortest paths and analyze the paths to select
the paths that do not use the restrictions

Signatures
-------------------------------------------------------------------------------

.. index::
   single: turnRestrictedPath - Experimental on v3.0

.. admonition:: \ \
   :class: signatures

   | pgr_turnRestrictedPath(`Edges SQL`_, `Restrictions SQL`_, **start vid**, **end vid**, **K**, [**options**])
   | **options:** ``[directed, heap_paths, stop_on_first, strict]``

   | RETURNS SET OF |ksp-result|
   | OR EMPTY SET

:Example: From vertex :math:`3` to vertex  :math:`8` on a directed graph

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q1
   :end-before: -- q2

Parameters
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
    :start-after: ksp_parameters_start
    :end-before: ksp_parameters_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

KSP Optional parameters
-------------------------------------------------------------------------------

.. include:: pgr_KSP.rst
    :start-after: ksp_optionals_start
    :end-before: ksp_optionals_end

Special optional parameters
-------------------------------------------------------------------------------

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``stop_on_first``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` stops on first path found that dos not violate
         restrictions
       * When ``false`` returns at most K paths
   * - ``strict``
     - ``BOOLEAN``
     - ``false``
     - * When ``true`` returns only paths that do not violate restrictions
       * When ``false`` returns the paths found

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end


Result Columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_all_columns_start
    :end-before: return_path_all_columns_end

Additional Examples
-------------------------------------------------------------------------------

:Example: From vertex :math:`3` to :math:`8` with ``strict`` flag on.

No results because the only path available follows a restriction.

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q2
   :end-before: -- q3

:Example: From vertex :math:`3` to vertex  :math:`8` on an undirected graph

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q3
   :end-before: -- q4

:Example: From vertex :math:`3` to vertex  :math:`8` with more alternatives

.. literalinclude:: doc-pgr_turnRestrictedPath.queries
   :start-after: -- q4
   :end-before: -- q5


See Also
-------------------------------------------------------------------------------

* :doc:`KSP-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
