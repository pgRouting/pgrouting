..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_drivingDistance``
===============================================================================

``pgr_drivingDistance`` - Returns the driving distance from a start node.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/libs/graph/doc/table_of_contents.html

   Boost Graph Inside

.. rubric:: Availability

.. rubric:: Version 3.6.0

* Standarizing output columns to |result-spantree|

  * ``pgr_drivingdistance`` (Single vertex)

    * Added ``depth`` and ``start_vid`` result columns.

  * ``pgr_drivingdistance`` (Multiple vertices)

    * Result column name change: ``from_v`` to ``start_vid``.
    * Added ``depth`` and ``pred`` result columns.

.. rubric:: Version 2.1.0

* Signature change pgr_drivingDistance(single vertex)
* New **Official** pgr_drivingDistance(multiple vertices)

.. rubric:: Version 2.0.0

* Official:: pgr_drivingDistance(single vertex)


Description
-------------------------------------------------------------------------------

Using the Dijkstra algorithm, extracts all the nodes that have costs less than
or equal to the value ``distance``.
The edges extracted will conform to the corresponding spaning tree.

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_drivingDistance(`Edges SQL`_, **Root vid**, **distance**, [``directed``])
   | pgr_drivingDistance(`Edges SQL`_, **Root vids**, **distance**, [**options**])
   | **options:** [directed, equicost]

   | Returns set of |result-spantree|

.. index::
   single: drivingDistance(Single vertex)

Single Vertex
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_drivingDistance(`Edges SQL`_, **Root vid**, **distance**, [``directed``])

   | Returns set of |result-spantree|

:Example: From vertex :math:`11` for a distance of :math:`3.0`

.. literalinclude:: drivingDistance.queries
   :start-after: --q5
   :end-before: --q6

.. index::
   single: drivingDistance(Multiple vertices)

Multiple Vertices
...............................................................................

.. admonition:: \ \
   :class: signatures

   | pgr_drivingDistance(`Edges SQL`_, **Root vids**, **distance**, [**options**])
   | **options:** [directed, equicost]

   | Returns set of |result-spantree|

:Example: From vertices :math:`\{11, 16\}` for a distance of :math:`3.0` with
          equi-cost on a directed graph

.. literalinclude:: drivingDistance.queries
   :start-after: --q6
   :end-before: --q10

Parameters
-------------------------------------------------------------------------------

.. include:: drivingDistance-category.rst
    :start-after: spantree-params_start
    :end-before: spantree-params_end

Optional parameters
...............................................................................

.. include:: dijkstra-family.rst
    :start-after: dijkstra_optionals_start
    :end-before: dijkstra_optionals_end

Driving distance optional parameters
...............................................................................

.. equicost_start

.. list-table::
   :width: 81
   :widths: 12, 8, 8, 60
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``equicost``
     - ``BOOLEAN``
     - ``true``
     - * When ``true`` the node will only appear in the closest ``start_vid``
         list. Tie brakes are arbitrary.
       * When ``false`` which resembles several calls using the single vertex
         signature.

.. equicost_end

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
..............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result columns
-------------------------------------------------------------------------------

.. include:: drivingDistance-category.rst
   :start-after: spantree-result-columns-start
   :end-before: spantree-result-columns-end

Additional Examples
-------------------------------------------------------------------------------

:Example: From vertices :math:`\{11, 16\}` for a distance of :math:`3.0` on an
          undirected graph

.. literalinclude:: drivingDistance.queries
   :start-after: --q10
   :end-before: --q15

See Also
-------------------------------------------------------------------------------

* :doc:`pgr_alphaShape` - Alpha shape computation
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

