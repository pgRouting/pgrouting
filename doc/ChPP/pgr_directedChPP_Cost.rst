..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_directedChPP_Cost:

pgr_directedChPP_Cost - Experimental
============================================


Synopsis
-------------------------------------------------------------------------------

``pgr_directedChPP_Cost`` — Calculates the minimum costs of a circuit path which contains every edge in a directed graph and starts and ends on the same vertex.


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
  - There is no return value when the graph is not connected.
  - We will return **no path found** error if there is no path.
  - It does not returns a path.
  - Running time: :math:`O(E * (E + V * logV))`


Signature Summary
-----------------

.. code-block:: none

    pgr_directedChPP_Cost(edges_sql)
    RETURNS FLOAT


.. index::
    single: directedChPP_Cost - Experimental


Signature
-----------------

:Example:

.. literalinclude:: doc-pgr_directedChPP_Cost.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: directedChPP_Cost - Experimental


Description of the Signatures
--------------------------------------------------------


.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end


.. pgr_directedChPP_parameters_start

Description of the Parameters of the Chinese Postman Problem Signatures
...............................................................................

============== ================== ======== =================================================
Column         Type               Default     Description
============== ================== ======== =================================================
**edges_sql**  ``TEXT``                    The edges SQL query as described above.
============== ================== ======== =================================================

.. pgr_directedChPP_parameters_end



Description of the return value
.....................................................................

====================== =================================================
Type                   Description
====================== =================================================
``FLOAT``              Minimum costs of a circuit path.
====================== =================================================

See Also
--------

* :doc:`ChPP-family`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

