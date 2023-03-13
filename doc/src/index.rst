..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/index.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/index.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/index.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/index.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/index.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/index.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/index.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/index.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/index.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/index.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/index.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/index.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/index.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/index.html>`__
  `2.0 <https://docs.pgrouting.org/2.0/en/index.html>`__

*******************************************************************************
Table of Contents
*******************************************************************************

pgRouting extends the `PostGIS <https://postgis.net>`__/`PostgreSQL
<https://www.postgresql.org>`__ geospatial database to provide geospatial
routing and other network analysis functionality.

This is the manual for pgRouting |release|.

.. image:: _static/images/ccbysa.png
    :align: left
    :alt: Creative Commons Attribution-Share Alike 3.0 License

The pgRouting Manual is licensed under a `Creative Commons Attribution-Share
Alike 3.0 License <https://creativecommons.org/licenses/by-sa/3.0/>`_.
Feel free to use this material any way you like, but we ask that you attribute
credit to the pgRouting Project and wherever possible, a link back to
https://pgrouting.org.
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

.. toctree::
    :maxdepth: 2

    release_notes


.. toctree::
    :hidden:

    migration

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

