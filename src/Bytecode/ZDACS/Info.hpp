//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef Bytecode__ZDACS__Info_H__
#define Bytecode__ZDACS__Info_H__

#include "../Info.hpp"

#include "GDCC/Counter.hpp"
#include "GDCC/Number.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info
      //
      class Info : public InfoBase
      {
      public:
         Info();

         virtual void put(std::ostream &out);

      protected:
         void putByte(std::ostream &out, GDCC::FastU i);

         void putChunk(std::ostream &out);
         void putChunkSFLG(std::ostream &out);
         void putChunkSPTR(std::ostream &out);
         void putChunkSVCT(std::ostream &out);

         void putExpWord(std::ostream &out, GDCC::IR::Exp const *exp);

         void putHWord(std::ostream &out, GDCC::FastU i);

         void putStmnt(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Cspe(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Move_W(std::ostream &out, GDCC::IR::Statement const &stmnt);
         void putStmnt_Move_W__Arr_Stk(std::ostream &out, GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Move_W__Stk_Arr(std::ostream &out, GDCC::IR::ArgPtr2 const &arr, GDCC::FastU i);
         void putStmnt_Retn(std::ostream &out, GDCC::IR::Statement const &stmnt);

         void putWord(std::ostream &out, GDCC::FastU i);

         virtual void trFunc(GDCC::IR::Function &func);

         virtual void trStmnt(GDCC::IR::Statement &stmnt);
         void trStmnt_Cspe(GDCC::IR::Statement &stmnt);
         void trStmnt_Move_W(GDCC::IR::Statement &stmnt);
         void trStmnt_Move_W__Arr_Stk(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Move_W__Stk_Arr(GDCC::IR::Statement &stmnt, GDCC::IR::ArgPtr2 const &arr);
         void trStmnt_Retn(GDCC::IR::Statement &stmnt);


         static void CheckArg(GDCC::IR::Arg const &arg, GDCC::Origin pos);

         static GDCC::CounterRef<GDCC::IR::Exp> ResolveGlyph(GDCC::String glyph);
         static GDCC::FastU ResolveValue(GDCC::IR::Value const &val);


         GDCC::FastU jumpPos;
         GDCC::FastU numChunkSFLG;
         GDCC::FastU numChunkSPTR;
         GDCC::FastU numChunkSVCT;
      };
   }
}

#endif//Bytecode__MgC__Info_H__
