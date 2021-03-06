//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation storage arguments.
//
// This is the SR equivalent to IR::Arg, used to designate run-time data
// sources and sinks.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Arg_H__
#define GDCC__SR__Arg_H__

#include "../Core/Counter.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace IR
   {
      enum class AddrBase;

      class Arg;
      class Program;
   }

   namespace SR
   {
      class Exp;
      class Type;

      //
      // Arg
      //
      class Arg
      {
      protected:
         using ExpCPtr  = Core::CounterPtr<Exp const>;
         using TypeCRef = Core::CounterRef<Type const>;

      public:
         Arg(Arg const &arg);
         explicit Arg(Type const *type);
         Arg(Type const *type, Exp const *data);
         Arg(Type const *type, IR::AddrBase base, Exp const *data = nullptr);
         ~Arg();

         IR::Arg getIRArg(IR::Program &prog) const;

         bool isIRArg() const;

         TypeCRef type;
         ExpCPtr  data;
      };
   }
}

#endif//GDCC__SR__Arg_H__

