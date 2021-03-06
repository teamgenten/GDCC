//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS target information and handling class.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__ZDACS__Info_H__
#define GDCC__BC__ZDACS__Info_H__

#include "../../BC/Info.hpp"

#include "../../Core/Array.hpp"
#include "../../Core/Counter.hpp"
#include "../../Core/Number.hpp"
#include "../../Core/NumberAlloc.hpp"

#include "../../IR/Addr.hpp"
#include "../../IR/CallType.hpp"

#include <unordered_map>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace BC
   {
      namespace ZDACS
      {
         enum class Code;

         //
         // Info
         //
         class Info : public InfoBase
         {
         public:
            Info();


            static IR::Type_Fixed const TypeWord;

            static Core::FastU FarJumpIndex;

            static bool         InitDelay;
            static Core::String InitScriptName;
            static bool         InitScriptNamed;
            static Core::FastU  InitScriptNumber;

            static Core::FastU StaArray;

            static bool UseChunkSTRE;
            static bool UseFakeACS0;

         protected:
            //
            // InitTag
            //
            enum class InitTag
            {
               Empty,
               Fixed,
               Funct,
               StrEn,
            };

            //
            // InitVal
            //
            class InitVal
            {
            public:
               InitVal() : val{0}, tag{InitTag::Empty} {}

               Core::FastU val;
               InitTag     tag;
            };

            //
            // InitData
            //
            class InitData
            {
            public:
               InitData() : max{0}, needTag{false}, onlyNil{true}, onlyStr{true} {}

               std::unordered_map<Core::FastU, InitVal> vals;

               Core::FastU max;

               bool needTag : 1;
               bool onlyNil : 1;
               bool onlyStr : 1;
            };


            void backGlyphDJump(Core::String glyph, Core::FastU val);
            void backGlyphFunc(Core::String glyph, Core::FastU val, IR::CallType ctype);
            void backGlyphGlyph(Core::String glyph, Core::String val);
            void backGlyphObj(Core::String glyph, Core::FastU val);
            void backGlyphStr(Core::String glyph, Core::FastU val);
            void backGlyphWord(Core::String glyph, Core::FastU val);

            virtual void gen();

            virtual void genDJump();

            virtual void genFunc();

            void genIniti();
            void genInitiSpace(IR::Space &space);

            virtual void genObj();

            virtual void genSpace();
            void genSpaceIniti();
            void genSpaceIniti(IR::Space &space);
            void genSpaceInitiValue(InitData &ini, Core::FastU &itr, IR::Value const &val);

            virtual void genStmnt();
            void genStmnt_AdXU_W();
            void genStmnt_AddU_W();
            void genStmnt_BNot_W();
            void genStmnt_Bclz_W();
            void genStmnt_Bges_W();
            void genStmnt_Bget_W();
            void genStmnt_Bset_W();
            void genStmnt_Call();
            void genStmnt_CmpI_W();
            void genStmnt_CmpU_EQ_W();
            void genStmnt_Cscr_IA();
            void genStmnt_Cscr_IS();
            void genStmnt_Cscr_SA();
            void genStmnt_Cscr_SS();
            void genStmnt_Cspe();
            void genStmnt_DiXI_W();
            void genStmnt_DivI_W();
            void genStmnt_DivU_W();
            void genStmnt_DivX_W();
            void genStmnt_Jcnd_Nil();
            void genStmnt_Jcnd_Tab();
            void genStmnt_Jcnd_Tru();
            void genStmnt_Jfar();
            void genStmnt_Jset();
            void genStmnt_Jump();
            void genStmnt_ModI_W();
            void genStmnt_ModU_W();
            void genStmnt_Move_W();
            void genStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr);
            void genStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr);
            void genStmnt_NegF_W();
            void genStmnt_NegI_W();
            void genStmnt_Retn();
            void genStmnt_ShLF_W();
            void genStmnt_ShLU_W();
            void genStmnt_ShRI_W();
            void genStmnt_ShRU_W();
            void genStmnt_ShRU_W1();
            void genStmnt_SuXU_W();
            void genStmnt_Swap_W();

            void genStmntBitwise();

            void genStmntCall(Core::FastU ret);

            void genStmntDropArg(IR::Arg const &arg, Core::FastU w);
            void genStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void genStmntPushArg(IR::Arg const &arg, Core::FastU w);
            void genStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            virtual void genStr();

            Core::NumberAllocMerge<Core::FastU> &getAllocDJump();
            Core::NumberAllocMerge<Core::FastU> &getAllocFunc(IR::CallType call);
            Core::NumberAllocMerge<Core::FastU> &getAllocObj(IR::AddrSpace addr);
            Core::NumberAllocMerge<Core::FastU> &getAllocSpace(IR::AddrBase addr);
            Core::NumberAllocMerge<Core::FastU> &getAllocStrEnt();

            Core::String getCallName();
            Core::String getCallName(IR::OpCode op);

            Core::FastU getInitGblArray();
            Core::FastU getInitGblIndex();
            Core::FastU getInitHubArray();
            Core::FastU getInitHubIndex();

            Core::FastU getSpaceInitiSize(IR::Type const &type);

            Core::FastU getStkPtrIdx();

            bool isCopyArg(IR::Arg const &arg);

            bool isDropArg(IR::Arg const &arg);

            bool isFastArg(IR::Arg const &arg);

            bool isInitiGblArr();
            bool isInitiHubArr();

            bool isPushArg(IR::Arg const &arg);

            Core::FastU lenChunk();
            Core::FastU lenChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
            Core::FastU lenChunkAIMP();
            Core::FastU lenChunkAINI();
            Core::FastU lenChunkARAY();
            Core::FastU lenChunkASTR();
            Core::FastU lenChunkATAG();
            Core::FastU lenChunkCODE();
            Core::FastU lenChunkFARY();
            Core::FastU lenChunkFNAM();
            Core::FastU lenChunkFUNC();
            Core::FastU lenChunkJUMP();
            Core::FastU lenChunkLOAD();
            Core::FastU lenChunkMEXP();
            Core::FastU lenChunkMIMP();
            Core::FastU lenChunkMINI();
            Core::FastU lenChunkMSTR();
            Core::FastU lenChunkSARY();
            Core::FastU lenChunkSFLG();
            Core::FastU lenChunkSNAM();
            Core::FastU lenChunkSPTR();
            Core::FastU lenChunkSTRL();
            Core::FastU lenChunkSVCT();

            Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU w);
            Core::FastU lenPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            Core::FastU lenPushIdx(IR::Arg const &arg, Core::FastU w);

            std::size_t lenString(Core::String str);

            virtual void preFunc();

            virtual void preObj();

            virtual void preStmnt();
            void preStmnt_AdXU_W();
            void preStmnt_AddF_W();
            void preStmnt_AddU_W();
            void preStmnt_Bclz_W(bool ones);
            void preStmnt_CmpF_EQ_W();
            void preStmnt_CmpF_GE_W();
            void preStmnt_CmpF_GT_W();
            void preStmnt_CmpF_LE_W();
            void preStmnt_CmpF_LT_W();
            void preStmnt_CmpF_NE_W();
            void preStmnt_CmpI_GE_W();
            void preStmnt_CmpI_GT_W();
            void preStmnt_CmpI_LE_W();
            void preStmnt_CmpI_LT_W();
            void preStmnt_CmpU_EQ_W(IR::Code codeCmp);
            void preStmnt_CmpU_GE_W();
            void preStmnt_CmpU_GT_W();
            void preStmnt_CmpU_LE_W();
            void preStmnt_CmpU_LT_W();
            void preStmnt_CmpU_W1(IR::Code code, IR::Code codeCmp, bool resGT, bool resLT);
            void preStmnt_DiXI_W();
            void preStmnt_DiXU_W();
            void preStmnt_DiXU_W1();
            void preStmnt_DivF_W();
            void preStmnt_DivX_W(IR::Code code);
            void preStmnt_MuXU_W();
            void preStmnt_MuXU_W1();
            void preStmnt_MulF_W();
            void preStmnt_MulK_W();
            void preStmnt_MulU_W();
            void preStmnt_MulX_W();
            void preStmnt_Retn();
            void preStmnt_ShLF_W();
            void preStmnt_ShLU_W();
            void preStmnt_SuXU_W();
            void preStmnt_SubF_W();
            void preStmnt_SubU_W();

            void preStmntCall(Core::FastU retrn, Core::FastU param);
            void preStmntCall(Core::String name, Core::FastU retrn, Core::FastU param);

            IR::Function *preStmntCallDef(Core::String name, Core::FastU retrn,
               Core::FastU param, Core::FastU localReg, char const *file,
               std::size_t line);

            virtual void preStr();

            virtual void put();

            virtual void putExtra();

            void putByte(Core::FastU i);

            void putChunk();
            void putChunk(char const *name, Core::Array<Core::String> const &strs, bool junk);
            void putChunkAIMP();
            void putChunkAINI();
            void putChunkARAY();
            void putChunkASTR();
            void putChunkATAG();
            void putChunkCODE();
            void putChunkFARY();
            void putChunkFNAM();
            void putChunkFUNC();
            void putChunkJUMP();
            void putChunkLOAD();
            void putChunkMEXP();
            void putChunkMIMP();
            void putChunkMINI();
            void putChunkMSTR();
            void putChunkSARY();
            void putChunkSFLG();
            void putChunkSNAM();
            void putChunkSPTR();
            void putChunkSTRL();
            void putChunkSVCT();

            void putCode(Code code);
            void putCode(Code code, Core::FastU arg0);
            void putCode(Code code, Core::FastU arg0, Core::FastU);

            using InfoBase::putFunc;
            virtual void putFunc();

            void putHWord(Core::FastU i);

            void putIniti();
            void putInitiSpace(IR::Space &space, Code code);

            virtual void putStmnt();
            void putStmnt_AdXU_W();
            void putStmnt_AddU_W();
            void putStmnt_BNot_W();
            void putStmnt_Bclz_W(bool ones = false);
            void putStmnt_Bges_W();
            void putStmnt_Bget_W();
            void putStmnt_Bset_W();
            void putStmnt_Call();
            void putStmnt_Casm();
            void putStmnt_CmpI_W(Code code);
            void putStmnt_CmpU_EQ_W();
            void putStmnt_CmpU_NE_W();
            void putStmnt_Cnat();
            void putStmnt_Cscr_IA();
            void putStmnt_Cscr_IS();
            void putStmnt_Cscr_SA();
            void putStmnt_Cscr_SS();
            void putStmnt_Cspe();
            void putStmnt_DiXI_W();
            void putStmnt_DivI_W();
            void putStmnt_DivU_W();
            void putStmnt_DivX_W();
            void putStmnt_Jcnd_Nil();
            void putStmnt_Jcnd_Tab();
            void putStmnt_Jcnd_Tru();
            void putStmnt_Jfar();
            void putStmnt_Jset();
            void putStmnt_Jump();
            void putStmnt_LNot();
            void putStmnt_ModI_W();
            void putStmnt_ModU_W();
            void putStmnt_Move_W();
            void putStmnt_Move_W__Arr_Stk(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_Move_W__Stk_Arr(IR::ArgPtr2 const &arr, Code code);
            void putStmnt_NegF_W();
            void putStmnt_NegI_W();
            void putStmnt_Pltn();
            void putStmnt_Retn();
            void putStmnt_ShLF_W();
            void putStmnt_ShLU_W();
            void putStmnt_ShRI_W();
            void putStmnt_ShRU_W();
            void putStmnt_ShRU_W1();
            void putStmnt_SuXU_W();
            void putStmnt_SubU_W();
            void putStmnt_Swap_W();
            void putStmnt_Xcod_SID();

            void putStmntBitwise(Code code);

            void putStmntCall(Core::FastU ret);
            void putStmntCall(Core::String name, Core::FastU ret);

            void putStmntDropArg(IR::Arg const &arg, Core::FastU w);
            void putStmntDropArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntDropRetn(Core::FastU ret);

            void putStmntIncUArg(IR::Arg const &arg, Core::FastU w);
            void putStmntIncUArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntPushArg(IR::Arg const &arg, Core::FastU w);
            void putStmntPushArg(IR::Arg const &arg, Core::FastU lo, Core::FastU hi);

            void putStmntPushFunct(Core::FastU value);

            void putStmntPushIdx(IR::Arg const &arg, Core::FastU w);

            void putStmntPushRetn(Core::FastU ret);

            void putStmntPushStrEn(Core::FastU value);

            void putStmntShiftRU(Core::FastU shift);

            void putString(Core::String str);
            void putString(Core::String str, Core::FastU key);

            void putWord(Core::FastU i);

            Core::CounterRef<IR::Exp const> resolveGlyph(Core::String glyph);

            virtual void trFunc();

            virtual void trStmnt();
            void trStmnt_AdXU_W();
            void trStmnt_AddU_W();
            void trStmnt_BNot_W();
            void trStmnt_Bclz_W();
            void trStmnt_Bget_W();
            void trStmnt_Bset_W();
            void trStmnt_Call();
            void trStmnt_Casm();
            void trStmnt_CmpU_EQ_W();
            void trStmnt_Cnat();
            void trStmnt_Cscr_IA();
            void trStmnt_Cscr_IS();
            void trStmnt_Cscr_SA();
            void trStmnt_Cscr_SS();
            void trStmnt_Cspe();
            void trStmnt_DiXI_W();
            void trStmnt_DiXU_W();
            void trStmnt_Jcnd_Nil();
            void trStmnt_Jcnd_Tab();
            void trStmnt_Jcnd_Tru();
            void trStmnt_Jfar();
            void trStmnt_Jset();
            void trStmnt_Jump();
            void trStmnt_ModI_W();
            void trStmnt_ModU_W();
            void trStmnt_Move_W();
            void trStmnt_NegI_W();
            void trStmnt_Retn();
            void trStmnt_ShLF_W();
            void trStmnt_ShLU_W();
            void trStmnt_ShRI_W();
            void trStmnt_ShRU_W();
            void trStmnt_ShRU_W1();
            void trStmnt_SuXU_W();
            void trStmnt_SubU_W();
            void trStmnt_Swap_W();

            void trStmntBitwise();

            std::unique_ptr<Core::NumberAllocMerge<Core::FastU>> allocDJump;
            std::unordered_map<IR::CallType, Core::NumberAllocMerge<Core::FastU>> allocFunc;
            std::unordered_map<IR::AddrSpace, Core::NumberAllocMerge<Core::FastU>> allocObj;
            std::unordered_map<IR::AddrBase, Core::NumberAllocMerge<Core::FastU>> allocSpace;
            std::unique_ptr<Core::NumberAllocMerge<Core::FastU>> allocStrEnt;

            std::unordered_map<IR::Space const *, InitData> init;

            Core::FastU codeInit;
            Core::FastU codeInitEnd;

            Core::FastU numChunkAIMP;
            Core::FastU numChunkAINI;
            Core::FastU numChunkARAY;
            Core::FastU numChunkASTR;
            Core::FastU numChunkATAG;
            Core::FastU numChunkCODE;
            Core::FastU numChunkFNAM;
            Core::FastU numChunkFUNC;
            Core::FastU numChunkJUMP;
            Core::FastU numChunkLOAD;
            Core::FastU numChunkMEXP;
            Core::FastU numChunkMIMP;
            Core::FastU numChunkMINI;
            Core::FastU numChunkMSTR;
            Core::FastU numChunkSFLG;
            Core::FastU numChunkSNAM;
            Core::FastU numChunkSPTR;
            Core::FastU numChunkSTRL;
            Core::FastU numChunkSVCT;


            static void CheckArg(IR::Arg const &arg, Core::Origin pos);

            static Core::FastU CodeBase();

            static Core::FastU GetParamMax(IR::CallType call);

            static Core::FastU GetScriptValue(IR::Function const &script);

            static bool IsNull(IR::Value_Funct const &val);
            static bool IsNull(IR::Value_StrEn const &val);

            static bool IsNull_Funct(Core::FastU val);
            static bool IsNull_StrEn(Core::FastU val);

            static bool IsScript(IR::CallType ctype);
            static bool IsScriptS(IR::CallType ctype);
         };
      }
   }
}

#endif//GDCC__BC__ZDACS__Info_H__

