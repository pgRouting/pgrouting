.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _recipes:

Use's Recipes contributions 
===============================================================================

.. rubric:: How to contribute.

Use an issue traker (see :ref:`support`) with a title containing: *Proposing a Recipe: Myrecipename*. The body will contain: 

  - author:  Required
  - mail: if you are subscibed to the developers list this is not necessary
  - date: Date posted
  - comments and code:  using reStructuredText format

Any contact with the author will be done using the developers mailing list. The pgRouting team will evaluate the recipe and will be include it in this section when approved.

Comparing topology of a unnodded network with a noded network
-------------------------------------------------------------


:Author: pgRouting team.

This recipe uses the :ref:`sampledata` network.

.. code-block:: sql

	SELECT pgr_createTopology('edge_table', 0.001);
	SELECT pgr_analyzegraph('edge_table', 0.001);
	SELECT pgr_nodeNetwork('edge_table', 0.001);
	SELECT pgr_createTopology('edge_table_noded', 0.001);
	SELECT pgr_analyzegraph('edge_table_noded', 0.001); 

*No more contributions*
