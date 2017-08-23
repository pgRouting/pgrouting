..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _type_cost_result:

pgr_costResult[]
===============================================================================

.. index::
    single: pgr_costResult[]

Name
-------------------------------------------------------------------------------

``pgr_costResult[]`` — A set of records to describe a path result with cost attribute.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_costResult AS
    (
        seq integer,
        id1 integer,
        id2 integer,
        cost float8
    );

:seq:   sequential ID indicating the path order
:id1:   generic name, to be specified by the function, typically the node id
:id2:   generic name, to be specified by the function, typically the edge id
:cost:  cost attribute


.. _type_cost_result3:

pgr_costResult3[] - Multiple Path Results with Cost
===============================================================================

.. index::
    single: pgr_costResult3[]

Name
-------------------------------------------------------------------------------

``pgr_costResult3[]`` — A set of records to describe a path result with cost attribute.


Description
-------------------------------------------------------------------------------

.. code-block:: sql

    CREATE TYPE pgr_costResult3 AS
    (
        seq integer,
        id1 integer,
        id2 integer,
        id3 integer,
        cost float8
    );

:seq:   sequential ID indicating the path order
:id1:   generic name, to be specified by the function, typically the path id
:id2:   generic name, to be specified by the function, typically the node id
:id3:   generic name, to be specified by the function, typically the edge id
:cost:  cost attribute


.. rubric:: History

* New in version 2.0.0
* Replaces ``path_result``


See Also
-------------------------------------------------------------------------------

* :ref:`introduction`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
