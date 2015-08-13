//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS special-declaration parsing.
//
//-----------------------------------------------------------------------------

#include "ACC/Parse.hpp"

#include "ACC/Scope.hpp"

#include "AST/Attribute.hpp"
#include "AST/Function.hpp"
#include "AST/Statement.hpp"
#include "AST/Type.hpp"

#include "CC/Exp.hpp"
#include "CC/Scope/Global.hpp"
#include "CC/Type.hpp"

#include "Core/Exception.hpp"
#include "Core/TokenStream.hpp"

#include "IR/CallType.hpp"
#include "IR/Exp.hpp"
#include "IR/Linkage.hpp"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // ParseAddress
      //
      static void ParseAddress(Parser &ctx, CC::Scope &scope, AST::Attribute &attr)
      {
         // special-address:
         //    integer-constant :
         //    - integer-constant :
         //    { integer-constant } :
         //    { integer-constant , integer-constant } :
         if(ctx.in.peek(Core::TOK_NumInt))
         {
            attr.callt = IR::CallType::Special;
            attr.addrI = ctx.getExp_Prim(scope)->getIRExp();
         }
         else if(ctx.in.drop(Core::TOK_Sub))
         {
            attr.callt = IR::CallType::Native;
            attr.addrI = ctx.getExp_Prim(scope)->getIRExp();
         }
         else if(ctx.in.drop(Core::TOK_BraceO))
         {
            attr.callt = IR::CallType::AsmFunc;

            if(!ctx.in.peek(Core::TOK_NumInt))
               throw Core::ParseExceptExpect(ctx.in.peek(), "integer-constant", false);

            attr.addrI = ctx.getExp_Prim(scope)->getIRExp();

            if(ctx.in.drop(Core::TOK_Comma))
            {
               if(!ctx.in.peek(Core::TOK_NumInt))
                  throw Core::ParseExceptExpect(ctx.in.peek(), "integer-constant", false);

               attr.addrL = ctx.getExp_Prim(scope)->getIRExp();
            }

            if(!ctx.in.drop(Core::TOK_BraceC))
               throw Core::ParseExceptExpect(ctx.in.peek(), "}", true);
         }
         else
            throw Core::ParseExceptExpect(ctx.in.peek(), "special-address", false);

         if(!ctx.in.drop(Core::TOK_Colon))
            throw Core::ParseExceptExpect(ctx.in.peek(), ":", true);
      }

      //
      // ParseParameters
      //
      static void ParseParameters(Parser &ctx, CC::Scope &scope, AST::Attribute &attr)
      {
         // special-parameters:
         //    integer-constant
         //    integer-constant , integer-constant
         //    integer-constant , parameter-type-list
         //    parameter-type-list
         if(ctx.in.peek(Core::TOK_NumInt))
         {
            auto argMin = CC::ExpToFastU(ctx.getExp_Prim(scope));

            AST::TypeSet::CPtr types;
            if(ctx.in.drop(Core::TOK_Comma))
            {
               if(ctx.in.peek(Core::TOK_NumInt))
               {
                  auto argMax = CC::ExpToFastU(ctx.getExp_Prim(scope));

                  Core::Array<AST::Type::CRef> param{argMax, CC::TypeIntegPrS};

                  types = AST::TypeSet::Get(param.data(), param.size(), false);
               }
               else
                  types = std::get<0>(ctx.getTypeList(scope));
            }
            else
            {
               Core::Array<AST::Type::CRef> param{argMin, CC::TypeIntegPrS};

               types = AST::TypeSet::Get(param.data(), param.size(), false);
            }

            attr.type     = attr.type->getTypeFunction(types, attr.callt);
            attr.paramOpt = types->size() - argMin;
         }
         else
         {
            auto types = std::get<0>(ctx.getTypeList(scope));
            attr.type = attr.type->getTypeFunction(types, attr.callt);
         }
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      //
      // Parser::getDecl_Special
      //
      AST::Statement::CRef Parser::getDecl_Special(Scope_Global &scope)
      {
         if(!in.peek(Core::TOK_KeyWrd, Core::STR_special))
            throw Core::ParseExceptExpect(in.peek(), "special-declaration", false);

         auto pos = in.get().pos;

         // special-list:
         //    special-item
         //    special-list , special-item
         do
         {
            AST::Attribute attr;
            attr.linka = IR::Linkage::ExtACS;

            // special-item:
            //    type-name(opt) special-address identifier
            //       ( special-parameters )

            // type-name(opt)
            if(isType(scope))
            {
               attr.type = getType(scope);
            }
            else
            {
               attr.type = CC::TypeIntegPrS;
            }

            // special-address
            ParseAddress(*this, scope, attr);

            // identifier
            if(!in.peek(Core::TOK_Identi))
               throw Core::ParseExceptExpect(in.peek(), "identifier", false);

            attr.setName(in.get());

            // (
            if(!in.drop(Core::TOK_ParenO))
               throw Core::ParseExceptExpect(in.peek(), "(", true);

            ParseParameters(*this, scope, attr);

            // )
            if(!in.drop(Core::TOK_ParenC))
               throw Core::ParseExceptExpect(in.peek(), ")", true);

            // Check compatibility with existing symbol, if any.
            if(auto lookup = scope.find(attr.name))
            {
               if(lookup.res != CC::Lookup::Func)
                  throw Core::ExceptStr(attr.namePos,
                     "name redefined as different kind of symbol");

               if(lookup.resFunc->retrn != attr.type->getBaseType())
                  throw Core::ExceptStr(attr.namePos,
                     "function redeclared with different return type");
            }

            auto fn = scope.getFunction(attr);

            if(attr.addrI)
               fn->valueInt = attr.addrI;

            if(attr.addrL)
               fn->valueLit = attr.addrL;

            scope.add(attr.name, fn);
         }
         while(in.drop(Core::TOK_Comma));

         if(!in.drop(Core::TOK_Semico))
            throw Core::ParseExceptExpect(in.peek(), ";", true);

         return AST::StatementCreate_Empty(pos);
      }
   }
}

// EOF
