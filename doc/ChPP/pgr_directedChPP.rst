..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************


.. _pgr_directedChPP:

pgr_directedChPP - Experimental 
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_directedChPP`` — Calculates the shortest circuit path which contains every edge in a directed graph and starts and ends on the same vertex.


.. figure:: images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph/doc/push_relabel_max_flow.html

   Boost Graph Inside

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Characteristics
-----------------
The main characteristics are:

  - Process is done only on edges with **positive** costs.
  - There is no path when the graph is not connected.
  - We will return **no path found** error if there is no path.
  - Running time: :math:`O(E * (E + V * logV))`


Signature Summary
-----------------

.. code-block:: none

    pgr_directedChPP(edges_sql)
    RETURNS SET OF (seq, node, edge, cost, agg_cost)
    OR EMPTY SET


.. index::
    single: directedChPP - Experimental 


Signature
-----------------

:Example:

.. literalinclude:: doc-pgr_directedChPP.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: directedChPP - Experimental 


Description of the Signatures
--------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


.. include::  ./pgr_directedChPP_Cost.rst
    :start-after: pgr_directedChPP_parameters_start
    :end-before: pgr_directedChPP_parameters_end


.. include:: pgRouting-concepts.rst
    :start-after: result_path_start
    :end-before: result_path_end


See Also
--------

* :ref:`ChPP`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

