//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression: value primitive.
//
//-----------------------------------------------------------------------------

#include "ValueRoot.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Exp_ValueRoot::v_writeIR
      //
      OArchive &Exp_ValueRoot::v_writeIR(OArchive &out) const
      {
         return Super::v_writeIR(out) << value;
      }

      //
      // ExpCreate_ValueRoot
      //
      Exp::Ref ExpCreate_ValueRoot(Value const &value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(value, pos));
      }

      //
      // ExpCreate_ValueRoot
      //
      Exp::Ref ExpCreate_ValueRoot(Value &&value, Origin pos)
      {
         return static_cast<Exp::Ref>(new Exp_ValueRoot(std::move(value), pos));
      }
   }
}

// EOF

