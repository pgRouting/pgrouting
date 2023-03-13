..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/migration.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/migration.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/migration.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/migration.html>`__

Migration guide
===============================================================================

Several functions are having changes on the signatures, and/or have been
replaced by new functions.

Results can be different because of the changes.

Migrating functions:

:doc:`pgr_maxCardinalityMatch` works only for undirected graphs, therefore the
``directed`` flag has been removed.

:doc:`pgr_trsp` signatures have changed and many issues have been fixed in the new
signatures. This section will show how to migrate from the old signatures to the
new replacement functions. This also affects the restrictions.

.. warning::
   All deprecated functions will be removed on next mayor version 4.0.0

.. contents:: Contents

Migration of ``pgr_dijkstra``
-------------------------------------------------------------------------------

Starting from `v3.5.0 <https://docs.pgrouting.org/3.5/en/migration.html>`__

Signatures to be migrated:

* ``pgr_dijkstra`` (`One to One`)
* ``pgr_dijkstra`` (`One to Many`)
* ``pgr_dijkstra`` (`Many to One`)

:Before Migration:

* Output columns are |old-generic-result|

  * Depending on the overload used the columns ``start_vid`` and ``end_vid``
    might be missing:

    * ``pgr_dijkstra`` (`One to One`) does not have ``start_vid`` and
      ``end_vid``.
    * ``pgr_dijkstra`` (`One to Many`) does not have ``start_vid``.
    * ``pgr_dijkstra`` (`Many to One`) does not have ``end_vid``.

:Migration:

* Be aware of the existance of the additional columns.

* In ``pgr_dijkstra`` (`One to One`)

  * ``start_vid`` contains the **start vid** parameter value.
  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra1
   :end-before: --dijkstra2

* In ``pgr_dijkstra`` (`One to Many`)

  * ``start_vid`` contains the **start vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra2
   :end-before: --dijkstra3

* In ``pgr_dijkstra`` (`Many to One`)

  * ``end_vid`` contains the **end vid** parameter value.

.. literalinclude:: migration.queries
   :start-after: --dijkstra3
   :end-before: --dijkstra4

* If needed filter out the added columns, for example:

.. literalinclude:: migration.queries
   :start-after: --dijkstra4
   :end-before: --dijkstra5

* If needed add the new columns, for example:

  * In `v3.0 <https://docs.pgrouting.org/3.0/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` uses ``pgr_dijkstra``.
  * Starting from `v3.5 <https://docs.pgrouting.org/3.5/en/contraction-family.html#case-1-both-source-and-target-belong-to-the-contracted-graph>`__
    the function ``my_dijkstra`` returns the new additional columns of
    ``pgr_dijkstra``.

Migration of ``pgr_maxCardinalityMatch``
-------------------------------------------------------------------------------

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

Signature to be migrated:

.. parsed-literal::

  pgr_maxCardinalityMatch(Edges SQL, [directed])
   RETURNS SETOF (seq, edge, source, target)

Migration is needed, because:

* Use ``cost`` and ``reverse_cost`` on the inner query
* Results are ordered
* Works for undirected graphs.
* New signature

  * ``pgr_maxCardinalityMatch(text)`` returns only ``edge`` column.
  * The optional flag ``directed`` is removed.

:Before migration:

.. literalinclude:: migration.queries
   :start-after: --maxcard1
   :end-before: --maxcard2

* Columns used are ``going`` and ``coming`` to represent the existence of an
  edge.
* Flag ``directed`` was used to indicate if it was for a **directed** or
  **undirected** graph.

  * The flag ``directed`` is ignored.

    * Regardless of it's value it gives the result considering the graph as
      **undirected**.

:Migration:

* Use the columns ``cost`` and ``reverse_cost`` to represent the existence of an
  edge.
* Do not use the flag ``directed``.
* In the query returns only ``edge`` column.

.. literalinclude:: migration.queries
   :start-after: --maxcard2
   :end-before: --maxcard3

Migration of restrictions
-------------------------------------------------------------------------------

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

The structure of the restrictions have changed:

Old restrictions structure
...............................................................................

