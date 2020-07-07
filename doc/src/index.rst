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

pgRouting extends the `PostGIS <https://postgis.net>`__/`PostgreSQL <https://www.postgresql.org>`__
geospatial database to provide geospatial routing and other network analysis functionality.

This is the manual for pgRouting |release|.

.. image:: _static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share Alike 3.0 License
<https://creativecommons.org/licenses/by-sa/3.0/>`_.
Feel free to use this material any way you like, but we ask that you attribute credit to
the pgRouting Project and wherever possible, a link back to https://pgrouting.org.
For other licenses used in pgRouting see the :ref:`license` page.

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

* :doc:`pgr_version` - Get pgRouting's version information.
* :doc:`pgr_full_version` - Get pgRouting's details of version.

.. toctree::
   :hidden:

   pgr_version
   pgr_full_version


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

* :doc:`proposed`
* :doc:`experimental`

.. toctree::
    :hidden:

    proposed
    experimental



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

