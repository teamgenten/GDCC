//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement.hpp"

#include "Core/Exception.hpp"

#include "SR/Statement.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // StatementCreate_Decl
      //
      SR::Statement::CRef StatementCreate_Decl(Core::Origin pos, Scope &)
      {
         return SR::StatementCreate_Empty(pos);
      }
   }
}

// EOF

