..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. index::
   single: Utilities ; pgr_separateCrossing - proposed on v3.8.0
   single: separateCrossing - proposed on v3.8.0

|

``pgr_separateCrossing``
===============================================================================

``pgr_separateCrossing`` - From crossing geometries generates geometries that
do not cross.

.. rubric:: Availability

.. rubric:: Version 3.8.0

* New proposed function.

Description
-------------------------------------------------------------------------------

This is an auxiliary function for separating crossing edges.

Signature
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_separateCrossing(`Edges SQL`_, [``tolerance``, ``dryrun``])

   | RETURNS |result-separate|

:Example: Get the segments of the crossing geometries

.. literalinclude:: separateCrossing.queries
   :start-after: --q1
   :end-before: --q2

Parameters
-------------------------------------------------------------------------------

============== ========  ================================
Parameter      Type      Description
============== ========  ================================
`Edges SQL`_   ``TEXT``  `Edges SQL`_ as described below
============== ========  ================================

Optional parameters
-------------------------------------------------------------------------------

============= =========== ========= =======================================
Parameter     Type        Default   Description
============= =========== ========= =======================================
``tolerance`` ``FLOAT``   0.01      Used in ST_Split
``dryrun``    ``BOOLEAN`` ``false`` * When true do not process and get in a
                                      NOTICE the resulting query.
============= =========== ========= =======================================

Inner Queries
-------------------------------------------------------------------------------

Edges SQL
...............................................................................

================= =================== ===================================
Column            Type                Description
================= =================== ===================================
``id``            ``BIGINT``          (Optional) identifier of the edge.
``geom``          ``LINESTRING``      Geometry of the edge.
================= =================== ===================================

Examples
-------------------------------------------------------------------------------

.. contents::
   :local:

Get the code for further refinement.
...............................................................................

When there are special details that need to be taken care of because of the
final application or the quality of the data, the code can be obtained On a
PostgreSQL ``NOTICE`` using the ``dryrun`` flag.

.. literalinclude:: separateCrossing.queries
   :start-after: --q2
   :end-before: --q3

.. fix_intersection_start

Fixing an intersection
...............................................................................

In this example the original edge table will be used to store the additional
geometries.

Create the topology for the data in :doc:`sampledata`

.. literalinclude:: sampledata.queries
   :start-after: -- q1
   :end-before: -- q1-1

.. literalinclude:: sampledata.queries
   :start-after: -- q3
   :end-before: -- q4

Routing from :math:`1` to :math:`2` gives no solution.

.. literalinclude:: separateCrossing.queries
   :start-after: --q3
   :end-before: --q4

Analyze the network for intersections.

.. literalinclude:: separateCrossing.queries
   :start-after: --q4
   :end-before: --q5

The analysis tell us that the network has an intersection.


Additional columns to control the origin of the segments.

.. literalinclude:: separateCrossing.queries
   :start-after: --q5
   :end-before: --q6

Calling :doc:`pgr_separateCrossing` and adding the new segments to the edges table.

.. literalinclude:: separateCrossing.queries
   :start-after: --q6
   :end-before: --q7

Update the cost and reverse_cost values.

In this example the costs are based on the length of the geometry and the
directionality is kept using the ``sign`` function.

.. literalinclude:: separateCrossing.queries
   :start-after: --q7
   :end-before: --q8

.. rubric:: Update the topology

Insert the intersections as new vertices.

.. literalinclude:: separateCrossing.queries
   :start-after: --q8
   :end-before: --q9

Update source and target information on the edges table.

.. literalinclude:: separateCrossing.queries
   :start-after: --q9
   :end-before: --q10

Routing from :math:`1` to :math:`18` gives a solution.

.. literalinclude:: separateCrossing.queries
   :start-after: --q10
   :end-before: --q11

.. fix_intersection_end

See Also
-------------------------------------------------------------------------------

:doc:`topology-functions` for an overview of a topology for routing algorithms.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
