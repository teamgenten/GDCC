//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation expression statement.
//
//-----------------------------------------------------------------------------

#include "SR/Statement/Exp.hpp"

#include "SR/Exp.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Core::Array<Core::String> const &labels_,
         Core::Origin pos_, Exp const *exp_) :
         Super{labels_, pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Core::Array<Core::String> &&labels_,
         Core::Origin pos_, Exp const *exp_) :
         Super{std::move(labels_), pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp constructor
      //
      Statement_Exp::Statement_Exp(Core::Origin pos_, Exp const *exp_) :
         Super{pos_}, exp{exp_}
      {
      }

      //
      // Statement_Exp destructor
      //
      Statement_Exp::~Statement_Exp()
      {
      }

      //
      // Statement_Exp::v_genStmnt
      //
      void Statement_Exp::v_genStmnt(GenStmntCtx const &ctx) const
      {
         exp->genStmnt(ctx);
      }

      //
      // Statement_Exp::v_isEffect
      //
      bool Statement_Exp::v_isEffect() const
      {
         return exp->isEffect();
      }

      //
      // Statement_Exp::v_isNoAuto
      //
      bool Statement_Exp::v_isNoAuto() const
      {
         return exp->isNoAuto();
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(
         Core::Array<Core::String> const &labels, Core::Origin pos,
         Exp const *exp)
      {
         return static_cast<Statement::CRef>(
            new Statement_Exp(labels, pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Core::Array<Core::String> &&labels,
         Core::Origin pos, Exp const *exp)
      {
         return static_cast<Statement::CRef>(
            new Statement_Exp(std::move(labels), pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Core::Origin pos, Exp const *exp)
      {
         return static_cast<Statement::CRef>(new Statement_Exp(pos, exp));
      }

      //
      // StatementCreate_Exp
      //
      Statement::CRef StatementCreate_Exp(Exp const *exp)
      {
         return static_cast<Statement::CRef>(new Statement_Exp(exp->pos, exp));
      }
   }
}

// EOF

