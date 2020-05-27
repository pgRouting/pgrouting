..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/

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

:doc:`topology-functions`

.. include:: topology-functions.rst
   :start-after: topology_proposed_start
   :end-before: topology_proposed_end

:doc:`transformation-family`

.. include:: transformation-family.rst
   :start-after: index from here
   :end-before: index to here

.. toctree::
   :hidden:

   chinesePostmanProblem-family

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
- :doc:`pgr_binaryBreadthFirstSearch`
- :doc:`pgr_breadthFirstSearch`
- :doc:`pgr_dagShortestPath`
- :doc:`pgr_edwardMoore`
- :doc:`pgr_stoerWagner`
- :doc:`pgr_topologicalSort`
- :doc:`pgr_transitiveClosure`
- :doc:`pgr_turnRestrictedPath`

.. toctree::
  :hidden:

  pgr_bellmanFord
  pgr_binaryBreadthFirstSearch
  pgr_breadthFirstSearch
  pgr_dagShortestPath
  pgr_edwardMoore
  pgr_stoerWagner
  pgr_topologicalSort
  pgr_transitiveClosure
  pgr_turnRestrictedPath


See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
