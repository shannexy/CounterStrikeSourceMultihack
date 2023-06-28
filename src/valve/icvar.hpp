#pragma once
#include <cassert>
#include "iappsystem.hpp"
#include "ccolor.hpp"
#include "cutlvector.hpp"
#include "cutlstring.hpp"

#define PRINTF_FORMAT_STRING

typedef int CVarDLLIdentifier_t;
struct characterset_t;

class ConVar;
class CCommand
{
public:
	CCommand() {};
	CCommand(int nArgC, const char** ppArgV) {};
	virtual bool Tokenize(const char* pCommand, characterset_t* pBreakSet = NULL) = 0;
	virtual void Reset() = 0;

	virtual int ArgC() const = 0;
	virtual const char** ArgV() const = 0;
	virtual const char* ArgS() const = 0;					// All args that occur after the 0th arg, in string form
	virtual const char* GetCommandString() const = 0;		// The entire command in string form, including the 0th arg
	virtual const char* operator[](int nIndex) const = 0;	// Gets at arguments
	virtual const char* Arg(int nIndex) const = 0;		// Gets at arguments

	// Helper functions to parse arguments to commands.
	virtual const char* FindArg(const char* pName) const = 0;
	virtual int FindArgInt(const char* pName, int nDefaultVal) const = 0;

private:
	enum
	{
		COMMAND_MAX_ARGC = 64,
		COMMAND_MAX_LENGTH = 512,
	};

	int		m_nArgc;
	int		m_nArgv0Size;
	char	m_pArgSBuffer[COMMAND_MAX_LENGTH];
	char	m_pArgvBuffer[COMMAND_MAX_LENGTH];
	const char* m_ppArgv[COMMAND_MAX_ARGC];
};

class ConCommand;
class ConCommandBase;
class IConVar;
struct characterset_t;


typedef void (*FnChangeCallback_t)(IConVar* var, const char* pOldValue, float flOldValue);

class IConCommandBaseAccessor
{
public:
	// Flags is a combination of FCVAR flags in cvar.h.
	// hOut is filled in with a handle to the variable.
	virtual bool RegisterConCommandBase(ConCommandBase* pVar) = 0;
};


class ConCommandBase
{
	friend class CCvar;
	friend class ConVar;
	friend class ConCommand;

	// FIXME: Remove when ConVar changes are done
	friend class CDefaultCvar;

	ConCommandBase(void) {};
	ConCommandBase(const char* pName, const char* pHelpString = 0,
		int flags = 0) {};

	virtual						~ConCommandBase(void) = 0;

	virtual	bool				IsCommand(void) const = 0;

	// Check flag
	virtual bool				IsFlagSet(int flag) const = 0;
	// Set flag
	virtual void				AddFlags(int flags) = 0;

	// Return name of cvar
	virtual const char* GetName(void) const = 0;

	// Return help text for cvar
	virtual const char* GetHelpText(void) const = 0;

	// Deal with next pointer
	virtual const ConCommandBase* GetNext(void) const = 0;
	virtual ConCommandBase* GetNext(void) = 0;

	virtual bool				IsRegistered(void) const = 0;

	// Returns the DLL identifier
	virtual CVarDLLIdentifier_t	GetDLLIdentifier() const = 0;

protected:
	virtual void				CreateBase(const char* pName, const char* pHelpString = 0,
		int flags = 0) = 0;

	// Used internally by OneTimeInit to initialize/shutdown
	virtual void				Init() = 0;
	virtual void						Shutdown() = 0;

	// Internal copy routine ( uses new operator from correct module )
	virtual char* CopyString(const char* from) = 0;

private:
	// Next ConVar in chain
	// Prior to register, it points to the next convar in the DLL.
	// Once registered, though, m_pNext is reset to point to the next
	// convar in the global list
	ConCommandBase* m_pNext;

	// Has the cvar been added to the global list?
	bool						m_bRegistered;

	// Static data
	const char* m_pszName;
	const char* m_pszHelpString;

	// ConVar flags
	int							m_nFlags;

protected:
	// ConVars add themselves to this list for the executable. 
	// Then ConVar_Register runs through  all the console variables 
	// and registers them into a global list stored in vstdlib.dll
	static ConCommandBase* s_pConCommandBases;

	// ConVars in this executable use this 'global' to access values.
	static IConCommandBaseAccessor* s_pAccessor;
};

typedef void (*FnCommandCallbackVoid_t)(void);
typedef void (*FnCommandCallback_t)(const CCommand& command);

class ICommandCallback
{
public:
	virtual void CommandCallback(const CCommand& command) = 0;
};

typedef int  (*FnCommandCompletionCallback)(const char* partial, char commands[64][64]);

class ICommandCompletionCallback
{
public:
	virtual int  CommandCompletionCallback(const char* pPartial, CUtlVector< CUtlString >& commands) = 0;
};


class ConCommand : public ConCommandBase
{
	friend class CCvar;

public:
	typedef ConCommandBase BaseClass;

