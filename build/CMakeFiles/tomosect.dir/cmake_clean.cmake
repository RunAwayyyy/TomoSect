file(REMOVE_RECURSE
  "libtomosect.a"
  "libtomosect.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/tomosect.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
