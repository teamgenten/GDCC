//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C "operator %" expressions.
//
//-----------------------------------------------------------------------------

#include "CC/Exp.hpp"

#include "CC/Exp/Arith.hpp"

#include "IR/CodeSet/Arith.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // ExpCreate_Mod
      //
      SR::Exp::CRef ExpCreate_Mod(SR::Exp const *l, SR::Exp const *r,
         Core::Origin pos)
      {
         auto expL = ExpPromo_Int(ExpPromo_LValue(l, pos), pos);
         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic % arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            auto type = SR::Type::None;
            std::tie(type, expL, expR) = ExpPromo_Arith(expL, expR, pos);

            if(!type->isCTypeInteg() && !type->isCTypeFixed())
               throw Core::ExceptStr(pos, "expected integer or fixed-point");

            return ExpCreate_Arith<SR::Exp_Mod, IR::CodeSet_Mod>(type, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator %'");
      }

      //
      // ExpCreate_ModEq
      //
      SR::Exp::CRef ExpCreate_ModEq(SR::Exp const *expL, SR::Exp const *r,
         Core::Origin pos)
      {
         if(!IsModLValue(expL))
            throw Core::ExceptStr(pos, "expected modifiable lvalue");

         auto expR = ExpPromo_Int(ExpPromo_LValue(r, pos), pos);

         auto typeL = expL->getType();
         auto typeR = expR->getType();

         // arithmetic %= arithmetic
         if(typeL->isCTypeArith() && typeR->isCTypeArith())
         {
            SR::Type::CPtr evalT;
            std::tie(evalT, std::ignore, expR) = ExpPromo_Arith(expL, expR, pos);

            if(!evalT->isCTypeInteg() && !evalT->isCTypeFixed())
               throw Core::ExceptStr(pos, "expected integer or fixed-point");

            return ExpCreate_ArithEq<SR::Exp_Mod, IR::CodeSet_Mod>(
               evalT, typeL, expL, expR, pos);
         }

         throw Core::ExceptStr(pos, "invalid operands to 'operator %='");
      }
   }
}

// EOF

