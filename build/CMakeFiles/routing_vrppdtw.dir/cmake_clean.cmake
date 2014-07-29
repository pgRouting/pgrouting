FILE(REMOVE_RECURSE
  "lib/librouting_vrppdtw.pdb"
  "lib/librouting_vrppdtw.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/routing_vrppdtw.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
