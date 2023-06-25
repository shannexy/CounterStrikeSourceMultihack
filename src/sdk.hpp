#pragma once
#include <assert.h>

#include "valve/iclientnetworkable.hpp"
#include "utils/datatable.hpp"

struct RecvTable;

extern ClientClass* g_pClientClassHead;

typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

struct datamap_t;

class ClientClass
{
public:
	ClientClass(const char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;

		// Link it in
		m_pNext = g_pClientClassHead;
		g_pClientClassHead = this;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};


inline int InternalCheckDeclareClass(const char* pClassName, const char* pClassNameMatch, void* pTestPtr, void* pBasePtr)
{
	// This makes sure that casting from ThisClass to BaseClass works right. You'll get a compiler error if it doesn't
	// work at all, and you'll get a runtime error if you use multiple inheritance.
	assert(pTestPtr == pBasePtr);

	// This is triggered by IMPLEMENT_SERVER_CLASS. It does DLLClassName::CheckDeclareClass( #DLLClassName ).
	// If they didn't do a DECLARE_CLASS in DLLClassName, then it'll be calling its base class's version
	// and the class names won't match.
	assert((void*)pClassName == (void*)pClassNameMatch);
	return 0;
}

template <typename T>
inline int CheckDeclareClass_Access(T*, const char* pShouldBe)
{
	return T::CheckDeclareClass(pShouldBe);
}


#define DECLARE_CLASS( className, baseClassName ) \
		typedef baseClassName BaseClass; \
		typedef className ThisClass; \
		template <typename T> friend int CheckDeclareClass_Access(T *, const char *pShouldBe); \
		static int CheckDeclareClass( const char *pShouldBe ) \
		{ \
			InternalCheckDeclareClass( pShouldBe, #className, (ThisClass*)0xFFFFF, (BaseClass*)(ThisClass*)0xFFFFF ); \
			return CheckDeclareClass_Access( (BaseClass *)NULL, #baseClassName ); \
		}

#define DECLARE_CLASS_GAMEROOT( className, baseClassName ) \
		typedef baseClassName BaseClass; \
		typedef className ThisClass; \
		template <typename T> friend int CheckDeclareClass_Access(T *, const char *pShouldBe); \
		static int CheckDeclareClass( const char *pShouldBe ) \
		{ \
			return InternalCheckDeclareClass( pShouldBe, #className, (ThisClass*)0xFFFFF, (BaseClass*)(ThisClass*)0xFFFFF ); \
		}


#define DECLARE_CLASS_NOFRIEND( className, baseClassName ) \
		DECLARE_CLASS( className, baseClassName )

#define BEGIN_SIMPLE_DATADESC( className ) \
	datamap_t className::m_DataMap = { 0, 0, #className, NULL }; \
	datamap_t *className::GetBaseMap() { return NULL; } \
	BEGIN_DATADESC_GUTS( className )

#define DECLARE_CLASS_NOBASE( className ) \
		typedef className ThisClass; \
		template <typename T> friend int CheckDeclareClass_Access(T *, const char *pShouldBe); \
		static int CheckDeclareClass( const char *pShouldBe ) \
		{ \
			return InternalCheckDeclareClass( pShouldBe, #className, 0, 0 ); \
		}

#define DECLARE_CLIENTCLASS() \
	virtual int YouForgotToImplementOrDeclareClientClass() = 0;\
	virtual ClientClass* GetClientClass() = 0;\
	static RecvTable *m_pClassRecvTable; \
	DECLARE_CLIENTCLASS_NOBASE()

#define ALLOW_DATATABLES_PRIVATE_ACCESS() \
	template <typename T> friend int ClientClassInit(T *);


#define DECLARE_CLIENTCLASS_NOBASE ALLOW_DATATABLES_PRIVATE_ACCESS


#define DECLARE_SIMPLE_DATADESC() \
	static datamap_t m_DataMap; \
	static datamap_t *GetBaseMap(); \
	template <typename T> friend void DataMapAccess(T *, datamap_t **p); \
	template <typename T> friend datamap_t *DataMapInit(T *);


#define	DECLARE_DATADESC() \
	DECLARE_SIMPLE_DATADESC() \
	virtual datamap_t *GetDataDescMap( void ) = 0;

#define DECLARE_PREDICTABLE()

#define MyOffsetOf( type, var ) ( (int)&((type*)0)->var )
#define CHECK_USENETWORKVARS if(true)

#define END_DATADESC() \
		}; \
		\
		if ( sizeof( dataDesc ) > sizeof( dataDesc[0] ) ) \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = SIZE_OF_ARRAY( dataDesc ) - 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = &dataDesc[1]; \
		} \
		else \
		{ \
			classNameTypedef::m_DataMap.dataNumFields = 1; \
			classNameTypedef::m_DataMap.dataDesc 	  = dataDesc; \
		} \
		return &classNameTypedef::m_DataMap; \
	}

#define DECLARE_BYTESWAP_DATADESC() DECLARE_SIMPLE_DATADESC()
#define BEGIN_BYTESWAP_DATADESC(name) BEGIN_SIMPLE_DATADESC(name) 
#define BEGIN_BYTESWAP_DATADESC_(name,base) BEGIN_SIMPLE_DATADESC_(name,base) 
#define END_BYTESWAP_DATADESC() END_DATADESC()