On the deprecated signatures:

* Column ``rid`` is ignored
* ``via_path``

  * Must be in reverse order.
  * Is of type ``TEXT``.
  * When more than one via edge must be separated with ``,``.

* ``target_id``

  * Is the last edge of the forbidden path.
  * Is of type ``INTEGER``.

* ``to_cost``

  * Is of type ``FLOAT``.

Creation of the old restrictions table

.. literalinclude:: migration.queries
   :start-after: --rest00
   :end-before: --rest01

Old restrictions fill up

.. literalinclude:: migration.queries
   :start-after: --rest01
   :end-before: --rest1

Old restrictions contents
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. literalinclude:: migration.queries
   :start-after: --rest1
   :end-before: --rest2

The restriction with ``rid = 2`` is representing :math:`3 \rightarrow 5
\rightarrow9`

* :math:`3\rightarrow5`

  * is on column ``via_path`` in reverse order
  * is of type ``TEXT``

* :math:`9`

  * is on column ``target_id``
  * is of type ``INTEGER``


New restrictions structure
...............................................................................

* Column ``id`` is ignored
* Column ``path``

  * Is of type ``ARRAY[ANY-INTEGER]``.
  * Contains all the edges involved on the restriction.
  * The array has the ordered edges of the restriction.

* Column ``cost``

  * Is of type ``ANY-NUMERICAL``

.. include:: sampledata.rst
   :start-after: the_restrictions_start
   :end-before: the_restrictions_end


The restriction with ``rid = 2`` represents the path :math:`3 \rightarrow5
\rightarrow9`.

* By inspection the path is clear.

Migration
...............................................................................

To transform the old restrictions table to the new restrictions structure,

* Create a new table with the new restrictions structure.

  * In this migration guide ``new_restrictions`` is been used.

* For this migration pgRouting supplies an auxiliary function for reversal of an
  array ``_pgr_array_reverse`` needed for the migration.

  * ``_pgr_array_reverse``:

    * Was created temporally for this migration
    * Is not documented.
    * Will be removed on the next mayor version 4.0.0

.. literalinclude:: migration.queries
   :start-after: --rest3
   :end-before: --rest4

The migrated table contents:

.. literalinclude:: migration.queries
   :start-after: --rest4
   :end-before: --rest5


Migration of ``pgr_trsp`` (Vertices)
-------------------------------------------------------------------------------

Starting from `v3.4.0 <https://docs.pgrouting.org/3.4/en/migration.html>`__

Signature to be migrated:

