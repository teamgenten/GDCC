//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Conditional expression.
//
//-----------------------------------------------------------------------------

#include "SR/Exp/Cnd.hpp"

#include "SR/Function.hpp"
#include "SR/Type.hpp"

#include "IR/Block.hpp"
#include "IR/Exp.hpp"
#include "IR/Glyph.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_Cnd::v_genStmnt
      //
      void Exp_Cnd::v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const
      {
         IR::Glyph labelNil = {ctx.prog, ctx.fn->genLabel()};
         IR::Glyph labelEnd = {ctx.prog, ctx.fn->genLabel()};

         // Evaluate condition.
         expC->genStmntStk(ctx);
         ctx.block.addStatementArgs({IR::Code::Jcnd_Nil, 1}, IR::Arg_Stk(), labelNil);

         // Left (true) expression.
         expL->genStmnt(ctx, dst);
         ctx.block.addStatementArgs({IR::Code::Jump, 0}, labelEnd);

         // Right (false) expression.
         ctx.block.addLabel(labelNil);
         expR->genStmnt(ctx, dst);

         ctx.block.addLabel(labelEnd);
      }

      //
      // Exp_Cnd::v_getIRExp
      //
      IR::Exp::CRef Exp_Cnd::v_getIRExp() const
      {
         return IR::ExpCreate_Cnd(expC->getIRExp(), expL->getIRExp(),
            expR->getIRExp(), pos);
      }

      //
      // Exp_Cnd::v_isEffect
      //
      bool Exp_Cnd::v_isEffect() const
      {
         // TODO: For a known condition value, only check needed expressions.

         return expC->isEffect() || expL->isEffect() || expR->isEffect();
      }

      //
      // Exp_Cnd::v_isIRExp
      //
      bool Exp_Cnd::v_isIRExp() const
      {
         return expC->isIRExp() && expL->isIRExp() && expR->isIRExp();
      }

      //
      // Exp_Cnd::v_isNoAuto
      //
      bool Exp_Cnd::v_isNoAuto() const
      {
         return expC->isNoAuto() && expL->isNoAuto() && expR->isNoAuto();
      }
   }
}

// EOF

