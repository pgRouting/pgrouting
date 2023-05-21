..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_degree`` -- Proposed
===============================================================================

``pgr_degree`` — For each vertex in an undirected graph, return the count of edges incident to the vertex.


.. include:: proposed.rst
   :start-after: stable-begin-warning
   :end-before: stable-end-warning

.. rubric:: Availability

* Version 3.4.0

  * New **proposed** function


Description
-------------------------------------------------------------------------------

Calculates the degree of the vertices of an **undirected** graph


Signatures
-------------------------------------------------------------------------------

.. index::
    single: pgr_degree - Proposed on v3.4

.. admonition:: \ \
   :class: signatures

   | pgr_degree(`Edges SQL`_ , `Vertex SQL`_, [``dryrun``])

   | RETURNS SETOF |result-degree|
   | OR EMTPY SET

:Example: Extracting the vertex information

pgr_degree can utilize output from `pgr_extractVertices` or can have `pgr_extractVertices` embedded in the call.
For decent size networks, it is best to prep your vertices table before hand and use that vertices table
for pgr_degree calls.

.. literalinclude:: degree.queries
   :start-after: -- q1
   :end-before: -- q2


Parameters
-------------------------------------------------------------------------------

============== ========  ================================
Parameter      Type      Description
============== ========  ================================
`Edges SQL`_   ``TEXT``  `Edges SQL`_ as described below
`Vertex SQL`_  ``TEXT``  `Vertex SQL`_ as described below
============== ========  ================================

Optional parameters
-------------------------------------------------------------------------------

.. TODO move to pgRouting concepts

=========== =============  ========== =======================================
Parameter    Type          Default    Description
=========== =============  ========== =======================================
``dryrun``  ``BOOLEAN``    ``false``  * When true do not process and get in a
                                        NOTICE the resulting query.
=========== =============  ========== =======================================

Inner Queries
-------------------------------------------------------------------------------

.. contents::
   :local:

Edges SQL
...............................................................................

.. TODO move to pgRouting concepts

================= =================== ===================================
Column            Type                Description
================= =================== ===================================
``id``            ``BIGINT``          Identifier of the edge.
================= =================== ===================================

Vertex SQL
...............................................................................

.. TODO move to pgRouting concepts

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - ``BIGINT``
     - Identifier of the first end point vertex of the edge.
   * - ``in_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *first
       end point*.

       * When missing, ``out_edges`` must exist.
   * - ``out_edges``
     - ``BIGINT[]``
     - Array of identifiers of the edges that have the vertex ``id`` as *second
       end point*.

       * When missing, ``in_edges`` must exist.


Result Columns
-------------------------------------------------------------------------------

.. TODO move to pgRouting concepts

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``node``
     - ``BIGINT``
     - Vertex identifier
   * - ``degree``
     - ``BIGINT``
     - Number of edges that are incident to the vertex ``id``

Additional Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Degree of a sub graph
...............................................................................

.. literalinclude:: degree.queries
   :start-after: -- q2
   :end-before: -- q3

Dry run execution
...............................................................................

To get the query generated used to get the vertex information, use ``dryrun =>
true``.

The results can be used as base code to make a refinement based on the back end
development needs.

.. literalinclude:: degree.queries
   :start-after: -- q3
   :end-before: -- q4

Degree from an existing table
...............................................................................
If you have a vertices table already built using ``pgr_extractVertices``
and want the degree of the whole graph rather than a subset, you can forgo using pgr_degree
and work with the ``in_edges`` and ``out_edges`` columns directly.


.. include:: pgRouting-concepts.rst
   :start-after: degree_from_table_start
   :end-before: degree_from_table_end


See Also
-------------------------------------------------------------------------------

* :doc:`topology-functions`
* :doc:`pgr_extractVertices`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
