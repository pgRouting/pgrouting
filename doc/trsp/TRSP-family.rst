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
  (`3.4 <https://docs.pgrouting.org/3.4/en/TRSP-family.html>`__)

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
- :doc:`pgr_trsp_withPoints` - Vertex/Point - Vertex/Point routing with restrictions.

.. index proposed to here

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
    pgr_turnRestrictedPath

Introduction
-------------------------------------------------------------------------------

Restrictions
...............................................................................

On road networks, there are restrictions such as left or right turn restrictions,
no U turn restrictions.

A restriction is a sequence of edges, called **path** and that **path** is to be avoided.

.. figure:: /images/with_restrictions.png
   :scale: 50%

   **Restrictions on the road network**

These restrictions are represented on a table as follows:

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --RESTRICTIONS CREATE start
   :end-before: --RESTRICTIONS CREATE end

.. note:: The table has an identifier, which maybe is needed for the administration of the restrictions, but the
   algorithms do not need that information. If given it will be ignored.

Restrictions SQL
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. restrictions_columns_start

========= ===============================  =================================================================================
Column             Type                    Description
========= ===============================  =================================================================================
``path``  ``ARRAY[`` **ANYINTEGER** ``]``  Sequence of Edges identifiers that form a path that is not allowed to be taken
``Cost``  **ANY-NUMERICAL**                Cost of taking the forbidden path
========= ===============================  =================================================================================

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. restrictions_columns_end


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
