FILE(REMOVE_RECURSE
  "lib/librouting_cvrptw.pdb"
  "lib/librouting_cvrptw.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/routing_cvrptw.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
