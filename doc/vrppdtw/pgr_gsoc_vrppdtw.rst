..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_gsocvrppdtw:

pgr_gsoc_vrppdtw - Experimental
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_gsoc_vrppdtw`` — Returns a solution for `Pick and Delivery` with `time windows` Vehicle Routing Problem

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Signature Summary
-----------------


.. code-block:: none

    pgr_gsoc_vrppdtw(sql, vehicle_num, capacity)
    RETURNS SET OF pgr_costResult[]:

Signatures
-----------

.. index::
    single: gsoc_vrppdtw(Complete Signature) - proposed


Complete signature
...................

.. code-block:: none

    pgr_gsoc_vrppdtw(sql, vehicle_num, capacity)
    Returns set of pgr_costResult[]:


.. rubric:: Example: Show the id1

.. literalinclude:: ../doc/doc-gsoc_vrppdtw.queries
   :start-after: --q1
   :end-before: --q2


Description of the Signatures
-------------------------------

Description of the sql query
.........................................................................................

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the customer.

                                        - A value of ``0`` identifies the starting location

**x**             ``ANY-NUMERICAL``     ``X`` coordinate of the location.
**y**             ``ANY-NUMERICAL``     ``Y`` coordinate of the location.
**demand**        ``ANY-NUMERICAL``     How much is added / removed from the vehicle.

                                        - Negative value is a delivery,
                                        - Positive value is a pickup,

**openTime**      ``ANY-NUMERICAL``     The time relative to 0, when the customer opens.
**closeTime**     ``ANY-NUMERICAL``     The time relative to 0, when the customer closes.
**serviceTime**   ``ANY-NUMERICAL``     The duration of the loading / unloading.
**pIndex**        ``ANY-INTEGER``       Value used when the current customer is a Delivery to find the corresponding Pickup
**dIndex**        ``ANY-INTEGER``       Value used when the current customer is a Pickup to find the corresponding Delivery
================  ===================   =================================================

Description of the parameters of the signatures
.........................................................................................

================== ===========  =================================================
Column             Type            Description
================== ===========  =================================================
**sql**            ``TEXT``     SQL query as described above.
**vehicle_num**    ``INTEGER``  Maximum number of vehicles in the result. (currently is ignored)
**capacity**       ``INTEGER``  Capacity of the vehicle.
================== ===========  =================================================

Description of the result
.........................................................................................

RETURNS SET OF pgr_costResult[]:

================== =========== =================================================
Column             Type            Description
================== =========== =================================================
**seq**            ``INTEGER`` Sequential value starting from **1**.
**id1**            ``INTEGER`` Current vehicle identifier.
**id2**            ``INTEGER`` Customer identifier.
**cost**           ``FLOAT``   Previous ``cost`` plus `travel time` plus `wait time` plus `service time`.
                                - when ``id2 = 0`` for the second time for the same ``id1``, then has the total time for the current ``id1``
================== =========== =================================================



Examples
-------------------------------------------------------------------------------

.. rubric:: Example: Total number of rows returned

.. literalinclude:: ../doc/doc-gsoc_vrppdtw.queries
   :start-after: --q2
   :end-before: --q3

.. rubric:: Example: Results for only id1 values: 1, 5, and 9

.. literalinclude:: ../doc/doc-gsoc_vrppdtw.queries
   :start-after: --q3
   :end-before: --q4


See Also
-------------------------------------------------------------------------------

* The examples use :ref:`pickDeliverData`
* http://en.wikipedia.org/wiki/Vehicle_routing_problem

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
