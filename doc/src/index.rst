..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _index:

*******************************************************************************
Table of Contents
*******************************************************************************

pgRouting extends the `PostGIS <https://postgis.net>`_/`PostgreSQL <http://postgresql.org>`_ geospatial database to provide geospatial routing and other network analysis functionality.

This is the manual for pgRouting |release|.

.. image:: _static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share Alike 3.0 License <https://creativecommons.org/licenses/by-sa/3.0/>`_. Feel free to use this material any way you like, but we ask that you attribute credit to the pgRouting Project and wherever possible, a link back to http://pgrouting.org. For other licenses used in pgRouting see the :ref:`license` page.

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

.. include:: topology-functions.rst
   :start-after: topology_index_start
   :end-before: topology_index_end

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

