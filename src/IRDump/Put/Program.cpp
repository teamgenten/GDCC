//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// IR::Program dump/disassembly functions.
//
//-----------------------------------------------------------------------------

#include "IRDump/Put.hpp"

#include "Core/Option.hpp"

#include "IR/Program.hpp"

#include "Option/Bool.hpp"


//----------------------------------------------------------------------------|
// Options                                                                    |
//

namespace GDCC::IRDump
{
   //
   // --dump-djump
   //
   static Option::Bool DumpDJump
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-djump")
         .setGroup("output")
         .setDescS("Dump IR::DJump objects."),

      &OptDJump
   };

   //
   // --dump-function
   //
   static Option::Bool DumpFunction
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-function")
         .setGroup("output")
         .setDescS("Dump IR::Function objects."),

      &OptFunction
   };

   //
   // --dump-glyph
   //
   static Option::Bool DumpGlyph
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-glyph")
         .setGroup("output")
         .setDescS("Dump IR::GlyphData objects."),

      &OptGlyph
   };

   //
   // --dump-headers
   //
   static Option::Bool DumpHeaders
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-headers")
         .setGroup("output")
         .setDescS("Include comment headers in output."),

      &OptHeaders
   };

   //
   // --dump-import
   //
   static Option::Bool DumpImport
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-import")
         .setGroup("output")
         .setDescS("Dump IR::Import objects."),

      &OptImport
   };

   //
   // --dump-object
   //
   static Option::Bool DumpObject
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-object")
         .setGroup("output")
         .setDescS("Dump IR::Object objects."),

      &IRDump::OptObject
   };

   //
   // --dump-space
   //
   static Option::Bool DumpSpace
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-space")
         .setGroup("output")
         .setDescS("Dump IR::Space objects."),

      &OptSpace
   };

   //
   // --dump-statistics
   //
   static Option::Bool DumpStatistics
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-statistics")
         .setGroup("output")
         .setDescS("Dump overall statistics."),

      &OptStatistics
   };

   //
   // --dump-strent
   //
   static Option::Bool DumpStrEnt
   {
      &Core::GetOptionList(), Option::Base::Info()
         .setName("dump-strent")
         .setGroup("output")
         .setDescS("Dump IR::StrEnt objects."),

      &OptStrEnt
   };
}


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutHeader
   //
   static void PutHeader(std::ostream &out, char const *str)
   {
      auto sp = 75 - std::strlen(str);

      out << "\n\n;;";     for(int i = 76; i--;) out << '-'; out << "|\n";
      out << ";; " << str; for(int i = sp; i--;) out << ' '; out << "|\n";
      out << ";;\n";
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::IRDump
{
   //
   // PutProgram
   //
   void PutProgram(std::ostream &out, IR::Program const &prog)
   {
      // File header.
      if(OptHeaders)
      {
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         out << ";;\n";
         out << ";; IR dump from gdcc-irdump " << Core::GetOptionList().version << '\n';
         out << ";;\n";
         out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
      }

      // Overall statistics.
      if(OptStatistics)
      {
         if(OptHeaders) out << ";;\n";

         out << ";; DJumps: "                  << prog.sizeDJump()       << '\n';
         out << ";; Functions: "               << prog.sizeFunction()    << '\n';
         out << ";; Glyphs: "                  << prog.sizeGlyphData()   << '\n';
         out << ";; Imports: "                 << prog.sizeImport()      << '\n';
         out << ";; Objects: "                 << prog.sizeObject()      << '\n';
         out << ";; Address Spaces (GblArs): " << prog.sizeSpaceGblArs() << '\n';
         out << ";; Address Spaces (HubArs): " << prog.sizeSpaceHubArs() << '\n';
         out << ";; Address Spaces (LocArs): " << prog.sizeSpaceLocArs() << '\n';
         out << ";; Address Spaces (ModArs): " << prog.sizeSpaceModArs() << '\n';
         out << ";; String Table Entries: "    << prog.sizeStrEnt()      << '\n';

         if(OptHeaders)
         {
            out << ";;\n";
            out << ";;"; for(int i = 77; i--;) out << '-'; out << '\n';
         }
      }

      // Functions
      if(OptDJump)
      {
         if(OptHeaders) PutHeader(out, "DJumps");
         for(auto const &jump : prog.rangeDJump()) PutDJump(out, jump);
      }

      // Functions
      if(OptFunction)
      {
         if(OptHeaders) PutHeader(out, "Functions");
         for(auto const &fn : prog.rangeFunction()) PutFunction(out, fn);
      }

      // Glyphs
      if(OptGlyph)
      {
         if(OptHeaders) PutHeader(out, "Glyphs");
         for(auto const &glyph : prog.rangeGlyphData()) PutGlyphData(out, glyph);
      }

      // Imports
      if(OptImport)
      {
         if(OptHeaders) PutHeader(out, "Imports");
         for(auto const &imp : prog.rangeImport()) PutImport(out, imp);
      }

      // Objects
      if(OptObject)
      {
         if(OptHeaders) PutHeader(out, "Objects");
         for(auto const &obj : prog.rangeObject()) PutObject(out, obj);
      }

      // Spaces
      if(OptSpace)
      {
         if(OptHeaders) PutHeader(out, "Address Spaces");
         for(auto const &sp : prog.rangeSpaceGblArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceHubArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceLocArs()) PutSpace(out, sp);
         for(auto const &sp : prog.rangeSpaceModArs()) PutSpace(out, sp);
      }

      // StrEnts
      if(OptStrEnt)
      {
         if(OptHeaders) PutHeader(out, "String Table Entries");
         for(auto const &str : prog.rangeStrEnt()) PutStrEnt(out, str);
      }

      // File footer.
      if(OptHeaders)
      {
         out << "\n;; EOF\n\n";
      }
   }
}

// EOF

