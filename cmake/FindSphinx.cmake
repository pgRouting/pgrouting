# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

find_program(SPHINX_EXECUTABLE NAMES sphinx-build
  HINTS
  $ENV{SPHINX_DIR}
  PATH_SUFFIXES bin\r\n  DOC "Sphinx documentation generator"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx DEFAULT_MSG\r\n  SPHINX_EXECUTABLE
)

mark_as_advanced(
  SPHINX_EXECUTABLE
)
