..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/

.. _stable:

Stable Proposed Functions
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

:doc:`components-family`

.. include:: components-family.rst
   :start-after: index from here
   :end-before: index to here


:doc:`withPoints-family`

.. include:: withPoints-family.rst
   :start-after: index proposed from here
   :end-before: index proposed to here

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

.. toctree::
    :hidden:

   components-family
    withPoints-family
    cost-category
    costMatrix-category
    KSP-category


:doc:`topology-functions`

.. include:: topology-functions.rst
   :start-after: topology_proposed_start
   :end-before: topology_proposed_end

.. _proposed:

Experimental Functions
===============================================================================


..  begin-warn-expr

.. warning:: Experimental functions

  - They are not officially of the current release.
  - They likely will not be officially be part of the next release:

    - The functions might not make use of ANY-INTEGER and ANY-NUMERICAL
    - Name might change.
    - Signature might change.
    - Functionality might change.
    - pgTap tests might be missing.
    - Might need c/c++ coding.
    - May lack documentation.
    - Documentation if any might need to be rewritten.
    - Documentation examples might need to be automatically generated.
    - Might need a lot of feedback from the comunity.
    - Might depend on a proposed function of pgRouting
    - Might depend on a deprecated function of pgRouting

..  end-warn-expr

.. rubric:: Families

:doc:`costFlow-family`

.. include:: costFlow-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`chinesePostmanProblem-family`

.. include:: chinesePostmanProblem-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`contraction-family` - Reduce network size using contraction techniques

.. include:: contraction-family.rst
   :start-after: index from here
   :end-before: index to here


:doc:`transformation-family`

.. include:: transformation-family.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
   :hidden:

   costFlow-family
   chinesePostmanProblem-family
   contraction-family
   transformation-family

.. rubric:: Others

.. rubric:: VRP

- :doc:`pgr_gsoc_vrppdtw`
- :doc:`pgr_vrpOneDepot`

.. toctree::
  :hidden:

  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot

.. rubric:: Shortest Path Algorithms

- :doc:`pgr_bellmanFord`
- :doc:`pgr_dagShortestPath`
- :doc:`pgr_stoerWagner`

.. toctree::
  :hidden:

  pgr_bellmanFord
  pgr_dagShortestPath
  pgr_stoerWagner



See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
