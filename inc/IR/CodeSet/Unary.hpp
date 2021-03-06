//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation unary code sets.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CodeSet__Unary_H__
#define GDCC__IR__CodeSet__Unary_H__

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      //
      // CodeSet_Inv
      //
      struct CodeSet_Inv
      {
         static constexpr Code
            CodeI_W = Code::BNot_W,
            CodeU_W = Code::BNot_W;
      };

      //
      // CodeSet_Neg
      //
      struct CodeSet_Neg
      {
         static constexpr Code
            CodeA_B = Code::NegI_B,
            CodeA_W = Code::NegI_W,
            CodeF_W = Code::NegF_W,
            CodeI_W = Code::NegI_W,
            CodeK_W = Code::NegI_W,
            CodeR_B = Code::NegI_B,
            CodeR_W = Code::NegI_W,
            CodeU_W = Code::NegI_W,
            CodeX_W = Code::NegI_W;
      };
   }
}

#endif//GDCC__IR__CodeSet__Unary_H__

