//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ACS parsing.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__ACC__Parse_H__
#define GDCC__ACC__Parse_H__

#include "../CC/Parse.hpp"

#include <utility>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      class TypeSet;
   }

   namespace ACC
   {
      //
      // ParserCtx
      //
      class ParserCtx : public CC::ParserCtx
      {
      public:
         template<typename Prag>
         ParserCtx(Core::TokenStream &in_, Prag &prag_, IR::Program &prog_) :
            CC::ParserCtx{in_, prag_, prog_},
            importing{false}
         {
         }

         ParserCtx(ParserCtx const &data, Core::TokenStream &in_) :
            CC::ParserCtx{data, in_},
            importing{data.importing}
         {
         }

         bool importing;
      };
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace ACC
   {
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &ctx,
         CC::Scope_Global &scope);
      Core::CounterRef<AST::Statement const> GetDecl(ParserCtx const &ctx,
         CC::Scope_Local &scope);

      Core::CounterRef<AST::Exp const> GetExp_Init(ParserCtx const &ctx, CC::Scope &scope,
         AST::Type const *type);

      Core::CounterRef<AST::Exp const> GetExp_Prim(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Post(ParserCtx const &ctx, CC::Scope &scope,
         AST::Exp const *exp);
      Core::CounterRef<AST::Exp const> GetExp_Unar(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Cast(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Mult(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Addi(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Shft(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Rela(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Equa(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BAnd(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BOrX(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_BOrI(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_LAnd(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_LOrI(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Cond(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp_Assi(ParserCtx const &ctx, CC::Scope &scope);
      Core::CounterRef<AST::Exp const> GetExp(ParserCtx const &ctx, CC::Scope &scope);

      Core::CounterRef<AST::Statement const> GetStatement(ParserCtx const &ctx,
         CC::Scope_Local &scope);

      Core::CounterRef<AST::Type const> GetType(ParserCtx const &ctx, CC::Scope &scope);

      std::pair<Core::CounterRef<AST::TypeSet const>, Core::Array<AST::Attribute>>
      GetTypeList(ParserCtx const &ctx, CC::Scope &scope);

      bool IsDecl(ParserCtx const &ctx, CC::Scope &scope);

      bool IsDeclSpec(ParserCtx const &ctx, CC::Scope &scope);

      bool IsDeclarator(ParserCtx const &ctx, CC::Scope &scope);

      bool IsExp_Cast(ParserCtx const &ctx, CC::Scope &scope);

      bool IsSpecQual(ParserCtx const &ctx, CC::Scope &scope);

      bool IsType(ParserCtx const &ctx, CC::Scope &scope);

      bool IsTypeQual(ParserCtx const &ctx, CC::Scope &scope);

      bool IsTypeSpec(ParserCtx const &ctx, CC::Scope &scope);

      void ParseDeclSpec(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr);

      void ParseDeclarator(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr);
      void ParseDeclaratorSuffix(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr);

      void ParseSpecQual(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr);

      void ParseTypeQual(ParserCtx const &ctx, CC::Scope &scope,
         AST::TypeQual &qual);

      void ParseTypeSpec(ParserCtx const &ctx, CC::Scope &scope,
         AST::Attribute &attr, CC::TypeSpec &spec);
   }
}

#endif//GDCC__ACC__Parse_H__
