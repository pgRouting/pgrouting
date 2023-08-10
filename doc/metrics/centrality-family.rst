..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/centrality-family.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/centrality-family.html>`__)


centrality - Family of functions
===============================================================================

.. index proposed to here

.. rubric:: Experimental

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here

* :doc:`pgr_edgeBetweennessCentrality`   - Algorithm to calculate edge betweenness 
                                           centrality of each edge in the graph.

* :doc:`pgr_vertexBetweennessCentrality` - Algorithm to calculate vertex betweenness 
                                           centrality of each vertex in the graph.

* :doc:`pgr_centralPointDominance`       - Algorithm to calculate central point dominance of 
                                           the graph. 

.. index to here


.. toctree::
    :hidden:

    pgr_edgeBetweennessCentrality
    pgr_vertexBetweennessCentrality
    pgr_centralPointDominance


Result Columns
-------------------------------------------------------------------------------

.. result columns start edgeBetweennessCentrality

Returns SET OF ``(edge_id, source, target, cost, absolute_bc, relative_bc)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``edge_id``     ``BIGINT``  Identifier of the vertex.
``source``      ``BIGINT``  Identifier of the initial vertex of the
                            edge.
``target``      ``BIGINT``  Identifier of the final vertex of the
                            edge.
``cost``        ``DOUBLE``  Weight of the edge.
``absolute_bc`` ``DOUBLE``  Value of the absolute betweenness 
                            centrality of the edge.
``relative_bc`` ``DOUBLE``  Value of the relative betweenness
                            centrality of the edge.

===============  =========== ======================================

.. result columns end edgeBetweennessCentrality

.. result columns start vertexBetweennessCentrality

Return SET OF ``(vertex_id, absolute_bc, relative_bc)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``vertex_id``    ``BIGINT``  Identifier of the vertex.
``absolute_bc``  ``DOUBLE``  Value of the absolute betweenness
                             centrality of the vertex.
``relative_bc``  ``DOUBLE``  Value of the relative betweenness
                             centrality of the vertex.

===============  =========== ======================================

.. result columns end vertexBetweennessCentrality

.. result columns start centralPointDominance

Return SET OF ``(central_pd)``

===============  =========== ======================================
Column           Type        Description
===============  =========== ======================================
``central_pd``   ``DOUBLE``  Value of the central point dominance
                             of the graph.

===============  =========== ======================================

.. result columns end centralPointDominance

See Also
-------------------------------------------------------------------------------

.. include:: pgr_edgeBetweennessCentrality.rst
    :start-after: see also start
    :end-before: see also end

.. include:: pgr_vertexBetweennessCentrality.rst
    :start-after: see also start
    :end-before: see also end

.. include:: pgr_centralPointDominance.rst
    :start-after: see also start
    :end-before: see also end

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
