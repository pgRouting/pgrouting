..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_pickDeliverEuclidean:

pgr_pickDeliverEuclidean:
===============================================================================


Synopsis
-------------------------------------------------------------------------------

Problem: Distribute and optimize the pickup-delivery pairs into a fleet of vehicles.

- Optimization problem is NP-hard.
- pickup and Delivery with time windows.
- All vehicles are equal.

  - Same Starting location.
  - Same Ending location which is the same as Starting location.
  - All vehicles travel at the same speed.

- A customer is for doing a pickup or doing a deliver.

  - has an open time.
  - has a closing time.
  - has a service time.
  - has an (x, y) location.

- There is a customer where to deliver a pickup.

  - travel time between customers is distance / speed
  - pickup and delivery pair is done with the same vehicle.
  - A pickup is done before the delivery.



Characteristics:
----------------

- All trucks depart at time 0.
- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- the algorithm will raise an exception when

  - If there is a pickup-deliver pair than violates time window
  - The speed, max_cycles, ma_capacity have illegal values

- Six different initial will be optimized
  - the best solution found will be result


Signature Summary
-----------------

.. code-block:: none

    _pgr_pickDeliver(orders_sql, vehicles_sql, max_cycles, initial)
    RETURNS SET OF (seq, vehicle_id, vehicle_seq, stop_id,
         travel_time, arrival_time, wait_time, service_time,  departure_time)





Signatures
-------------------------------------------------------------------------------



.. index::
    single: _pgr_pickDeliveriEuclidean(One to Many)

pgr_dijkstra One to many
.......................................

.. code-block:: none

    _pgr_pickDeliver(orders_sql, vehicles_sql, max_cycles, initial)
    RETURNS SET OF (seq, vehicle_id, vehicle_seq, stop_id,
         travel_time, arrival_time, wait_time, service_time,  departure_time)

This signature finds the shortest path from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform a one to one `pgr_dijkstra`
where the starting vertex is fixed, and stop when all ``end_vids`` are reached.

  - The result is equivalent to the union of the results of the one to one `pgr_dijkstra`.
  - The extra ``end_vid`` in the result is used to distinguish to which path it belongs.

:Example:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: --q2
   :end-before: --q3






Description of the Signatures
-------------------------------------------------------------------------------

.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: pd_euclidean_sql_start
    :end-before: pd_euclidean_sql_end


.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: pd_vehicle_sql_start
    :end-before: pd_vehicle_sql_end


.. pgr_dijkstra_parameters_start

Description of the parameters of the signatures
...............................................................................

================= ==================  =================================================
Column            Type                    Description
================= ==================  =================================================
**orders_sql**     ``TEXT``            Orders SQL query as described above.
**vehicles_sql**  ``TEXT``             Vehciles SQL query as described above.
**max_cycles**    ``INTEGER``          Array of identifiers of starting vertices.
**initial**       ``INTEGER``           Identifier of the ending vertex of the path.
================= ==================  =================================================

.. pgr_dijkstra_parameters_end


.. include:: ../../../doc/src/tutorial/custom_query.rst
    :start-after: return_path_start
    :end-before: return_path_end



See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

