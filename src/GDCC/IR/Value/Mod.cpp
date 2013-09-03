//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value operator %.
//
//-----------------------------------------------------------------------------

#include "Util.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      GDCC_IR_ValueBinOpImplEq(%, Fixed);
      GDCC_IR_ValueBinOpImplEq(%, Float);

      //
      // operator Value_Fixed %= Value_Fixed
      //
      Value_Fixed &operator %= (Value_Fixed &l, Value_Fixed const &r)
      {
         GDCC_IR_ValueBinOpBitsOp(%);

         return l.clamp();
      }

      //
      // operator Value_Float %= Value_Float
      //
      Value_Float &operator %= (Value_Float &l, Value_Float const &r)
      {
         if(l.vtype.bitsI < r.vtype.bitsI) l.vtype.bitsI = r.vtype.bitsI;
         if(l.vtype.bitsF < r.vtype.bitsF) l.vtype.bitsF = r.vtype.bitsF;
         l.value = std::fmod(l.value.get_d(), r.value.get_d()); // FIXME

         return l.clamp();
      }
   }
}

// EOF

