/*
 * refit/icns.c
 * Loader for .icns icon files
 *
 * Copyright (c) 2006-2007 Christoph Pfisterer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *  * Neither the name of Christoph Pfisterer nor the names of the
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Platform.h"
#include "../include/egemb_icons.h"

#ifndef DEBUG_ALL
#define DEBUG_ICNS 1
#else
#define DEBUG_ICNS DEBUG_ALL
#endif

#if DEBUG_ICNS == 0
#define DBG(...)
#else
#define DBG(...) DebugLog(DEBUG_ICNS, __VA_ARGS__)
#endif

#if defined(ADVICON)

//
// well-known icons
//

BUILTIN_ICON BuiltinIconTable[BUILTIN_ICON_COUNT] = {
  { NULL, L"icons\\func_about"             , L"png",  /*48*/32 },
  { NULL, L"icons\\func_options"           , L"png",  /*48*/32 },
  { NULL, L"icons\\func_clover"            , L"png",  /*48*/32 },
  { NULL, L"icons\\func_secureboot"        , L"png",  /*48*/32 },
  { NULL, L"icons\\func_secureboot_config" , L"png",  /*48*/32 },
  { NULL, L"icons\\func_reset"             , L"png",  /*48*/32 },
  { NULL, L"icons\\func_shutdown"          , L"png",  /*48*/32 },
  { NULL, L"icons\\func_exit"              , L"png",  /*48*/32 },
  { NULL, L"icons\\func_help"              , L"png",  /*48*/32 },
  { NULL, L"icons\\tool_shell"             , L"png",  /*48*/32 },
  { NULL, L"icons\\tool_part"              , L"png",  /*48*/32 },
  { NULL, L"icons\\tool_rescue"            , L"png",  /*48*/32 },
  { NULL, L"icons\\pointer"                , L"png",  /*48*/32 },//12

  { NULL, L"icons\\vol_internal"           , L"icns", 128 },
  { NULL, L"icons\\vol_external"           , L"icns", 128 },
  { NULL, L"icons\\vol_optical"            , L"icns", 128 },
  { NULL, L"icons\\vol_firewire"           , L"icns", 128 },
  { NULL, L"icons\\vol_clover"             , L"icns", 128 },
  { NULL, L"icons\\vol_internal_hfs"       , L"icns", 128 },
  { NULL, L"icons\\vol_internal_ntfs"      , L"icns", 128 },
  { NULL, L"icons\\vol_internal_ext3"      , L"icns", 128 },
  { NULL, L"icons\\vol_recovery"           , L"icns", 128 },//21

  { NULL, L"logo"                          , L"png",  128 },
  { NULL, L"selection_small"               , L"png",  64  },
  { NULL, L"selection_big"                 , L"png",  144 }
};

#define DEC_BUILTIN_ICON(id, ico) BuiltinIconTable[id].Image = egDecodePNG(ico, sizeof(ico), BuiltinIconTable[id].PixelSize, TRUE)

CHAR16 * GetIconsExt(IN CHAR16 *Icon, IN CHAR16 *Def)
{
  return PoolPrint(L"%s.%s", Icon, ((GlobalConfig.IconFormat != ICON_FORMAT_DEF) && (IconFormat != NULL)) ? IconFormat : Def);
}

EG_IMAGE * GetSmallHover(IN UINTN Id)
{
  CHAR16 *Path = AllocateZeroPool(256);
  Path = GetIconsExt(PoolPrint(L"%s_hover", BuiltinIconTable[Id].Path), BuiltinIconTable[Id].Format);
  return egLoadIcon(ThemeDir, Path, BuiltinIconTable[Id].PixelSize);
}