.. parsed-literal::

  pgr_trsp(Edges SQL, source, target,
           directed boolean, has_rcost boolean
           [,restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, cost)

* The integral type of the ``Edges SQL`` can only be ``INTEGER``.
* The floating point type of the ``Edges SQL`` can only be ``FLOAT``.
* ``directed`` flag is compulsory.

  * Does not have a default value.

* Does not autodetect if ``reverse_cost`` column exist.

  * User must be careful to match the existence of the column with the value of
    ``has_rcost`` parameter.

* The restrictions inner query is optional.
* The output column names are meaningless

Migrate by using:

*  :doc:`pgr_dijkstra` when there are no restrictions,
*  :doc:`pgr_trsp` (One to One) when there are restrictions.


Migrating ``pgr_trsp`` (Vertices) using ``pgr_dijkstra``
...............................................................................

The following query does not have restrictions.

.. literalinclude:: migration.queries
   :start-after: --verticesv1
   :end-before: --verticesv2

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

Use :doc:`pgr_dijkstra` instead.


.. literalinclude:: migration.queries
   :start-after: --verticesv2
   :end-before: --verticesv3

* The types casting has been removed.
* :doc:`pgr_dijkstra`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.

When the need of using strictly the same (meaningless) names and types of the
function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --verticesv3
   :end-before: --verticesv4

* ``id1`` is the node
* ``id2`` is the edge


Migrating ``pgr_trsp`` (Vertices) using ``pgr_trsp``
...............................................................................

The following query has restrictions.

.. literalinclude:: migration.queries
   :start-after: --verticesv4
   :end-before: --verticesv5

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

* The restrictions are the last parameter of the function

  * Using the old structure of restrictions

Use :doc:`pgr_trsp` (One to One) instead.

.. literalinclude:: migration.queries
   :start-after: --verticesv5
   :end-before: --verticesv6

* The new structure of restrictions is been used.

  * It is the second parameter.

* The types casting has been removed.
* :doc:`pgr_trsp`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.

When the need of using strictly the same (meaningless) names and types of the
function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --verticesv6
   :end-before: --verticesv7

* ``id1`` is the node
* ``id2`` is the edge

Migration of ``pgr_trsp`` (Edges)
-------------------------------------------------------------------------------

Signature to be migrated:

.. parsed-literal::

   pgr_trsp(sql text, source_edge integer, source_pos float8,
                    target_edge integer, target_pos float8,
                    directed boolean, has_rcost boolean
                    [,restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, cost)

* The integral types of the ``sql`` can only be ``INTEGER``.
* The floating point type of the ``sql`` can only be ``FLOAT``.
* ``directed`` flag is compulsory.

  * Does not have a default value.

* Does not autodetect if ``reverse_cost`` column exist.

  * User must be careful to match the existence of the column with the value of
    ``has_rcost`` parameter.

* The restrictions inner query is optional.

For these migration guide the following points will be used:

.. literalinclude:: migration.queries
   :start-after: --verticesv7
   :end-before: --edgesv1

Migrate by using:

*  :doc:`pgr_withPoints` when there are no restrictions,
*  :doc:`pgr_trsp_withPoints` (One to One) when there are restrictions.

Migrating ``pgr_trsp`` (Edges) using ``pgr_withPoints``
...............................................................................

The following query does not have restrictions.

.. literalinclude:: migration.queries
   :start-after: --edgesv1
   :end-before: --edgesv2

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

Use :doc:`pgr_withPoints` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesv2
   :end-before: --edgesv3

* The types casting has been removed.
* Do not show details, as the deprecated function does not show details.
* :doc:`pgr_withPoints`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types, and node
values of the function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --edgesv3
   :end-before: --edgesv4

* ``id1`` is the node
* ``id2`` is the edge


Migrating ``pgr_trsp`` (Edges) using ``pgr_trsp_withPoints``
...............................................................................

The following query has restrictions.

.. literalinclude:: migration.queries
   :start-after: --edgesv4
   :end-before: --edgesv5

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

* The restrictions are the last parameter of the function

  * Using the old structure of restrictions

Use :doc:`pgr_trsp_withPoints` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesv5
   :end-before: --edgesv6

* The new structure of restrictions is been used.

  * It is the second parameter.

* The types casting has been removed.
* Do not show details, as the deprecated function does not show details.
* :doc:`pgr_trsp_withPoints`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types, and node
values of the function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --edgesv6
   :end-before: --edgesv7

* ``id1`` is the node
* ``id2`` is the edge

Migration of ``pgr_trspViaVertices``
-------------------------------------------------------------------------------

Signature to be migrated:

.. parsed-literal::

   pgr_trspViaVertices(sql text, vids integer[],
                     directed boolean, has_rcost boolean
                     [, turn_restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, id3, cost)


* The integral types of the ``Edges SQL`` can only be ``INTEGER``.
* The floating point type of the ``Edges SQL`` can only be ``FLOAT``.
* ``directed`` flag is compulsory.

  * Does not have a default value.

* Does not autodetect if ``reverse_cost`` column exist.

  * User must be careful to match the existence of the column with the value of
    ``has_rcost`` parameter.

* The restrictions inner query is optional.


Migrate by using:

*  :doc:`pgr_dijkstraVia` when there are no restrictions,
*  :doc:`pgr_trspVia` when there are restrictions.

Migrating ``pgr_trspViaVertices`` using ``pgr_dijkstraVia``
...............................................................................

The following query does not have restrictions.

.. literalinclude:: migration.queries
   :start-after: --viav1
   :end-before: --viav2

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

Use :doc:`pgr_dijkstraVia` instead.

.. literalinclude:: migration.queries
   :start-after: --viav2
   :end-before: --viav3

* The types casting has been removed.
* :doc:`pgr_dijkstraVia`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types of the
function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --viav3
   :end-before: --viav4

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge


Migrating ``pgr_trspViaVertices`` using ``pgr_trspVia``
...............................................................................

The following query has restrictions.

.. literalinclude:: migration.queries
   :start-after: --viav4
   :end-before: --viav5

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

* The restrictions are the last parameter of the function

  * Using the old structure of restrictions

Use :doc:`pgr_trspVia` instead.

.. literalinclude:: migration.queries
   :start-after: --viav5
   :end-before: --viav6

* The new structure of restrictions is been used.

  * It is the second parameter.

* The types casting has been removed.
* :doc:`pgr_trspVia`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types of the
function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --viav6
   :end-before: --viav7

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Migration of ``pgr_trspViaEdges``
-------------------------------------------------------------------------------

Signature to be migrated:

.. parsed-literal::

   pgr_trspViaEdges(sql text, eids integer[], pcts float8[],
                     directed boolean, has_rcost boolean
                     [, turn_restrict_sql text]);
   RETURNS SETOF (seq, id1, id2, id3, cost)


* The integral types of the ``Edges SQL`` can only be ``INTEGER``.
* The floating point type of the ``Edges SQL`` can only be ``FLOAT``.
* ``directed`` flag is compulsory.

  * Does not have a default value.

* Does not autodetect if ``reverse_cost`` column exist.

  * User must be careful to match the existence of the column with the value of
    ``has_rcost`` parameter.

* The restrictions inner query is optional.

For these migration guide the following points will be used:

.. literalinclude:: migration.queries
   :start-after: --viav7
   :end-before: --edgesvia1

And will travel thru the following Via points :math:`4\rightarrow3\rightarrow6`

Migrate by using:

*  :doc:`pgr_withPointsVia` when there are no restrictions,
*  :doc:`pgr_trspVia_withPoints` when there are restrictions.

Migrating ``pgr_trspViaEdges`` using ``pgr_withPointsVia``
...............................................................................

The following query does not have restrictions.

.. literalinclude:: migration.queries
   :start-after: --edgesvia1
   :end-before: --edgesvia2

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

Use :doc:`pgr_withPointsVia` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesvia2
   :end-before: --edgesvia3

* The types casting has been removed.
* Do not show details, as the deprecated function does not show details.
* :doc:`pgr_withPointsVia`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types, and node
values of the function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --edgesvia3
   :end-before: --edgesvia4

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

Migrating ``pgr_trspViaEdges`` using ``pgr_trspVia_withPoints``
...............................................................................

The following query has restrictions.

.. literalinclude:: migration.queries
   :start-after: --edgesvia4
   :end-before: --edgesvia5

* A message about deprecation is shown

  * Deprecated functions will be removed on the next mayor version 4.0.0

* The restrictions are the last parameter of the function

  * Using the old structure of restrictions

Use :doc:`pgr_trspVia_withPoints` instead.

.. literalinclude:: migration.queries
   :start-after: --edgesvia5
   :end-before: --edgesvia6

* The new structure of restrictions is been used.

  * It is the second parameter.

* The types casting has been removed.
* Do not show details, as the deprecated function does not show details.
* :doc:`pgr_trspVia_withPoints`:

  * Autodetects if ``reverse_cost`` column is in the edges
    SQL.
  * Accepts ``ANY-INTEGER`` on integral types
  * Accepts ``ANY-NUMERICAL`` on floating point types
  * ``directed`` flag has a default value of ``true``.

    * Use the same value that on the original query.
    * In this example it is ``true`` which is the default value.

      * The flag has been omitted and the default is been used.
  * On the points query do not include the ``side`` column.

When the need of using strictly the same (meaningless) names and types, and node
values of the function been migrated then:

.. literalinclude:: migration.queries
   :start-after: --edgesvia6
   :end-before: --edgesvia7

* ``id1`` is the path identifier
* ``id2`` is the node
* ``id3`` is the edge

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* :doc:`withPoints-category`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
