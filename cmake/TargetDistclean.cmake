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
      rm -rf `find . -name "CMakeFiles" -o -name "Makefile" -o -name "CMakeCache.txt" -o -name "Testing" -o -name "*.cmake"` install_manifest.txt
    TARGET  distclean
  )
ENDIF(UNIX)
