//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#include "Bytecode/ZDACS/Info.hpp"

#include "Core/Exception.hpp"
#include "Core/Option.hpp"

#include "IR/CallType.hpp"
#include "IR/Program.hpp"

#include "Option/Bool.hpp"
#include "Option/Exception.hpp"
#include "Option/Int.hpp"

#include <climits>
#include <iostream>


//----------------------------------------------------------------------------|
// Options                                                                    |
//

//
// --bc-zdacs-Sta-array
//
static GDCC::Option::Int<GDCC::Core::FastU> LocArsArrayOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-Sta-array")
      .setGroup("codegen")
      .setDescS("Sets the global array used for Sta."),

   &GDCC::Bytecode::ZDACS::Info::StaArray
};

//
// --bc-zdacs-chunk-STRE
//
static GDCC::Option::Bool ChunkSTREOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-chunk-STRE")
      .setGroup("output")
      .setDescS("Generates an encrypted string table."),

   &GDCC::Bytecode::ZDACS::Info::UseChunkSTRE
};

//
// --bc-zdacs-fake-ACS0
//
static GDCC::Option::Bool FakeACS0Opt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-fake-ACS0")
      .setGroup("output")
      .setDescS("Generates a fake ACS0 header."),

   &GDCC::Bytecode::ZDACS::Info::UseFakeACS0
};

//
// --bc-zdacs-init-gbl-array
//
static GDCC::Core::FastU InitGblArray = 0;
static GDCC::Option::Int<GDCC::Core::FastU> InitGblArrayOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-init-gbl-array")
      .setGroup("codegen")
      .setDescS("Sets the global array used to store init status.")
      .setDescL(
         "Sets the global array used to store initialization status. "
         "Default is to use the LocArs array."),

   &InitGblArray
};

//
// --bc-zdacs-init-gbl-index
//
static GDCC::Core::FastU InitGblIndex = 0;
static GDCC::Option::Int<GDCC::Core::FastU> InitGblIndexOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-init-gbl-index")
      .setGroup("codegen")
      .setDescS("Sets the global array index used to store init status.")
      .setDescL(
         "Sets the global array index used to store initialization status. "
         "Default is 0 if using the LocArs array, -1 otherwise."),

   &InitGblIndex
};

//
// --bc-zdacs-init-script-number
//
static GDCC::Option::Int<GDCC::Core::FastU> InitScriptNumberOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-init-script-number")
      .setGroup("codegen")
      .setDescS("Sets the number for the init script.")
      .setDescL(
         "Sets the script number for the initialization script. "
         "Default is 999."),

   &GDCC::Bytecode::ZDACS::Info::InitScriptNumber
};

//
// --bc-zdacs-init-wld-array
//
static GDCC::Core::FastU InitWldArray = 0;
static GDCC::Option::Int<GDCC::Core::FastU> InitWldArrayOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-init-wld-array")
      .setGroup("codegen")
      .setDescS("Sets the world array used to store init status.")
      .setDescL(
         "Sets the world array used to store initialization status. "
         "Default is to use any world array with an initializer."),

   &InitWldArray
};

//
// --bc-zdacs-init-wld-index
//
static GDCC::Core::FastU InitWldIndex = 0xFFFFFFFF;
static GDCC::Option::Int<GDCC::Core::FastU> InitWldIndexOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-init-wld-index")
      .setGroup("codegen")
      .setDescS("Sets the world array index used to store init status.")
      .setDescL(
         "Sets the world array index used to store initialization status. "
         "Default is -1."),

   &InitWldIndex
};

//
// --bc-zdacs-scripti-param
//
static GDCC::Core::FastU ScriptIParam = 4;
static GDCC::Option::Int<GDCC::Core::FastU> ScriptIParamOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-scripti-param")
      .setGroup("codegen")
      .setDescS("Sets the number of native parameters for numbered scripts.")
      .setDescL("Sets the number of native parameters for numbered scripts. "
         "Default is 4."),

   &ScriptIParam
};

