FILE(REMOVE_RECURSE
  "lib/librouting.pdb"
  "lib/librouting.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/routing.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
