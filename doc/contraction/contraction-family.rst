..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. index:: Contraction Family

|

Contraction - Family of functions
===============================================================================

.. official-start

* :doc:`pgr_contraction`

.. official-end

.. include:: proposed.rst
    :start-after: warning-begin
    :end-before: end-warning

.. proposed-start

* :doc:`pgr_contractionDeadEnd`

.. proposed-end

.. toctree::
    :hidden:

    pgr_contraction
    pgr_contractionDeadEnd
    pgr_contractionLinear


Introduction
-------------------------------------------------------------------------------

In large graphs, like the road graphs, or electric networks, graph contraction
can be used to speed up some graph algorithms.
Contraction reduces the size of the graph by removing some of the vertices and
edges and, for example, might add edges that represent a sequence of original
edges decreasing the total time and space used in graph algorithms.

This implementation gives a flexible framework for adding contraction algorithms
in the future, currently, it supports two algorithms:

1. Dead end contraction
2. Linear contraction

Allowing the user to:

- Forbid contraction on a set of nodes.
- Decide the order of the contraction algorithms and set the maximum number of
  times they are to be executed.

See Also
-------------------------------------------------------------------------------

* https://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* https://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
