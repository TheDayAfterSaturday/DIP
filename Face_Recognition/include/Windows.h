
/*++ BUILD Version: 0001    Increment this if a change has global effects

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:


    windows.h

Abstract:

    Master include file for Windows applications.

--*/

#ifndef _WINDOWS_
#define _WINDOWS_


#include <sdkddkver.h>

#ifndef _INC_WINDOWS
#define _INC_WINDOWS

// ADDED TO BY CVI
#ifdef _CVI_
#ifndef WINVER
#define WINVER			_WIN32_WINNT_WINXP
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT	_WIN32_WINNT_WINXP
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS	_WIN32_WINNT_WINXP
#endif
#ifndef NTDDI_VERSION
#define NTDDI_VERSION	NTDDI_WINXP
#endif
#ifndef ISOLATION_AWARE_ENABLED
#if (!defined(_LINK_CVI_LVRT_) && !defined(_INTERACTIVE_WINDOW_BUILD_))
#if (_CVI_RTE_SXS_FOR_ENTIRE_APP_ || _CVI_RTE_SXS_FOR_EXE_ONLY_ || _CVI_RTE_SXS_FOR_DLL_ONLY_) 
#define ISOLATION_AWARE_ENABLED 1
#endif
#endif
#endif

#if defined(INCLUDE_AFTER_WINDOWS_H)
#error  Because of name conflicts, Windows headers must be included before utility.h and formatio.h.
#error  Note that other common CVI headers, such as toolbox.h, toolbar.h, and inifile.h themselves include utility.h or formatio.h.
#error  Therefore, you must also include Windows headers before these other headers.
#endif /* INCLUDE_AFTER_WINDOWS_H */
#define CVI_LOCAL_INLINE
#if (_CVI_C99_EXTENSIONS_ == 1)
#define __inline        inline
#else
#define __inline        static
#endif
#if !defined(__clang__) && !defined(__int32)
#define __int32         int
#endif

#define _STDCALL_SUPPORTED
#define DISABLE_INLINE_ASSEMBLY
#define _INTEGRAL_MAX_BITS      64
#endif	/* _CVI_ */
// END ADDED TO BY CVI

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <driverspecs.h>


/*  If defined, the following flags inhibit definition
 *     of the indicated items.
 *
 *  NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
 *  NOVIRTUALKEYCODES - VK_*
 *  NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
 *  NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
 *  NOSYSMETRICS      - SM_*
 *  NOMENUS           - MF_*
 *  NOICONS           - IDI_*
 *  NOKEYSTATES       - MK_*
 *  NOSYSCOMMANDS     - SC_*
 *  NORASTEROPS       - Binary and Tertiary raster ops
 *  NOSHOWWINDOW      - SW_*
 *  OEMRESOURCE       - OEM Resource values
 *  NOATOM            - Atom Manager routines
 *  NOCLIPBOARD       - Clipboard routines
 *  NOCOLOR           - Screen colors
 *  NOCTLMGR          - Control and Dialog routines
 *  NODRAWTEXT        - DrawText() and DT_*
 *  NOGDI             - All GDI defines and routines
 *  NOKERNEL          - All KERNEL defines and routines
 *  NOUSER            - All USER defines and routines
 *  NONLS             - All NLS defines and routines
 *  NOMB              - MB_* and MessageBox()
 *  NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
 *  NOMETAFILE        - typedef METAFILEPICT
 *  NOMINMAX          - Macros min(a,b) and max(a,b)
 *  NOMSG             - typedef MSG and associated routines
 *  NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
 *  NOSCROLL          - SB_* and scrolling routines
 *  NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
 *  NOSOUND           - Sound driver routines
 *  NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
 *  NOWH              - SetWindowsHook and WH_*
 *  NOWINOFFSETS      - GWL_*, GCL_*, associated routines
 *  NOCOMM            - COMM driver routines
 *  NOKANJI           - Kanji support stuff.
 *  NOHELP            - Help engine interface.
 *  NOPROFILER        - Profiler interface.
 *  NODEFERWINDOWPOS  - DeferWindowPos routines
 *  NOMCX             - Modem Configuration Extensions
 */

