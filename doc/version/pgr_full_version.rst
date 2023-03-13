..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_full_version.html>`__
  (`3.6 <https://docs.pgrouting.org/3.6/en/pgr_full_version.html>`__)
  `3.5 <https://docs.pgrouting.org/3.5/en/pgr_full_version.html>`__
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_full_version.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_full_version.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_full_version.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_full_version.html>`__
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_full_version.html>`__

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
