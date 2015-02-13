##-----------------------------------------------------------------------------
##
## Copyright (C) 2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-acc.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-acc-lib
##
add_library(gdcc-acc-lib ${GDCC_SHARED_DECL}
   IncludeDTBuf.cpp
   IStream.cpp
   Parse.cpp
   Parse/Declaration.cpp
   Parse/Declarator.cpp
   Parse/DeclSpec.cpp
   Parse/Exp.cpp
   Parse/Exp/Cast.cpp
   Parse/Exp/Init.cpp
   Parse/Exp/Post.cpp
   Parse/Exp/Prim.cpp
   Parse/Exp/Unar.cpp
   Parse/SpecQual.cpp
   Parse/Statement.cpp
   Parse/TypeQual.cpp
   Parse/TypeSpec.cpp
   PPTokenTBuf.cpp

   ../../inc/ACC/IStream.hpp
   ../../inc/ACC/Parse.hpp
   ../../inc/ACC/TStream.hpp
)

##
## gdcc-acc
##
add_executable(gdcc-acc
   main_acc.cpp
)

target_link_libraries(gdcc-acc-lib
   gdcc-as-lib
   gdcc-ast-lib
   gdcc-cc-lib
   gdcc-cpp-lib
)

target_link_libraries(gdcc-acc gdcc-acc-lib)

GDCC_INSTALL_PART(acc TRUE TRUE)

## EOF
