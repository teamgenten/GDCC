//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation output.
//
//-----------------------------------------------------------------------------

#include "IR/OArchive.hpp"

#include "Core/Origin.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC
{
   namespace IR
   {
      //
      // OArchive::putHeader
      //
      OArchive &OArchive::putHeader()
      {
         out << "DGE_NTS" << '\0' << "GDCC::IR" << '\0' << '\0';
         putTablesString();
         return *this;
      }

      //
      // OArchive::putTablesString
      //
      OArchive &OArchive::putTablesString()
      {
         auto begin = Core::String::GetDataV() + Core::STRMAX;
         auto end   = Core::String::GetDataV() + Core::String::GetDataC();

         *this << std::distance(begin, end);

         for(auto itr = begin; itr != end; ++itr)
         {
            for(auto ci = itr->data(), ce = ci + itr->size(); ci != ce; ++ci)
            {
               if(*ci)
                  out << *ci;
               else
                  out << '\xC0' << '\x80';
            }

            out << '\0';
         }

         return *this;
      }

      //
      // operator OArchive << Core::Origin
      //
      OArchive &operator << (OArchive &out, Core::Origin const &in)
      {
         return out << in.file << in.line << in.col;
      }

      //
      // operator OArchive << Core::String
      //
      OArchive &operator << (OArchive &out, Core::String in)
      {
         return out << static_cast<std::size_t>(in);
      }

      //
      // operator OArchive << Core::StringIndex
      //
      OArchive &operator << (OArchive &out, Core::StringIndex in)
      {
         return out << static_cast<std::size_t>(in);
      }
   }
}

// EOF

