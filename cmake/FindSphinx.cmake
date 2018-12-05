# - Try to find Sphinx (sphinx-build)

# Once done this will define
#
#   SPHINX_FOUND      - system has Sphinx
#   SPHINX_EXECUTABLE - full path to the Sphinx documentation generator tool
#   SPHINX_VERSION    - the version of Sphinx which was found, e.g. "1.0.7"

#=============================================================================
#
# Copyright (C) 2018 Vicky Vergara
# Modification for python3
#
# Copyright (C) 2011 Marie Rognes and Johannes Ring
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

message(STATUS "Checking for package 'Sphinx'")

# Make sure Python is available
if (NOT PYTHON_EXECUTABLE)
  find_package(PythonInterp)
endif()

# Try to find sphinx-build
find_program(SPHINX_EXECUTABLE sphinx-build
  HINTS ${SPHINX_DIR} $ENV{SPHINX_DIR}
  PATH_SUFFIXES bin
  DOC "Sphinx documentation generator tool"
)

if (SPHINX_EXECUTABLE)
  # Try to check Sphinx version by importing Sphinx
  execute_process(
    COMMAND ${SPHINX_EXECUTABLE} --version
    OUTPUT_VARIABLE SPHINX_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  message(STATUS "SPHINX_VERSION ${SPHINX_VERSION}")
  string(REGEX MATCH "[0-9]+\.[0-9]+.\[0-9]+" SPHINX_VERSION "${SPHINX_VERSION}")
  message(STATUS "SPHINX_VERSION ${SPHINX_VERSION}")
  if (Sphinx_FIND_VERSION)
    # Check if version found is >= required version
    if (NOT "${SPHINX_VERSION}" VERSION_LESS "${Sphinx_FIND_VERSION}")
      set(SPHINX_VERSION_OK TRUE)
    endif()
  else()
    # No specific version of Sphinx is requested
    set(SPHINX_VERSION_OK TRUE)
  endif()
endif()

mark_as_advanced(
  SPHINX_EXECUTABLE
  SPHINX_VERSION
  SPHINX_VERSION_OK
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sphinx DEFAULT_MSG
  SPHINX_EXECUTABLE SPHINX_VERSION_OK)
