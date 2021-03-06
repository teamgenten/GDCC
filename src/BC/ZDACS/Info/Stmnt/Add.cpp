//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Addition and subtraction statement handling.
//
//-----------------------------------------------------------------------------

#include "BC/ZDACS/Info.hpp"

#include "BC/ZDACS/Code.hpp"

#include "Core/Exception.hpp"

#include "IR/Program.hpp"


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
         // Info::genStmnt_AdXU_W
         //
         void Info::genStmnt_AdXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
            Core::FastU lenAdXU_W1 =
               lenPushArg(stmnt->args[lop], 0) * 2 +
               lenPushArg(stmnt->args[rop], 0) + 16;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenAdXU_W1;

            // With carry.
            else
               numChunkCODE += lenAdXU_W1 * 2 + 56;
         }

         //
         // Info::genStmnt_AddU_W
         //
         void Info::genStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               genStmntCall(stmnt->op.size);
               return;
            }

            if(stmnt->args[0].a == IR::ArgBase::Stk)
               numChunkCODE += 4;
            else
               numChunkCODE += 8;
         }

         //
         // Info::genStmnt_SuXU_W
         //
         void Info::genStmnt_SuXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;
            Core::FastU lenSuXU_W1 =
               lenPushArg(stmnt->args[lop], 0) * 2 +
               lenPushArg(stmnt->args[rop], 0) + 20;

            // No carry.
            if(stmnt->args.size() == 3)
               numChunkCODE += lenSuXU_W1;

            // With carry.
            else
               numChunkCODE += lenSuXU_W1 * 2 + 64;
         }

         //
         // Info::preStmnt_AdXU_W
         //
         void Info::preStmnt_AdXU_W()
         {
            preStmnt_CmpU_W1(IR::Code::CmpU_LT_W, IR::Code::CmpI_LT_W, false, true);
         }

         //
         // Info::preStmnt_AddF_W
         //
         void Info::preStmnt_AddF_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_AddF_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_AddU_W
         //
         void Info::preStmnt_AddU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_AddU_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_SuXU_W
         //
         void Info::preStmnt_SuXU_W()
         {
            preStmnt_CmpU_W1(IR::Code::CmpU_GT_W, IR::Code::CmpI_GT_W, true, false);
         }

         //
         // Info::preStmnt_SubF_W
         //
         void Info::preStmnt_SubF_W()
         {
            if(stmnt->op.size == 0)
               return;

            addFunc_SubF_W(stmnt->op.size);
         }

         //
         // Info::preStmnt_SubU_W
         //
         void Info::preStmnt_SubU_W()
         {
            if(stmnt->op.size <= 1)
               return;

            addFunc_SubU_W(stmnt->op.size);
         }

         //
         // Info::putStmnt_AdXU_W
         //
         void Info::putStmnt_AdXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

            //
            // putAdXU_W1
            //
            auto putAdXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntPushArg(stmnt->args[rop], 0);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntCall("___GDCC__CmpU_LT_W1", 1);
            };

            // No carry.
            if(stmnt->args.size() == 3)
               putAdXU_W1();

            // With carry.
            else
            {
               Core::FastU lenAdXU_W1 =
                  lenPushArg(stmnt->args[lop], 0) * 2 +
                  lenPushArg(stmnt->args[rop], 0) + 16;

               Core::FastU lenCarry0 = lenAdXU_W1 +  8;
               Core::FastU lenCarry1 = lenAdXU_W1 + 40;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putAdXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putAdXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::AddU);
               putCode(Code::Copy);
               putCode(Code::LNot);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::BOrI);
            }
         }

         //
         // Info::putStmnt_AddU_W
         //
         void Info::putStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code add, Code inc)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? inc : add);
               putWord(getWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::AddU_GblReg, Code::IncU_GblReg);
               break;

            case IR::ArgBase::HubReg:
               putReg(stmnt->args[0].aHubReg, Code::AddU_HubReg, Code::IncU_HubReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::AddU_LocReg, Code::IncU_LocReg);
               break;

            case IR::ArgBase::ModReg:
               putReg(stmnt->args[0].aModReg, Code::AddU_ModReg, Code::IncU_ModReg);
               break;

            default:
               putCode(Code::AddU);
               break;
            }
         }

         //
         // Info::putStmnt_SuXU_W
         //
         void Info::putStmnt_SuXU_W()
         {
            Core::FastU lop = stmnt->args.size() == 3 ? 1 : 2, rop = lop + 1;

            //
            // putSuXU_W1
            //
            auto putSuXU_W1 = [&]()
            {
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntPushArg(stmnt->args[rop], 0);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putStmntPushArg(stmnt->args[lop], 0);
               putStmntCall("___GDCC__CmpU_GT_W1", 1);
               putCode(Code::NegI);
            };

            // No carry.
            if(stmnt->args.size() == 3)
               putSuXU_W1();

            // With carry.
            else
            {
               Core::FastU lenSuXU_W1 =
                  lenPushArg(stmnt->args[lop], 0) * 2 +
                  lenPushArg(stmnt->args[rop], 0) + 20;

               Core::FastU lenCarry0 = lenSuXU_W1 +  8;
               Core::FastU lenCarry1 = lenSuXU_W1 + 48;

               putCode(Code::Jcnd_Tru, putPos + lenCarry0 + 8);

               putSuXU_W1();
               putCode(Code::Jump_Lit, putPos + lenCarry1 + 8);

               putSuXU_W1();
               putCode(Code::Drop_LocReg, func->localReg + 0);
               putCode(Code::Push_Lit,    1);
               putCode(Code::SubU);
               putCode(Code::Copy);
               putCode(Code::BNot);
               putCode(Code::LNot);
               putCode(Code::NegI);
               putCode(Code::Push_LocReg, func->localReg + 0);
               putCode(Code::BOrI);
            }
         }

         //
         // Info::putStmnt_SubU_W
         //
         void Info::putStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               putStmntCall(stmnt->op.size);
               return;
            }

            //
            // putReg
            //
            auto putReg = [this](IR::ArgPtr1 const &a, Code sub, Code dec)
            {
               putCode(stmnt->args[2].a == IR::ArgBase::Lit ? dec : sub);
               putWord(getWord(a.idx->aLit) + a.off);
            };

            switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
               putReg(stmnt->args[0].aGblReg, Code::SubU_GblReg, Code::DecU_GblReg);
               break;

            case IR::ArgBase::HubReg:
               putReg(stmnt->args[0].aHubReg, Code::SubU_HubReg, Code::DecU_HubReg);
               break;

            case IR::ArgBase::LocReg:
               putReg(stmnt->args[0].aLocReg, Code::SubU_LocReg, Code::DecU_LocReg);
               break;

            case IR::ArgBase::ModReg:
               putReg(stmnt->args[0].aModReg, Code::SubU_ModReg, Code::DecU_ModReg);
               break;

            default:
               putCode(Code::SubU);
               break;
            }
         }

         //
         // Info::trStmnt_AdXU_W
         //
         void Info::trStmnt_AdXU_W()
         {
            CheckArgC(stmnt, 3);

            if(stmnt->op.size != 1)
               throw Core::ExceptStr(stmnt->pos, "unsupported AdXU_W size");

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[1], 1);
            }
         }

         //
         // Info::trStmnt_AddU_W
         //
         void Info::trStmnt_AddU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, false);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  getWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }
            else if(stmnt->args[0] == stmnt->args[2]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               std::swap(stmnt->args[1], stmnt->args[2]);

               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  getWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

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

            moveArgStk_dst(stmnt->args[0], 2);

            if(stmnt->args.size() > 3)
            {
               func->setLocalTmp(1);
               moveArgStk_src(stmnt->args[1], 1);
            }
         }

         //
         // Info::trStmnt_SubU_W
         //
         void Info::trStmnt_SubU_W()
         {
            if(stmnt->op.size != 1)
            {
               trStmntStk3(stmnt->op.size, stmnt->op.size, true);
               return;
            }

            if(stmnt->args[0] == stmnt->args[1]) switch(stmnt->args[0].a)
            {
            case IR::ArgBase::GblReg:
            case IR::ArgBase::HubReg:
            case IR::ArgBase::LocReg:
            case IR::ArgBase::ModReg:
               if(stmnt->args[2].a != IR::ArgBase::Lit ||
                  !stmnt->args[2].aLit.value->isValue() ||
                  getWord(stmnt->args[2].aLit) != 1)
               {
                  moveArgStk_src(stmnt->args[2], stmnt->op.size);
               }

               return;

            default:
               break;
            }

            trStmntStk3(stmnt->op.size, stmnt->op.size, true);
         }
      }
   }
}

// EOF

