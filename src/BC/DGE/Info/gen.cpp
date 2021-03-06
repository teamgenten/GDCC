//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Doominati glyph generation.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "IR/Object.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace DGE
      {
         //
         // Info::genObj
         //
         void Info::genObj()
         {
            // Generic actions to take for allocated objects.
            if(!obj->alloc)
            {
               // Back address glyph.
               backGlyphObj(obj->glyph, obj->value);
            }
         }
      }
   }
}

// EOF

