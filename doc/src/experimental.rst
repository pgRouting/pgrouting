..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
    ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/experimental.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/experimental.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/experimental.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/experimental.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/experimental.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/experimental.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/experimental.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/experimental.html>`__
* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/proposed.html#experimental-functions>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/proposed.html#experimental-functions>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/proposed.html#experimental-and-proposed-functions>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/proposed.html#experimental-and-proposed-functions>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/proposed.html#experimental-and-proposed-functions>`__

Experimental Functions
===============================================================================

..  begin-warn-expr

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
    - Might need a lot of feedback from the comunity.
    - Might depend on a proposed function of pgRouting
    - Might depend on a deprecated function of pgRouting

..  end-warn-expr

.. rubric:: Families

:doc:`flow-family`

.. include:: flow-family.rst
   :start-after: index experimental from here
   :end-before: index experimental to here

:doc:`chinesePostmanProblem-family`

.. include:: chinesePostmanProblem-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`coloring-family`

.. include:: coloring-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`transformation-family`

.. include:: transformation-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`traversal-family`

.. include:: traversal-family.rst
   :start-after: index experimental from here
   :end-before: index experimental to here

:doc:`components-family`

.. include:: components-family.rst
   :start-after: index experimental from here
   :end-before: index experimental to here

:doc:`ordering-family`

.. include:: ordering-family.rst
   :start-after: index from here
   :end-before: index to here

:doc:`TRSP-family`

.. include:: TRSP-family.rst
   :start-after: index experimental from here
   :end-before: index experimental to here

.. toctree::
   :hidden:

   chinesePostmanProblem-family
   transformation-family
   components-family
   ordering-family

.. rubric:: categories

:doc:`VRP-category`

.. include:: VRP-category.rst
   :start-after: index experimental from here
   :end-before: index experimental to here

.. toctree::
  :hidden:

  VRP-category

.. rubric:: Not classified

- :doc:`pgr_bellmanFord`
- :doc:`pgr_dagShortestPath`
- :doc:`pgr_edwardMoore`
- :doc:`pgr_isPlanar`
- :doc:`pgr_stoerWagner`
- :doc:`pgr_topologicalSort`
- :doc:`pgr_transitiveClosure`
- :doc:`pgr_lengauerTarjanDominatorTree`
- :doc:`pgr_hawickCircuits`

.. toctree::
  :hidden:

  pgr_bellmanFord
  pgr_binaryBreadthFirstSearch
  pgr_breadthFirstSearch
  pgr_dagShortestPath
  pgr_edwardMoore
  pgr_isPlanar
  pgr_stoerWagner
  pgr_topologicalSort
  pgr_transitiveClosure
  pgr_lengauerTarjanDominatorTree
  pgr_hawickCircuits


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