EG_IMAGE * BuiltinIcon(IN UINTN Id)
{
  INTN      Size;
  EG_IMAGE  *TextBuffer = NULL;
  CHAR16    *p;
  CHAR16    *Text;
  CHAR16    *Path = AllocateZeroPool(256);

  if (Id >= BUILTIN_ICON_COUNT) {
    return NULL;
  }

  if (BuiltinIconTable[Id].Image != NULL) {
    return BuiltinIconTable[Id].Image;
  }

  Size = BuiltinIconTable[Id].PixelSize;
  //DBG("Load Icon [id:%d]");

  //if (BuiltinIconTable[Id].Image == NULL) {
    if (ThemeDir) {
      Path = GetIconsExt(BuiltinIconTable[Id].Path, BuiltinIconTable[Id].Format);
      BuiltinIconTable[Id].Image = LoadIcnsFallback(ThemeDir, Path, Size);
      if (!BuiltinIconTable[Id].Image) {
        DebugLog(1, "Icon %d (%s) not found (path: %s, ThemeDir: %p)\n", Id, Path, ThemePath, ThemeDir);
        if (Id >= BUILTIN_ICON_VOL_INTERNAL) {
          Path = GetIconsExt(BuiltinIconTable[BUILTIN_ICON_VOL_INTERNAL].Path, BuiltinIconTable[BUILTIN_ICON_VOL_INTERNAL].Format);
          BuiltinIconTable[Id].Image = LoadIcnsFallback(ThemeDir, Path, Size);
        }
      }
    }

    if (BuiltinIconTable[Id].Image) {
      FreePool(Path);
      return BuiltinIconTable[Id].Image;
    }

    switch (Id) {
      case BUILTIN_ICON_POINTER:
        DEC_BUILTIN_ICON(Id, emb_pointer); break;
      case BUILTIN_ICON_FUNC_ABOUT:
        DEC_BUILTIN_ICON(Id, emb_func_about); break;
      case BUILTIN_ICON_FUNC_OPTIONS:
        DEC_BUILTIN_ICON(Id, emb_func_options); break;
      case BUILTIN_ICON_FUNC_CLOVER:
        DEC_BUILTIN_ICON(Id, emb_func_clover); break;
      case BUILTIN_ICON_FUNC_SECURE_BOOT:
        DEC_BUILTIN_ICON(Id, emb_func_secureboot); break;
      case BUILTIN_ICON_FUNC_SECURE_BOOT_CONFIG:
        DEC_BUILTIN_ICON(Id, emb_func_secureboot_config); break;
      case BUILTIN_ICON_FUNC_RESET:
        DEC_BUILTIN_ICON(Id, emb_func_reset); break;
      case BUILTIN_ICON_FUNC_SHUTDOWN:
        DEC_BUILTIN_ICON(Id, emb_func_shutdown); break;
      case BUILTIN_ICON_FUNC_EXIT:
        DEC_BUILTIN_ICON(Id, emb_func_exit); break;
      case BUILTIN_ICON_FUNC_HELP:
        DEC_BUILTIN_ICON(Id, emb_func_help); break;
      case BUILTIN_ICON_TOOL_SHELL:
        DEC_BUILTIN_ICON(Id, emb_func_shell); break;
      case BUILTIN_ICON_VOL_INTERNAL:
        DEC_BUILTIN_ICON(Id, emb_vol_internal); break;
      case BUILTIN_ICON_VOL_EXTERNAL:
        DEC_BUILTIN_ICON(Id, emb_vol_internal); break;
      case BUILTIN_ICON_VOL_BOOTER:
        DEC_BUILTIN_ICON(Id, emb_vol_internal_booter); break;
      case BUILTIN_ICON_VOL_INTERNAL_HFS:
        DEC_BUILTIN_ICON(Id, emb_vol_internal_hfs); break;
      case BUILTIN_ICON_VOL_INTERNAL_NTFS:
        DEC_BUILTIN_ICON(Id, emb_vol_internal_ntfs); break;
      case BUILTIN_ICON_VOL_INTERNAL_EXT3:
        DEC_BUILTIN_ICON(Id, emb_vol_internal_ext); break;
      case BUILTIN_ICON_VOL_INTERNAL_REC:
        DEC_BUILTIN_ICON(Id, emb_vol_internal_recovery); break;
      case BUILTIN_ICON_BANNER:
        DEC_BUILTIN_ICON(Id, emb_logo); break;
      case BUILTIN_SELECTION_SMALL:
        DEC_BUILTIN_ICON(Id, emb_selection_small); break;
      case BUILTIN_SELECTION_BIG:
        DEC_BUILTIN_ICON(Id, emb_selection_big); break;
    }
  //}

  if (!BuiltinIconTable[Id].Image) {
    TextBuffer = egCreateImage(Size, Size, TRUE);
    egFillImage(TextBuffer, &MenuBackgroundPixel);
    p = StrStr(BuiltinIconTable[Id].Path, L"_"); p++;
    Text = (CHAR16*)AllocateCopyPool(30, (VOID*)p);
    p = StrStr(Text, L".");
    *p = L'\0';
    //if (StrCmp(Text, L"shutdown") == 0) {
    //if (StrCmp(Text, L"exit") == 0) {
    //   // icon name is shutdown from historic reasons, but function is now exit
    //   UnicodeSPrint(Text, 30, L"exit");
    //}
    egRenderText(Text, TextBuffer, 0, 0, 0xFFFF);
    BuiltinIconTable[Id].Image = TextBuffer;
    DebugLog(1, "Icon %d: Text <%s> rendered\n", Id, Text);
    FreePool(Text);
  }

  return BuiltinIconTable[Id].Image;
}

