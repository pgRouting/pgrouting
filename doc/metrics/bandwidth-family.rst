..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/bandwidth-family.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/bandwidth-family.html>`__)


Bandwidth - Family of functions
===============================================================================

.. index proposed to here

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_ithBandwidth` - Algorithm to calculate ith-bandwidth of each vertex
                            in the graph.

* :doc:`pgr_bandwidth`    - Algorithm to calculate bandwidth of the graph.

.. index to here


.. toctree::
    :hidden:

    pgr_ithBandwidth
    pgr_bandwidth


Result Columns
-------------------------------------------------------------------------------

.. result columns start ithBandwidth

Returns SET OF ``(vertex_id, ith_bandwidth)``

===============  =========== ======================================
  Column            Type      Description
===============  =========== ======================================
``vertex_id``     ``BIGINT``  Identifier of the vertex.
``ith_bandwidth`` ``BIGINT``  Value of the ith-bandwidth of the vertex.

===============  =========== ======================================

.. result columns end ithBandwidth

.. result columns start bandwidth

Return SET OF ``(bandwidth)``

===============  =========== ======================================
  Column            Type      Description
===============  =========== ======================================
``bandwidth``     ``BIGINT``  Value of the bandwidth of the graph.

===============  =========== ======================================

.. result columns end bandwidth

See Also
-------------------------------------------------------------------------------

.. include:: pgr_ithBandwidth.rst
    :start-after: see also start
    :end-before: see also end

.. include:: pgr_bandwidth.rst
    :start-after: see also start
    :end-before: see also end
    

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
