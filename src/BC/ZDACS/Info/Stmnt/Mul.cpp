//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Multiplication statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "IR/Function.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         //
         // Info::preStmnt_MuXU_W
         //
         void Info::preStmnt_MuXU_W()
         {
            if(stmnt->op.size == 0)
               return;

            if(stmnt->op.size == 1)
            {
               preStmnt_MuXU_W1();
               return;
            }

            addFunc_MuXU_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_MuXU_W1
         //
         void Info::preStmnt_MuXU_W1()
         {
            Core::String name = getCallName();
            auto newFunc = preStmntCallDef(name, 2, 2, 5, __FILE__, __LINE__);

            if(!newFunc)
               return;

            IR::Arg_LocReg lop{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg rop{IR::Arg_Lit(newFunc->block.getExp(1))};

            IR::Arg_LocReg lop_lo{IR::Arg_Lit(newFunc->block.getExp(0))};
            IR::Arg_LocReg lop_hi{IR::Arg_Lit(newFunc->block.getExp(1))};
            IR::Arg_LocReg rop_lo{IR::Arg_Lit(newFunc->block.getExp(2))};
            IR::Arg_LocReg rop_hi{IR::Arg_Lit(newFunc->block.getExp(3))};
            IR::Arg_LocReg tmp   {IR::Arg_Lit(newFunc->block.getExp(4))};

            IR::Arg_Stk stk{};

            #define AS_Stmnt newFunc->block.addStatementArgs

            // Split arguments to 16-bit pieces. Take care due to aliasing.
            AS_Stmnt({IR::Code::BAnd_W, 1}, rop_lo, rop, 0xFFFF);
            AS_Stmnt({IR::Code::ShRU_W, 1}, rop_hi, rop, 16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, lop_hi, lop, 16);
            AS_Stmnt({IR::Code::BAnd_W, 1}, lop_lo, lop, 0xFFFF);

            // Column 0.
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, lop_lo, rop_lo);
            AS_Stmnt({IR::Code::Move_W, 1}, stk, 0);

            // Column 1.
            AS_Stmnt({IR::Code::MulU_W, 1}, tmp, lop_lo, rop_hi);
            AS_Stmnt({IR::Code::ShLU_W, 1}, stk, tmp,    16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, stk, tmp,    16);

            AS_Stmnt({IR::Code::MulU_W, 1}, tmp, lop_hi, rop_lo);
            AS_Stmnt({IR::Code::ShLU_W, 1}, stk, tmp,    16);
            AS_Stmnt({IR::Code::ShRU_W, 1}, stk, tmp,    16);

            // Column 2.
            AS_Stmnt({IR::Code::MulU_W, 1}, stk, lop_hi, rop_hi);
            AS_Stmnt({IR::Code::AddU_W, 1}, stk, stk,    stk);

            AS_Stmnt({IR::Code::AddU_W, 2}, stk, stk,    stk);
            AS_Stmnt({IR::Code::AddU_W, 2}, stk, stk,    stk);

            AS_Stmnt({IR::Code::Retn,   2}, stk);

            #undef AS_Stmnt

            throw ResetFunc();
         }

         //
         // Info:preStmnt_MulF_W
         //
         void Info::preStmnt_MulF_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_MulF_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_MulK_W
         //
         void Info::preStmnt_MulK_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_MulK_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_MulU_W
         //
         void Info::preStmnt_MulU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_MulU_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_MulX_W
         //
         void Info::preStmnt_MulX_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_MulX_W(stmnt->op.size);
         }
      }
   }
}

// EOF