//
// Load an icon for an operating system
//

EG_IMAGE * LoadOSIcon(IN OUT CHAR16 *OSIconName OPTIONAL, IN CHAR16 *FallbackIconName, IN UINTN PixelSize, IN BOOLEAN BootLogo, IN BOOLEAN WantDummy)
{
  EG_IMAGE        *Image;
  CHAR16          CutoutName[16]/*, FirstName[16]*/, TmpName[64];
  CHAR16          FileName[256];
  UINTN           StartIndex, Index, NextIndex;

  if (GlobalConfig.TextOnly)      // skip loading if it's not used anyway
    return NULL;
  Image = NULL;
  //*FirstName = 0;

  // try the names from OSIconName
  for (StartIndex = 0; OSIconName != NULL && OSIconName[StartIndex]; StartIndex = NextIndex) {
    // find the next name in the list
    NextIndex = 0;
    for (Index = StartIndex; OSIconName[Index]; Index++) {
      if (OSIconName[Index] == ',') {
        NextIndex = Index + 1;
        break;
      }
    }
    if (OSIconName[Index] == 0)
      NextIndex = Index;

    // construct full path
    if (Index > StartIndex + 15)   // prevent buffer overflow
      continue;
    CopyMem(CutoutName, OSIconName + StartIndex, (Index - StartIndex) * sizeof(CHAR16));
    CutoutName[Index - StartIndex] = 0;
    UnicodeSPrint(TmpName, 64, L"%s_%s",
                  BootLogo ? L"boot" : L"os", CutoutName);
    UnicodeSPrint(FileName, 512, GetIconsExt(PoolPrint(L"icons\\%s", TmpName), L"icns"));

    // try to load it
    Image = egLoadIcon(ThemeDir, FileName, PixelSize);
    if (Image != NULL) {
      UnicodeSPrint(OSIconName, 64, L"%s_hover", TmpName);
      return Image;
    }

    //if (*FirstName == '\0') {
    //  CopyMem(FirstName, CutoutName, StrSize(CutoutName));
    //  if ('a' <= FirstName[0] && FirstName[0] <= 'z') {
    //    FirstName[0] = (CHAR16) (FirstName[0] - 0x20);
    //  }
    //}
  }

  // try the fallback name
  UnicodeSPrint(TmpName, 64, L"%s_%s",
                BootLogo ? L"boot" : L"os", FallbackIconName);
  UnicodeSPrint(FileName, 512, GetIconsExt(PoolPrint(L"icons\\%s", TmpName), L"icns"));
  Image = egLoadIcon(ThemeDir, FileName, PixelSize);
  if (Image != NULL) {
    UnicodeSPrint(OSIconName, 64, L"%s_hover", TmpName);
    return Image;
  }

  // try the fallback name with os_ instead of boot_
  if (BootLogo) {
    Image = LoadOSIcon(NULL, FallbackIconName, PixelSize, FALSE, WantDummy);
    if (Image != NULL)
      return Image;
  }

  if (!WantDummy) {
    return NULL;
  }

  if (!GlobalConfig.Theme) { // embedded theme - return rendered icon name
    EG_IMAGE  *TextBuffer = egCreateImage(PixelSize, PixelSize, TRUE);
    egFillImage(TextBuffer, &MenuBackgroundPixel);
//    egRenderText(FirstName, TextBuffer, PixelSize/4, PixelSize/3, 0xFFFF);
//    DebugLog(1, "Text <%s> rendered\n", FirstName);
    return TextBuffer;
  }

  return DummyImage(PixelSize);
}

