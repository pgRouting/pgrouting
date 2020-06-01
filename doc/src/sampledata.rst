..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

Sample Data
===============================================================================

The documentation provides very simple example queries based on a small sample network.
To be able to execute the sample queries, run the following SQL commands to create a table with a small network data set.


.. rubric:: Create table

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --EDGE TABLE CREATE start
   :end-before: --EDGE TABLE CREATE end


.. rubric:: Insert data

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --EDGE TABLE ADD DATA start
   :end-before: --EDGE TABLE ADD DATA end


.. rubric:: Updating geometry

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --EDGE TABLE update geometry start
   :end-before: --EDGE TABLE update geometry end


.. rubric:: Topology

- Before you test a routing function use this query to create a topology (fills the ``source`` and ``target`` columns).

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --EDGE TABLE TOPOLOGY start
   :end-before: --EDGE TABLE TOPOLOGY end

.. rubric:: Combinations of start and end vertices

- Used to test the combinations_sql signature in dijkstra-like functions.

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --COMBINATIONS CREATE start
   :end-before: --COMBINATIONS CREATE end

.. rubric:: Points of interest

- When points outside of the graph.
- Used with the :doc:`withPoints-family` functions.

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --POINTS CREATE start
   :end-before: --POINTS CREATE end

.. rubric:: Restrictions

- Used with the :doc:`pgr_trsp` functions.

.. literalinclude:: ../../tools/testers/sampledata.sql
   :start-after: --RESTRICTIONS CREATE start
   :end-before: --RESTRICTIONS CREATE end

Images
----------------

* Red arrows correspond when ``cost`` > 0 in the edge table.
* Blue arrows correspond when ``reverse_cost`` > 0 in the edge table.
* Points are outside the graph.
* Click on the graph to enlarge.

.. _fig1:

Network for queries marked as ``directed`` and ``cost`` and ``reverse_cost`` columns are used
......................................................................................................

When working with city networks, this is recommended for point of view of vehicles.


.. figure:: /images/Fig1-originalData.png

   **Graph 1: Directed, with cost and reverse cost**

.. _fig2:

Network for queries marked as ``undirected`` and ``cost`` and ``reverse_cost`` columns are used
......................................................................................................

When working with city networks, this is recommended for point of view of pedestrians.


.. figure:: /images/Fig6-undirected.png
   :scale: 50%

   **Graph 2: Undirected, with cost and reverse cost**

.. _fig3:

Network for queries marked as ``directed`` and only ``cost`` column is used
......................................................................................................



.. figure:: /images/Fig2-cost.png
   :scale: 20%

   **Graph 3: Directed, with cost**

.. _fig4:

Network for queries marked as ``undirected`` and only ``cost`` column is used
......................................................................................................


.. figure:: /images/Fig4-costUndirected.png
   :scale: 20%

   **Graph 4: Undirected, with cost**


.. _pickdeliverdata:


Pick & Deliver Data
...............................................................................

.. literalinclude:: ../../tools/testers/vrppdtw_data.sql