//
// --bc-zdacs-scripts-param
//
static GDCC::Core::FastU ScriptSParam = 4;
static GDCC::Option::Int<GDCC::Core::FastU> ScriptSParamOpt
{
   &GDCC::Core::GetOptionList(), GDCC::Option::Base::Info()
      .setName("bc-zdacs-scripts-param")
      .setGroup("codegen")
      .setDescS("Sets the number of native parameters for named scripts.")
      .setDescL("Sets the number of native parameters for named scripts. "
         "Default is 4."),

   &ScriptSParam
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         IR::Type_Fixed const Info::TypeWord{32, 0, false, false};

         Core::FastU Info::InitScriptNumber = 999;

         Core::FastU Info::StaArray = 0;

         bool Info::UseChunkSTRE = false;
         bool Info::UseFakeACS0  = false;
      }
   }
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Info default constructor
         //
         Info::Info() :
            codeInit   {0},
            codeInitEnd{0},

            numChunkAIMP{0},
            numChunkAINI{0},
            numChunkARAY{0},
            numChunkASTR{0},
            numChunkATAG{0},
            numChunkCODE{0},
            numChunkFNAM{0},
            numChunkFUNC{0},
            numChunkLOAD{0},
            numChunkMEXP{0},
            numChunkMIMP{0},
            numChunkMINI{0},
            numChunkMSTR{0},
            numChunkSFLG{0},
            numChunkSNAM{0},
            numChunkSPTR{0},
            numChunkSTRL{0},
            numChunkSVCT{0}
         {
         }

         //
         // Info::backGlyphFunc
         //
         void Info::backGlyphFunc(Core::String glyph, Core::FastU val,
            IR::CallType ctype)
         {
            auto &data = prog->getGlyphData(glyph);

            data.type  = IR::Type_Funct(ctype);
            data.value = IR::ExpCreate_Value(
               IR::Value_Funct(val, IR::Type_Funct(ctype)),
               Core::Origin(Core::STRNULL, 0));
         }

         //
         // Info::backGlyphGlyph
         //
         void Info::backGlyphGlyph(Core::String glyph, Core::String val)
         {
            auto &data = prog->getGlyphData(glyph);

            data.type  = prog->getGlyphData(val).type;
            data.value = IR::ExpCreate_Glyph(
               IR::Glyph(prog, val),
               Core::Origin(Core::STRNULL, 0));
         }

         //
         // Info::backGlyphObj
         //
         void Info::backGlyphObj(Core::String glyph, Core::FastU val)
         {
            auto &data = prog->getGlyphData(glyph);

            data.value = IR::ExpCreate_Value(IR::Value_Point(val,
               data.type.tPoint.reprB, data.type.tPoint.reprN, data.type.tPoint), {nullptr, 0});
         }

         //
         // Info::backGlyphStr
         //
         void Info::backGlyphStr(Core::String glyph, Core::FastU val)
         {
            auto &data = prog->getGlyphData(glyph);

            data.type  = IR::Type_StrEn();
            data.value = IR::ExpCreate_Value(
               IR::Value_StrEn(val, IR::Type_StrEn()),
               Core::Origin(Core::STRNULL, 0));
         }

         //
         // Info::backGlyphWord
         //
         void Info::backGlyphWord(Core::String glyph, Core::FastU val)
         {
            auto &data = prog->getGlyphData(glyph);

            data.type  = TypeWord;
            data.value = IR::ExpCreate_Value(
               IR::Value_Fixed(val, TypeWord), Core::Origin(Core::STRNULL, 0));
         }

         //
         // Info::getCallName
         //
         Core::String Info::getCallName()
         {
            char        buf[sizeof("___GDCC__Code_XX_Wx")];
            std::size_t len;

            char const *code;
            int         size;

            // Some codes get mapped to other function names.
            IR::Code opCode = stmnt->op.code;
            switch(opCode)
            {
            case IR::Code::AddI_W:    opCode = IR::Code::AddU_W;    break;
            case IR::Code::CmpI_EQ_W: opCode = IR::Code::CmpU_EQ_W; break;
            case IR::Code::CmpI_NE_W: opCode = IR::Code::CmpU_NE_W; break;
            case IR::Code::MulI_W:    opCode = IR::Code::MulU_W;    break;
            case IR::Code::SubI_W:    opCode = IR::Code::SubU_W;    break;
            default: break;
            }

            // Convert code to string.
            switch(opCode)
            {
               #define GDCC_IR_CodeList(c) case IR::Code::c: code = #c; break;
               #include "IR/CodeList.hpp"

            default:
               throw Core::ExceptStr(stmnt->pos, "bad getCallName code");
            }

            // Convert size to int.
            if(stmnt->op.size > INT_MAX)
               throw Core::ExceptStr(stmnt->pos, "bad getCallName size");

            size = static_cast<int>(stmnt->op.size);

            // Format function name.
            len = std::snprintf(buf, sizeof(buf), "___GDCC__%s%i", code, size);

            if(len >= sizeof(buf))
               throw Core::ExceptStr(stmnt->pos, "bad getCallName");

            return {buf, len};
         }

         //
         // Info::getInitGblArray
         //
         Core::FastU Info::getInitGblArray()
         {
            if(InitGblArrayOpt.processed)
               return InitGblArray;

            return StaArray;
         }

         //
         // Info::getInitGblIndex
         //
         Core::FastU Info::getInitGblIndex()
         {
            if(InitGblIndexOpt.processed)
               return InitGblIndex;

            if(!InitGblArrayOpt.processed || InitGblArray == StaArray)
               return 0;

            return -1;
         }

         //
         // Info::getInitWldArray
         //
         Core::FastU Info::getInitWldArray()
         {
            if(InitWldArrayOpt.processed)
               return InitWldArray;

            for(auto const &itr : prog->rangeSpaceWldArs())
            {
               if(!init[&itr].vals.empty())
                  return itr.value;
            }

            return 0;
         }

         //
         // Info::getInitWldIndex
         //
         Core::FastU Info::getInitWldIndex()
         {
            return InitWldIndex;
         }

         //
         // Info::getStkPtrIdx
         //
         Core::FastU Info::getStkPtrIdx()
         {
            switch(func->ctype)
            {
            case IR::CallType::SScriptI:
            case IR::CallType::SScriptS:
            case IR::CallType::StdCall:
               return 0;

            case IR::CallType::ScriptI:
            case IR::CallType::ScriptS:
               return func->localReg - 1;

            default:
               std::cerr << "ERROR: " << stmnt->pos << ": bad getStkPtrIdx\n";
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::isDropArg
         //
         bool Info::isDropArg(IR::Arg const &arg)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
            case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
            case IR::ArgBase::GblReg: return true;
            case IR::ArgBase::LocReg: return true;
            case IR::ArgBase::MapArr: return isPushArg(*arg.aMapArr.idx);
            case IR::ArgBase::MapReg: return true;
            case IR::ArgBase::Nul:    return true;
            case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
            case IR::ArgBase::WldArr: return isPushArg(*arg.aWldArr.idx);
            case IR::ArgBase::WldReg: return true;
            default:                  return false;
            }
         }

         //
         // Info::isCopyArg
         //
         bool Info::isCopyArg(IR::Arg const &arg)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
            case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
            case IR::ArgBase::GblReg: return true;
            case IR::ArgBase::LocReg: return true;
            case IR::ArgBase::MapArr: return isPushArg(*arg.aMapArr.idx);
            case IR::ArgBase::MapReg: return true;
            case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
            case IR::ArgBase::WldArr: return isPushArg(*arg.aWldArr.idx);
            case IR::ArgBase::WldReg: return true;
            default:                  return false;
            }
         }

         //
         // Info::isFastArg
         //
         bool Info::isFastArg(IR::Arg const &arg)
         {
            switch(arg.a)
            {
            case IR::ArgBase::GblReg: return true;
            case IR::ArgBase::Lit:    return true;
            case IR::ArgBase::LocReg: return true;
            case IR::ArgBase::MapReg: return true;
            case IR::ArgBase::WldReg: return true;
            default:                  return false;
            }
         }

         //
         // Info::isInitiGblArr
         //
         bool Info::isInitiGblArr()
         {
            for(auto const &itr : prog->rangeSpaceGblArs())
               if(!init[&itr].vals.empty()) return true;

            return !init[&prog->getSpaceSta()].vals.empty();
         }

         //
         // Info::isInitiWldArr
         //
         bool Info::isInitiWldArr()
         {
            for(auto const &itr : prog->rangeSpaceWldArs())
               if(!init[&itr].vals.empty()) return true;

            return false;
         }

         //
         // Info::isPushArg
         //
         bool Info::isPushArg(IR::Arg const &arg)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Aut:    return isPushArg(*arg.aAut.idx);
            case IR::ArgBase::GblArr: return isPushArg(*arg.aGblArr.idx);
            case IR::ArgBase::GblReg: return true;
            case IR::ArgBase::Lit:    return true;
            case IR::ArgBase::LocReg: return true;
            case IR::ArgBase::MapArr: return isPushArg(*arg.aMapArr.idx);
            case IR::ArgBase::MapReg: return true;
            case IR::ArgBase::Sta:    return isPushArg(*arg.aSta.idx);
            case IR::ArgBase::WldArr: return isPushArg(*arg.aWldArr.idx);
            case IR::ArgBase::WldReg: return true;
            default:                  return false;
            }
         }

         //
         // Info::lenDropArg
         //
         Core::FastU Info::lenDropArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // lenArr
            //
            auto lenArr = [&](IR::ArgPtr2 const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 20;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 12;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            //
            // lenAut
            //
            auto lenAut = [&](IR::Arg_Aut const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 32;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 24;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            //
            // lenSta
            //
            auto lenSta = [&](IR::Arg_Sta const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 20;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 12;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            switch(arg.a)
            {
            case IR::ArgBase::Aut:    return lenAut(arg.aAut);
            case IR::ArgBase::GblArr: return lenArr(arg.aGblArr);
            case IR::ArgBase::GblReg: return 8;
            case IR::ArgBase::LocReg: return 8;
            case IR::ArgBase::MapArr: return lenArr(arg.aMapArr);
            case IR::ArgBase::MapReg: return 8;
            case IR::ArgBase::Nul:    return 4;
            case IR::ArgBase::Sta:    return lenSta(arg.aSta);
            case IR::ArgBase::WldArr: return lenArr(arg.aWldArr);
            case IR::ArgBase::WldReg: return 8;
            default:
               throw Core::ExceptStr(stmnt->pos, "bad lenDropArg");
            }
         }

         //
         // Info::lenDropArg
         //
         Core::FastU Info::lenDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            Core::FastU len = 0;

            for(; lo != hi; ++lo)
               len += lenDropArg(arg, lo);

            return len;
         }

         //
         // Info::lenIncUArg
         //
         Core::FastU Info::lenIncUArg(IR::Arg const &arg, Core::FastU)
         {
            switch(arg.a)
            {
            case IR::ArgBase::GblReg: return 8;
            case IR::ArgBase::LocReg: return 8;
            case IR::ArgBase::MapReg: return 8;
            case IR::ArgBase::WldReg: return 8;
            default:
               throw Core::ExceptStr(stmnt->pos, "bad lenIncUArg");
            }
         }

         //
         // Info::lenIncUArg
         //
         Core::FastU Info::lenIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            Core::FastU len = 0;

            for(; lo != hi; ++lo)
               len += lenIncUArg(arg, lo);

            return len;
         }

         //
         // Info::lenPushArg
         //
         Core::FastU Info::lenPushArg(IR::Arg const &arg, Core::FastU w)
         {
            //
            // lenArr
            //
            auto lenArr = [&](IR::ArgPtr2 const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 16;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 8;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            //
            // lenAut
            //
            auto lenAut = [&](IR::Arg_Aut const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 28;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 20;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            //
            // lenLit
            //
            auto lenLit = [&]() -> Core::FastU
            {
               auto type = arg.aLit.value->getType();
               auto wLit = arg.aLit.off + w;

               switch(type.t)
               {
               case IR::TypeBase::Funct:
                  return wLit == 0 && IsScriptS(type.tFunct.callT) ? 12 : 8;

               case IR::TypeBase::StrEn: return wLit == 0 ? 12 : 8;
               default:                  return 8;
               }
            };

            //
            // lenSta
            //
            auto lenSta = [&](IR::Arg_Sta const &a) -> Core::FastU
            {
               if(a.idx->a == IR::ArgBase::Lit)
                  return 16;
               else
               {
                  Core::FastU len = lenPushArg(*a.idx, 0) + 8;

                  if(a.off + w)
                     len += 12;

                  return len;
               }
            };

            switch(arg.a)
            {
            case IR::ArgBase::Aut:    return lenAut(arg.aAut);
            case IR::ArgBase::GblArr: return lenArr(arg.aGblArr);
            case IR::ArgBase::GblReg: return 8;
            case IR::ArgBase::Lit:    return lenLit();
            case IR::ArgBase::LocReg: return 8;
            case IR::ArgBase::MapArr: return lenArr(arg.aMapArr);
            case IR::ArgBase::MapReg: return 8;
            case IR::ArgBase::Sta:    return lenSta(arg.aSta);
            case IR::ArgBase::WldArr: return lenArr(arg.aWldArr);
            case IR::ArgBase::WldReg: return 8;

            default:
               throw Core::ExceptStr(stmnt->pos, "bad lenPushArg");
            }
         }

         //
         // Info::lenPushArg
         //
         Core::FastU Info::lenPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi)
         {
            Core::FastU len = 0;

            for(; lo != hi; ++lo)
               len += lenPushArg(arg, lo);

            return len;
         }

         //
         // Info::lenString
         //
         std::size_t Info::lenString(Core::String s)
         {
            std::size_t len = 0;

            for(auto i = s.begin(), e = s.end(); i != e; ++i) switch(*i)
            {
            case '\0':
               if('0' <= i[1] && i[1] <= '7')
                  len += 4;
               else
                  len += 2;
               break;

            case '\\':
               len += 2;
               break;

            default:
               len += 1;
               break;
            }

            return len + 1;
         }

         //
         // Info::resolveGlyph
         //
         IR::Exp::CRef Info::resolveGlyph(Core::String glyph)
         {
            if(auto exp = prog->getGlyphData(glyph).value)
               return static_cast<IR::Exp::CRef>(exp);

            std::cerr << "ERROR: unbacked glyph: '" << glyph << "'\n";
            throw EXIT_FAILURE;
         }

         //
         // Info::CheckArg
         //
         void Info::CheckArg(IR::Arg const &arg, Core::Origin pos)
         {
            switch(arg.a)
            {
            case IR::ArgBase::Aut:
               CheckArg(*arg.aAut.idx, pos);
               break;

            case IR::ArgBase::GblArr:
               CheckArgB(*arg.aGblArr.arr, IR::ArgBase::Lit, pos);
               CheckArg (*arg.aGblArr.idx, pos);
               break;

            case IR::ArgBase::GblReg:
               CheckArgB(*arg.aGblReg.idx, IR::ArgBase::Lit, pos);
               break;

            case IR::ArgBase::Lit: break;
            case IR::ArgBase::Nul: break;
            case IR::ArgBase::Stk: break;

            case IR::ArgBase::LocArr:
               CheckArgB(*arg.aLocArr.arr, IR::ArgBase::Lit, pos);
               CheckArg (*arg.aLocArr.idx, pos);
               break;

            case IR::ArgBase::LocReg:
               CheckArgB(*arg.aLocReg.idx, IR::ArgBase::Lit, pos);
               break;

            case IR::ArgBase::MapArr:
               CheckArgB(*arg.aMapArr.arr, IR::ArgBase::Lit, pos);
               CheckArg (*arg.aMapArr.idx, pos);
               break;

            case IR::ArgBase::MapReg:
               CheckArgB(*arg.aMapReg.idx, IR::ArgBase::Lit, pos);
               break;

            case IR::ArgBase::Sta:
               CheckArg(*arg.aSta.idx, pos);
               break;

            case IR::ArgBase::StrArs:
               CheckArg(*arg.aStrArs.idx, pos);
               break;

            case IR::ArgBase::WldArr:
               CheckArgB(*arg.aWldArr.arr, IR::ArgBase::Lit, pos);
               CheckArg (*arg.aWldArr.idx, pos);
               break;

            case IR::ArgBase::WldReg:
               CheckArgB(*arg.aWldReg.idx, IR::ArgBase::Lit, pos);
               break;

            default:
               std::cerr << "ERROR: " << pos << ": ZDACS cannot use Arg: "
                  << arg.a << '\n';
               throw EXIT_FAILURE;
            }
         }

         //
         // Info::CodeBase
         //
         Core::FastU Info::CodeBase()
         {
            return 24;
         }

         //
         // Info:GetParamMax
         //
         Core::FastU Info::GetParamMax(IR::CallType call)
         {
            switch(call)
            {
               // Numbered Scripts.
            case IR::CallType::SScriptI:
            case IR::CallType::ScriptI:
               return ScriptIParam;

               // Named scripts.
            case IR::CallType::SScriptS:
            case IR::CallType::ScriptS:
               return ScriptSParam;

               // Normal functions.
            case IR::CallType::StdCall:
            case IR::CallType::StkCall:
               return 255;

            default:
               return -1;
            }
         }

         //
         // Info::GetScriptValue
         //
         Core::FastU Info::GetScriptValue(IR::Function const &script)
         {
            if(IsScriptS(script.ctype))
               return -static_cast<Core::FastI>(script.valueInt) - 1;
            else
               return script.valueInt;
         }

         //
         // Info::GetWord
         //
         Core::FastU Info::GetWord(IR::Arg_Lit const &arg, Core::FastU w)
         {
            return GetWord(arg.value, arg.off + w);
         }

         //
         // Info::GetWord
         //
         Core::FastU Info::GetWord(IR::Exp const *exp, Core::FastU w)
         {
            auto val = exp->getValue();

            switch(val.v)
            {
            case IR::ValueBase::Fixed: return GetWord_Fixed(val.vFixed, w);
            case IR::ValueBase::Float: return GetWord_Float(val.vFloat, w);
            case IR::ValueBase::Funct: return w ? 0 : val.vFunct.value;
            case IR::ValueBase::Point: return w ? 0 : val.vPoint.value;
            case IR::ValueBase::StrEn: return w ? 0 : val.vFunct.value;

            default:
               throw Core::ExceptStr(exp->pos, "bad GetWord Value");
            }
         }

         //
         // GetWord_Fixed
         //
         Core::FastU Info::GetWord_Fixed(IR::Value_Fixed const &val, Core::FastU w)
         {
            Core::FastU valI;

            if(w)
            {
               if(val.vtype.bitsS)
                  valI = Core::NumberCast<Core::FastI>(val.value >> (w * 32));
               else
                  valI = Core::NumberCast<Core::FastU>(val.value >> (w * 32));
            }
            else
            {
               if(val.vtype.bitsS)
                  valI = Core::NumberCast<Core::FastI>(val.value);
               else
                  valI = Core::NumberCast<Core::FastU>(val.value);
            }

            return valI & 0xFFFFFFFF;
         }

         //
         // GetWord_Float
         //
         Core::FastU Info::GetWord_Float(IR::Value_Float const &val, Core::FastU w)
         {
            // Special handling for 0.
            // TODO: Negative zero. May require replacing mpf_class.
            if(!val.value) return 0;

            // Convert float to binary string.
            // The sign and mantissa bits are stored in the string, while the
            // exponent is stored in exp. The string includes the implicit
            // leading 1 which will be skipped.
            std::unique_ptr<char[]> str{new char[val.vtype.bitsI + 3]};
            mp_exp_t                exp;
            mpf_get_str(str.get(), &exp, 2, val.vtype.bitsI + 1, val.value.get_mpf_t());

            Core::Integ valI = 0;

            // Sign bit.
            auto start = str.get();
            if(*start == '-') valI |= 1, ++start;

            // Exponent bits.
            exp += (1 << (val.vtype.bitsF - 1)) - 2;
            exp &= (1 << (val.vtype.bitsF    )) - 1;

            valI <<= val.vtype.bitsF;
            valI |= exp;

            // Mantissa bits. Skip first bit because it is an implicit 1.
            auto itr = ++start;
            for(; *itr; ++itr) {valI <<= 1; if(*itr == '1') ++valI;}
            valI <<= val.vtype.bitsI - (itr - start);

            // Return requested word.
            if(w) valI >>= w * 32;
            return Core::NumberCast<Core::FastU>(valI) & 0xFFFFFFFF;
         }

         //
         // Info::IsScript
         //
         bool Info::IsScript(IR::CallType ctype)
         {
            return
               ctype == IR::CallType::SScriptI ||
               ctype == IR::CallType::SScriptS ||
               ctype == IR::CallType::ScriptI ||
               ctype == IR::CallType::ScriptS;
         }

         //
         // Info::IsScriptS
         //
         bool Info::IsScriptS(IR::CallType ctype)
         {
            return
               ctype == IR::CallType::SScriptS ||
               ctype == IR::CallType::ScriptS;
         }
      }
   }
}

// EOF

