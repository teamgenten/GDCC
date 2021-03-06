//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Path string manipulation utilities.
//
//-----------------------------------------------------------------------------

#include "Core/Path.hpp"

#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace Core
   {
      //
      // GetSystemPath
      //
      std::string const &GetSystemPath()
      {
         static std::string path;

         if(path.empty())
         {
            #ifdef _WIN32
            TCHAR buffer[MAX_PATH+1];
            DWORD size = MAX_PATH+1;
            DWORD len  = GetModuleFileName(NULL, buffer, size);

            // 0 means failure, size means buffer too small.
            if(len == 0 || len == size)
               return path;

            path = {buffer, len};
            Core::PathDirnameEq(path);
            Core::PathNormalizeEq(path);
            #else
            path = "/usr/share/gdcc";
            #endif
         }

         return path;
      }

      //
      // PathAppend
      //
      std::string &PathAppend(std::string &l, String r)
      {
         if(l.empty()) return l = {r.data(), r.size()};

         PathTerminateEq(l);

         if(!r.empty())
         {
            if(IsPathSep(r.front()))
               l.append(r.data() + 1, r.size() - 1);
            else
               l.append(r.data(), r.size());
         }

         return l;
      }

      //
      // PathAppend
      //
      std::string &PathAppend(std::string &l, char const *r)
      {
         if(l.empty()) return l = r;

         return PathTerminateEq(l).append(IsPathSep(*r) ? r + 1 : r);
      }

      //
      // PathConcat
      //
      String PathConcat(char const *l, String r)
      {
         std::string tmp{l};
         PathAppend(tmp, r);
         return {tmp.data(), tmp.size()};
      }

      //
      // PathConcat
      //
      String PathConcat(String l, String r)
      {
         if(l.empty()) return r;
         if(r.empty()) return l;

         if(IsPathSep(l.back()))
         {
            if(!IsPathSep(r.front()))
               return l + r;

            std::string tmp{l.data(), l.size() - 1};
            tmp.append(r.data(), r.size());

            return {tmp.data(), tmp.size()};
         }
         else
         {
            if(IsPathSep(r.front()))
               return l + r;

            std::string tmp{l.data(), l.size()};
            tmp += PathSep(l.data());
            tmp.append(r.data(), r.size());

            return {tmp.data(), tmp.size()};
         }
      }

      //
      // PathDirname
      //
      String PathDirname(String path)
      {
         char const *itr = path.end();

         for(;;)
         {
            if(itr == path.data()) return STR_;
            if(IsPathSep(*--itr)) break;
         }

         std::size_t len = itr - path.data();
         return {path.data(), len};
      }

      //
      // PathDirnameEq
      //
      std::string &PathDirnameEq(std::string &path)
      {
         auto itr = path.end();

         for(;;)
         {
            if(itr == path.begin()) return path.clear(), path;
            if(IsPathSep(*--itr)) break;
         }

         path.erase(itr, path.end());
         return path;
      }

      //
      // PathFilename
      //
      char const *PathFilename(char const *path)
      {
         char const *filename = path;
         for(; *path; ++path)
         {
            if(IsPathSep(*path))
               filename = path + 1;
         }

         return filename;
      }

      //
      // PathNormalize
      //
      String PathNormalize(String path)
      {
         #ifdef _WIN32
         char const sep = PathSep(path);
         for(char c : path) if(IsPathSep(c) && c != sep)
         {
            std::unique_ptr<char[]> tmp{new char[path.size() + 1]};
            std::memcpy(tmp.get(), path.data(), path.size() + 1);
            PathNormalize(tmp.get());

            return {tmp.get(), path.size()};
         }
         #endif

         return path;
      }

      //
      // PathNormalizeEq
      //
      char *PathNormalizeEq(char *path)
      {
         #ifdef _WIN32
         char const sep = PathSep(path);
         for(auto itr = path; *itr; ++itr)
            if(IsPathSep(*itr)) *itr = sep;
         #endif

         return path;
      }

      //
      // PathNormalizeEq
      //
      std::string &PathNormalizeEq(std::string &path)
      {
         #ifdef _WIN32
         char const sep = PathSep(path.data());
         for(auto &c : path)
            if(IsPathSep(c)) c = sep;
         #endif

         return path;
      }

      //
      // PathTerminateEq
      //
      std::string &PathTerminateEq(std::string &path)
      {
         if(!path.empty() && !IsPathSep(path.back()))
            path += PathSep(path.data());

         return path;
      }
   }
}

// EOF

