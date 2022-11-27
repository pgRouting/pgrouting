..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/TRSP-family.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/TRSP-family.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/TRSP-family.html>`__

TRSP - Family of functions
===============================================================================

When points are also given as input:

.. rubric:: Proposed

.. include:: proposed.rst
   :start-after: begin-warning
   :end-before: end-warning

.. index proposed from here

- :doc:`pgr_trsp` - Vertex - Vertex routing with restrictions.
- :doc:`pgr_trspVia` - Via Vertices routing with restrictions.
- :doc:`pgr_trsp_withPoints` - Vertex/Point routing with restrictions.
- :doc:`pgr_trspVia_withPoints` - Via Vertex/point routing with restrictions.

.. index proposed to here

.. Warning:: Read the :doc:`migration` about how to migrate from the
   deprecated TRSP functionality to the new signatures or replacement functions.

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index experimental from here

- :doc:`pgr_turnRestrictedPath` - Routing with restrictions.

.. index experimental to here

.. toctree::
    :hidden:

    pgr_trsp
    pgr_trspVia
    pgr_trsp_withPoints
    pgr_trspVia_withPoints
    pgr_turnRestrictedPath

Introduction
-------------------------------------------------------------------------------

Road restrictions are a sequence of road segments that can not be taken in a
sequential manner.
Some restrictions are implicit on a directed graph, for example, one way roads
where the wrong way edge is not even inserted on the graph.
But normally on turns like no left turn or no right turn, hence the name turn
restrictions, there are sometimes restrictions.

.. image:: images/restrictions.png


TRSP algorithm
...............................................................................

The internal TRSP algorithm performs a lookahead over the dijkstra algorithm in
order to find out if the attempted path has a restriction. This allows the
algorithm to pass twice on the same vertex.

Parameters
...............................................................................

.. via_parameters_start

.. list-table::
   :width: 81
   :widths: 18 20 40
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ query as described.
   * - `Restrictions SQL`_
     - ``TEXT``
     - `Restrictions SQL`_ query as described.
   * - **via vertices**
     - ``ARRAY[`` **ANY-INTEGER** ``]``
     - Array of ordered vertices identifiers that are going to be visited.

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT

.. via_parameters_end

Restrictions
...............................................................................

On road networks, there are restrictions such as left or right turn restrictions,
no U turn restrictions.

A restriction is a sequence of edges, called path and that path is to be
avoided.

.. TODO update the image

.. figure:: /images/with_restrictions.png
   :scale: 50%

   **Restrictions on the road network**

These restrictions are represented on a table as follows:

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --RESTRICTIONS CREATE start
   :end-before: --RESTRICTIONS CREATE end

.. note:: The table has an identifier, which maybe is needed for the
   administration of the restrictions, but the algorithms do not need that
   information. If given it will be ignored.


Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Restrictions SQL
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. include:: pgRouting-concepts.rst
   :start-after: restrictions_columns_start
   :end-before: restrictions_columns_end


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
