..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


Vehicle Routing Functions - Category (Experimental)
===============================================================================

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index experimental from here

* Pickup and delivery problem

  - :doc:`pgr_pickDeliver` - Pickup & Delivery using a Cost Matrix
  - :doc:`pgr_pickDeliverEuclidean` - Pickup & Delivery with Euclidean distances

* Distribution problem

  - :doc:`pgr_vrpOneDepot` - From a single depot, distributes orders

.. index experimental to here

.. contents::

.. toctree::
  :hidden:

  pgr_pickDeliver
  pgr_pickDeliverEuclidean
  pgr_vrpOneDepot


Introduction
-------------------------------------------------------------------------------

Vehicle Routing Problems `VRP` are **NP-hard** optimization problem, it
generalises the travelling salesman problem (TSP).

- The objective of the VRP is to minimize the total route cost.
- There are several variants of the VRP problem,

**pgRouting does not try to implement all variants.**


Characteristics
...............................................................................

- Capacitated Vehicle Routing Problem `CVRP` where The vehicles have limited
  carrying capacity of the goods.
- Vehicle Routing Problem with Time Windows `VRPTW` where the locations have
  time windows within which the vehicle's visits must be made.
- Vehicle Routing Problem with Pickup and Delivery `VRPPD` where a number of
  goods need to be moved from certain pickup locations to other delivery
  locations.


.. Rubric:: Limitations

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.


Pick & Delivery
-------------------------------------------------------------------------------

Problem: `CVRPPDTW` Capacitated Pick and Delivery Vehicle Routing problem with
Time Windows

- Times are relative to `0`
- The vehicles

  - have start and ending service duration times.
  - have opening and closing times for the start and ending locations.
  - have a capacity.

- The orders

  - Have pick up and delivery locations.
  - Have opening and closing times for the pickup and delivery locations.
  - Have pickup and delivery duration service times.
  - have a demand request for moving goods from the pickup location to the
    delivery location.

- Time based calculations:

  - Travel time between customers is :math:`distance / speed`
  - Pickup and delivery order pair is done by the same vehicle.
  - A pickup is done before the delivery.

Parameters
-------------------------------------------------------------------------------

Pick & deliver
...............................................................................

Used in :doc:`pgr_pickDeliverEuclidean`

