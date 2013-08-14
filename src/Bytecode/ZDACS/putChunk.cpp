//-----------------------------------------------------------------------------
//
// Copyright (C) 2013 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACSE chunk output.
//
//-----------------------------------------------------------------------------

#include "Info.hpp"

#include "GDCC/IR/Function.hpp"
#include "GDCC/IR/Import.hpp"
#include "GDCC/IR/Object.hpp"
#include "GDCC/IR/StrEnt.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace Bytecode
{
   namespace ZDACS
   {
      //
      // Info::putChunk
      //
      void Info::putChunk()
      {
         // Not a real ACSE chunk, but it should be.
         putChunkCODE();

         putChunkAIMP();
         putChunkAINI();
         putChunkARAY();
         putChunkASTR();
         putChunkATAG();
         putChunkFNAM();
         putChunkFUNC();
         putChunkLOAD();
         putChunkMEXP();
         putChunkMIMP();
         putChunkMINI();
         putChunkMSTR();
         putChunkSFLG();
         putChunkSNAM();
         putChunkSPTR();
         putChunkSTRL();
         putChunkSVCT();
      }

      //
      // Info::putChunk
      //
      void Info::putChunk(char const *name, GDCC::Array<GDCC::String> const &strs, bool junk)
      {
         std::size_t base;
         std::size_t off;

         // Calculate base offset.
         base = strs.size() * 4 + 4;
         if(junk) base += 8;

         // Calculate size of chunk.
         off = base;
         for(auto const &str : strs)
            off += lenString(str);

         // Write chunk header.
         putData(name, 4);
         putWord(off);

         // Write string count.
         if(junk) putWord(0);
         putWord(strs.size());
         if(junk) putWord(0);

         // Write string offsets.
         off = base;
         for(auto const &str : strs)
         {
            putWord(off);
            off += lenString(str);
         }

         // Write strings.
         for(auto const &str : strs)
            putString(str);
      }

      //
      // Info::putChunkAIMP
      //
      void Info::putChunkAIMP()
      {
         if(!numChunkAIMP) return;

         GDCC::Array<GDCC::IR::Space const *> imps{numChunkAIMP};

         auto itr = imps.begin();
         for(auto const &sp : GDCC::IR::Space::MapArs)
            if(!sp.second.defin) *itr++ = &sp.second;

         GDCC::FastU size = numChunkAIMP * 8;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         putData("AIMP", 4);
         putWord(size);

         for(auto const &imp : imps)
         {
            putWord(imp->value);
            putWord(imp->words);
            putString(imp->glyph);
         }
      }

      //
      // Info::putChunkAINI
      //
      void Info::putChunkAINI()
      {
         if(!numChunkAINI) return;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            putData("AINI", 4);
            putWord(itr.second.vals.size() * 4 + 4);
            putWord(itr.first->value);

            for(auto const &i : itr.second.vals)
               putWord(i.val);
         }
      }

      //
      // Info::putChunkARAY
      //
      void Info::putChunkARAY()
      {
         if(!numChunkARAY) return;

         putData("ARAY", 4);
         putWord(numChunkARAY * 8);

         for(auto const &itr : GDCC::IR::Space::MapArs)
         {
            auto const &space = itr.second;

            if(!space.defin) continue;

            putWord(space.value);
            putWord(space.words);
         }
      }

      //
      // Info::putChunkASTR
      //
      void Info::putChunkASTR()
      {
         if(!numChunkASTR) return;

         putData("ASTR", 4);
         putWord(numChunkASTR * 4);

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            if(itr.second.needTag && itr.second.onlyStr)
               putWord(itr.first->value);
         }
      }

      //
      // Info::putChunkATAG
      //
      void Info::putChunkATAG()
      {
         if(!numChunkATAG) return;

         for(auto const &itr : init) if(itr.first->space == GDCC::IR::AddrBase::MapArr)
         {
            auto const &ini = itr.second;

            if(!ini.needTag || ini.onlyStr) continue;

            putData("ATAG", 4);
            putWord(ini.vals.size() + 5);

            putByte(0); // version
            putWord(itr.first->value);

            for(auto const &i : itr.second.vals) switch(i.tag)
            {
            case InitTag::Empty: putByte(0); break;
            case InitTag::Fixed: putByte(0); break;
            case InitTag::Funct: putByte(2); break;
            case InitTag::StrEn: putByte(1); break;
            }
         }
      }

      //
      // Info::putChunkCODE
      //
      void Info::putChunkCODE()
      {
         putData("CODE", 4);
         putWord(numChunkCODE);

         // Put statements.
         for(auto &itr : GDCC::IR::FunctionRange()) try
         {
            func = &itr.second;
            for(auto const &stmnt : itr.second.block)
               putStmnt(stmnt);
            func = nullptr;
         }
         catch(...) {func = nullptr; throw;}
      }

      //
      // Info::putChunkFNAM
      //
      void Info::putChunkFNAM()
      {
         if(!numChunkFNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkFNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            strs[func.valueInt] = func.glyph;
         }

         putChunk("FNAM", strs, false);
      }

      //
      // Info::putChunkFUNC
      //
      void Info::putChunkFUNC()
      {
         if(!numChunkFUNC) return;

         GDCC::Array<GDCC::IR::Function const *> funcs{numChunkFUNC};

         for(auto &func : funcs) func = nullptr;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::LangACS)
               continue;

            funcs[func.valueInt] = &func;
         }

         putData("FUNC", 4);
         putWord(numChunkFUNC * 8);

         for(auto func : funcs)
         {
            if(func)
            {
               putByte(func->param);
               putByte(std::max(func->localReg, func->param));
               putByte(!!func->retrn);
               putByte(0);

               if(func->defin)
                  putExpWord(ResolveGlyph(func->label));
               else
                  putWord(0);
            }
            else
               putData("\0\0\0\0\0\0\0\0", 8);
         }
      }

      //
      // Info::putChunkLOAD
      //
      void Info::putChunkLOAD()
      {
         numChunkLOAD = GDCC::IR::ImportRange().size();

         if(!numChunkLOAD) return;

         GDCC::FastU size = 0;

         for(auto const &imp : GDCC::IR::ImportRange())
            size += lenString(imp.glyph);

         putData("LOAD", 4);
         putWord(size);

         for(auto const &imp : GDCC::IR::ImportRange())
            putString(imp.glyph);
      }

      //
      // Info::putChunkMEXP
      //
      void Info::putChunkMEXP()
      {
         if(!numChunkMEXP) return;

         GDCC::Array<GDCC::String> strs{numChunkMEXP};
         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(obj->defin) strs[obj->value] = obj->glyph;

         for(auto const &itr : GDCC::IR::Space::MapArs)
            if(itr.second.defin) strs[itr.second.value] = itr.second.glyph;

         putChunk("MEXP", strs, false);
      }

      //
      // Info::putChunkMIMP
      //
      void Info::putChunkMIMP()
      {
         if(!numChunkMIMP) return;

         GDCC::Array<GDCC::IR::Object const *> imps{numChunkMIMP};

         auto itr = imps.begin();
         for(auto const &obj : GDCC::IR::Space::MapReg.obset)
            if(!obj->defin) *itr++ = obj;

         GDCC::FastU size = numChunkMIMP * 4;
         for(auto const &imp : imps)
            size += lenString(imp->glyph);

         putData("MIMP", 4);
         putWord(size);

         for(auto const &imp : imps)
         {
            putWord(imp->value);
            putString(imp->glyph);
         }
      }

      //
      // Info::putChunkMINI
      //
      void Info::putChunkMINI()
      {
         if(!numChunkMINI) return;

         auto const &ini = init[&GDCC::IR::Space::MapReg];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i) if(ini.vals[i].val)
         {
            putData("MINI", 4);
            putWord(8);
            putWord(i);
            putWord(ini.vals[i].val);
         }
      }

      //
      // Info::putChunkMSTR
      //
      void Info::putChunkMSTR()
      {
         if(!numChunkMSTR) return;

         putData("MSTR", 4);
         putWord(numChunkMSTR * 4);

         auto const &ini = init[&GDCC::IR::Space::MapReg];

         for(std::size_t i = 0, e = ini.vals.size(); i != e; ++i)
            if(ini.vals[i].tag == InitTag::StrEn) putWord(i);
      }

      //
      // Info::putChunkSFLG
      //
      void Info::putChunkSFLG()
      {
         if(!numChunkSFLG) return;

         putData("SFLG", 4);
         putWord(numChunkSFLG * 4);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            GDCC::FastU flags = 0;

            if(func.sflagClS) flags |= 0x0002;
            if(func.sflagNet) flags |= 0x0001;

            if(!flags) continue;

            putHWord(func.valueInt);
            putHWord(flags);
         }
      }

      //
      // Info::putChunkSNAM
      //
      void Info::putChunkSNAM()
      {
         if(!numChunkSNAM) return;

         GDCC::Array<GDCC::String> strs{numChunkSNAM};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            strs[func.valueInt] = func.valueStr;
         }

         putChunk("SNAM", strs, false);
      }

      //
      // Info::putChunkSPTR
      //
      void Info::putChunkSPTR()
      {
         if(!numChunkSPTR) return;

         putData("SPTR", 4);
         putWord(numChunkSPTR * 12);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            if(func.ctype == GDCC::IR::CallType::ScriptS)
               putHWord(-static_cast<GDCC::FastI>(func.valueInt) - 1);
            else
               putHWord(func.valueInt);

            switch(func.stype)
            {
            case GDCC::IR::ScriptType::None:       putHWord( 0); break;
            case GDCC::IR::ScriptType::Death:      putHWord( 3); break;
            case GDCC::IR::ScriptType::Disconnect: putHWord(14); break;
            case GDCC::IR::ScriptType::Enter:      putHWord( 4); break;
            case GDCC::IR::ScriptType::Lightning:  putHWord(12); break;
            case GDCC::IR::ScriptType::Open:       putHWord( 1); break;
            case GDCC::IR::ScriptType::Respawn:    putHWord( 2); break;
            case GDCC::IR::ScriptType::Return:     putHWord(15); break;
            case GDCC::IR::ScriptType::Unloading:  putHWord(13); break;
            }

            putExpWord(ResolveGlyph(func.label));
            putWord(func.param);
         }
      }

      //
      // Info::putChunkSTRL
      //
      void Info::putChunkSTRL()
      {
         if(!numChunkSTRL) return;

         GDCC::Array<GDCC::String> strs{numChunkSTRL};

         for(auto &str : strs) str = GDCC::STR_;

         for(auto const &itr : GDCC::IR::StrEntRange()) if(itr.second.defin)
            strs[itr.second.valueInt] = itr.second.valueStr;

         putChunk("STRL", strs, true);
      }

      //
      // Info::putChunkSVCT
      //
      void Info::putChunkSVCT()
      {
         if(!numChunkSVCT) return;

         putData("SVCT", 4);
         putWord(numChunkSFLG * 4);

         for(auto const &itr : GDCC::IR::FunctionRange())
         {
            auto const &func = itr.second;

            if(func.ctype != GDCC::IR::CallType::Script  &&
               func.ctype != GDCC::IR::CallType::ScriptI &&
               func.ctype != GDCC::IR::CallType::ScriptS)
               continue;

            if(!func.defin) continue;

            if(func.localReg <= 20) continue;

            putHWord(func.valueInt);
            putHWord(func.localReg);
         }
      }
   }
}

// EOF

