//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation expression value casts.
//
//-----------------------------------------------------------------------------

#include "IR/Value.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Fixed const &value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{value_.value}
      {
         if(vtype.bitsF > value_.vtype.bitsF)
            value <<= vtype.bitsF - value_.vtype.bitsF;
         else
            value >>= value_.vtype.bitsF - vtype.bitsF;

         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Fixed &&value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value)}
      {
         if(vtype.bitsF > value_.vtype.bitsF)
            value <<= vtype.bitsF - value_.vtype.bitsF;
         else
            value >>= value_.vtype.bitsF - vtype.bitsF;

         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Float const &value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{value_.value << vtype.bitsF}
      {
         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Float &&value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value) << vtype.bitsF}
      {
         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Point const &value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{value_.value}
      {
         value <<= vtype.bitsF;
         clamp();
      }

      //
      // Value_Fixed constructor
      //
      Value_Fixed::Value_Fixed(Value_Point &&value_, Type_Fixed const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value)}
      {
         value <<= vtype.bitsF;
         clamp();
      }

      //
      // Value_Float constructor
      //
      Value_Float::Value_Float(Value_Fixed const &value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{value_.value}
      {
         value >>= value_.vtype.bitsF;

         clamp();
      }

      //
      // Value_Float constructor
      //
      Value_Float::Value_Float(Value_Fixed &&value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value)}
      {
         value >>= value_.vtype.bitsF;

         clamp();
      }

      //
      // Value_Float constructor
      //
      Value_Float::Value_Float(Value_Float const &value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{value_.value}
      {
         clamp();
      }

      //
      // Value_Float constructor
      //
      Value_Float::Value_Float(Value_Float &&value_, Type_Float const &vtype_) :
         vtype{vtype_}, value{std::move(value_.value)}
      {
         clamp();
      }

      //
      // Value_Point constructor
      //
      Value_Point::Value_Point(Value_Point const &value_, Type_Point const &vtype_) :
         vtype{vtype_},
         value{value_.value},
         addrB{vtype_.reprB},
         addrN{vtype_.reprN}
      {
         if(value_.vtype.reprW > vtype.reprW)
            value *= value_.vtype.reprW / vtype.reprW;
         else if(value_.vtype.reprW < vtype.reprW)
            value /= vtype.reprW / value_.vtype.reprW;
      }

      //
      // Value_Point constructor
      //
      Value_Point::Value_Point(Value_Fixed const &value_, Type_Point const &vtype_) :
         vtype{vtype_},
         value{Core::NumberCast<Core::FastU>(value_.value >> value_.vtype.bitsF)},
         addrB{vtype_.reprB},
         addrN{vtype_.reprN}
      {
      }
   }
}

// EOF

