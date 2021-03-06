//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation statement base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Statement_H__
#define GDCC__SR__Statement_H__

#include "../SR/GenStmnt.hpp"

#include "../Core/Array.hpp"
#include "../Core/Counter.hpp"
#include "../Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      class Exp;
      class Function;

      //
      // Statement
      //
      class Statement : public Core::Counter
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::SR::Statement, GDCC::Core::Counter);

      public:
         using Labels = Core::Array<Core::String>;
         using Stmnts = Core::Array<CRef>;


         void genLabel(IR::Block &block) const;
         void genStmnt(GenStmntCtx const &ctx) const;

         // Does this statement branch to another statement?
         bool isBranch() const {return v_isBranch();}

         // Does this statement have side effects?
         bool isEffect() const {return v_isEffect();}

         // Does this statement have or contain labels?
         bool isLabel() const {return !labels.empty() || v_isLabel();}

         // Does this statement never use automatic storage?
         bool isNoAuto() const {return v_isNoAuto();}

         // Does this statement never return?
         bool isNoReturn() const {return v_isNoReturn();}

         // Does this statement necessarily return?
         bool isReturn() const {return v_isReturn();}

         // Can this statement forego codegen?
         bool isTrivial() const {return !isBranch() && !isEffect();}

         Labels       const labels;
         Core::Origin const pos;

      protected:
         Statement(Labels const &labels_, Core::Origin pos_) :
            labels{labels_}, pos{pos_} {}
         Statement(Labels &&labels_, Core::Origin pos_) :
            labels{std::move(labels_)}, pos{pos_} {}
         explicit Statement(Core::Origin pos_) : labels{}, pos{pos_} {}

         virtual void v_genLabel(IR::Block &block) const;
         virtual void v_genStmnt(GenStmntCtx const &ctx) const = 0;

         virtual bool v_isBranch() const {return false;}
         virtual bool v_isEffect() const = 0;
         virtual bool v_isLabel() const {return false;}
         virtual bool v_isNoAuto() const = 0;
         virtual bool v_isNoReturn() const {return true;}
         virtual bool v_isReturn() const {return false;}
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace SR
   {
      Statement::CRef StatementCreate_Empty(Statement::Labels const &labels,
         Core::Origin pos);
      Statement::CRef StatementCreate_Empty(Statement::Labels      &&labels,
         Core::Origin pos);
      Statement::CRef StatementCreate_Empty(Core::Origin pos);

      Statement::CRef StatementCreate_Exp(Statement::Labels const &labels,
         Core::Origin pos, Exp const *exp);
      Statement::CRef StatementCreate_Exp(Statement::Labels      &&labels,
         Core::Origin pos, Exp const *exp);
      Statement::CRef StatementCreate_Exp(Core::Origin pos, Exp const *exp);
      Statement::CRef StatementCreate_Exp(Exp const *exp);

      Statement::CRef StatementCreate_Multi(Statement::Labels const &labels,
         Core::Origin pos, Statement::Stmnts const &stmnts);
      Statement::CRef StatementCreate_Multi(Statement::Labels const &labels,
         Core::Origin pos, Statement::Stmnts      &&stmnts);
      Statement::CRef StatementCreate_Multi(Statement::Labels      &&labels,
         Core::Origin pos, Statement::Stmnts const &stmnts);
      Statement::CRef StatementCreate_Multi(Statement::Labels      &&labels,
         Core::Origin pos, Statement::Stmnts      &&stmnts);
      Statement::CRef StatementCreate_Multi(
         Core::Origin pos, Statement::Stmnts const &stmnts);
      Statement::CRef StatementCreate_Multi(
         Core::Origin pos, Statement::Stmnts      &&stmnts);
   }
}

#endif//GDCC__SR__Statement_H__

