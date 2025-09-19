..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Turn Restriction Shortest Path Family ; pgr_trsp
   single: Shortest Path Category ; pgr_trsp
   single: Restrictions Category ; pgr_trsp
   single: trsp

|

``pgr_trsp``
===============================================================================

``pgr_trsp`` - routing vertices with restrictions.

.. rubric:: Availability

.. rubric:: Version 4.0.0

* Function promoted to official.

.. Breaking change

* Breaking change, signatures no longer available:

  * pgr_trsp(text,integer,integer,boolean,boolean,text)
  * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)

.. rubric:: Version 3.4.0

* New proposed signatures:

  * pgr_trsp(One to One)
  * pgr_trsp(One to Many)
  * pgr_trsp(Many to One)
  * pgr_trsp(Many to Many)
  * pgr_trsp(Combinations)

* Deprecated signatures

  * pgr_trsp(text,integer,integer,boolean,boolean,text)
  * pgr_trsp(text,integer,double precision,integer,double precision,boolean,boolean,text)
  * pgr_trspViaVertices(text,anyarray,boolean,boolean,text)
  * pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)

.. rubric:: Version 2.1.0

* New prototypes

  * pgr_trspViaVertices
  * pgr_trspViaEdges

.. rubric:: Version 2.0.0

* Official function.

Description
-------------------------------------------------------------------------------

Turn restricted shortest path (TRSP) is an algorithm that receives
turn restrictions in form of a query like those found in
real world navigable road networks.

The main characteristics are:

* It does no guarantee the shortest path as it might contain restriction paths.

The general algorithm is as follows:

* Execute a Dijkstra.
* If the solution passes thru a restriction then.

  * Execute the **TRSP** algorithm with restrictions.

|Boost| Boost Graph Inside

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. admonition:: \ \
   :class: signatures

   | pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vid**, **end vid**, [``directed``])
   | pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vid**, **end vids**, [``directed``])
   | pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vids**, **end vid**, [``directed``])
   | pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vids**, **end vids**, [``directed``])
   | pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

.. index::
    single: trsp ; One to One

One to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vid**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertex :math:`10` on an undirected graph.

.. literalinclude:: trsp.queries
   :start-after: -- q2
   :end-before: -- q3

.. index::
    single: trsp ; One to Many

One to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vid**, **end vids**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertex :math:`6` to vertices :math:`\{10, 1\}` on an
          undirected graph.

.. literalinclude:: trsp.queries
   :start-after: -- q3
   :end-before: -- q4

.. index::
    single: trsp ; Many to One

Many to One
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vids**, **end vid**, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertex :math:`8` on a directed
          graph.

.. literalinclude:: trsp.queries
   :start-after: -- q4
   :end-before: -- q5

.. index::
    single: trsp ; Many to Many

Many to Many
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, **start vids**, **end vids**,
   [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: From vertices :math:`\{6, 1\}` to vertices :math:`\{10, 8\}` on an
          undirected graph.

.. literalinclude:: trsp.queries
   :start-after: -- q5
   :end-before: -- q6

.. index::
    single: trsp ; Combinations

Combinations
...............................................................................

.. admonition:: \ \
   :class: signatures

   pgr_trsp(`Edges SQL`_, `Restrictions SQL`_, `Combinations SQL`_, [``directed``])

   | Returns set of |short-generic-result|
   | OR EMPTY SET

:Example: Using a combinations table on an undirected graph.

.. literalinclude:: trsp.queries
   :start-after: -- q6
   :end-before: -- q7


Parameters
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: restriction_parameters_start
    :end-before: restriction_parameters_end


Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

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

Combinations SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_combinations_sql_start
    :end-before: basic_combinations_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: return_path_complete_start
    :end-before: return_path_complete_end

See Also
-------------------------------------------------------------------------------

* :doc:`TRSP-family`
* `Deprecated documentation <https://docs.pgrouting.org/3.3/en/pgr_trsp.html>`_
* :doc:`migration`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
