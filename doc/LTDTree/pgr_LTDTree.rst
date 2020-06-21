..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_LTDTree:

pgr_LTDTree
===============================================================================

``pgr_LTDTree`` — Returns the immediate dominator of all vertices.
In particular, the Lengauer Tarjan Dominator Tree algorithm implemented by Boost.Graph.

.. figure:: images/boost-inside.jpeg
   :target: https://www.boost.org/doc/libs/1_50_0/libs/graph/doc/lengauer_tarjan_dominator.htm
   Boost Graph Inside
.. rubric:: Availability


Description
-------------------------------------------------------------------------------
Lengauer Tarjan Dominator Tree algorithm is used to make dominator tree of any graph. It calculates the immidiate dominator of each vertex called idom, once idom of each vertex is calculated then by making every idom of each vertex as its parent, the dominator tree can be built.

Signatures
-------------------------------------------------------------------------------

.. rubric:: Summary

.. code-block:: sql

    pgr_LTDTree(edges_sql,root )
    RETURNS SET OF (id,vid,idom)

.. rubric:: Using defaults

.. code-block:: sql

:Example: **TBD**

.. index::
    single: LTDTree

Complete Signature
...............................................................................

.. code-block:: sql

:Example: **TBD**

Parameters
-------------------------------------------------------------------------------

============== ============ =================================================
Column         Type           Description
============== ============ =================================================
**edges_sql**   ``TEXT``    SQL query as described above.
**root_vid**   ``BIGINT``  Identifier of the starting vertex.
============== ============ =================================================


Inner query
-------------------------------------------------------------------------------

.. include:: pgRouting-concepts.rst
    :start-after: basic_edges_sql_start
    :end-before: basic_edges_sql_end

Result Columns
-------------------------------------------------------------------------------

Returns set of ``(seq, vid,idom)``

============== =========== =================================================
Column         Type        Description
============== =========== =================================================
**seq**        ``INTEGER`` Sequential value starting from **1**.
**vid**        ``BIGINT``  Identifier of vertex .
**idom**       ``BIGINT``  Immediate dominator of vertex.
============== =========== =================================================

Additional Examples
------------------------------------------------------------------------------------------

:Example: To handle the one flag to choose signatures

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-LTDTree.queries
    :start-after: --q1
    :end-before: --q2

The examples in this section use the following :ref:`fig1`

.. literalinclude:: doc-LTDTree.queries
    :start-after: --q2
    :end-before: --q3
See Also
-------------------------------------------------------------------------------

* https://www.boost.org/doc/libs/1_50_0/libs/graph/doc/lengauer_tarjan_dominator.htm
* :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
