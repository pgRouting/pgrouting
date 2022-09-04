..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/ordering-family.html>`__
  (`3.4 <https://docs.pgrouting.org/3.4/en/ordering-family.html>`__)

Ordering - Family of functions
===============================================================================

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


.. index from here

* :doc:`cuthillMckeeOrdering` - Return reverse Cuthill-McKee ordering of an undirected graph.

.. index to here

.. toctree::
    :hidden:

    cuthillMckeeOrdering

Parameters
...............................................................................

.. parameters_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Parameter
     - Type
     - Description
   * - `Edges SQL`_
     - ``TEXT``
     - `Edges SQL`_ as described below.

.. parameters_end

Edges SQL
...............................................................................

.. include:: pgRouting-concepts.rst
   :start-after: basic_edges_sql_start
   :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

.. result_columns_start

Returns SET OF ``(seq, node)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``seq``           ``BIGINT``  Sequence of the order starting from 1.
``node``          ``BIGINT``  New ordering in reverse order.
===============  =========== ======================================

.. result_columns_end

See Also
-------------------------------------------------------------------------------

.. include:: cuthillMckeeOrdering.rst
    :start-after: see also start
    :end-before: see also end


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`