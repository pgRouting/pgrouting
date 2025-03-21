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

Linear contraction
-------------------------------------------------------------------------------
In the algorithm, linear contraction is represented by 2.

Linear
................................................................................

In case of an undirected graph, a node is considered a `linear` node when

* The number of adjacent vertices is 2.

In case of a directed graph, a node is considered a `linear` node when

* The number of adjacent vertices is 2.
* Linearity is symmetrical

Linear vertex on undirected graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `linear`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.


.. rubric:: Undirected

.. graphviz::

    graph G {
        u, w [shape=circle;style=filled;width=.4;color=deepskyblue];
        v [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;
           color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        w -- G -- u [dir=none, weight=1, penwidth=3];
        u -- v -- w;
    }

.. list-table::
   :width: 80
   :widths: auto
   :header-rows: 1

   * - Node
     - Adjacent nodes
     - Number of adjacent nodes
   * - :math:`v`
     - :math:`\{u, w\}`
     - 2

Linear vertex on directed graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `linear`_ nodes
- The blue nodes have an unlimited number of incoming and outgoing edges.
- The white node is not linear because the linearity is not symmetrical.

  * It is possible to go :math:`y \rightarrow c \rightarrow z`
  * It's not possible to go :math:`z \rightarrow c \rightarrow y`

.. graphviz::

    digraph G {
        u, v, w, x, y, z [shape=circle;style=filled;width=.4;color=deepskyblue];
        a, b [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;
          color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        {u, v} -> G -> {x, w, y, z} [dir=none, weight=1, penwidth=3];
        u -> a -> v;
        w -> b -> x;
        x -> b -> w [color=darkgray];
        y -> c -> z -> c;
    }

.. list-table::
   :width: 80
   :widths: auto
   :header-rows: 1

   * - Node
     - Adjacent nodes
     - Number of adjacent nodes
     - Is symmetrical?
   * - :math:`a`
     - :math:`\{u, v\}`
     - 2
     - yes
   * - :math:`b`
     - :math:`\{w, x\}`
     - 2
     - yes
   * - :math:`c`
     - :math:`\{y, z\}`
     - 2
     - no

Operation: Linear Contraction
...............................................................................

The linear contraction will stop when there are no more linear nodes.
For example from the following graph where :math:`v` and :math:`w` are `linear`_
nodes:

.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        v, w [style=filled; color=green];
        "G" [shape=tripleoctagon; style=filled;
             color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> v -> w -> z;
    }

Contracting :math:`w`,

* The vertex :math:`w` is removed from the graph
* The edges :math:`v \rightarrow w` and :math:`w \rightarrow z` are removed
  from the graph.
* A new edge :math:`v \rightarrow z` is inserted represented with red color.

.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        v [style=filled; color=green];
        "G" [shape=tripleoctagon; style=filled;
             color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> v;
        v -> z [label="{w}";color=red]
    }

Contracting :math:`v`:

* The vertex :math:`v` is removed from the graph
* The edges :math:`u \rightarrow v` and :math:`v \rightarrow z` are removed
  from the graph.
* A new edge :math:`u \rightarrow z` is inserted represented with red color.


.. graphviz::

    digraph G {
        u, z [shape=circle;style=filled;color=deepskyblue];
        "G" [shape=tripleoctagon; style=filled;
             color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, z} [dir=none, weight=1, penwidth=3];
        u -> z [label="{v, w}";color=red]
    }

Edge :math:`u \rightarrow z` has the information of nodes that were contracted.


See Also
-------------------------------------------------------------------------------

* :doc:`pgr_contraction`
* :doc:`sampledata`
* https://www.cs.cmu.edu/afs/cs/academic/class/15210-f12/www/lectures/lecture16.pdf
* https://algo2.iti.kit.edu/documents/routeplanning/geisberger_dipl.pdf

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