#if defined(RC_INVOKED) && !defined(NOWINRES)

#include <winresrc.h>

#else

#if defined(RC_INVOKED)
/* Turn off a bunch of stuff to ensure that RC files compile OK. */
#define NOATOM
#define NOGDI
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOCRYPT
#define NOMCX
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_IX86)
#define _X86_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_AMD64)
#define _AMD64_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_M68K)
#define _68K_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && defined(_M_MPPC)
#define _MPPC_
#endif

#if !defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_M_IX86) && !defined(_AMD64_) && defined(_M_IA64)
#if !defined(_IA64_)
#define _IA64_
#endif /* !_IA64_ */
#endif

#ifndef _MAC
#if defined(_68K_) || defined(_MPPC_)
#define _MAC
#endif
#endif

#if defined (_MSC_VER)
#if ( _MSC_VER >= 800 )
#ifndef __cplusplus
#pragma warning(disable:4116)       /* TYPE_ALIGNMENT generates this - move it */
                                    /* outside the warning push/pop scope. */
#endif
#endif
#endif

#ifndef RC_INVOKED
#if     ( _MSC_VER >= 800 )
#pragma warning(disable:4514)
#ifndef __WINDOWS_DONT_DISABLE_PRAGMA_PACK_WARNING__
#pragma warning(disable:4103)
#endif
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4001)
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#endif
#ifndef _CVI_           // EXCLUDED BY CVI
#include <excpt.h>
#endif /* _CVI_ */
#include <stdarg.h>
#endif /* RC_INVOKED */

#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <winuser.h>
#if !defined(_MAC) || defined(_WIN32NLS)
#include <winnls.h>
#endif
#ifndef _MAC
#include <wincon.h>
#include <winver.h>
#endif
#if !defined(_MAC) || defined(_WIN32REG)
#include <winreg.h>
#endif
#ifndef _MAC
#include <winnetwk.h>
#endif

#ifndef WIN32_LEAN_AND_MEAN
#include <cderr.h>
#include <dde.h>
#include <ddeml.h>
#include <dlgs.h>
#ifndef _MAC
#include <lzexpand.h>
#include <mmsystem.h>
#include <nb30.h>
#include <rpc.h>
#endif
#include <shellapi.h>
#ifndef _MAC
#include <winperf.h>
#include <winsock.h>
#endif
#ifndef NOCRYPT
#include <wincrypt.h>
#include <winefs.h>
#include <winscard.h>
#endif

#ifndef NOGDI
#ifndef _MAC
#include <winspool.h>
#ifdef INC_OLE1
#include <ole.h>
#else
#include <ole2.h>
#endif /* !INC_OLE1 */
#endif /* !MAC */
#include <commdlg.h>
#endif /* !NOGDI */
#endif /* WIN32_LEAN_AND_MEAN */

#include <stralign.h>

#ifdef _MAC
#include <winwlm.h>
#endif


#ifdef INC_OLE2
#include <ole2.h>
#endif /* INC_OLE2 */

#ifndef _MAC
#ifndef NOSERVICE
#include <winsvc.h>
#endif

#if(WINVER >= 0x0400)
#ifndef NOMCX
#include <mcx.h>
#endif /* NOMCX */

#ifndef NOIME
#include <imm.h>
#endif
#endif /* WINVER >= 0x0400 */
#endif

#ifndef RC_INVOKED
#if     ( _MSC_VER >= 800 )
#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4001)
#pragma warning(default:4201)
#pragma warning(default:4214)
/* Leave 4514 disabled.  It's an unneeded warning anyway. */
#endif
#endif
#endif /* RC_INVOKED */

#endif /* RC_INVOKED */

// ADDED TO BY CVI
#ifdef CVI_LOCAL_INLINE
#undef CVI_LOCAL_INLINE
#undef __inline
#endif

#endif /* _INC_WINDOWS */
#endif /* _WINDOWS_ */


