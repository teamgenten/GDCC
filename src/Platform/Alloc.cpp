//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Allocation algorithm information.
//
//-----------------------------------------------------------------------------

#include "Platform/Alloc.hpp"

#include "Platform/Platform.hpp"

#include "Core/Option.hpp"

#include "IR/Addr.hpp"
#include "IR/CallType.hpp"

#include "Option/Exception.hpp"
#include "Option/Function.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC
{
   namespace Platform
   {
      static std::unordered_map<IR::AddrSpace, Core::FastU> AllocMin;
      static std::unordered_map<IR::CallType, Core::FastU> FuncMin;

      //
      // --alloc-minimum
      //
      static Option::Function AllocMinOpt
      {
         &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
            .setName("alloc-minimum")
            .setGroup("codegen")
            .setDescS("Sets the minimum address for an address space.")
            .setDescL(
               "Sets the minimum address for an address space. Consumes three "
               "arguments, the address space base, the address space name, "
               "and the minimum value.\n"
               "\n"
               "If the long option is negated with no-, the minimum for that "
               "space is removed and the option only takes the first two "
               "arguments."),

         [](Option::Base *, Option::Args const &args) -> std::size_t
         {
            if(args.optFalse)
            {
               if(args.argC < 2)
                  Option::Exception::Error(args, "2 arguments required");
            }
            else if(args.argC < 3)
               Option::Exception::Error(args, "3 arguments required");

            IR::AddrSpace space;

            switch(Core::String::Find(args.argV[0]))
            {
               #define GDCC_IR_AddrList(name) \
                  case Core::STR_##name: space.base = IR::AddrBase::name; break;
               #include "IR/AddrList.hpp"

            default:
               Option::Exception::Error(args, "invalid address space base");
            }

            space.name = args.argV[1];

            if(args.optFalse)
            {
               AllocMin.erase(space);
               return 2;
            }

            Core::FastU min = 0;
            for(auto s = args.argV[2]; *s; ++s)
            {
               if(*s < '0' || *s > '9')
                  Option::Exception::Error(args, "invalid integer");

               min = min * 10 + (*s - '0');
            }

            AllocMin[space] = min;

            return 3;
         }
      };

      //
      // --func-minimum
      //
      static Option::Function FuncMinOpt
      {
         &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
            .setName("func-minimum")
            .setGroup("codegen")
            .setDescS("Sets the minimum address for functions.")
            .setDescL(
               "Sets the minimum address for functions. Consumes two "
               "arguments, the calling convention and the minimum value.\n"
               "\n"
               "If the long option is negated with no-, the minimum for that "
               "calling convention is removed and the option only takes the "
               "first argument."),

         [](Option::Base *, Option::Args const &args) -> std::size_t
         {
            if(args.optFalse)
            {
               if(args.argC < 1)
                  Option::Exception::Error(args, "1 argument required");
            }
            else if(args.argC < 2)
               Option::Exception::Error(args, "2 arguments required");

            IR::CallType ctype;

            switch(Core::String::Find(args.argV[0]))
            {
               #define GDCC_IR_CallTypeList(name) \
                  case Core::STR_##name: ctype = IR::CallType::name; break;
               #include "IR/CallTypeList.hpp"

            default:
               Option::Exception::Error(args, "invalid calling convention");
            }

            if(args.optFalse)
            {
               FuncMin.erase(ctype);
               return 1;
            }

            Core::FastU min = 0;
            for(auto s = args.argV[1]; *s; ++s)
            {
               if(*s < '0' || *s > '9')
                  Option::Exception::Error(args, "invalid integer");

               min = min * 10 + (*s - '0');
            }

            FuncMin[ctype] = min;

            return 2;
         }
      };
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace Platform
   {
      //
      // GetAllocMin
      //
      Core::FastU GetAllocMin(IR::AddrSpace space)
      {
         auto itr = AllocMin.find(space);

         if(itr != AllocMin.end())
            return itr->second;

         // Special rules for allocation minimum.
         if(IsFamily_ZDACS() && space.base == IR::AddrBase::Sta)
            return 2;

         return IsZeroNull_Point(space.base) ? 1 : 0;
      }

      //
      // GetAllocMin_Funct
      //
      Core::FastU GetAllocMin_Funct(IR::CallType ctype)
      {
         auto itr = FuncMin.find(ctype);

         if(itr != FuncMin.end())
            return itr->second;

         switch(ctype)
         {
         case IR::CallType::SScriptI:
         case IR::CallType::ScriptI:
            return 1;

         default:
            return 0;
         }
      }

      //
      // GetAllocMin_StrEn
      //
      Core::FastU GetAllocMin_StrEn()
      {
         return IsZeroNull_StrEn() ? 1 : 0;
      }
   }
}

// EOF

