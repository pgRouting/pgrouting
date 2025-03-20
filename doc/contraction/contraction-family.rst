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

.. toctree::
    :hidden:

    pgr_contraction


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

Dead end contraction
-------------------------------------------------------------------------------

Contraction of the leaf nodes of the graph.

Dead end
..............................................................................

A node is considered a **dead end** node when

* On undirected graphs:

  * The number of adjacent vertices is 1.

* On directed graphs:

  * The number of adjacent vertices is 1.
  * There are no outgoing edges and has at least one incoming edge.
  * There are no incoming edges and has at least one outgoing edge.


When the conditions are true then the `Operation: Dead End Contraction`_ can be
done.

Dead end vertex on undirected graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `dead end`_ nodes
- The blue nodes have an unlimited number of edges.

.. graphviz::

    graph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        a, b [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;
           color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -- {u, v} [dir=none, weight=1, penwidth=3];
        u -- a [color=black];
        u -- a [color=darkgray];
        v -- b;
    }

.. list-table::
   :width: 80
   :widths: auto
   :header-rows: 1

   * - Node
     - Adjacent nodes
     - Number of adjacent nodes
   * - :math:`a`
     - :math:`\{u\}`
     - 1
   * - :math:`b`
     - :math:`\{v\}`
     - 1


Dead end vertex on directed graph
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- The green nodes are `dead end`_ nodes
- The blue nodes have an unlimited number of incoming and/or outgoing edges.

.. graphviz::

    digraph G {
        u, v, w, x, y [shape=circle;style=filled;width=.4;color=deepskyblue];
        a, b, c, d, e [style=filled; color=green];
        G [shape=tripleoctagon;width=1.5;style=filled;
           color=deepskyblue;label = "Rest of the Graph"];

        rankdir=LR;
        G -> {u, v, w} [dir=none, weight=1, penwidth=3];
        {x, y} -> G [dir=none, weight=1, penwidth=3];
        u -> a -> u;
        v -> b;
        {w, v} -> c;
        d -> x;
        e -> {x, y};
    }


.. list-table::
   :width: 80
   :widths: auto
   :header-rows: 1

   * - Node
     - Adjacent nodes
     - Number of adjacent nodes
     - Number of incoming edges
     - Number of outgoing edges
   * - :math:`a`
     - :math:`\{u\}`
     - 1
     -
     -
   * - :math:`b`
     - :math:`\{v\}`
     - 1
     -
     -
   * - :math:`c`
     - :math:`\{v, w\}`
     - 2
     - 2
     - 0
   * - :math:`d`
     - :math:`\{x\}`
     - 1
     -
     -
   * - :math:`e`
     - :math:`\{x, y\}`
     - 2
     - 0
     - 2


From above, nodes :math:`\{a, b, d\}` are dead ends because the
number of adjacent vertices is 1. No further checks are needed for those nodes.

On the following table, nodes :math:`\{c, e\}` because the
even that the number of adjacent vertices is not 1 for

* :math:`c`

  * There are no outgoing edges and has at least one incoming edge.

* :math:`e`

  * There are no incoming edges and has at least one outgoing edge.

Operation: Dead End Contraction
...............................................................................

The dead end contraction will stop until there are no more dead end nodes.
For example from the following graph where :math:`w` is the `dead end`_ node:

.. graphviz::

    digraph G {
        u, v [shape=circle;style=filled;width=.4;color=deepskyblue];
        w [style=filled; color=green];
        "G" [shape=tripleoctagon;style=filled;
        color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
        u -> v -> w;
    }


After contracting :math:`w`, node :math:`v` is now a `dead end`_ node and is
contracted:

.. graphviz::

    digraph G {
        u [shape=circle;style=filled;width=.4;color=deepskyblue];
        v [style=filled; color=green, label="v{w}"];
        "G" [shape=tripleoctagon;style=filled;
            color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
        u -> v;
    }

After contracting :math:`v`, stop. Node :math:`u` has the information of nodes
that were contrcted.

.. graphviz::

    digraph G {
        u [style=filled; color=green, label="u{v,w}"];
        "G" [shape=tripleoctagon;style=filled;
             color=deepskyblue; label = "Rest of the Graph"];

        rankdir=LR;
        G -> u [dir=none, weight=1, penwidth=3];
    }

Node :math:`u` has the information of nodes that were contracted.


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
