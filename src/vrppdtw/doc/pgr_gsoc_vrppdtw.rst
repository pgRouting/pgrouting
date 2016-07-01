.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_gsocvrppdtw:

pgr_gsoc_vrppdtw
===============================================================================



.. index:: 
    single: pgr_gsoc_vrppdtw(compĺete signature) - proposed

Name
-------------------------------------------------------------------------------

``pgr_gsoc_vrppdtw`` — Returns optimized  solution

.. warning::  This is a proposed function.

   - Is not officially in the current release
   - The implementation currently is a wrapper of :ref:`pgr_pickDeliver`

   .. code-block:: sql

        CREATE OR REPLACE FUNCTION pgr_gsoc_vrppdtw(
            sql text,
            vehicle_num INTEGER,
            capacity INTEGER,
            OUT seq INTEGER,
            OUT rid INTEGER,
            OUT nid INTEGER,
            OUT cost INTEGER
        )
        RETURNS SETOF record AS
        $BODY$
        DECLARE
        has_reverse BOOLEAN;
        customers_sql TEXT;
        BEGIN
            RETURN query
                 SELECT a.seq, vehicle_id::INTEGER, stop_id::INTEGER AS id2, departure_time::INTEGER
                FROM _pgr_pickDeliver($1, $2, $3, 1, 30) AS a;
        END
        $BODY$
        LANGUAGE plpgsql VOLATILE
        COST 100
        ROWS 1000;

     



Examples
-------------------------------------------------------------------------------

.. literalinclude:: ../../pickDeliver/doc/gsoc_vrppdtw.queries
   :start-after: --q1
   :end-before: --q2



See Also
-------------------------------------------------------------------------------

* :ref:`pgr_pickDeliver`
* http://en.wikipedia.org/wiki/Vehicle_routing_problem
