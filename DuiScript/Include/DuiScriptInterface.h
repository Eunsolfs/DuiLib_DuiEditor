#pragma once	

#ifndef __DUISCRIPT_H__
#define __DUISCRIPT_H__

#ifdef UILIB_STATIC
#define UISCRIPT_API 
#else
#if defined(UISCRIPT_EXPORTS)
#	if defined(_MSC_VER)
#		define UISCRIPT_API __declspec(dllexport)
#	else
#		define UISCRIPT_API 
#	endif
#else
#	if defined(_MSC_VER)
#		define UISCRIPT_API __declspec(dllimport)
#	else
#		define UISCRIPT_API 
#	endif
#endif
#endif

#include "../angelscript/angelscript/include/angelscript.h"

#define SCRIPT_DEBUG_CONTINUE		1
#define SCRIPT_DEBUG_SETP_INTO		2
#define SCRIPT_DEBUG_SETP_OVER		3
#define SCRIPT_DEBUG_SETP_OUT		4
#define SCRIPT_DEBUG_SETP_CURSOR	5
#define SCRIPT_DEBUG_ABORT			6

typedef enum enumUIScriptMsgType
{
	usMsg_GoToLine			= 0,	//ִ�е�line��
	usMsg_CheckBreakPoint	= 1,	//�жϵ�ǰ���Ƿ���Ҫ�ϵ�
	usMsg_PrintContext		= 2,	//PrintContext, ͨ�������Ϣ��ȡ��ǰ�Ľű���ջ����
	usMsg_Message			= 3,	//����ı���Ϣ
	usMsg_RunBegin			= 4,	//�ű���ʼ����
	usMsg_RunEnd			= 5,	//�ű���������
	usMsg_RunAbort			= 6		//�ű��쳣����
}UIScriptMsgType;

typedef struct tagScriptMessage
{
	UIScriptMsgType type; 
	int line;
	bool breakpoint; //�ӻص����������Ƿ�ϵ�
	const char *message;
	asIScriptContext *ctx;
}TScriptMessage;
typedef void(CALLBACK *SCRIPTMESSAGECALLBACK)(TScriptMessage *msg, UINT_PTR userdata);

class IScriptHelper
{
public:
	virtual void SetScriptMessageCallBack(SCRIPTMESSAGECALLBACK pFun, UINT_PTR userdata) = 0;
	virtual asIScriptEngine *GetEngine() const = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual bool CreateModule(LPCTSTR moduleName = NULL) = 0;
	virtual void DeleteModule() = 0;
	virtual bool AddScriptFile(LPCTSTR pstrFileName) = 0;
	virtual bool AddScriptCode(LPCTSTR pstrCode) = 0;
	virtual bool CompileScript() = 0;

	virtual BOOL IsRunning()			= 0;
	virtual void DebugRun()				= 0;
	virtual BOOL IsCanDebugRun()		= 0;
	virtual void DebugStop()			= 0;
	virtual BOOL IsCanDebugStop()		= 0;
	virtual void DebugStepInto()		= 0;
	virtual BOOL IsCanDebugStepInto()	= 0;
	virtual void DebugStepOver()		= 0;
	virtual BOOL IsCanDebugStepOver()	= 0;
	virtual void DebugStepReturn()		= 0;
	virtual BOOL IsCanDebugStepReturn() = 0;
	virtual void DebugStepCursor()		= 0;
	virtual BOOL IsCanDebugStepCursor() = 0;

	//������ں���
	virtual bool SetMainFun(LPCTSTR lpszMainFun) = 0;

	//������ں�������
	virtual BOOL	SetArgByte(UINT arg, BYTE value) = 0;
	virtual BOOL	SetArgWord(UINT arg, WORD value) = 0;
	virtual BOOL	SetArgDWord(UINT arg, DWORD value) = 0;
	virtual BOOL	SetArgFloat(UINT arg, float value) = 0;
	virtual BOOL	SetArgDouble(UINT arg, double value) = 0;
	virtual BOOL	SetArgAddress(UINT arg, void *addr) = 0;
	virtual BOOL	SetArgObject(UINT arg, void *obj) = 0;
	virtual void *  GetAddressOfArg(UINT arg) = 0;

	//ִ�нű�
	virtual bool	Excute() = 0;

	//��ȡ�����ķ���ֵ
	virtual BYTE	GetReturnByte() = 0;
	virtual WORD	GetReturnWord() = 0;
	virtual DWORD	GetReturnDWord() = 0;
	virtual float	GetReturnFloat() = 0;
	virtual double	GetReturnDouble() = 0;
	virtual void *	GetReturnAddress() = 0;
	virtual void *	GetReturnObject() = 0;
	virtual void *	GetAddressOfReturnValue() = 0;
};

UISCRIPT_API DuiLib::IScriptManager* __stdcall CreateScriptEngine();
UISCRIPT_API void __stdcall DeleteScriptEngine(DuiLib::IScriptManager *pEngine);

UISCRIPT_API void __stdcall DuiScriptRegister();

typedef IScriptHelper* (__stdcall *CREATE_SCRIPT_HELPER)();
typedef void (__stdcall *DELETE_SCRIPT_HELPER)(IScriptHelper *pHelper);

UISCRIPT_API IScriptHelper* __stdcall CreateScriptHelper();
UISCRIPT_API void __stdcall DeleteScriptHelper(IScriptHelper *pHelper);


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef UISCRIPT_EXPORTS

#ifndef _LIBPATH_
#define _LIBPATH_(p,f)   p##f
#endif
//////////////////////////////////////////////////////////////////////////
#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_xsRelease.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_xsUnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_xsUnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_xsDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_xsRelease.lib"))
#		endif
#	endif
#endif

#else //#ifdef UILIB_STATIC

#ifdef _WIN64
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x64_Release.lib"))
#		endif
#	endif
#else
#	ifdef _UNICODE
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_UnicodeDebug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_UnicodeRelease.lib"))
#		endif
#	else
#		ifdef _DEBUG
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_Debug.lib"))
#		else
#			pragma comment(lib,  _LIBPATH_(__FILE__,   "/../../../libs/DuiScript_x86_Release.lib"))
#		endif
#	endif
#endif

#endif //#ifdef UILIB_STATIC
#endif //#ifndef UILIB_EXPORTS


#endif // __DUISCRIPT_H__
