FILE(REMOVE_RECURSE
  "libmyrouting_library.pdb"
  "libmyrouting_library.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/myrouting_library.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
