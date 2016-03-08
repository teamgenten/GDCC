//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Argument "moving" functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "IR/Arg.hpp"
#include "IR/Block.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      //
      // Info::moveArgStk_dst
      //
      // If idx is not Stk, makes it one by adding a new Move_W statement.
      //
      bool Info::moveArgStk_dst(IR::Arg &idx, Core::FastU sizeMove)
      {
         if(idx.a == IR::ArgBase::Stk) return false;

         block->setOrigin(stmnt->pos);
         block->addStatementArgs(stmnt->next, {IR::Code::Move_W, sizeMove},
            std::move(idx), IR::Arg_Stk());

         idx = IR::Arg_Stk();

         return true;
      }

      //
      // Info::moveArgStk_src
      //
      // If idx is not Stk, makes it one by adding a new Move_W statement.
      //
      bool Info::moveArgStk_src(IR::Arg &idx, Core::FastU sizeMove)
      {
         if(idx.a == IR::ArgBase::Stk) return false;

         block->setOrigin(stmnt->pos);
         block->addLabel(std::move(stmnt->labs));
         block->addStatementArgs(stmnt, {IR::Code::Move_W, sizeMove},
            IR::Arg_Stk(), std::move(idx));

         // Reset iterator for further translation.
         stmnt = stmnt->prev->prev;

         idx = IR::Arg_Stk();

         return true;
      }
   }
}

// EOF
