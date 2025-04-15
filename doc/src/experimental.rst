..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
    ****************************************************************************

|

Experimental Functions
===============================================================================

.. warning-begin

.. collapse:: Experimental

   .. warning:: Possible server crash

     - These functions might create a server crash

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
       - Might need a lot of feedback from the community.
       - Might depend on a proposed function of pgRouting
       - Might depend on a deprecated function of pgRouting

.. end-warning

.. rubric:: Families

:doc:`flow-family`

.. include:: flow-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`chinesePostmanProblem-family`

.. include:: chinesePostmanProblem-family.rst
   :start-after: official-start
   :end-before: official-end

:doc:`coloring-family`

.. include:: coloring-family.rst
   :start-after: official-start
   :end-before: official-end

:doc:`contraction-family`

.. include:: contraction-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`transformation-family`

.. include:: transformation-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`traversal-family`

.. include:: traversal-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`components-family`

.. include:: components-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`ordering-family`

.. include:: ordering-family.rst
   :start-after: official-start
   :end-before: official-end

:doc:`metrics-family`

.. include:: metrics-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

:doc:`TRSP-family`

.. include:: TRSP-family.rst
   :start-after: experimental-start
   :end-before: experimental-end

.. toctree::
   :hidden:

   chinesePostmanProblem-family
   transformation-family
   components-family
   ordering-family

.. rubric:: categories

:doc:`VRP-category`

.. include:: VRP-category.rst
   :start-after: experimental-start
   :end-before: experimental-end

.. toctree::
  :hidden:

  VRP-category

.. rubric:: Shortest Path Category

- :doc:`pgr_bellmanFord`
- :doc:`pgr_dagShortestPath`
- :doc:`pgr_edwardMoore`

.. toctree::
  :hidden:

  pgr_bellmanFord
  pgr_dagShortestPath
  pgr_edwardMoore

.. rubric:: Planar Family

- :doc:`pgr_isPlanar`

.. toctree::
  :hidden:

  pgr_isPlanar

.. rubric:: Miscellaneous Algorithms

- :doc:`pgr_lengauerTarjanDominatorTree`
- :doc:`pgr_stoerWagner`
- :doc:`pgr_transitiveClosure`
- :doc:`pgr_hawickCircuits`

.. toctree::
  :hidden:

  pgr_lengauerTarjanDominatorTree
  pgr_stoerWagner
  pgr_transitiveClosure
  pgr_hawickCircuits


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
