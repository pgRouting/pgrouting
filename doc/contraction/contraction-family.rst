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
* :doc:`pgr_contractionLinear`

.. proposed-end

.. include:: experimental.rst
    :start-after: warning-begin
    :end-before: end-warning

.. experimental-start

* :doc:`pgr_contractionHierarchies`

.. experimental-end

.. toctree::
    :hidden:

    pgr_contraction
    pgr_contractionDeadEnd
    pgr_contractionLinear
    pgr_contractionHierarchies


Introduction
-------------------------------------------------------------------------------

In large graphs, like road graphs or electric networks, graph contraction
can be used to speed up some graph algorithms. Contraction can reduce the size
of the graph by removing some of the vertices and edges and adding edges 
that represent a sequence of original edges (the original ones can be kept
in some methods). In this way, it decreases the total time and space used
by graph algorithms, particularly those searching for an optimal path.

This implementation gives a flexible framework for adding contraction algorithms
in the future. Currently, it supports three algorithms. 

1. Dead end contraction
2. Linear contraction
3. Contraction hierarchies

The two first ones can be combined through a iterative procedure, via the 
``pgr_contraction`` method. The third one is implemented on its own.

All functions allow the user to forbid contraction on a set of nodes.

See Also
-------------------------------------------------------------------------------

* https://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* https://ae.iti.kit.edu/download/diploma_thesis_geisberger.pdf
* https://jlazarsfeld.github.io/ch.150.project/contents/

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
