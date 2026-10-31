:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2022-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0


.. index:: Circuits Family

|

Circuits - Family of Functions
===============================================================================

The following functions work on circuits of directed graphs.

.. include:: experimental.rst
   :start-after: warning-begin
   :end-before: end-warning

.. experimental-start

* :doc:`pgr_hawickCircuits` - Hawick's circuits algorithm.

.. experimental-end

.. toctree::
    :hidden:

    pgr_hawickCircuits

Description
-------------------------------------------------------------------------------

Parameters
-------------------------------------------------------------------------------

.. circuits_parameters_start

.. list-table::
   :width: 81
   :widths: 10,15,40
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below.
   * - ``start vid``
     - |ANY-INTEGER|
     - A vertex of the circuits:
       - All the circuits that contain the vertex are returned.
       - When ``0`` all the circuits are returned in canonical order.
   * - ``start vids``
     - ``ARRAY[`` |ANY-INTEGER| ``]``
     - Vertices of the circuits: for each requested vertex:
       - All the circuits that contain the vertex are returned:
       - When empty all the circuits are returned in canonical order.


.. circuits_parameters_end

Optional parameters
...............................................................................

.. circuits_optionals_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Default
     - Description
   * - ``min_depth``
     - ``BIGINT``
     - ``1``
     - Minimum number of vertices in a circuit.
       * Exclude 2 edge circuits by increasing this number.
   * - ``max_depth``
     - ``BIGINT``
     - ``9223372036854775807``
     - Maximum number of vertices in a circuit.
   * - ``self_loops``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` self loops are included in the results.
       * When ``false`` self loops are excluded from the results.
       * Self loops are included or excluded regardless of ``min_depth`` and ``max_depth``.
   * - ``u_turn_on_edge``
     - ``BOOLEAN``
     - ``true``
     - * A u-turn on an edge is a 2 edge circuit that uses the same ``id`` on
         both directions of the edge.
       * When ``true``: u-turns are part of the results.
       * When ``false``: u-turns are discarded when the circuit can be formed
         with two different edges; a u-turn is kept when it is the only
         option, that is, when the two vertices are connected by a single
         edge.
       * Self loops are not affected.

.. circuits_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. circuits_result_columns_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from ``1``
   * - ``path_id``
     - ``INTEGER``
     - Id of the circuit starting from ``1``
   * - ``depth``
     - ``INTEGER``
     - Relative position in the circuit. Has value **0** for the beginning of a
       circuit
   * - ``start_vid``
     - ``BIGINT``
     - Identifier of the starting vertex of the circuit.
   * - ``end_vid``
     - ``BIGINT``
     - Identifier of the ending vertex of the circuit.
   * - ``pred``
     - ``BIGINT``
     - Identifier of the node that precedes ``node`` in the circuit.

       - When ``node`` is the first node of the circuit, it has the same value
         as ``node``.
   * - ``node``
     - ``BIGINT``
     - Identifier of the node in the path from a vid to next vid.
   * - ``edge``
     - ``BIGINT``
     - Identifier of the edge used to arrive from ``pred`` to ``node``.
       - ``-1`` when :math:`depth = 0`.
   * - ``cost``
     - ``FLOAT``
     - Cost to traverse from ``pred`` to ``node`` using ``edge``.
       - ``0`` when :math:`depth = 0`.
   * - ``agg_cost``
     - ``FLOAT``
     - Aggregate cost from ``start_vid`` to ``node``.

.. circuits_result_columns_end

See Also
-------------------------------------------------------------------------------

* :doc:`sampledata`
* `Boost: Hawick Circuit Algorithm
  <https://www.boost.org/doc/libs/1_78_0/libs/graph/doc/hawick_circuits.html>`__

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
