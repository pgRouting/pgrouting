..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_full_version``
===============================================================================

``pgr_full_version`` — Get the details of pgRouting version information.

.. rubric:: Availability

* Version 3.0.0

  * New **official** function


Description
-------------------------------------------------------------------------------

Get complete details of pgRouting version information

.. index::
    single: full_version

Signatures
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_full_version()

   | RETURNS |result-version|


:Example: Information about when this documentation was built

.. literalinclude:: doc-full_version.queries
   :start-after: -- q1
   :end-before: -- q2

Return columns
-------------------------------------------------------------------------------

================  =========== ===============================
Column             Type       Description
================  =========== ===============================
``version``       ``TEXT``    pgRouting version
``build_type``    ``TEXT``    The Build type
``compile_date``  ``TEXT``    Compilation date
``library``       ``TEXT``    Library name and version
``system``        ``TEXT``    Operative system
``postgreSQL``    ``TEXT``    pgsql used
``compiler``      ``TEXT``    Compiler and version
``boost``         ``TEXT``    Boost version
``hash``          ``TEXT``    Git hash of pgRouting build
================  =========== ===============================

See Also
-------------------------------------------------------------------------------

* :doc:`reference`
* :doc:`pgr_version`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
