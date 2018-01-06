..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _index:

*******************************************************************************
Table of Contents
*******************************************************************************

pgRouting extends the `PostGIS <http://postgis.net>`_/`PostgreSQL <http://postgresql.org>`_ geospatial database to provide geospatial routing and other network analysis functionality.

This is the manual for pgRouting |release|.

.. image:: _static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share Alike 3.0 License <http://creativecommons.org/licenses/by-sa/3.0/>`_. Feel free to use this material any way you like, but we ask that you attribute credit to the pgRouting Project and wherever possible, a link back to http://pgrouting.org. For other licenses used in pgRouting see the :ref:`license` page.

General
===============================================================================

.. toctree::
    :maxdepth: 2

    pgRouting-introduction
    pgRouting-installation
    support


- :doc:`sampledata` that is used in the examples of this manual.

.. toctree::
   :hidden:

   sampledata

Pgrouting Concepts
===============================================================================

.. toctree::
   :maxdepth: 2

   pgRouting-concepts


.. rubric:: Reference

:ref:`pgr_version` - to get pgRouting's version information.

.. toctree::
   :hidden:

   pgr_version


:ref:`Data Types <data_types>`
===============================================================================


- :ref:`type_cost_result` -  A set of records to describe a path result with cost attribute.
- :ref:`pgr_costResult3[]<type_cost_result3>` - A set of records to describe a path result with cost attribute.
- :ref:`pgr_geomResult<type_geom_result>` - A set of records to describe a path result with geometry attribute.

.. toctree::
   :hidden:

   types_index


:ref:`Topology Functions <topology_functions>`
===============================================================================

-  :ref:`pgr_create_topology` -  to create a topology based on the geometry.
-  :ref:`pgr_create_vert_table` - to reconstruct the vertices table based on the source and target information.
-  :ref:`pgr_analyze_graph`  - to analyze the edges and vertices of the edge table.
-  :ref:`pgr_analyze_oneway` - to analyze directionality of the edges.
-  :ref:`pgr_node_network`  -to create nodes to a not noded edge table.

.. toctree::
   :hidden:

   topology-functions


:ref:`Routing functions <routing_functions>`
===============================================================================

.. toctree::
    :hidden:

    routingFunctions


.. include:: routingFunctions.rst
   :start-after: from-here
   :end-before: to-here



Available Functions but not official pgRouting functions
===============================================================================

- :ref:`Stable`
- :ref:`proposed`

.. toctree::
    :hidden:

    proposed



:ref:`Change Log <change_log>`
===============================================================================

.. include:: release_notes.rst
   :start-after: changelog start
   :end-before: changelog end


.. toctree::
    :hidden:

    release_notes


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

