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

.. image:: static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share Alike 3.0 License <http://creativecommons.org/licenses/by-sa/3.0/>`_. Feel free to use this material any way you like, but we ask that you attribute credit to the pgRouting Project and wherever possible, a link back to http://pgrouting.org. For other licenses used in pgRouting see the :ref:`license` page.

*******************************************************************************
General
*******************************************************************************

.. toctree::
    :maxdepth: 2

    src/introduction/introduction
    src/installation/installation
    src/installation/build
    src/introduction/support


*******************************************************************************
Tutorial
*******************************************************************************

:ref:`Tutorial <tutorial1>`

  - :ref:`Getting started <tutorial>`
  - :ref:`topology` for an overview of a topology for routing algorithms.
  - :ref:`analytics` for an overview of the analysis of a graph.
  - :ref:`custom_query` that is used in the routing algorithms.
  - :ref:`performance` to improve your performance.
  - :ref:`recipes`
  - :ref:`developer`

For a more complete introduction how to build a routing application read the `pgRouting Workshop <http://workshop.pgrouting.org>`_.


.. toctree::
   :hidden:

   src/tutorial/index
   src/recipes/index
   src/developer/developer_guide
   
*******************************************************************************
Sample Data
*******************************************************************************

  - :ref:`sampledata` that is used in the examples of this manual.

.. toctree::
   :hidden:

   src/developer/sampledata



*******************************************************************************
Functions
*******************************************************************************

Version
===============================================================================

:ref:`pgr_version` - to get pgRouting's version information.

.. toctree::
   :hidden:

   ../src/common/doc/pgr_version




Data Types
===============================================================================

:ref:`data_types`

  -  :ref:`type_cost_result` -  A set of records to describe a path result with cost attribute.
  -  :ref:`pgr_costResult3[]<type_cost_result3>` - A set of records to describe a path result with cost attribute.
  -  :ref:`pgr_geomResult<type_geom_result>` - A set of records to describe a path result with geometry attribute.

.. toctree::
   :hidden:

   ../src/common/doc/types/index


*******************************************************************************
Topology functions
*******************************************************************************

:ref:`topology_functions`

  -  :ref:`pgr_create_topology` -  to create a topology based on the geometry.
  -  :ref:`pgr_create_vert_table` - to reconstruct the vertices table based on the source and target information.
  -  :ref:`pgr_analyze_graph`  - to analyze the edges and vertices of the edge table.
  -  :ref:`pgr_analyze_oneway` - to analyze directionality of the edges.
  -  :ref:`pgr_node_network`  -to create nodes to a not noded edge table.

.. toctree::
   :hidden:

   ../src/topology/doc/topology


*******************************************************************************
Routing Functions
*******************************************************************************

..
    ROUTING FUNCTIONS SECTION
    :hidden:
    :maxdepth: 0
    
.. toctree::

    ../src/routingFunctions

.. include:: ../src/routingFunctions.rst
   :start-after: from-here
   :end-before: to-here
    

*******************************************************************************
Stable Proposed Functions
*******************************************************************************

..
    ROUTING FUNCTIONS SECTION
    :hidden:
    :maxdepth: 0
    
.. toctree::
    :hidden:

    ../src/proposedNext

.. include:: ../src/proposedNext.rst
   :start-after: from-here
   :end-before: to-here


*******************************************************************************
Available Functions but not official pgRouting functions
*******************************************************************************

- :ref:`proposed`

.. toctree::
    :hidden:

    ../src/proposed


*******************************************************************************
Discontinued & Deprecated Functions
*******************************************************************************

  - :ref:`discontinued`
  - :ref:`deprecated`

.. toctree::
   :hidden:

   src/developer/discontinued
   deprecated




*******************************************************************************
Change Log
*******************************************************************************

:ref:`change_log`

   - :ref:`changelog_2_2_3`
   - :ref:`changelog_2_2_2`
   - :ref:`changelog_2_2_1`
   - :ref:`changelog_2_2_0`
   - :ref:`changelog_2_1_0`
   - :ref:`changelog_2_0_1`
   - :ref:`changelog_2_0_0`
   - :ref:`changelog_1_x`

.. toctree::
    :hidden:

    src/changelog/release_notes


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