	ConCommand(const char* pName, FnCommandCallbackVoid_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0) {}
	ConCommand(const char* pName, FnCommandCallback_t callback,
		const char* pHelpString = 0, int flags = 0, FnCommandCompletionCallback completionFunc = 0) {}
	ConCommand(const char* pName, ICommandCallback* pCallback,
		const char* pHelpString = 0, int flags = 0, ICommandCompletionCallback* pCommandCompletionCallback = 0) {}

	virtual ~ConCommand(void) = 0;

	virtual	bool IsCommand(void) const = 0;

	virtual int AutoCompleteSuggest(const char* partial, CUtlVector< CUtlString >& commands) = 0;

	virtual bool CanAutoComplete(void) = 0;

	// Invoke the function
	virtual void Dispatch(const CCommand& command) = 0;

private:
	// NOTE: To maintain backward compat, we have to be very careful:
	// All public virtual methods must appear in the same order always
	// since engine code will be calling into this code, which *does not match*
	// in the mod code; it's using slightly different, but compatible versions
	// of this class. Also: Be very careful about adding new fields to this class.
	// Those fields will not exist in the version of this class that is instanced
	// in mod code.

	// Call this function when executing the command
	union
	{
		FnCommandCallbackVoid_t m_fnCommandCallbackV1;
		FnCommandCallback_t m_fnCommandCallback;
		ICommandCallback* m_pCommandCallback;
	};

	union
	{
		FnCommandCompletionCallback	m_fnCompletionCallback;
		ICommandCompletionCallback* m_pCommandCompletionCallback;
	};

	bool m_bHasCompletionCallback : 1;
	bool m_bUsingNewCommandCallback : 1;
	bool m_bUsingCommandCallbackInterface : 1;
};

class IConVar {
public:
	virtual void SetValue(const char* pValue) = 0;
	virtual void SetValue(float flValue) = 0;
	virtual void SetValue(int nValue) = 0;

	// Return name of command
	virtual const char* GetName(void) const = 0;

	// Accessors.. not as efficient as using GetState()/GetInfo()
	// if you call these methods multiple times on the same IConVar
	virtual bool IsFlagSet(int nFlag) const = 0;
};


class ConVar : public ConCommandBase, public IConVar
{
	friend class CCvar;
	friend class ConVarRef;
public:
	typedef ConCommandBase BaseClass;

	ConVar(const char* pName, const char* pDefaultValue, int flags = 0) {}

	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString) {}
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax) {}
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, FnChangeCallback_t callback) {}
	ConVar(const char* pName, const char* pDefaultValue, int flags,
		const char* pHelpString, bool bMin, float fMin, bool bMax, float fMax,
		FnChangeCallback_t callback) {}

	virtual						~ConVar(void) = 0;

	virtual bool				IsFlagSet(int flag) const {
		return (flag & m_pParent->m_nFlags) ? true : false;
	}
	virtual const char* GetHelpText(void) const
	{
		return m_pParent->m_pszHelpString;
	}
	virtual bool				IsRegistered(void) const
	{
		return m_pParent->m_bRegistered;
	}
	virtual const char* GetName(void) const
	{
		return m_pParent->m_pszName;
	}
	virtual void				AddFlags(int flags)
	{
		m_pParent->m_nFlags |= flags;
	}
	virtual	bool				IsCommand(void) const
	{
		return false;
	}

	virtual void InstallChangeCallback(FnChangeCallback_t callback)
	{
		assert(!m_pParent->m_fnChangeCallback || !callback);
		m_pParent->m_fnChangeCallback = callback;

		if (m_pParent->m_fnChangeCallback)
		{
			// Call it immediately to set the initial value...
			m_pParent->m_fnChangeCallback(this, m_pszString, m_fValue);
		}
	};

	// Retrieve value
	__forceinline float			GetFloat(void) const
	{
		return m_pParent->m_fValue;
	}
	__forceinline int			GetInt(void) const
	{
		return m_pParent->m_nValue;
	}
	__forceinline bool			GetBool() const
	{
		return !!GetInt();
	}

	__forceinline char const* GetString(void) const
	{
		if (m_nFlags & (1 << 12))
			return "FCVAR_NEVER_AS_STRING";

		return (m_pParent->m_pszString) ? m_pParent->m_pszString : "";
	};

	// Any function that allocates/frees memory needs to be virtual or else you'll have crashes
	//  from alloc/free across dll/exe boundaries.

	// These just call into the IConCommandBaseAccessor to check flags and set the var (which ends up calling InternalSetValue).
	virtual void				SetValue(const char* value)
	{
		ConVar* var = (ConVar*)m_pParent;
		var->InternalSetValue(value);
	}
	virtual void				SetValue(float value)
	{
		ConVar* var = (ConVar*)m_pParent;
		var->InternalSetFloatValue(value);
	};
	virtual void				SetValue(int value)
	{
		ConVar* var = (ConVar*)m_pParent;
		var->InternalSetIntValue(value);
	}

	// Reset to default value
	virtual void						Revert(void)
	{
		ConVar* var = (ConVar*)m_pParent;
		var->SetValue(var->m_pszDefaultValue);
	}

	// True if it has a min/max setting
	virtual bool						GetMin(float& minVal) const {
		minVal = m_pParent->m_fMinVal;
		return m_pParent->m_bHasMin;
	}

	virtual bool						GetMax(float& maxVal) const
	{
		maxVal = m_pParent->m_fMaxVal;
		return m_pParent->m_bHasMax;
	}

	virtual const char* GetDefault(void) const
	{
		return m_pParent->m_pszDefaultValue;
	}
	virtual void SetDefault(const char* pszDefault)
	{
		static char* empty_string;
		m_pszDefaultValue = pszDefault ? pszDefault : empty_string;
		assert(m_pszDefaultValue);
	}

