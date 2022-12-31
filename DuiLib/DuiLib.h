#pragma once

#include "UIlib.h"
#include "Utils/UIDataExchange.hpp"

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_xsRelease.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_xsRelease.lib"))
#		endif
#	endif
#endif

//////////////////////////////////////////////////////////////////////////
#else

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../libs/DuiLib_x64_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../libs/DuiLib_x64_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x64_Release.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../libs/DuiLib_x86_Release.lib"))
#		endif
#	endif
#endif



//////////////////////////////////////////////////////////////////////////
#endif