.. pde_parameters_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Orders SQL`_
     - ``TEXT``
     - `Orders SQL`_ as described below.
   * - `Vehicles SQL`_
     - ``TEXT``
     - `Vehicles SQL`_ as described below.

.. pde_parameters_end

Used in :doc:`pgr_pickDeliver`

.. pd_parameters_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - `Orders SQL`_
     - ``TEXT``
     - `Orders SQL`_ as described below.
   * - `Vehicles SQL`_
     - ``TEXT``
     - `Vehicles SQL`_ as described below.
   * - `Matrix SQL`_
     - ``TEXT``
     - `Matrix SQL`_ as described below.

.. pd_parameters_end

Pick-Deliver optional parameters
...............................................................................

.. pd_optionals_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Default
     - Description
   * - ``factor``
     - ``NUMERIC``
     - 1
     - Travel time multiplier. See :ref:`pd_factor`
   * - ``max_cycles``
     - ``INTEGER``
     - 10
     - Maximum number of cycles to perform on the optimization.
   * - ``initial_sol``
     - ``INTEGER``
     - 4
     - Initial solution to be used.

       - ``1`` One order per truck
       - ``2`` Push front order.
       - ``3`` Push back order.
       - ``4`` Optimize insert.
       - ``5`` Push back order that allows more orders to be inserted at the
         back
       - ``6`` Push front order that allows more orders to be inserted at the
         front

.. pd_optionals_end

Inner Queries
-------------------------------------------------------------------------------

Orders SQL
...............................................................................

Common columns for the orders SQL in both implementations:

.. pd_orders_sql_general_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - |ANY-INTEGER|
     - Identifier of the pick-delivery order pair.
   * - ``demand``
     - |ANY-NUMERICAL|
     - Number of units in the order
   * - ``p_open``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the pickup location opens.
   * - ``p_close``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the pickup location closes.
   * - [``p_service``]
     - |ANY-NUMERICAL|
     - The duration of the loading at the pickup location.

       * When missing: 0 time units are used
   * - ``d_open``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the delivery location opens.
   * - ``d_close``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the delivery location closes.
   * - [``d_service``]
     - |ANY-NUMERICAL|
     - The duration of the unloading at the delivery location.

       * When missing: 0 time units are used

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``
:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. pd_orders_sql_general_end

For :doc:`pgr_pickDeliver` the pickup and delivery identifiers of the locations
are needed:

.. pd_orders_sql_matrix_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``p_node_id``
     - |ANY-INTEGER|
     - The node identifier of the pickup, must match a vertex identifier in the
       `Matrix SQL`_.
   * - ``d_node_id``
     - |ANY-INTEGER|
     - The node identifier of the delivery, must match a vertex identifier in
       the `Matrix SQL`_.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. pd_orders_sql_matrix_end

For :doc:`pgr_pickDeliverEuclidean` the :math:`(x, y)` values of the locations
are needed:

.. pd_orders_euclidean_sql_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``p_x``
     - |ANY-NUMERICAL|
     - :math:`x` value of the pick up location
   * - ``p_y``
     - |ANY-NUMERICAL|
     - :math:`y` value of the pick up location
   * - ``d_x``
     - |ANY-NUMERICAL|
     - :math:`x` value of the delivery location
   * - ``d_y``
     - |ANY-NUMERICAL|
     - :math:`y` value of the delivery location

Where:

:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. pd_orders_euclidean_sql_end


..
    see include/c_types/pickDeliver/vehicle_t.h documentation

Vehicles SQL
...............................................................................

Common columns for the vehicles SQL in both implementations:

.. pd_vehicle_sql_general_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``id``
     - |ANY-NUMERICAL|
     - Identifier of the vehicle.
   * - ``capacity``
     - |ANY-NUMERICAL|
     - Maiximum capacity units
   * - ``start_open``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the starting location opens.
   * - ``start_close``
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the starting location closes.
   * - [``start_service``]
     - |ANY-NUMERICAL|
     - The duration of the loading at the starting location.

       * When missing: A duration of :math:`0` time units is used.
   * - [``end_open``]
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the ending location opens.

       * When missing: The value of ``start_open`` is used
   * - [``end_close``]
     - |ANY-NUMERICAL|
     - The time, relative to 0, when the ending location closes.

       * When missing: The value of ``start_close`` is used
   * - [``end_service``]
     - |ANY-NUMERICAL|
     - The duration of the loading at the ending location.

       * When missing: A duration in ``start_service`` is used.



.. pd_vehicle_sql_general_end

For :doc:`pgr_pickDeliver` the starting and ending identifiers of the locations
are needed:

.. pd_vehicle_sql_matrix_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``start_node_id``
     - |ANY-INTEGER|
     - The node identifier of the start location, must match a vertex identifier
       in the `Matrix SQL`_.
   * - [``end_node_id``]
     - |ANY-INTEGER|
     - The node identifier of the end location, must match a vertex identifier
       in the `Matrix SQL`_.

       * When missing: ``end_node_id`` is used.

Where:

:ANY-INTEGER: ``SMALLINT``, ``INTEGER``, ``BIGINT``

.. pd_vehicle_sql_matrix_end

For :doc:`pgr_pickDeliverEuclidean` the :math:`(x, y)` values of the locations
are needed:

.. pd_vehicle_sql_euclidean_start

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``start_x``
     - |ANY-NUMERICAL|
     - :math:`x` value of the starting location
   * - ``start_y``
     - |ANY-NUMERICAL|
     - :math:`y` value of the starting location
   * - [``end_x``]
     - |ANY-NUMERICAL|
     - :math:`x` value of the ending location

       * When missing: ``start_x`` is used.
   * - [``end_y``]
     - |ANY-NUMERICAL|
     - :math:`y` value of the ending location

       * When missing: ``start_y`` is used.

Where:

:ANY-NUMERICAL: ``SMALLINT``, ``INTEGER``, ``BIGINT``, ``REAL``, ``FLOAT``

.. pd_vehicle_sql_euclidean_end


Matrix SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: return_cost_start
    :end-before: return_cost_end

Return columns
-------------------------------------------------------------------------------

.. pd_returns_start

.. parsed-literal::

     RETURNS SET OF
      (seq, vehicle_seq, vehicle_id, stop_seq, stop_type,
          travel_time, arrival_time, wait_time, service_time,  departure_time)
      UNION
      (summary row)

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Sequential value starting from **1**.
   * - ``vehicle_seq``
     - ``INTEGER``
     - Sequential value starting from **1** for current vehicles. The
       :math:`n_{th}` vehicle in the solution.

       * Value :math:`-2` indicates it is the summary row.
   * - ``vehicle_id``
     - BIGINT
     - Current vehicle identifier.

       * Sumary row has the **total capacity violations**.

         * A capacity violation happens when overloading or underloading a
           vehicle.
   * - ``stop_seq``
     - INTEGER
     - Sequential value starting from **1** for the stops made by the current
       vehicle. The :math:`m_{th}` stop of the current vehicle.

       * Sumary row has the **total time windows violations**.

         * A time window violation happens when arriving after the location has
           closed.
   * - ``stop_type``
     - ``INTEGER``
     - * Kind of stop location the vehicle is at

         * :math:`-1`: at the solution summary row
         * :math:`1`: Starting location
         * :math:`2`: Pickup location
         * :math:`3`: Delivery location
         * :math:`6`: Ending location and indicates the vehicle's summary row
   * - ``order_id``
     - ``BIGINT``
     - Pickup-Delivery order pair identifier.

       * Value :math:`-1`: When no order is involved on the current stop
         location.
   * - ``cargo``
     - ``FLOAT``
     - Cargo units of the vehicle when leaving the stop.

       * Value :math:`-1` on solution summary row.
   * - ``travel_time``
     - ``FLOAT``
     - Travel time from previous ``stop_seq`` to current ``stop_seq``.

       * Summary has the **total traveling time**:

         * The sum of all the ``travel_time``.
   * - ``arrival_time``
     - ``FLOAT``
     - Time spent waiting for current location to open.

       * :math:`-1`: at the solution summary row.
       * :math:`0`: at the starting location.

   * - ``wait_time``
     - ``FLOAT``
     - Time spent waiting for current location to open.

       * Summary row has the **total waiting time**:

         * The sum of all the ``wait_time``.
   * - ``service_time``
     - ``FLOAT``
     - Service duration at current location.

       * Summary row has the **total service time**:

         * The sum of all the ``service_time``.
   * - ``departure_time``
     - ``FLOAT``
     - * The time at which the vehicle departs from the stop.

         * :math:`arrival\_time + wait\_time + service\_time`.

       * The ending location has the **total time** used by the current
         vehicle.
       * Summary row has the **total solution time**:

         * :math:`total\ traveling\ time + total\ waiting\ time + total\ service\ time`.

.. pd_returns_end

Summary Row
...............................................................................

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Column
     - Type
     - Description
   * - ``seq``
     - ``INTEGER``
     - Continues the sequence
   * - ``vehicle_seq``
     - ``INTEGER``
     - Value :math:`-2` indicates it is the summary row.
   * - ``vehicle_id``
     - BIGINT
     - **total capacity violations**:

       * A capacity violation happens when overloading or underloading a
         vehicle.
   * - ``stop_seq``
     - INTEGER
     - **total time windows violations**:

       * A time window violation happens when arriving after the location has
         closed.
   * - ``stop_type``
     - ``INTEGER``
     - :math:`-1`
   * - ``order_id``
     - ``BIGINT``
     - :math:`-1`
   * - ``cargo``
     - ``FLOAT``
     - :math:`-1`
   * - ``travel_time``
     - ``FLOAT``
     - **total traveling time**:

       * The sum of all the ``travel_time``.
   * - ``arrival_time``
     - ``FLOAT``
     - :math:`-1`
   * - ``wait_time``
     - ``FLOAT``
     - **total waiting time**:

       * The sum of all the ``wait_time``.
   * - ``service_time``
     - ``FLOAT``
     - **total service time**:

       * The sum of all the ``service_time``.
   * - ``departure_time``
     - ``FLOAT``
     - Summary row has the **total solution time**:

       * :math:`total\ traveling\ time + total\ waiting\ time + total\ service\ time`.

Handling Parameters
-------------------------------------------------------------------------------

To define a problem, several considerations have to be done, to get consistent results.
This section gives an insight of how parameters are to be considered.

- `Capacity and Demand Units Handling`_
- `Locations`_
- `Time Handling`_
- `Factor Handling`_


Capacity and Demand Units Handling
...............................................................................

The `capacity` of a vehicle, can be measured in:

- Volume units like :math:`m^3`.
- Area units like :math:`m^2` (when no stacking is allowed).
- Weight units like :math:`kg`.
- Number of boxes that fit in the vehicle.
- Number of seats in the vehicle

The `demand` request of the pickup-deliver orders must use the same units as the
units used in the vehicle's `capacity`.

To handle problems like:  10 (equal dimension) boxes of apples and 5 kg of
feathers that are to be transported (not packed in boxes).

* If the vehicle's **capacity** is measured in `boxes`, a conversion of `kg of
  feathers` to `number of boxes` is needed.
* If the vehicle's **capacity** is measured in `kg`, a conversion of `box of
  apples` to `kg` is needed.

Showing how the 2 possible conversions can be done

Let:
- :math:`f\_boxes`: number of boxes needed for `1` kg of feathers.
- :math:`a\_weight`: weight of `1` box of apples.

=============== ====================== ==================
Capacity Units  apples                  feathers
=============== ====================== ==================
boxes            10                     :math:`5 * f\_boxes`
kg              :math:`10 * a\_weight`       5
=============== ====================== ==================

Locations
...............................................................................

- When using :doc:`pgr_pickDeliverEuclidean`:

  - The vehicles have :math:`(x, y)` pairs for start and ending locations.
  - The orders Have :math:`(x, y)` pairs for pickup and delivery locations.

- When using :doc:`pgr_pickDeliver`:

  - The vehicles have identifiers for the start and ending locations.
  - The orders have identifiers for the pickup and delivery locations.
  - All the identifiers are indices to the given matrix.


Time Handling
...............................................................................

The times are relative to **0**.
All time units have to be converted to a **0** reference and the same time units.

Suppose that a vehicle's driver starts the shift at 9:00 am and ends the shift
at 4:30 pm and the service time duration is 10 minutes with 30 seconds.

============ ========== ================= ===================== =========
Meaning of 0 time units  9:00 am          4:30 pm               10 min 30 secs
============ ========== ================= ===================== =========
0:00 am      hours      9                 16.5                  :math:`10.5 / 60  = 0.175`
0:00 am      minutes    :math:`9*60 = 54` :math:`16.5*60 = 990` 10.5
9:00 am      hours      0                 7.5                   :math:`10.5 / 60  = 0.175`
9:00 am      minutes    0                 :math:`7.5*60 = 540`  10.5
============ ========== ================= ===================== =========


.. _pd_factor:

Factor handling
...............................................................................

``factor`` acts as a multiplier to convert from distance values to time units
the matrix values or the euclidean values.

* When the values are already in the desired time units

  * ``factor`` should be **1**
  * When ``factor`` > 1 the travel times are faster
  * When ``factor`` < 1 the travel times are slower

For the :doc:`pgr_pickDeliverEuclidean`:

Working with time units in seconds, and x/y in lat/lon:
Factor: would depend on the location of the points and on the average velocity
say 25m/s is the velocity.

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Latitude
     - Conversion
     - Factor
   * - 45
     - 1 longitude degree is (78846.81m)/(25m/s)
     - 3153 s
   * - 0
     - 1 longitude degree is (111319.46 m)/(25m/s)
     - 4452 s

For the :doc:`pgr_pickDeliver`:

Given :math:`v = d / t` therefore :math:`t = d / v`
And the ``factor`` becomes :math:`1 / v`

Where:

:v: Velocity
:d: Distance
:t: Time

For the following equivalences
:math:`10m/s \approx 600m/min \approx 36 km/hr`

Working with time units in seconds and the matrix been in meters:
For a 1000m lenght value on the matrix:

.. list-table::
   :width: 81
   :widths: auto
   :header-rows: 1

   * - Units
     - velocity
     - Conversion
     - Factor
     - Result
   * - seconds
     - :math:`10 m/s`
     - :math:`\frac{1}{10m/s}`
     - :math:`0.1s/m`
     - :math:`1000m * 0.1s/m = 100s`
   * - minutes
     - :math:`600 m/min`
     - :math:`\frac{1}{600m/min}`
     - :math:`0.0016min/m`
     - :math:`1000m * 0.0016min/m = 1.6min`
   * - Hours
     - :math:`36 km/hr`
     - :math:`\frac{1}{36 km/hr}`
     - :math:`0.0277hr/km`
     - :math:`1km * 0.0277hr/km = 0.0277hr`

See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Vehicle_routing_problem
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

