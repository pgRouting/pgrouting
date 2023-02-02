..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
    ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/proposed.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/proposed.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/proposed.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/proposed.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/proposed.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/proposed.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/proposed.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/proposed.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/proposed.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/proposed.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/proposed.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/proposed.html>`__

Proposed Functions
==================================

..  stable-begin-warning

.. warning:: Proposed functions for next mayor release.

  - They are not officially in the current release.
  - They will likely officially be part of the next mayor release:

    - The functions make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might not change. (But still can)
    - Signature might not change. (But still can)
    - Functionality might not change. (But still can)
    - pgTap tests have being done. But might need more.
    - Documentation might need refinement.

..  stable-end-warning

.. rubric:: Families

:doc:`dijkstra-family`

.. include:: dijkstra-family.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`withPoints-family`

.. include:: withPoints-family.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`TRSP-family`

.. include:: TRSP-family.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

.. toctree::
   :hidden:

   TRSP-family

:doc:`topology-functions`

.. include:: topology-functions.rst
   :start-after: topology_proposed_start
   :end-before: topology_proposed_end

:doc:`coloring-family`

.. include:: coloring-family.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`traversal-family`

.. include:: traversal-family.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
   :hidden:

   traversal-family
   coloring-family

.. rubric:: categories

:doc:`cost-category`

.. include:: cost-category.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`costMatrix-category`

.. include:: costMatrix-category.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`drivingDistance-category`

.. include:: drivingDistance-category.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`KSP-category`

.. include:: KSP-category.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

:doc:`via-category`

.. include:: via-category.rst
   :start-after: proposed start
   :end-before: proposed end

:doc:`withPoints-category`

.. include:: withPoints-category.rst
   :start-after: proposed start
   :end-before: proposed end

.. toctree::
    :hidden:

    withPoints-family
    KSP-category
    via-category
    withPoints-category

.. rubric:: Utilities

:doc:`pgr_findCloseEdges`

.. toctree::
    :hidden:

    pgr_findCloseEdges

See Also
-------------------------------------------------------------------------------

* :doc:`experimental`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