private:
	// Called by CCvar when the value of a var is changing.
	virtual void				InternalSetValue(const char* value) = 0;
	// For CVARs marked FCVAR_NEVER_AS_STRING
	virtual void				InternalSetFloatValue(float fNewValue) = 0;
	virtual void				InternalSetIntValue(int nValue) = 0;

	virtual bool				ClampValue(float& value) = 0;
	virtual void				ChangeStringValue(const char* tempVal, float flOldValue) = 0;

	virtual void				Create(const char* pName, const char* pDefaultValue, int flags = 0,
		const char* pHelpString = 0, bool bMin = false, float fMin = 0.0,
		bool bMax = false, float fMax = false, FnChangeCallback_t callback = 0) = 0;

	// Used internally by OneTimeInit to initialize.
	virtual void				Init() = 0;
	int GetFlags() { return m_pParent->m_nFlags; }
private:

	// This either points to "this" or it points to the original declaration of a ConVar.
	// This allows ConVars to exist in separate modules, and they all use the first one to be declared.
	// m_pParent->m_pParent must equal m_pParent (ie: m_pParent must be the root, or original, ConVar).
	ConVar* m_pParent;

	// Static data
	const char* m_pszDefaultValue;

	// Value
	// Dynamically allocated
	char* m_pszString;
	int							m_StringLength;

	// Values
	float						m_fValue;
	int							m_nValue;

	// Min/Max values
	bool						m_bHasMin;
	float						m_fMinVal;
	bool						m_bHasMax;
	float						m_fMaxVal;

	// Call this function when ConVar changes
	FnChangeCallback_t			m_fnChangeCallback;
};

class ConCommand;

class IConsoleDisplayFunc {
public:
	virtual void ColorPrint(const CColor& clr, const char* pMessage) = 0;
	virtual void Print(const char* pMessage) = 0;
	virtual void DPrint(const char* pMessage) = 0;
};

class ICvarQuery : public IAppSystem {
public:
	virtual bool AreConVarsLinkable(const ConVar* child, const ConVar* parent) = 0;
};

#define FMTFUNCTION( a, b )

class ICVar : public IAppSystem {
public:
	virtual CVarDLLIdentifier_t AllocateDLLIdentifier() = 0;
	virtual void			RegisterConCommand(ConCommandBase* pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConCommandBase* pCommandBase) = 0;
	virtual void			UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
	virtual const char* GetCommandLineValue(const char* pVariableName) = 0;
	virtual ConCommandBase* FindCommandBase(const char* name) = 0;
	virtual const ConCommandBase* FindCommandBase(const char* name) const = 0;
	virtual ConVar* FindVar(const char* var_name) = 0;
	virtual const ConVar* FindVar(const char* var_name) const = 0;
	virtual ConCommand* FindCommand(const char* name) = 0;
	virtual const ConCommand* FindCommand(const char* name) const = 0;
	virtual ConCommandBase* GetCommands(void) = 0;
	virtual const ConCommandBase* GetCommands(void) const = 0;
	virtual void			InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
	virtual void			CallGlobalChangeCallbacks(ConVar* var, const char* pOldString, float flOldValue) = 0;
	virtual void			InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void			RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
	virtual void			ConsoleColorPrintf(const CColor& clr, PRINTF_FORMAT_STRING const char* pFormat, ...) = 0;
	virtual void			ConsolePrintf(PRINTF_FORMAT_STRING const char* pFormat, ...) = 0;
	virtual void			ConsoleDPrintf(PRINTF_FORMAT_STRING const char* pFormat, ...) = 0;
	virtual void			RevertFlaggedConVars(int nFlag) = 0;
	virtual void			InstallCVarQuery(ICvarQuery* pQuery) = 0;
	virtual bool			IsMaterialThreadSetAllowed() const = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar* pConVar, const char* pValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar* pConVar, int nValue) = 0;
	virtual void			QueueMaterialThreadSetValue(ConVar* pConVar, float flValue) = 0;
	virtual bool			HasQueuedMaterialThreadConVarSets() const = 0;
	virtual int				ProcessQueuedMaterialThreadConVarSets() = 0;
};