EG_IMAGE * LoadHoverIcon(IN CHAR16 *OSIconName, IN UINTN PixelSize)
{
  EG_IMAGE        *Image = NULL;

  if (GlobalConfig.TextOnly)      // skip loading if it's not used anyway
    return NULL;

  Image = egLoadIcon(ThemeDir, OSIconName, PixelSize);

  return Image;
}
#else //ADVICON
//
// well-known icons
//
BUILTIN_ICON BuiltinIconTable[BUILTIN_ICON_COUNT] = {
  { NULL, L"icons\\func_about.png",              48 },
  { NULL, L"icons\\func_options.png",            48 },
  { NULL, L"icons\\func_clover.png",             48 },
  { NULL, L"icons\\func_secureboot.png",         48 },
  { NULL, L"icons\\func_secureboot_config.png",  48 },
  { NULL, L"icons\\func_reset.png",              48 },
  { NULL, L"icons\\func_shutdown.png",           48 },
  { NULL, L"icons\\func_exit",                   48 },
  { NULL, L"icons\\func_help.png",               48 }, //7
  { NULL, L"icons\\tool_shell.png",              48 },
  { NULL, L"icons\\tool_part.png",               48 },
  { NULL, L"icons\\tool_rescue.png",             48 },
  { NULL, L"icons\\pointer.png",                 32 },

  { NULL, L"icons\\vol_internal.icns",          128 }, //13
  { NULL, L"icons\\vol_external.icns",          128 },
  { NULL, L"icons\\vol_optical.icns",           128 },
  { NULL, L"icons\\vol_firewire.icns",          128 },
  { NULL, L"icons\\vol_clover.icns",            128 },
  { NULL, L"icons\\vol_internal_hfs.icns",      128 },
  { NULL, L"icons\\vol_internal_ntfs.icns",     128 },
  { NULL, L"icons\\vol_internal_ext3.icns",     128 },
  { NULL, L"icons\\vol_recovery.icns",          128 }, //21

  { NULL, L"logo.png",                          128 },
  { NULL, L"selection_small.png",               64 },
  { NULL, L"selection_big.png",                 144 }
};

