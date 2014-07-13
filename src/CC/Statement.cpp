//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C statements.
//
//-----------------------------------------------------------------------------

#include "CC/Statement.hpp"

#include "AST/Statement.hpp"

#include "Core/Exception.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace CC
   {
      //
      // StatementCreate_Decl
      //
      AST::Statement::CRef StatementCreate_Decl(Core::Origin pos, Scope &)
      {
         return AST::StatementCreate_Empty(pos);
      }
   }
}

// EOF
