:file: This file is part of the pgRouting project.
:copyright: Copyright (c) 2013-2026 pgRouting developers
:license: Creative Commons Attribution-Share Alike 3.0 https://creativecommons.org/licenses/by-sa/3.0


.. index::
   single: Reference ; pgr_version
   single: version

|

``pgr_version``
===============================================================================

``pgr_version`` — Query for pgRouting version information.

.. rubric:: Availability

* Version 3.0.0

  * Breaking change on result columns
  * Support for old signature ends

* Version 2.0.0

  * Official function.


Description
-------------------------------------------------------------------------------

Returns pgRouting version information.

Signature
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_version()

   | RETURNS ``TEXT``

:Example: pgRouting Version for this documentation

.. literalinclude:: version.queries
   :start-after: -- q1
   :end-before: -- q2

Result columns
-------------------------------------------------------------------------------

=========== ===============================
 Type       Description
=========== ===============================
``TEXT``    pgRouting version
=========== ===============================

See Also
-------------------------------------------------------------------------------

* :doc:`reference`
* :doc:`pgr_full_version`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
