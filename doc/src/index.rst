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

:doc:`pgr_version` - to get pgRouting's version information.

.. toctree::
   :hidden:

   pgr_version



:doc:`topology-functions`
===============================================================================

-  :doc:`pgr_createTopology` -  to create a topology based on the geometry.
-  :doc:`pgr_createVerticesTable` - to reconstruct the vertices table based on the source and target information.
-  :doc:`pgr_analyzeGraph`  - to analyze the edges and vertices of the edge table.
-  :doc:`pgr_analyzeOneWay` - to analyze directionality of the edges.
-  :doc:`pgr_nodeNetwork`  -to create nodes to a not noded edge table.

.. toctree::
   :hidden:

   topology-functions


:doc:`routingFunctions`
===============================================================================

.. toctree::
    :hidden:

    routingFunctions


.. include:: routingFunctions.rst
   :start-after: from-here
   :end-before: to-here



Available Functions but not official pgRouting functions
===============================================================================

- :ref:`stable`
- :ref:`proposed`

.. toctree::
    :hidden:

    proposed



:doc:`release_notes`
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

