//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression code functions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__ExpCode_H__
#define GDCC__SR__ExpCode_H__

#include "../SR/Type.hpp"

#include "../IR/OpCode.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // ExpCode_ArithFixed
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithFixed(Type const *t)
      {
         if(t->isCTypeAccum())
         {
            if(t->getSizeBitsS())
               return {Codes::CodeX_W, t->getSizeWords()};
            else
               return {Codes::CodeK_W, t->getSizeWords()};
         }
         else
         {
            if(t->getSizeBitsS())
               if(t->isTypeSubWord())
                  return {Codes::CodeR_B, t->getSizeBytes()};
               else
                  return {Codes::CodeR_W, t->getSizeWords()};
            else
               if(t->isTypeSubWord())
                  return {Codes::CodeA_B, t->getSizeBytes()};
               else
                  return {Codes::CodeA_W, t->getSizeWords()};
         }
      }

      //
      // ExpCode_ArithFloat
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithFloat(Type const *t)
      {
         return {Codes::CodeF_W, t->getSizeWords()};
      }

      //
      // ExpCode_ArithInteg
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithInteg(Type const *t)
      {
         if(t->getSizeBitsS())
            return {Codes::CodeI_W, t->getSizeWords()};
         else
            return {Codes::CodeU_W, t->getSizeWords()};
      }

      //
      // ExpCode_ArithPoint
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithPoint(Type const *t)
      {
         return {Codes::CodeU_W, t->getSizeWords()};
      }

      //
      // ExpCode_Arith
      //
      template<typename Codes>
      IR::OpCode ExpCode_Arith(Type const *t)
      {
         if(t->isCTypeInteg())
            return ExpCode_ArithInteg<Codes>(t);

         if(t->isCTypeFixed())
            return ExpCode_ArithFixed<Codes>(t);

         if(t->isCTypeRealFlt())
            return ExpCode_ArithFloat<Codes>(t);

         return {IR::Code::None, 0};
      }
   }
}

#endif//GDCC__SR__ExpCode_H__

