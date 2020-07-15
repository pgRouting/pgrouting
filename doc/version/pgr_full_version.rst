..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_full_version
===============================================================================

``pgr_full_version`` — Get the details of pgRouting version information.

.. rubric:: Availability

* Version 3.0.0

  * New **official** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_full_version.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_full_version.html>`__


Description
-------------------------------------------------------------------------------

Get the details of pgRouting version information

.. index::
    single: full_version

Signatures
-------------------------------------------------------------------------------

.. code-block:: sql

	pgr_full_version()
   RETURNS RECORD OF (version, build_type, compile_date, library, system, PostgreSQL, compiler, boost, hash)


:Example: Information when this documentation was build

.. literalinclude:: doc-full_version.queries
   :start-after: -- q1
   :end-before: -- q2

Result Columns
-------------------------------------------------------------------------------

================  =========== ===============================
Column             Type       Description
================  =========== ===============================
**version**       ``TEXT``    pgRouting version
**build_type**    ``TEXT``    The Build type
**compile_date**  ``TEXT``    Compilation date
**library**       ``TEXT``    Library name and version
**system**        ``TEXT``    Operative system
**postgreSQL**    ``TEXT``    pgsql used
**compiler**      ``TEXT``    Compiler and version
**boost**         ``TEXT``    Boost version
**hash**          ``TEXT``    Git hash of pgRouting build
================  =========== ===============================


See Also
-------------------------------------------------------------------------------

* :doc:`pgr_version`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