EG_IMAGE * BuiltinIcon(IN UINTN Id)
{
  INTN      Size;
  EG_IMAGE  *TextBuffer = NULL;
  CHAR16    *p;
  CHAR16    *Text;

  if (Id >= BUILTIN_ICON_COUNT) {
    return NULL;
  }

  Size = BuiltinIconTable[Id].PixelSize;
  if (BuiltinIconTable[Id].Image == NULL) {
    if (ThemeDir) {
      BuiltinIconTable[Id].Image = LoadIcnsFallback(ThemeDir, BuiltinIconTable[Id].Path, Size);
      if (!BuiltinIconTable[Id].Image){
        DebugLog(1, "  Icon %d (%s) not found\n", Id, BuiltinIconTable[Id].Path);
        DebugLog(1, "  Theme path %s, ThemeDir=%p\n", ThemePath, ThemeDir);
      }
      if (!BuiltinIconTable[Id].Image && (Id >= BUILTIN_ICON_VOL_INTERNAL)) {
        BuiltinIconTable[Id].Image = LoadIcnsFallback(ThemeDir, BuiltinIconTable[BUILTIN_ICON_VOL_INTERNAL].Path, Size);
      }
    }
    if (!BuiltinIconTable[Id].Image) {
      if (Id == BUILTIN_ICON_POINTER) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_pointer[0], sizeof(emb_pointer), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_ABOUT) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_about[0], sizeof(emb_func_about), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_OPTIONS) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_options[0], sizeof(emb_func_options), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_CLOVER) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_clover[0], sizeof(emb_func_clover), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_SECURE_BOOT) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_secureboot[0], sizeof(emb_func_secureboot), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_SECURE_BOOT_CONFIG) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_secureboot_config[0], sizeof(emb_func_secureboot_config), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_RESET) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_reset[0], sizeof(emb_func_reset), 32, TRUE);
      } else if (Id == BUILTIN_ICON_FUNC_SHUTDOWN) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_shutdown[0], sizeof(emb_func_shutdown), 32, TRUE);
      } else if (Id == BUILTIN_ICON_TOOL_SHELL) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_func_shell[0], sizeof(emb_func_shell), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_INTERNAL) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal[0], sizeof(emb_vol_internal), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_EXTERNAL) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal[0], sizeof(emb_vol_internal), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_BOOTER) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal_booter[0], sizeof(emb_vol_internal_booter), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_INTERNAL_HFS) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal_hfs[0], sizeof(emb_vol_internal_hfs), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_INTERNAL_NTFS) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal_ntfs[0], sizeof(emb_vol_internal_ntfs), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_INTERNAL_EXT3) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal_ext[0], sizeof(emb_vol_internal_ext), 32, TRUE);
      } else if (Id == BUILTIN_ICON_VOL_INTERNAL_REC) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_vol_internal_recovery[0], sizeof(emb_vol_internal_recovery), 32, TRUE);
      } else if (Id == BUILTIN_ICON_BANNER) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_logo[0], sizeof(emb_logo), 32, TRUE);
      } else if (Id == BUILTIN_SELECTION_SMALL) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_selection_small[0], sizeof(emb_selection_small), 32, TRUE);
      } else if (Id == BUILTIN_SELECTION_BIG) {
        BuiltinIconTable[Id].Image = egDecodePNG(&emb_selection_big[0], sizeof(emb_selection_big), 32, TRUE);
      }
    }
  }
  if (!BuiltinIconTable[Id].Image) {
    TextBuffer = egCreateImage(Size, Size, TRUE);
    egFillImage(TextBuffer, &MenuBackgroundPixel);
    p = StrStr(BuiltinIconTable[Id].Path, L"_"); p++;
    Text = (CHAR16*)AllocateCopyPool(30, (VOID*)p);
    p = StrStr(Text, L".");
    *p = L'\0';
     if (StrCmp(Text, L"shutdown") == 0) {
       // icon name is shutdown from historic reasons, but function is now exit
       UnicodeSPrint(Text, 30, L"exit");
    }
    egRenderText(Text, TextBuffer, 0, 0, 0xFFFF);
    BuiltinIconTable[Id].Image = TextBuffer;
    DebugLog(1, "Text <%s> rendered\n", Text);
    FreePool(Text);
  }

  return BuiltinIconTable[Id].Image;
}

//
// Load an icon for an operating system
//

