//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/DGE/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::BC::DGE
{
   //
   // Info::preStmnt_AddU_W
   //
   void Info::preStmnt_AddU_W()
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline multiword push-args.

      addFunc_AddU_W(stmnt->op.size);
   }

   //
   // Info::preStmnt_SubU_W
   //
   void Info::preStmnt_SubU_W()
   {
      if(stmnt->op.size <= 1)
         return;

      // TODO: Inline multiword push-args.

      addFunc_SubU_W(stmnt->op.size);
   }

   //
   // Info::putStmnt_AdXU_W
   //
   void Info::putStmnt_AdXU_W()
   {
      if(stmnt->args.size() > 3)
         putCode("Ad3U");
      else
         putCode("AdXU");
   }

   //
   // Info::putStmnt_AddU_W
   //
   void Info::putStmnt_AddU_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("AddU");

      // TODO: Inline multiword push-args.

      putStmntCall(getFuncName({IR::Code::AddU_W, stmnt->op.size}), stmnt->op.size * 2);
   }

   //
   // Info::putStmnt_SuXU_W
   //
   void Info::putStmnt_SuXU_W()
   {
      if(stmnt->args.size() > 3)
         putCode("Su3U");
      else
         putCode("SuXU");
   }

   //
   // Info::putStmnt_SubU_W
   //
   void Info::putStmnt_SubU_W()
   {
      if(stmnt->op.size == 0)
         return;

      if(stmnt->op.size == 1)
         return putCode("SubU");

      // TODO: Inline multiword push-args.

      putStmntCall(getFuncName({IR::Code::SubU_W, stmnt->op.size}), stmnt->op.size * 2);
   }

   //
   // Info::trStmnt_AdXU_W
   //
   void Info::trStmnt_AdXU_W()
   {
      CheckArgC(stmnt, 3);

      if(stmnt->op.size != 1)
         throw Core::ExceptStr(stmnt->pos, "unsupported AdXU_W size");

      if(stmnt->args.size() > 3)
      {
         moveArgStk_dst(stmnt->args[0], 2);
         moveArgStk_src(stmnt->args[1], 1);
         moveArgStk_src(stmnt->args[2], 1);
         moveArgStk_src(stmnt->args[3], 1);
      }
      else
         trStmntStk3(1, 1, false);
   }

   //
   // Info::trStmnt_AddU_W
   //
   void Info::trStmnt_AddU_W()
   {
      // TODO: Inline multiword push-args.

      trStmntStk3(stmnt->op.size, stmnt->op.size, false);
   }

   //
   // Info::trStmnt_SuXU_W
   //
   void Info::trStmnt_SuXU_W()
   {
      CheckArgC(stmnt, 3);

      if(stmnt->op.size != 1)
         throw Core::ExceptStr(stmnt->pos, "unsupported SuXU_W size");

      if(stmnt->args.size() > 3)
      {
         bool stk1 = stmnt->args[1].a == IR::ArgBase::Stk;
         bool stk2 = stmnt->args[2].a == IR::ArgBase::Stk;
         bool stk3 = stmnt->args[3].a == IR::ArgBase::Stk;

         if((!stk1 && (stk2 || stk3)) || (!stk2 && stk3))
            throw Core::ExceptStr(stmnt->pos, "SuXU_W disorder");

         moveArgStk_dst(stmnt->args[0], 2);
         moveArgStk_src(stmnt->args[1], 1);
         moveArgStk_src(stmnt->args[2], 1);
         moveArgStk_src(stmnt->args[3], 1);
      }
      else
         trStmntStk3(1, 1, true);
   }

   //
   // Info::trStmnt_SubU_W
   //
   void Info::trStmnt_SubU_W()
   {
      // TODO: Inline multiword push-args.

      trStmntStk3(stmnt->op.size, stmnt->op.size, true);
   }
}

// EOF

