##-----------------------------------------------------------------------------
##
## Copyright (C) 2013-2015 David Hill
##
## See COPYING for license information.
##
##-----------------------------------------------------------------------------
##
## CMake file for gdcc-as.
##
##-----------------------------------------------------------------------------


##----------------------------------------------------------------------------|
## Targets                                                                    |
##

##
## gdcc-as-lib
##
add_library(gdcc-as-lib ${GDCC_SHARED_DECL}
   IStream.cpp
   LabelTBuf.cpp
   Macro.cpp
   Parse.cpp
   Parse/Arg.cpp
   Parse/Block.cpp
   Parse/DJump.cpp
   Parse/Declaration.cpp
   Parse/Exp.cpp
   Parse/Function.cpp
   Parse/Glyph.cpp
   Parse/Import.cpp
   Parse/Object.cpp
   Parse/Space.cpp
   Parse/StrEnt.cpp
   Parse/Type.cpp
   Parse/Value.cpp
   ParseFile.cpp

   ../../inc/AS/IStream.hpp
   ../../inc/AS/LabelTBuf.hpp
   ../../inc/AS/Macro.hpp
   ../../inc/AS/Parse.hpp
   ../../inc/AS/TStream.hpp
)

##
## gdcc-as
##
add_executable(gdcc-as
   main_as.cpp
)

target_link_libraries(gdcc-as-lib gdcc-ir-lib)
target_link_libraries(gdcc-as gdcc-as-lib gdcc-ld-lib)

GDCC_INSTALL_PART(as TRUE TRUE)

## EOF

