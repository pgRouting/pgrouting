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


Tools
-------------------------------------------------------------------------------

*tools/*
    Miscellaneous scripts and tools.

pre-commit
**********

To keep version/branch/commit up to date install pelase do the following:

.. code::

  cp tools/pre-commit .git/hooks/pre-commit

After each commit a the file **VERSION** will remain. (The hash number will be one behind)


tester
******

The tester is executed from the top level of the tree layout:

.. code::

  tools/test-runner.pl --help

doxygen
*******

.. warning:: :ref:`developer_functions` documentation is going to be deleted from the pgRouting documentation and included in the doxygen documentation.

To use doxygen:


.. code::

  cd tools/doxygen/
  make

The code's documentation can be found in:


.. code::

  build/doxy/html/index.html


cpplint
*******

We try to follow the following guidelines for C++ coding:

https://google-styleguide.googlecode.com/svn/trunk/cppguide.html

Sample use:

.. code::

 python cpplint.py ../src/dijkstra/src/dijkstra_driver.h
  ../src/dijkstra/src/dijkstra_driver.h:34:  Lines should be <= 80 characters long  [whitespace/line_length] [2]
  ../src/dijkstra/src/dijkstra_driver.h:40:  Line ends in whitespace.  Consider deleting these extra spaces.  [whitespace/end_of_line] [4]
  Done processing ../src/dijkstra/src/dijkstra_driver.h
  Total errors found: 2


- Maybe line 34 is a very complicated calculation so you can just ignore the message
- Delete whitespace at end of line is easy fix.
- Use your judgement!!!

Some files like ``postgres.h`` are system dependent so don't include the directory.


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
-------------------------------------------------------------------------------

There is a very basic testing infrastructure put in place. Here are the
basics of how it works. We need more test cases. Longer term we should
probably get someone to setup travis-ci or jenkins testing frameworks.

Here is the graph for the TRSP tests.

.. image:: images/trsp-test-image.png

Tests are run via the script at the top level tools/test-runner.pl and it runs
all the test configured tests and at the moment just dumps the results
structure of the test. This can be prettied up later.

    It also assumes that you have installed the libraries as it tests
    using the installed postgresql. This probably needs to be made
    smarter so we can test out of the build tree. I'll need to think
    about that.

Basically each .../test/ directory should include one *test.conf* file that
is a perl script fragment that defines what data files to load and what
tests to run. I have built in some mechanisms to allow test and data to
be pg version and postgis version specific, but I'm not using that yet.
So for example, *core/trsp/test/test-any-00.data* is a sql plain text dump
that will load and needed data for a set of tests. This is also the graph
in the image above. You can specify multiple files to load, but as a
group they need to have unique names. 

core/trsp/test/test-any-00.test is a sql command to be run. It will get
run as:

.. code-block:: bash

    psql ... -A -t -q -f file.test dbname > tmpfile
    diff -w file.rest tmpfile 

Then if there is a difference then an test failure is reported.





