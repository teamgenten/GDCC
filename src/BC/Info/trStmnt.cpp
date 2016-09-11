//-----------------------------------------------------------------------------
//
// Copyright (C) 2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generic translation functions.
//
//-----------------------------------------------------------------------------

#include "BC/Info.hpp"

#include "Core/Exception.hpp"

#include "IR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace BC
   {
      //
      // Info::trStmntStk2
      //
      void Info::trStmntStk2(Core::FastU sizeDst, Core::FastU sizeSrc)
      {
         CheckArgC(stmnt, 2);

         moveArgStk_dst(stmnt->args[0], sizeDst);
         moveArgStk_src(stmnt->args[1], sizeSrc);
      }

      //
      // Info::trStmntStk3
      //
      void Info::trStmntStk3(Core::FastU sizeDst, Core::FastU sizeSrc, bool ordered)
      {
         CheckArgC(stmnt, 3);

         if(ordered && stmnt->args[1].a != IR::ArgBase::Stk &&
            stmnt->args[2].a == IR::ArgBase::Stk)
            throw Core::ExceptStr(stmnt->pos, "trStmntStk3 disorder");

         moveArgStk_dst(stmnt->args[0], sizeDst);
         moveArgStk_src(stmnt->args[1], sizeSrc);
         moveArgStk_src(stmnt->args[2], sizeSrc);
      }
   }
}

// EOF

