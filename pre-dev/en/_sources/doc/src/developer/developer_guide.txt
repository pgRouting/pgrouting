.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _developer:

Developer's Guide
===============================================================================

This contains some basic comments about developing.
More detailed information can be found on:

http://docs.pgrouting.org/doxy/2.2/index.html

Source Tree Layout
-------------------------------------------------------------------------------

*cmake/*
    cmake scripts used as part of our build system.

*src/*
    This is the algorithm source tree. Each algorithm is to be contained
    in its on sub-tree with /doc, /sql, /src, and /test sub-directories.

For example:

  - ``src/dijkstra``  Main direcotry for dijkstra algorithm.
  - ``src/dijkstra/doc``  Dijkstra's documentation directory.
  - ``src/dijkstra/src``  Dijkstra's C and/or C++ code.
  - ``src/dijkstra/sql``  Dijkstra's sql code.
  - ``src/dijkstra/test``  Dijkstra's tests.
  - ``src/dijkstra/test/pgtap``  Dijkstra's pgTaptests.


Tools
-------------------------------------------------------------------------------

*tools/*
    Miscellaneous scripts and tools.

pre-commit
**********

To keep version/branch/commit up to date install pelase do the following:

.. code-block:: bash

  cp tools/pre-commit .git/hooks/pre-commit

After each commit a the file **VERSION** will remain. (The hash number will be one behind)



doxygen
*******

To use doxygen:


.. code-block:: bash

  cd tools/doxygen/
  make

The code's documentation can be found in:


.. code-block:: bash

  build/doxy/html/index.html


cpplint
*******

We try to follow the following guidelines for C++ coding:

https://google-styleguide.googlecode.com/svn/trunk/cppguide.html

Sample use:

.. code-block:: bash

  python cpplint.py ../src/dijkstra/src/dijkstra_driver.h
  ../src/dijkstra/src/dijkstra_driver.h:34:  Lines should be <= 80 characters long  [whitespace/line_length] [2]
  ../src/dijkstra/src/dijkstra_driver.h:40:  Line ends in whitespace.  Consider deleting these extra spaces.  [whitespace/end_of_line] [4]
  Done processing ../src/dijkstra/src/dijkstra_driver.h
  Total errors found: 2


- Maybe line 34 is a very complicated calculation so you can just ignore the message
- Delete whitespace at end of line is easy fix.
- Use your judgement!!!

Some files like ``postgres.h`` are system dependant so don't include the directory.


Other tools
***********

Tools like:
 - doit
 - winnie
 - publish_doc.sh

are very specific for the deployment of new versions, so please ask first!

Documentation Layout
-------------------------------------------------------------------------------

.. note:: 
	All documentation should be in reStructuredText format.
	See: <http://docutils.sf.net/rst.html> for introductory docs.


Documentation is distributed into the source tree. This top level "doc"
directory is intended for high level documentation cover subjects like:

    * Compiling and testing
    * Installation
    * Tutorials
    * Users' Guide front materials
    * Reference Manual front materials
    * etc

Since the algorithm specific documentation is contained in the source
tree with the algorithm specific files, the process of building the 
documentation and publishing it will need to assemble the details with
the front material as needed.

Also, to keep the "doc" directory from getting cluttered, each major book
like those listed above, should be contained in a separate directory under
"doc". Any images or other materials related to the book should also be kept
in that directory.


Testing Infrastructure
************************************

Tests are part of the tree layout:

  - ``src/dijkstra/test``  Dijkstra's tests.

    - ``test.conf``  Configuraton file.
    - ``<name>.test.sql``  Test file
    - ``<name>.result``  Results file bash

  - ``src/dijkstra/test/pgtap``  Dijkstra's pgTaptests.

    - ``<name>.sql`` pgTap test file


.. rubric:: Testing

Testing is executed from the top level of the tree layout:

.. code-block:: bash

    tools/testers/algorithm-tester.pl
    createdb  -U <user> ___pgr___test___
    sh ./tools/testers/pg_prove_tests.sh <user>
    dropdb  -U <user> ___pgr___test___


.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

