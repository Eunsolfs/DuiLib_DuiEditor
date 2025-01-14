#pragma once

#ifndef __DUIPLUGINS_H__
#define __DUIPLUGINS_H__

#ifdef UILIB_STATIC
#define UILIB_EXP_API 
#else
#if defined(UILIB_EXPORTS)
#	if defined(_MSC_VER)
#		define UILIB_EXP_API extern "C" __declspec(dllexport)
#	else
#		define UILIB_EXP_API 
#	endif
#else
#	if defined(_MSC_VER)
#		define UILIB_EXP_API extern "C" __declspec(dllimport)
#	else
#		define UILIB_EXP_API 
#	endif
#endif
#endif

//////////////////////////////////////////////////////////////////////////

#define DUI_CTR_GRIDCTRL					(_T("GridCtrl"))
#include "../MFCGridCtrl/GridCtrl.h"
#include "../DuiLib/UIGridCtrl.h"

#include "../DuiLib/UIKeyBoard.h"
#include "../DuiLib/UIKeyBoardNumber.h"
#include "../DuiLib/UIKeyBoardSimple.h"

#define DUI_CTR_SCIEDIT						(_T("SciEdit"))
#include "../DuiLib/UISciEdit.h"


UILIB_EXP_API void __stdcall DuiPluginsRegister();

UILIB_EXP_API void __stdcall InsertMsgUI(LPCTSTR pstring, COLORREF cr=RGB(0,0,0));
UILIB_EXP_API void __stdcall InsertMsgUiV(LPCTSTR lpszFormat, ...);

//load grid from xml string content, virtual grid will be failed
UILIB_EXP_API bool __stdcall UIGrid_LoadXmlString(DuiLib::CGridUI *pGrid, LPCTSTR lpstring);

//save grid to xml string content, virtual grid will be failed
UILIB_EXP_API bool __stdcall UIGrid_SaveXmlString(DuiLib::CGridUI *pGrid, DuiLib::CDuiString &strXml);					

//load grid from xml file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_LoadXmlFile(DuiLib::CGridUI *pGrid, LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

//save grid to xml file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_SaveXmlFile(DuiLib::CGridUI *pGrid, LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

//load grid from excel file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_LoadExcelFile(DuiLib::CGridUI *pGrid, LPCTSTR filename=NULL, bool bOpenFileDialog=true);		

//save grid to excel file, virtual grid will be failed
//if bOpenFileDialog=true will open file dialog to select. otherwise, filename has to include full path
UILIB_EXP_API bool __stdcall UIGrid_SaveExcelFile(DuiLib::CGridUI *pGrid, LPCTSTR filename=NULL, bool bOpenFileDialog=true);	

#ifndef UILIB_STATIC
UILIB_EXP_API DuiLib::CControlUI *CreateControl(LPCTSTR pstrClass);
#endif


#ifndef UILIB_EXPORTS

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif

//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC


#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_xsRelease.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_xsRelease.lib"))
#		endif
#	endif
#endif

#else //#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x64_Release.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiPlugins_x86_Release.lib"))
#		endif
#	endif
#endif

#endif //#ifdef UILIB_STATIC

#endif //#ifndef UILIB_EXPORTS


#endif //#ifndef __DUIPLUGINS_H__