EG_IMAGE * LoadOSIcon(IN CHAR16 *OSIconName OPTIONAL, IN CHAR16 *FallbackIconName, IN UINTN PixelSize, IN BOOLEAN BootLogo, IN BOOLEAN WantDummy)
{
  EG_IMAGE        *Image;
  CHAR16          CutoutName[16], FirstName[16];
  CHAR16          FileName[256];
  UINTN           StartIndex, Index, NextIndex;

  if (GlobalConfig.TextOnly)      // skip loading if it's not used anyway
    return NULL;
  Image = NULL;
  *FirstName = 0;

  // try the names from OSIconName
  for (StartIndex = 0; OSIconName != NULL && OSIconName[StartIndex]; StartIndex = NextIndex) {
    // find the next name in the list
    NextIndex = 0;
    for (Index = StartIndex; OSIconName[Index]; Index++) {
      if (OSIconName[Index] == ',') {
        NextIndex = Index + 1;
        break;
      }
    }
    if (OSIconName[Index] == 0)
      NextIndex = Index;

    // construct full path
    if (Index > StartIndex + 15)   // prevent buffer overflow
      continue;
    CopyMem(CutoutName, OSIconName + StartIndex, (Index - StartIndex) * sizeof(CHAR16));
    CutoutName[Index - StartIndex] = 0;
    UnicodeSPrint(FileName, 512, L"icons\\%s_%s.icns",
                  BootLogo ? L"boot" : L"os", CutoutName);

    // try to load it
    Image = egLoadIcon(ThemeDir, FileName, PixelSize);
    if (Image != NULL)
      return Image;

    if (*FirstName == '\0') {
      CopyMem(FirstName, CutoutName, StrSize(CutoutName));
      if ('a' <= FirstName[0] && FirstName[0] <= 'z') {
        FirstName[0] = (CHAR16) (FirstName[0] - 0x20);
      }
    }
  }

  // try the fallback name
  UnicodeSPrint(FileName, 512, L"icons\\%s_%s.icns",
                BootLogo ? L"boot" : L"os", FallbackIconName);
  Image = egLoadIcon(ThemeDir, FileName, PixelSize);
  if (Image != NULL)
    return Image;

  // try the fallback name with os_ instead of boot_
  if (BootLogo) {
    Image = LoadOSIcon(NULL, FallbackIconName, PixelSize, FALSE, WantDummy);
    if (Image != NULL)
      return Image;
  }

  if (!WantDummy) {
    return NULL;
  }

  if (!GlobalConfig.Theme) { // embedded theme - return rendered icon name
    EG_IMAGE  *TextBuffer = egCreateImage(PixelSize, PixelSize, TRUE);
    egFillImage(TextBuffer, &MenuBackgroundPixel);
//    egRenderText(FirstName, TextBuffer, PixelSize/4, PixelSize/3, 0xFFFF);
//    DebugLog(1, "Text <%s> rendered\n", FirstName);
    return TextBuffer;
  }

  return DummyImage(PixelSize);
}
#endif //ADVICON

//
// Load an image from a .icns file
//

EG_IMAGE * LoadIcns(IN EFI_FILE_HANDLE BaseDir, IN CHAR16 *FileName, IN UINTN PixelSize)
{
    if (GlobalConfig.TextOnly)      // skip loading if it's not used anyway
        return NULL;
  if (BaseDir) {
    return egLoadIcon(BaseDir, FileName, PixelSize);
  }
  return DummyImage(PixelSize);
}

static EG_PIXEL BlackPixel  = { 0x00, 0x00, 0x00, 0 };
//static EG_PIXEL YellowPixel = { 0x00, 0xff, 0xff, 0 };

EG_IMAGE * DummyImage(IN UINTN PixelSize)
{
    EG_IMAGE        *Image;
    UINTN           x, y, LineOffset;
    CHAR8           *Ptr, *YPtr;

    Image = egCreateFilledImage(PixelSize, PixelSize, TRUE, &BlackPixel);

    LineOffset = PixelSize * 4;

    YPtr = (CHAR8 *)Image->PixelData + ((PixelSize - 32) >> 1) * (LineOffset + 4);
    for (y = 0; y < 32; y++) {
        Ptr = YPtr;
        for (x = 0; x < 32; x++) {
            if (((x + y) % 12) < 6) {
                *Ptr++ = 0;
                *Ptr++ = 0;
                *Ptr++ = 0;
            } else {
                *Ptr++ = 0;
                *Ptr++ = 255; //yellow
                *Ptr++ = 255;
            }
            *Ptr++ = 144;
        }
        YPtr += LineOffset;
    }

    return Image;
}

EG_IMAGE * LoadIcnsFallback(IN EFI_FILE_HANDLE BaseDir, IN CHAR16 *FileName, IN UINTN PixelSize)
{
    EG_IMAGE *Image;

    if (GlobalConfig.TextOnly)      // skip loading if it's not used anyway
        return NULL;

    Image = egLoadIcon(BaseDir, FileName, PixelSize);
//    if (Image == NULL)
//        Image = DummyImage(PixelSize);
    return Image;
}
