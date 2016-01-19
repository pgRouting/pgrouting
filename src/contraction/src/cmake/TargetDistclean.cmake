# add custom target distclean
# cleans and removes cmake generated files etc.
# Jan Woetzel 04/2003
# J Kishore kumar 05/2011
#

IF (UNIX)
  ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)
  ADD_CUSTOM_COMMAND(
    COMMENT "distribution clean"
    COMMAND $(MAKE_COMMAND) clean &&
      find . \\! -path "./cmake/\\*" -path "\\*.cmake" -o -name CMakeFiles -o -name Makefile -o -name CMakeCache.txt -o -name Testing -o -name cmake_install.cmake -o -name install_manifest.txt -o -name "*.so" | xargs rm -rf
    TARGET  distclean
  )
ENDIF(UNIX)
