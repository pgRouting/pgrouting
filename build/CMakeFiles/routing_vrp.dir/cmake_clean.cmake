FILE(REMOVE_RECURSE
  "lib/librouting_vrp.pdb"
  "lib/librouting_vrp.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/routing_vrp.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
