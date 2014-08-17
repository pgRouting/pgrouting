FILE(REMOVE_RECURSE
  "lib/librouting_bd.pdb"
  "lib/librouting_bd.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/routing_bd.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
