//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS glyph generation.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Glyph.hpp"
#include "GDCC/IR/StrEnt.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::genFunc
      //
      void Info::genFunc(GDCC::IR::Function &func)
      {
         if(func.defin) switch(func.ctype)
         {
         case GDCC::IR::CallType::LangACS:
            GDCC::IR::Glyph::GetData(func.glyph).type = GDCC::IR::Type_Funct(func.ctype);
            break;

         case GDCC::IR::CallType::ScriptI:
            GDCC::IR::Glyph::GetData(func.glyph).type = TypeWord;
            break;

         case GDCC::IR::CallType::ScriptS:
            GDCC::IR::Glyph::GetData(func.glyph).type = GDCC::IR::Type_StrEn();
            break;

         default: break;
         }
      }

      //
      // Info::genStr
      //
      void Info::genStr(GDCC::IR::StrEnt &str)
      {
         GDCC::IR::Glyph::GetData(str.glyph).type = GDCC::IR::Type_StrEn();
      }
   }
}

// EOF
