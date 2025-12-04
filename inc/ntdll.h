/*****************************************************************************/
/* ntdll.h                           Copyright (c) Ladislav Zezula 2005-2025 */
/*---------------------------------------------------------------------------*/
/* Header file for the import library "ntdll.lib"                            */
/*                                                                           */
/* This library has been created because of never-ending problems when       */
/* Ntdll.lib from Windows DDK with SDK libs (duplicate symbols, linker       */
/* errors etc).                                                              */
/* Now, it is possible to use native NT API with no problems, all you need   */
/* is just to include ntdll.h and link ntdll.lib from this project.          */
/*---------------------------------------------------------------------------*/
/*   Date    Ver   Who  Comment                                              */
/* --------  ----  ---  -------                                              */
/* 15.05.03  1.00  Lad  Created                                              */
/* 16.09.05  2.00  Lad  Far more functions                                   */
/*****************************************************************************/

#ifndef __NTDLL_H__
#define __NTDLL_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _NTDDK_
#error This header cannot be compiled together with NTDDK
#endif

#ifndef _NTDLL_SELF_                            // Auto-insert the library
#pragma comment(lib, "Ntdll.lib")
#endif

#ifdef _MSC_VER
#pragma warning(disable: 4201)                  // nonstandard extension used : nameless struct/union
#pragma warning(disable: 4214)                  // nonstandard extension used : bit field types other than int
#endif

//------------------------------------------------------------------------------
// Defines for NTSTATUS

#ifdef _Return_type_success_
typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#else
typedef LONG NTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status)   ((NTSTATUS)(Status) >= 0)
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS       ((NTSTATUS)0x00000000L)
#endif

#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL  ((NTSTATUS)0xC0000001L)
#endif

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else
#define ASSERT(x) /* x */
#endif
#endif

#ifndef DEVICE_TYPE
#define DEVICE_TYPE DWORD
#endif

//-----------------------------------------------------------------------------
// Definition of intervals for waiting functions

#define ABSOLUTE_INTERVAL(wait) (wait)

#define RELATIVE_INTERVAL(wait) (-(wait))

#define NANOSECONDS(nanos) \
(((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros) \
(((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILISECONDS(mili) \
(((signed __int64)(mili)) * MICROSECONDS(1000L))

#define SECONDS(seconds) \
(((signed __int64)(seconds)) * MILISECONDS(1000L))

//------------------------------------------------------------------------------
// Structures

#ifndef _NTDEF_
typedef enum _EVENT_TYPE
{
    NotificationEvent,
    SynchronizationEvent
} EVENT_TYPE;

//
// ANSI strings are counted 8-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} STRING, *PSTRING;

//
// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

//
// Bitmap type
//

typedef struct _RTL_BITMAP
{
    ULONG SizeOfBitmap;
    PULONG Buffer;
} RTL_BITMAP, *PRTL_BITMAP;

//
// Preallocated heap-growable buffers
//

typedef struct _RTL_BUFFER
{
    PUCHAR    Buffer;
    PUCHAR    StaticBuffer;
    SIZE_T    Size;
    SIZE_T    StaticSize;
    SIZE_T    ReservedForAllocatedSize;     // for future doubling
    PVOID     ReservedForIMalloc;           // for future pluggable growth
} RTL_BUFFER, *PRTL_BUFFER;

//
// A preallocated buffer that is "tied" to a UNICODE_STRING
//

typedef struct _RTL_UNICODE_STRING_BUFFER
{
    UNICODE_STRING String;
    RTL_BUFFER     ByteBuffer;
    UCHAR          MinimumStaticBufferForTerminalNul[sizeof(WCHAR)];

} RTL_UNICODE_STRING_BUFFER, *PRTL_UNICODE_STRING_BUFFER;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;
typedef CONST STRING* PCOEM_STRING;

typedef const UNICODE_STRING *PCUNICODE_STRING;

#define UNICODE_NULL ((WCHAR)0) // winnt

#ifndef RTL_CONSTANT_STRING         // UNICODE_STRING FooStr = RTL_CONSTANT_STRING(L"Foo");
#define RTL_CONSTANT_STRING(str)    {(USHORT)(sizeof(str) - sizeof(str[0])), (USHORT)(sizeof(str)), str}
#endif  // RTL_CONSTANT_STRING

//
// Definitions for Object Creation
//

#define OBJ_PROTECT_CLOSE                       0x00000001L
#define OBJ_INHERIT                             0x00000002L
#define OBJ_AUDIT_OBJECT_CLOSE                  0x00000004L
#define OBJ_NO_RIGHTS_UPGRADE                   0x00000008L
#define OBJ_PERMANENT                           0x00000010L
#define OBJ_EXCLUSIVE                           0x00000020L
#define OBJ_CASE_INSENSITIVE                    0x00000040L
#define OBJ_OPENIF                              0x00000080L
#define OBJ_OPENLINK                            0x00000100L
#define OBJ_KERNEL_HANDLE                       0x00000200L
#define OBJ_FORCE_ACCESS_CHECK                  0x00000400L
#define OBJ_IGNORE_IMPERSONATED_DEVICEMAP       0x00000800L
#define OBJ_DONT_REPARSE                        0x00001000L
#define OBJ_KERNEL_EXCLUSIVE                    0x00010000L

//
// Object Attributes structure
//

typedef struct _OBJECT_ATTRIBUTES
{
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

//
// IO_STATUS_BLOCK
//

typedef struct _IO_STATUS_BLOCK
{
    union
    {
        NTSTATUS Status;
        PVOID Pointer;
    };

    ULONG_PTR Information;

} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

//
// ClientId
//

typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
#endif // _NTDEF_

/*
typedef struct _IMAGE_RELOC
{
    WORD offset : 12;
    WORD type : 4;
} IMAGE_RELOC, *PIMAGE_RELOC;
*/

//
// KUSER_SHARED_DATA from ntdll.dll version 10.0.14361.1000 (rs1_release_prs.160603-2123)
//

typedef enum _NT_PRODUCT_TYPE
{
    NtProductWinNt = 1,
    NtProductLanManNt = 2,
    NtProductServer = 3

} NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
{
    StandardDesign = 0,
    NEC98x86 = 1,
    EndAlternatives = 2

} ALTERNATIVE_ARCHITECTURE_TYPE, *PALTERNATIVE_ARCHITECTURE_TYPE;

typedef struct _KSYSTEM_TIME
{
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;

} KSYSTEM_TIME, *PKSYSTEM_TIME;

#if _MSC_VER <= 1500

#define MAXIMUM_XSTATE_FEATURES             (64)

typedef struct _PROCESSOR_NUMBER
{
    USHORT Group;
    UCHAR Number;
    UCHAR Reserved;
} PROCESSOR_NUMBER;

typedef struct _XSTATE_FEATURE
{
    DWORD Offset;
    DWORD Size;
} XSTATE_FEATURE, * PXSTATE_FEATURE;

typedef struct _XSTATE_CONFIGURATION
{
    DWORD64 EnabledFeatures;
    DWORD64 EnabledVolatileFeatures;
    DWORD   Size;

    // Control Flags
    union
    {
        DWORD ControlFlags;
        struct
        {
            DWORD OptimizedSave : 1;
            DWORD CompactionEnabled : 1;
            DWORD ExtendedFeatureDisable : 1;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;

    // List of features
    XSTATE_FEATURE Features[MAXIMUM_XSTATE_FEATURES];

    // Mask of all supervisor features
    DWORD64 EnabledSupervisorFeatures;

    // Mask of features that require start address to be 64 byte aligned
    DWORD64 AlignedFeatures;

    // Total size of the save area for user and supervisor states
    DWORD AllFeatureSize;

    // List which holds size of each user and supervisor state supported by CPU
    DWORD AllFeatures[MAXIMUM_XSTATE_FEATURES];

    // Mask of all supervisor features that are exposed to user-mode
    DWORD64 EnabledUserVisibleSupervisorFeatures;

    // Mask of features that can be disabled via XFD
    DWORD64 ExtendedFeatureDisableFeatures;

    // Total size of the save area for non-large user and supervisor states
    DWORD AllNonLargeFeatureSize;

    // The maximum supported ARM64 SVE vector length that can be used in the
    // current environment, in bytes.
    WORD   MaxSveVectorLength;
    WORD   Spare1;
} XSTATE_CONFIGURATION, * PXSTATE_CONFIGURATION;
#endif

#pragma pack(push, 4)
typedef struct _KUSER_SHARED_DATA
{
    /* 0x000 */ ULONG TickCountLowDeprecated;
    /* 0x004 */ ULONG TickCountMultiplier;
    /* 0x008 */ KSYSTEM_TIME InterruptTime;
    /* 0x014 */ KSYSTEM_TIME SystemTime;
    /* 0x020 */ KSYSTEM_TIME TimeZoneBias;
    /* 0x02c */ USHORT ImageNumberLow;
    /* 0x02e */ USHORT ImageNumberHigh;
    /* 0x030 */ WCHAR NtSystemRoot[260];
    /* 0x238 */ ULONG MaxStackTraceDepth;
    /* 0x23c */ ULONG CryptoExponent;
    /* 0x240 */ ULONG TimeZoneId;
    /* 0x244 */ ULONG LargePageMinimum;
    /* 0x248 */ ULONG AitSamplingValue;
    /* 0x24c */ ULONG AppCompatFlag;
    /* 0x250 */ ULONGLONG RNGSeedVersion;
    /* 0x258 */ ULONG GlobalValidationRunlevel;
    /* 0x25c */ LONG TimeZoneBiasStamp;
    /* 0x260 */ ULONG NtBuildNumber;
    /* 0x264 */ NT_PRODUCT_TYPE NtProductType;
    /* 0x268 */ UCHAR ProductTypeIsValid;
    /* 0x269 */ UCHAR Reserved0[1];
    /* 0x26a */ USHORT NativeProcessorArchitecture;
    /* 0x26c */ ULONG NtMajorVersion;
    /* 0x270 */ ULONG NtMinorVersion;
    /* 0x274 */ UCHAR ProcessorFeatures[64];
    /* 0x2b4 */ ULONG Reserved1;
    /* 0x2b8 */ ULONG Reserved3;
    /* 0x2bc */ ULONG TimeSlip;
    /* 0x2c0 */ ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
    /* 0x2c4 */ ULONG BootId;
    /* 0x2c8 */ LARGE_INTEGER SystemExpirationDate;
    /* 0x2d0 */ ULONG SuiteMask;
    /* 0x2d4 */ BOOLEAN KdDebuggerEnabled;

    /* 0x2d5 */ union
                {
                    UCHAR MitigationPolicies;
                    struct
                    {
                        UCHAR NXSupportPolicy:2;
                        UCHAR SEHValidationPolicy:2;
                        UCHAR CurDirDevicesSkippedForDlls:2;
                        UCHAR Reserved:2;
                    };
                };

    /* 0x2d6 */ UCHAR Reserved6[2];
    /* 0x2d8 */ ULONG ActiveConsoleId;
    /* 0x2dc */ ULONG DismountCount;
    /* 0x2e0 */ ULONG ComPlusPackage;
    /* 0x2e4 */ ULONG LastSystemRITEventTickCount;
    /* 0x2e8 */ ULONG NumberOfPhysicalPages;
    /* 0x2ec */ UCHAR SafeBootMode;
    /* 0x2ed */ UCHAR Reserved12;

    /* 0x2f0 */ union
                {
                    ULONG SharedDataFlags;
                    struct
                    {
                        ULONG DbgErrorPortPresent : 1;
                        ULONG DbgElevationEnabled : 1;
                        ULONG DbgVirtEnabled : 1;
                        ULONG DbgInstallerDetectEnabled : 1;
                        ULONG DbgLkgEnabled : 1;
                        ULONG DbgDynProcessorEnabled : 1;
                        ULONG DbgConsoleBrokerEnabled : 1;
                        ULONG DbgSecureBootEnabled : 1;
                        ULONG DbgMultiSessionSku : 1;
                        ULONG DbgMultiUsersInSessionSku : 1;
                        ULONG DbgStateSeparationEnabled : 1;
                        ULONG SpareBits : 21;
                    };
                };

    /* 0x2f4 */ ULONG DataFlagsPad[1];
    /* 0x2f8 */ ULONGLONG TestRetInstruction;
    /* 0x300 */ ULONGLONG QpcFrequency;
    /* 0x308 */ ULONG SystemCall;
    /* 0x30C */ ULONG SystemCallPad0;
    /* 0x310 */ ULONGLONG SystemCallPad[2];

    /* 0x320 */ union
                {
                    KSYSTEM_TIME TickCount;
                    ULONGLONG TickCountQuad;
                    ULONG ReservedTickCountOverlay[3];
                };

    /* 0x32c */ ULONG TickCountPad[1];
    /* 0x330 */ ULONG Cookie;
    /* 0x334 */ ULONG CookiePad[1];
    /* 0x338 */ ULONGLONG ConsoleSessionForegroundProcessId;
    /* 0x340 */ ULONGLONG TimeUpdateLock;
    /* 0x348 */ ULONGLONG BaselineSystemTimeQpc;
    /* 0x350 */ ULONGLONG BaselineInterruptTimeQpc;
    /* 0x358 */ ULONGLONG QpcSystemTimeIncrement;
    /* 0x360 */ ULONGLONG QpcInterruptTimeIncrement;
    /* 0x368 */ UCHAR QpcSystemTimeIncrementShift;
    /* 0x369 */ UCHAR QpcInterruptTimeIncrementShift;
    /* 0x36a */ USHORT UnparkedProcessorCount;
    /* 0x36c */ UCHAR Reserved8[20];
    /* 0x380 */ USHORT UserModeGlobalLogger[16];
    /* 0x3a0 */ ULONG ImageFileExecutionOptions;
    /* 0x3a4 */ ULONG LangGenerationCount;
    /* 0x3a8 */ ULONGLONG Reserved4;
    /* 0x3b0 */ ULONGLONG InterruptTimeBias;
    /* 0x3b8 */ ULONGLONG QpcBias;
    /* 0x3c0 */ ULONG ActiveProcessorCount;
    /* 0x3c4 */ UCHAR ActiveGroupCount;
    /* 0x3c5 */ UCHAR Reserved9;

    
    /* 0x3c6 */ union
                {
                    USHORT QpcData;
                    struct
                    {
                        BYTE QpcBypassEnabled;
                        BYTE QpcShift;
                    };
                };

    /* 0x3c8 */ LARGE_INTEGER TimeZoneBiasEffectiveStart;
    /* 0x3d0 */ LARGE_INTEGER TimeZoneBiasEffectiveEnd;
    /* 0x3d8 */ XSTATE_CONFIGURATION XState;
    /* 0x710 */ KSYSTEM_TIME FeatureConfigurationChangeStamp;

} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;
#pragma pack(pop)

//------------------------------------------------------------------------------
// Loader-related structures

//
// Process Environment Block
//

typedef struct _PEB_FREE_BLOCK
{
    struct _PEB_FREE_BLOCK * Next;
    ULONG Size;

} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

// CURDIR structure
typedef struct _CURDIR
{
    UNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, * PCURDIR;

typedef struct _RTL_DRIVE_LETTER_CURDIR
{
    USHORT Flags;
    USHORT Length;
    ULONG  TimeStamp;
    STRING DosPath;

} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _PEB_LDR_DATA
{
    ULONG Length;
    BOOLEAN Initialized;
    HANDLE SsHandle;
    LIST_ENTRY InLoadOrderModuleList;               // Points to the loaded modules (main EXE usually)
    LIST_ENTRY InMemoryOrderModuleList;             // Points to all modules (EXE and all DLLs)
    LIST_ENTRY InInitializationOrderModuleList;
    PVOID      EntryInProgress;
    BOOLEAN    ShutdownInProgress;                  // Windows 10
    HANDLE     ShutdownThreadId;                    // Windows 10

} PEB_LDR_DATA, * PPEB_LDR_DATA;

#define LDRP_PACKED_BINARY              0x00000001
#define LDRP_MARKED_FOR_REMOVAL         0x00000002
#define LDRP_IMAGE_DLL                  0x00000004
#define LDRP_LOAD_NOTIFICATION_SENT     0x00000008
#define LDRP_TELEMETRY_ENTRY_PROCESSED  0x00000010
#define LDRP_PROCESS_STATIC_IMPORT      0x00000020
#define LDRP_IN_LEGACY_LISTS            0x00000040
#define LDRP_IN_INDEXES                 0x00000080
#define LDRP_SHIM_DLL                   0x00000100
#define LDRP_IN_EXCEPTION_TABLE         0x00000200
#define LDRP_RESERVED1                  0x00000400
#define LDRP_RESERVED2                  0x00000800
#define LDRP_LOAD_IN_PROGRESS           0x00001000
#define LDRP_LOAD_CONFIG_PROCESSED      0x00002000      // Was: LDRP_UNLOAD_IN_PROGRESS
#define LDRP_ENTRY_PROCESSED            0x00004000
#define LDRP_ENTRY_PROTECT_DELAY_LOAD   0x00008000      // Was: LDRP_ENTRY_INSERTED
#define LDRP_RESERVED3                  0x00010000
#define LDRP_RESERVED4                  0x00020000
#define LDRP_DONT_CALL_FOR_THREADS      0x00040000
#define LDRP_PROCESS_ATTACH_CALLED      0x00080000
#define LDRP_PROCESS_ATTACH_FAILED      0x00100000
#define LDRP_COR_DEFERRED_VALIDATE      0x00200000
#define LDRP_COR_IMAGE                  0x00400000
#define LDRP_DONT_RELOCATE              0x00800000
#define LDRP_COR_IL_ONLY                0x01000000
#define LDRP_RESERVED5                  0x02000000
#define LDRP_RESERVED6                  0x04000000
#define LDRP_RESERVED7                  0x08000000
#define LDRP_REDIRECTED                 0x10000000
#define LDRP_ENTRY_NATIVE               0x08000000
#define LDRP_REDIRECTED                 0x10000000
#define LDRP_RESERVED8                  0x20000000
#define LDRP_RESERVED9                  0x40000000      // Was: LDRP_MM_LOADED
#define LDRP_COMPAT_DATABASE_PROTECTED  0x80000000

typedef struct _RTL_BALANCED_NODE
{
    union
    {
        struct _RTL_BALANCED_NODE * Children[2];
        struct
        {
            struct _RTL_BALANCED_NODE * Left;
            struct _RTL_BALANCED_NODE * Right;
        };
    };
    union
    {
        struct
        {
            UCHAR Red : 1;
        };
        struct
        {
            UCHAR Balance:2;
        };
        ULONG ParentValue;
    };
} RTL_BALANCED_NODE, *PRTL_BALANCED_NODE;

typedef enum _LDR_DLL_LOAD_REASON
{
     LoadReasonStaticDependency = 0x0,
     LoadReasonStaticForwarderDependency = 0x1,
     LoadReasonDynamicForwarderDependency = 0x2,
     LoadReasonDelayloadDependency = 0x3,
     LoadReasonDynamicLoad = 0x4,
     LoadReasonAsImageLoad = 0x5,
     LoadReasonAsDataLoad = 0x6,
     LoadReasonUnknown = 0xFFFFFFFF,
} LDR_DLL_LOAD_REASON, *PLDR_DLL_LOAD_REASON;

typedef enum _LDR_HOT_PATCH_STATE
{
    LdrHotPatchBaseImage = 0,
    LdrHotPatchNotApplied = 1,
    LdrHotPatchAppliedReverse = 2,
    LdrHotPatchAppliedForward = 3,
    LdrHotPatchFailedToPatch = 4,
    LdrHotPatchStateMax = 5
} LDR_HOT_PATCH_STATE, *PLDR_HOT_PATCH_STATE;

typedef enum _LDR_DDAG_STATE
{
    LdrModulesMerged = -5,
    LdrModulesInitError = -4,
    LdrModulesSnapError = -3,
    LdrModulesUnloaded = -2,
    LdrModulesUnloading = -1,
    LdrModulesPlaceHolder = 0x0,
    LdrModulesMapping = 0x1,
    LdrModulesMapped = 0x2,
    LdrModulesWaitingForDependencies = 0x3,
    LdrModulesSnapping = 0x4,
    LdrModulesSnapped = 0x5,
    LdrModulesCondensed = 0x6,
    LdrModulesReadyToInit = 0x7,
    LdrModulesInitializing = 0x8,
    LdrModulesReadyToRun = 0x9,
} LDR_DDAG_STATE, *PLDR_DDAG_STATE;

typedef struct _LDR_SERVICE_TAG_RECORD
{
    struct _LDR_SERVICE_TAG_RECORD * Next;
    ULONG ServiceTag;
} LDR_SERVICE_TAG_RECORD, *PLDR_SERVICE_TAG_RECORD;

typedef struct _LDRP_CSLIST
{
    PSINGLE_LIST_ENTRY Tail;
} LDRP_CSLIST, *PLDRP_CSLIST;

typedef struct _LDR_DDAG_NODE
{
    LIST_ENTRY Modules;
    PLDR_SERVICE_TAG_RECORD ServiceTagList;
    ULONG LoadCount;
    ULONG LoadWhileUnloadingCount;
    ULONG LowestLink;
    LDRP_CSLIST Dependencies;
    LDRP_CSLIST IncomingDependencies;
    LDR_DDAG_STATE State;
    SINGLE_LIST_ENTRY CondenseLink;
    ULONG PreorderNumber;
} LDR_DDAG_NODE, *PLDR_DDAG_NODE;

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    LIST_ENTRY InMemoryOrderLinks;
    LIST_ENTRY InInitializationOrderLinks;
    PVOID DllBase;                             // Base address of the module
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;

    union
    {
        UCHAR FlagGroup[4];
        ULONG Flags;
        struct
        {
            ULONG PackagedBinary:1;
            ULONG MarkedForRemoval:1;
            ULONG ImageDll:1;
            ULONG LoadNotificationsSent:1;
            ULONG TelemetryEntryProcessed:1;
            ULONG ProcessStaticImport:1;
            ULONG InLegacyLists:1;
            ULONG InIndexes:1;
            ULONG ShimDll:1;
            ULONG InExceptionTable:1;
            ULONG VerifierProvider:1;
            ULONG ShimEngineCalloutSent:1;
            ULONG LoadInProgress:1;
            ULONG LoadConfigProcessed:1;
            ULONG EntryProcessed:1;
            ULONG ProtectDelayLoad:1;
            ULONG AuxIatCopyPrivate:1;
            ULONG ReservedFlags3:1;
            ULONG DontCallForThreads:1;
            ULONG ProcessAttachCalled:1;
            ULONG ProcessAttachFailed:1;
            ULONG ScpInExceptionTable:1;
            ULONG CorImage:1;
            ULONG DontRelocate:1;
            ULONG CorILOnly:1;
            ULONG ChpeImage:1;
            ULONG ChpeEmulatorImage:1;
            ULONG ReservedFlags5:1;
            ULONG Redirected:1;
            ULONG ReservedFlags6:2;
            ULONG CompatDatabaseProcessed:1;
        };
    };

    USHORT ObsoleteLoadCount;
    USHORT TlsIndex;
    LIST_ENTRY HashLinks;
    ULONG TimeDateStamp;
    struct _ACTIVATION_CONTEXT * EntryPointActivationContext;
    PVOID Lock;
    PLDR_DDAG_NODE DdagNode;
    LIST_ENTRY NodeModuleLink;
    struct _LDRP_LOAD_CONTEXT * LoadContext;

    PVOID ParentDllBase;
    PVOID SwitchBackContext;
    RTL_BALANCED_NODE BaseAddressIndexNode;
    RTL_BALANCED_NODE MappingInfoIndexNode;
    ULONG_PTR OriginalBase;
    LARGE_INTEGER LoadTime;
    ULONG BaseNameHashValue;
    LDR_DLL_LOAD_REASON LoadReason;
    ULONG ImplicitPathOptions;
    ULONG ReferenceCount;
    ULONG DependentLoadFlags;
    UCHAR SigningLevel;
    ULONG CheckSum;
    PVOID ActivePatchImageBase;
    LDR_HOT_PATCH_STATE HotPatchState;

} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _RTL_USER_PROCESS_PARAMETERS
{
    ULONG MaximumLength;                            // Should be set before call RtlCreateProcessParameters
    ULONG Length;                                   // Length of valid structure
    ULONG Flags;                                    // RTL_USER_PROC_PARAMS_XXX
    ULONG DebugFlags;

    PVOID ConsoleHandle;                            // HWND to console window associated with process (if any).
    ULONG ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;

    CURDIR CurrentDirectory;                        // Specified in DOS-like symbolic link path, ex: "C:/WinNT/SYSTEM32"
    UNICODE_STRING DllPath;                         // DOS-like paths separated by ';' where system should search for DLL files.
    UNICODE_STRING ImagePathName;                   // Full path in DOS-like format to process'es file image.
    UNICODE_STRING CommandLine;                     // Command line
    PVOID Environment;                              // Pointer to environment block (see RtlCreateEnvironment)
    ULONG StartingX;
    ULONG StartingY;
    ULONG CountX;
    ULONG CountY;
    ULONG CountCharsX;
    ULONG CountCharsY;
    ULONG FillAttribute;                            // Fill attribute for console window
    ULONG WindowFlags;
    ULONG ShowWindowFlags;
    UNICODE_STRING WindowTitle;
    UNICODE_STRING DesktopInfo;                     // Name of WindowStation and Desktop objects, where process is assigned
    UNICODE_STRING ShellInfo;
    UNICODE_STRING RuntimeData;
    RTL_DRIVE_LETTER_CURDIR CurrentDirectores[0x20];

    ULONG EnvironmentSize;
    ULONG EnvironmentVersion;
    PVOID PackageDependencyData;
    ULONG ProcessGroupId;
    ULONG LoaderThreads;

} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

#ifdef _WIN64
#define PEB_GDI_HANDLES             60
#define TEB_SYSTEM_RESERVED         30
#define INSTRUMENTATION_ARRAY_SIZE  11
#define WIN64_ADD_PADDING(n, c)     UCHAR n[c];
#else
#define PEB_GDI_HANDLES             34
#define TEB_SYSTEM_RESERVED         26
#define INSTRUMENTATION_ARRAY_SIZE  9
#define WIN64_ADD_PADDING(n,c)      /* NOTHING */
#endif


#pragma pack(push, 8)
typedef struct _PEB
{
    /* 0x000 */ UCHAR InheritedAddressSpace;
    /* 0x001 */ UCHAR ReadImageFileExecOptions;
    /* 0x002 */ UCHAR BeingDebugged;

    /* 0x003 */ union
                {
                    UCHAR BitField;
                    struct
                    {
                        UCHAR ImageUsesLargePages : 1;
                        UCHAR IsProtectedProcess : 1;
                        UCHAR IsImageDynamicallyRelocated : 1;
                        UCHAR SkipPatchingUser32Forwarders : 1;
                        UCHAR IsPackagedProcess : 1;
                        UCHAR IsAppContainer : 1;
                        UCHAR IsProtectedProcessLight : 1;
                        UCHAR IsLongPathAwareProcess : 1;
                    };
                };

    WIN64_ADD_PADDING(Padding0, 4)

    /* 0x004 */ PVOID Mutant;
    /* 0x008 */ PVOID ImageBaseAddress;
    /* 0x00c */ PPEB_LDR_DATA Ldr;
    /* 0x010 */ PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    /* 0x014 */ PVOID SubSystemData;
    /* 0x018 */ HANDLE ProcessHeap;
    /* 0x01c */ PRTL_CRITICAL_SECTION FastPebLock;
    /* 0x020 */ PSLIST_HEADER AtlThunkSListPtr;
    /* 0x024 */ PVOID IFEOKey;

    /* 0x028 */ union
                {
                    ULONG CrossProcessFlags;
                    struct
                    {
                        ULONG ProcessInJob : 1;
                        ULONG ProcessInitializing : 1;
                        ULONG ProcessUsingVEH : 1;
                        ULONG ProcessUsingVCH : 1;
                        ULONG ProcessUsingFTH : 1;
                        ULONG ProcessPreviouslyThrottled : 1;
                        ULONG ProcessCurrentlyThrottled : 1;
                        ULONG ProcessImagesHotPatched : 1;
                        ULONG ReservedBits0 : 24;
                    };
                };

    WIN64_ADD_PADDING(Padding1, 4)

    /* 0x02C */ union
                {
                    PVOID KernelCallbackTable;
                    PVOID UserSharedInfoPtr;
                };

    /* 0x030 */ ULONG SystemReserved;

#ifdef _WIN64
                ULONG AtlThunkSListPtr32;
#else
    /* 0x034 */ PSLIST_HEADER AtlThunkSListPtr32;
#endif

    /* 0x038 */ PVOID ApiSetMap;
    /* 0x03c */ ULONG TlsExpansionCounter;

    WIN64_ADD_PADDING(Padding2, 4)

    /* 0x040 */ PRTL_BITMAP TlsBitmap;
    /* 0x044 */ ULONG TlsBitmapBits[2];
    /* 0x04c */ PVOID ReadOnlySharedMemoryBase;
    /* 0x050 */ PBYTE SharedData;
    /* 0x054 */ PVOID * ReadOnlyStaticServerData;
    /* 0x058 */ PVOID AnsiCodePageData;
    /* 0x05c */ PVOID OemCodePageData;
    /* 0x060 */ PVOID UnicodeCaseTableData;
    /* 0x064 */ ULONG NumberOfProcessors;
    /* 0x068 */ ULONG NtGlobalFlag;
    /* 0x070 */ LARGE_INTEGER CriticalSectionTimeout;
    /* 0x078 */ SIZE_T HeapSegmentReserve;
    /* 0x07c */ SIZE_T HeapSegmentCommit;
    /* 0x080 */ SIZE_T HeapDeCommitTotalFreeThreshold;
    /* 0x084 */ SIZE_T HeapDeCommitFreeBlockThreshold;
    /* 0x088 */ ULONG NumberOfHeaps;
    /* 0x08c */ ULONG MaximumNumberOfHeaps;
    /* 0x090 */ PHANDLE ProcessHeaps;
    /* 0x094 */ PVOID GdiSharedHandleTable;
    /* 0x098 */ PVOID ProcessStarterHelper;
    /* 0x09c */ ULONG GdiDCAttributeList;

    WIN64_ADD_PADDING(Padding3, 4)

    /* 0x0a0 */ PRTL_CRITICAL_SECTION LoaderLock;
    /* 0x0a4 */ ULONG OSMajorVersion;
    /* 0x0a8 */ ULONG OSMinorVersion;
    /* 0x0ac */ USHORT OSBuildNumber;
    /* 0x0ae */ USHORT OSCSDVersion;
    /* 0x0b0 */ ULONG OSPlatformId;
    /* 0x0b4 */ ULONG ImageSubsystem;
    /* 0x0b8 */ ULONG ImageSubsystemMajorVersion;
    /* 0x0bc */ ULONG ImageSubsystemMinorVersion;

    WIN64_ADD_PADDING(Padding4, 4)

    /* 0x0c0 */ ULONG_PTR ActiveProcessAffinityMask;
    /* 0x0c4 */ ULONG GdiHandleBuffer[PEB_GDI_HANDLES];
    /* 0x14c */ void(WINAPI * PostProcessInitRoutine)();
    /* 0x150 */ PVOID TlsExpansionBitmap;
    /* 0x154 */ ULONG TlsExpansionBitmapBits[32];
    /* 0x1d4 */ ULONG SessionId;

    WIN64_ADD_PADDING(Padding5, 4)

    /* 0x1d8 */ ULARGE_INTEGER AppCompatFlags;
    /* 0x1e0 */ ULARGE_INTEGER AppCompatFlagsUser;
    /* 0x1e8 */ PVOID pShimData;
    /* 0x1ec */ PVOID AppCompatInfo;
    /* 0x1f0 */ UNICODE_STRING CSDVersion;
    /* 0x1f8 */ struct _ACTIVATION_CONTEXT_DATA * ActivationContextData;
    /* 0x1fc */ struct _ASSEMBLY_STORAGE_MAP * ProcessAssemblyStorageMap;
    /* 0x200 */ struct _ACTIVATION_CONTEXT_DATA * SystemDefaultActivationContextData;
    /* 0x204 */ struct _PASSEMBLY_STORAGE_MAP * SystemAssemblyStorageMap;
    /* 0x208 */ ULONG MinimumStackCommit;
    /* 0x20c */ PVOID SparePointers[4];
    /* 0x21c */ ULONG SpareUlongs[5];
    /* 0x230 */ PVOID WerRegistrationData;
    /* 0x234 */ PVOID WerShipAssertPtr;
    /* 0x238 */ PVOID pUnused;
    /* 0x23c */ PVOID pImageHeaderHash;

    /* 0x240 */ union
                {
                    ULONG TracingFlags;
                    struct
                    {
                        ULONG HeapTracingEnabled : 1;
                        ULONG CritSecTracingEnabled : 1;
                        ULONG LibLoaderTracingEnabled : 1;
                        ULONG SpareTracingBits : 29;
                    };
                };

    WIN64_ADD_PADDING(Padding6, 4)

    /* 0x248 */ ULONGLONG CsrServerReadOnlySharedMemoryBase;
    /* 0x250 */ ULONG TppWorkerpListLock;
    /* 0x254 */ LIST_ENTRY TppWorkerpList;
    /* 0x25C */ void * WaitOnAddressHashTable[128];

} PEB, * PPEB;
#pragma pack(pop)


//
// Thread environment block
//

typedef struct _GDI_TEB_BATCH
{
    ULONG Offset : 31;
    ULONG HasRenderingCommand : 1;
    HANDLE HDC;
    ULONG Buffer[310];
} GDI_TEB_BATCH, * PGDI_TEB_BATCH;

typedef struct _ACTIVATION_CONTEXT_STACK
{
    struct _RTL_ACTIVATION_CONTEXT_STACK_FRAME * ActiveFrame;
    LIST_ENTRY FrameListCache;
    ULONG Flags;
    ULONG NextCookieSequenceNumber;
    ULONG StackId;
} ACTIVATION_CONTEXT_STACK, * PACTIVATION_CONTEXT_STACK;

typedef struct _TEB
{
    /* 0x000 */ NT_TIB NtTib;
    /* 0x01c */ PVOID  EnvironmentPointer;
    /* 0x020 */ CLIENT_ID ClientId;
    /* 0x028 */ HANDLE ActiveRpcHandle;
    /* 0x02c */ PVOID  ThreadLocalStoragePointer;
    /* 0x030 */ PPEB   ProcessEnvironmentBlock;
    /* 0x034 */ ULONG  LastErrorValue;
    /* 0x038 */ ULONG  CountOfOwnedCriticalSections;
    /* 0x03c */ HANDLE CsrClientThread;
    /* 0x040 */ PVOID  Win32ThreadInfo;
    /* 0x044 */ ULONG  User32Reserved[26];
    /* 0x0ac */ ULONG  UserReserved[5];
    /* 0x0c0 */ PVOID  WOW32Reserved;
    /* 0x0c4 */ ULONG  CurrentLocale;
    /* 0x0c8 */ ULONG  FpSoftwareStatusRegister;
    /* 0x0cc */ PVOID  ReservedForDebuggerInstrumentation[16];
    /* 0x10c */ PVOID  SystemReserved1[TEB_SYSTEM_RESERVED];
    /* 0x174 */ CHAR PlaceholderCompatibilityMode;
    /* 0x175 */ UCHAR PlaceholderHydrationAlwaysExplicit;
    /* 0x176 */ CHAR PlaceholderReserved[10];
    /* 0x180 */ ULONG ProxiedProcessId;
    /* 0x184 */ ACTIVATION_CONTEXT_STACK _ActivationStack;
    /* 0x19c */ UCHAR  WorkingOnBehalfTicket[8];
    /* 0x1a4 */ NTSTATUS ExceptionCode;

    WIN64_ADD_PADDING(Padding0, 4)

    /* 0x1a8 */ PACTIVATION_CONTEXT_STACK ActivationContextStackPointer;
    /* 0x1ac */ ULONG_PTR InstrumentationCallbackSp;
    /* 0x1b0 */ ULONG_PTR InstrumentationCallbackPreviousPc;
    /* 0x1b4 */ ULONG_PTR InstrumentationCallbackPreviousSp;

#ifdef _WIN64
    ULONG TxFsContext;
    UCHAR InstrumentationCallbackDisabled;
    UCHAR UnalignedLoadStoreExceptions;
    WIN64_ADD_PADDING(Padding1, 2)
#else
    /* 0x1b8 */ UCHAR InstrumentationCallbackDisabled;
    /* 0x1b9 */ UCHAR SpareBytes[23];
    /* 0x1d0 */ ULONG TxFsContext;
#endif

    /* 0x1d4 */ GDI_TEB_BATCH GdiTebBatch;
    /* 0x6b4 */ CLIENT_ID RealClientId;
    /* 0x6bc */ PVOID GdiCachedProcessHandle;
    /* 0x6c0 */ ULONG GdiClientPID;
    /* 0x6c4 */ ULONG GdiClientTID;
    /* 0x6c8 */ PVOID GdiThreadLocalInfo;
    /* 0x6cc */ ULONG_PTR Win32ClientInfo[62];
    /* 0x7c4 */ PVOID glDispatchTable[233];
    /* 0xb68 */ ULONG_PTR glReserved1[29];
    /* 0xbdc */ PVOID glReserved2;
    /* 0xbe0 */ PVOID glSectionInfo;
    /* 0xbe4 */ PVOID glSection;
    /* 0xbe8 */ PVOID glTable;
    /* 0xbec */ PVOID glCurrentRC;
    /* 0xbf0 */ PVOID glContext;
    /* 0xbf4 */ NTSTATUS LastStatusValue;

    WIN64_ADD_PADDING(Padding2, 4)

    /* 0xbf8 */ UNICODE_STRING StaticUnicodeString;
    /* 0xc00 */ WCHAR StaticUnicodeBuffer[MAX_PATH + 1];

    WIN64_ADD_PADDING(Padding3, 6)

    /* 0xe0c */ PVOID DeallocationStack;
    /* 0xe10 */ PVOID TlsSlots[64];
    /* 0xf10 */ LIST_ENTRY TlsLinks;
    /* 0xf18 */ PVOID Vdm;
    /* 0xf1c */ PVOID ReservedForNtRpc;
    /* 0xf20 */ PVOID DbgSsReserved[2];
    /* 0xf28 */ ULONG HardErrorMode;

    WIN64_ADD_PADDING(Padding4, 4)

    /* 0xf2c */ PVOID Instrumentation[INSTRUMENTATION_ARRAY_SIZE];
    /* 0xf50 */ GUID  ActivityId;
    /* 0xf60 */ PVOID SubProcessTag;
    /* 0xf64 */ PVOID PerflibData;
    /* 0xf68 */ PVOID EtwTraceData;
    /* 0xf6c */ PVOID WinSockData;
    /* 0xf70 */ ULONG GdiBatchCount;

    /* 0xf74 */ union
                {
                    PROCESSOR_NUMBER CurrentIdealProcessor;
                    ULONG IdealProcessorValue;

                    struct
                    {
                        UCHAR ReservedPad0;
                        UCHAR ReservedPad1;
                        UCHAR ReservedPad2;
                        UCHAR IdealProcessor;
                    };
                };

    /* 0xf78 */ ULONG GuaranteedStackBytes;

    WIN64_ADD_PADDING(Padding5, 4)

    /* 0xf7c */ PVOID ReservedForPerf;
    /* 0xf80 */ PVOID ReservedForOle;
    /* 0xf84 */ ULONG WaitingOnLoaderLock;

    WIN64_ADD_PADDING(Padding6, 4)

    /* 0xf88 */ PVOID SavedPriorityState;
    /* 0xf8c */ ULONG ReservedForCodeCoverage;
    /* 0xf90 */ PVOID ThreadPoolData;
    /* 0xf94 */ PVOID * TlsExpansionSlots;

#ifdef _WIN64
    PVOID DeallocationBStore;
    PVOID BStoreLimit;
#endif

    /* 0xf98 */ ULONG MuiGeneration;
    /* 0xf9c */ ULONG IsImpersonating;
    /* 0xfa0 */ PVOID NlsCache;
    /* 0xfa4 */ PVOID pShimData;
    /* 0xfa8 */ ULONG HeapData;

    WIN64_ADD_PADDING(Padding7, 4)

    /* 0xfac */ HANDLE CurrentTransactionHandle;
    /* 0xfb0 */ struct _TEB_ACTIVE_FRAME * ActiveFrame;
    /* 0xfb4 */ PVOID FlsData;
    /* 0xfb8 */ PVOID PreferredLanguages;
    /* 0xfbc */ PVOID UserPrefLanguages;
    /* 0xfc0 */ PVOID MergedPrefLanguages;
    /* 0xfc4 */ ULONG MuiImpersonation;

    /* 0xfc8 */ union
                {
                    USHORT CrossTebFlags;
                    USHORT SpareCrossTebBits : 16;
                };

    /* 0xfca */ union
                {
                    USHORT SameTebFlags;
                    struct
                    {
                        USHORT SafeThunkCall : 1;
                        USHORT InDebugPrint : 1;
                        USHORT HasFiberData : 1;
                        USHORT SkipThreadAttach : 1;
                        USHORT WerInShipAssertCode : 1;
                        USHORT RanProcessInit : 1;
                        USHORT ClonedThread : 1;
                        USHORT SuppressDebugMsg : 1;
                        USHORT DisableUserStackWalk : 1;
                        USHORT RtlExceptionAttached : 1;
                        USHORT InitialThread : 1;
                        USHORT SessionAware : 1;
                        USHORT LoadOwner : 1;
                        USHORT LoaderWorker : 1;
                        USHORT SkipLoaderInit : 1;
                        USHORT SpareSameTebBits : 1;
                    };
                };

    /* 0xfcc */ PVOID TxnScopeEnterCallback;
    /* 0xfd0 */ PVOID TxnScopeExitCallback;
    /* 0xfd4 */ PVOID TxnScopeContext;
    /* 0xfd8 */ ULONG LockCount;
    /* 0xfdc */ ULONG WowTebOffset;
    /* 0xfe0 */ PVOID ResourceRetValue;
    /* 0xfe4 */ PVOID ReservedForWdf;
    /* 0xfe8 */ ULONGLONG ReservedForCrt;
    /* 0xff0 */ GUID EffectiveContainerId;

} TEB, * PTEB;

//
// Image load config from Windows 10 build 19041
//

// Flags for IMAGE_LOAD_CONFIG_DIRECTORY32_WIN10::GuardFlags
#ifndef IMAGE_GUARD_CF_INSTRUMENTED
#define IMAGE_GUARD_CF_INSTRUMENTED                    0x00000100 // Module performs control flow integrity checks using system-supplied support
#define IMAGE_GUARD_CFW_INSTRUMENTED                   0x00000200 // Module performs control flow and write integrity checks
#define IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT          0x00000400 // Module contains valid control flow target metadata
#define IMAGE_GUARD_SECURITY_COOKIE_UNUSED             0x00000800 // Module does not make use of the /GS security cookie
#define IMAGE_GUARD_PROTECT_DELAYLOAD_IAT              0x00001000 // Module supports read only delay load IAT
#define IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION   0x00002000 // Delayload import table in its own .didat section (with nothing else in it) that can be freely reprotected
#define IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT 0x00004000 // Module contains suppressed export information. This also infers that the address taken
                                                                  // taken IAT table is also present in the load config.
#define IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION       0x00008000 // Module enables suppression of exports
#define IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT          0x00010000 // Module contains longjmp target information
#define IMAGE_GUARD_RF_INSTRUMENTED                    0x00020000 // Module contains return flow instrumentation and metadata
#define IMAGE_GUARD_RF_ENABLE                          0x00040000 // Module requests that the OS enable return flow protection
#define IMAGE_GUARD_RF_STRICT                          0x00080000 // Module requests that the OS enable return flow protection in strict mode
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK        0xF0000000 // Stride of Guard CF function table encoded in these bits (additional count of bytes per element)
#define IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_SHIFT       28         // Shift to right-justify Guard CF function table stride

typedef struct _IMAGE_LOAD_CONFIG_CODE_INTEGRITY
{
    WORD    Flags;                      // Flags to indicate if CI information is available, etc.
    WORD    Catalog;                    // 0xFFFF means not available
    DWORD   CatalogOffset;
    DWORD   Reserved;                   // Additional bitmask to be defined later
} IMAGE_LOAD_CONFIG_CODE_INTEGRITY, * PIMAGE_LOAD_CONFIG_CODE_INTEGRITY;
#endif

#define CREATE_IMAGE_LOAD_CONFIG_DIRECTORY(cfg_name, sztype)        \
typedef struct _##cfg_name                                          \
{                                                                   \
    DWORD   Size;                                                   \
    DWORD   TimeDateStamp;                                          \
    WORD    MajorVersion;                                           \
    WORD    MinorVersion;                                           \
    DWORD   GlobalFlagsClear;                                       \
    DWORD   GlobalFlagsSet;                                         \
    DWORD   CriticalSectionDefaultTimeout;                          \
    sztype  DeCommitFreeBlockThreshold;                             \
    sztype  DeCommitTotalFreeThreshold;                             \
    sztype  LockPrefixTable;                            /* VA */    \
    sztype  MaximumAllocationSize;                                  \
    sztype  VirtualMemoryThreshold;                                 \
    sztype  ProcessAffinityMask;                                    \
    DWORD   ProcessHeapFlags;                                       \
    WORD    CSDVersion;                                             \
    WORD    DependentLoadFlags;                                     \
    sztype  EditList;                                   /* VA */    \
    sztype  SecurityCookie;                             /* VA */    \
                                                                    \
    /* Extra fields for Windows Vista */                            \
    sztype  SEHandlerTable;                             /* VA */    \
    sztype  SEHandlerCount;                                         \
                                                                    \
    /* Extra fields for Windows 10 */                               \
    sztype  GuardCFCheckFunctionPointer;                /* VA */    \
    sztype  GuardCFDispatchFunctionPointer;             /* VA */    \
    sztype  GuardCFFunctionTable;                       /* VA */    \
    sztype  GuardCFFunctionCount;                                   \
    DWORD   GuardFlags;                                             \
    IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;                 \
                                                                    \
    sztype  GuardAddressTakenIatEntryTable;             /* VA */    \
    sztype  GuardAddressTakenIatEntryCount;                         \
    sztype  GuardLongJumpTargetTable;                   /* VA */    \
    sztype  GuardLongJumpTargetCount;                               \
    sztype  DynamicValueRelocTable;                     /* VA */    \
    sztype  CHPEMetadataPointer;                                    \
    sztype  GuardRFFailureRoutine;                      /* VA */    \
    sztype  GuardRFFailureRoutineFunctionPointer;       /* VA */    \
    DWORD   DynamicValueRelocTableOffset;                           \
    WORD    DynamicValueRelocTableSection;                          \
    WORD    Reserved2;                                              \
    sztype  GuardRFVerifyStackPointerFunctionPointer;   /* VA */    \
    DWORD   HotPatchTableOffset;                                    \
    DWORD   Reserved3;                                              \
                                                                    \
    sztype EnclaveConfigurationPointer;                 /* VA */    \
    sztype VolatileMetadataPointer;                     /* VA */    \
    sztype GuardEHContinuationTable;                    /* VA */    \
    sztype GuardEHContinuationCount;                                \
    sztype GuardXFGCheckFunctionPointer;                /* VA */    \
    sztype GuardXFGDispatchFunctionPointer;             /* VA */    \
    sztype GuardXFGTableDispatchFunctionPointer;        /* VA */    \
    sztype CastGuardOsDeterminedFailureMode;            /* VA */    \
    sztype GuardMemcpyFunctionPointer;                  /* VA */    \
    sztype UmaFunctionPointers;                         /* VA */    \
                                                                    \
} cfg_name, *P##cfg_name

CREATE_IMAGE_LOAD_CONFIG_DIRECTORY(IMAGE_LOAD_CONFIG_DIRECTORY32_W10, DWORD);
CREATE_IMAGE_LOAD_CONFIG_DIRECTORY(IMAGE_LOAD_CONFIG_DIRECTORY64_W10, ULONGLONG);

#ifdef _WIN64
typedef IMAGE_LOAD_CONFIG_DIRECTORY64_W10     IMAGE_LOAD_CONFIG_DIRECTORY_W10;
typedef IMAGE_LOAD_CONFIG_DIRECTORY64_W10   *PIMAGE_LOAD_CONFIG_DIRECTORY_W10;
#else
typedef IMAGE_LOAD_CONFIG_DIRECTORY32_W10     IMAGE_LOAD_CONFIG_DIRECTORY_W10;
typedef IMAGE_LOAD_CONFIG_DIRECTORY32_W10   *PIMAGE_LOAD_CONFIG_DIRECTORY_W10;
#endif

#define RTL_IS_VALID_LOAD_CONFIG(cfg, size)   (cfg != NULL && size != 0 && (size == FIELD_OFFSET(IMAGE_LOAD_CONFIG_DIRECTORY_W10, SEHandlerTable) || size == cfg->Size))
#define RTL_IS_VALID_CONFIG_FIELD(cfg, name)  (cfg->Size >= FIELD_OFFSET(IMAGE_LOAD_CONFIG_DIRECTORY_W10, name) + sizeof(cfg->name))
#define RTL_IS_NONZERO_CONFIG_FIELD(cfg, name)  (cfg->Size >= (FIELD_OFFSET(IMAGE_LOAD_CONFIG_DIRECTORY_W10, name) + sizeof(cfg->name)) && cfg->name != 0)

//------------------------------------------------------------------------------
// Macros

#ifndef InitializeObjectAttributes
#define InitializeObjectAttributes( p, n, a, r, s ) {   \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }
#endif

//
// Macros for handling LIST_ENTRY-based lists
//

#if !defined(_WDMDDK_) && !defined(_LIST_ENTRY_MACROS_DEFINED_)
#define _LIST_ENTRY_MACROS_DEFINED_

BOOLEAN
FORCEINLINE
IsListEmpty(
    IN const LIST_ENTRY * ListHead
    )
{
    return (BOOLEAN)(ListHead->Flink == ListHead);
}

FORCEINLINE
VOID
InitializeListHead(
    IN PLIST_ENTRY ListHead
    )
{
    ListHead->Flink = ListHead->Blink = ListHead;
}

FORCEINLINE
VOID
InsertHeadList(
    IN OUT PLIST_ENTRY ListHead,
    IN OUT PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Flink;

    Flink = ListHead->Flink;
    Entry->Flink = Flink;
    Entry->Blink = ListHead;
    Flink->Blink = Entry;
    ListHead->Flink = Entry;
}

FORCEINLINE
VOID
InsertTailList(
    IN OUT PLIST_ENTRY ListHead,
    IN OUT PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Blink;

    Blink = ListHead->Blink;
    Entry->Flink = ListHead;
    Entry->Blink = Blink;
    Blink->Flink = Entry;
    ListHead->Blink = Entry;
}

FORCEINLINE
BOOLEAN
RemoveEntryList(
    IN PLIST_ENTRY Entry
    )
{
    PLIST_ENTRY Blink;
    PLIST_ENTRY Flink;

    Flink = Entry->Flink;
    Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;
    return (BOOLEAN)(Flink == Blink);
}
#endif  // #if !defined(_WDMDDK_) && !defined(_LIST_ENTRY_MACROS_DEFINED_)

//-----------------------------------------------------------------------------
// Bitmap functions

NTSYSAPI
VOID
NTAPI
RtlSetBit(
    PRTL_BITMAP BitMapHeader,
    ULONG BitNumber
    );

NTSYSAPI
VOID
NTAPI
RtlSetBits(
    PRTL_BITMAP BitMapHeader,
    ULONG StartingIndex,
    ULONG NumberToSet
    );

//-----------------------------------------------------------------------------
// Unicode string functions

NTSYSAPI
VOID
NTAPI
RtlInitString(
    PSTRING DestinationString,
    PCSTR SourceString
    );

NTSYSAPI
VOID
NTAPI
RtlInitUnicodeString(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlInitUnicodeStringEx(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlCreateUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN  PCWSTR SourceString
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlCreateUnicodeStringFromAsciiz(
    OUT PUNICODE_STRING Destination,
    IN  PCSTR Source
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlPrefixUnicodeString(
    IN PUNICODE_STRING String1,
    IN PUNICODE_STRING String2,
    IN BOOLEAN CaseInSensitive
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDuplicateUnicodeString(
    IN  BOOLEAN AllocateNew,
    IN  PUNICODE_STRING SourceString,
    OUT PUNICODE_STRING TargetString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAppendUnicodeToString(
    PUNICODE_STRING Destination,
    PCWSTR Source
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAppendUnicodeStringToString(
    IN OUT PUNICODE_STRING Destination,
    IN     PUNICODE_STRING Source
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToInteger(
    IN  PUNICODE_STRING String,
    IN  ULONG Base OPTIONAL,
    OUT PULONG Value
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlIntegerToUnicodeString(
    IN     ULONG Value,
    IN     ULONG Base OPTIONAL,
    IN OUT PUNICODE_STRING String
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGUIDFromString(
    IN  PUNICODE_STRING GuidString,
    OUT GUID *Guid
    );

NTSYSAPI
LONG
NTAPI
RtlCompareUnicodeString(
    IN PUNICODE_STRING String1,
    IN PUNICODE_STRING String2,
    IN BOOLEAN CaseInSensitive
    );

// Windows Vista or newer
NTSYSAPI
LONG
NTAPI
RtlCompareUnicodeStrings(
    IN PWCH String1,
    IN SIZE_T Length1,
    IN PWCH String2,
    IN SIZE_T Length2,
    IN BOOLEAN CaseInSensitive
    );

NTSYSAPI
VOID
NTAPI
RtlCopyUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN  PUNICODE_STRING SourceString
    );

NTSYSAPI
WCHAR
NTAPI
RtlUpcaseUnicodeChar(
    IN WCHAR SourceCharacter
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN  PUNICODE_STRING SourceString,
    IN  BOOLEAN AllocateDestinationString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDowncaseUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN  PUNICODE_STRING SourceString,
    IN  BOOLEAN AllocateDestinationString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeToMultiByteN(
    OUT PCH MultiByteString,
    IN  ULONG MaxBytesInMultiByteString,
    OUT PULONG BytesInMultiByteString OPTIONAL,
    IN  PWCH UnicodeString,
    IN  ULONG BytesInUnicodeString
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualUnicodeString(
    IN PUNICODE_STRING String1,
    IN PUNICODE_STRING String2,
    IN BOOLEAN CaseInSensitive
    );

NTSYSAPI
VOID
NTAPI
RtlFreeUnicodeString(
    IN  PUNICODE_STRING UnicodeString
    );

NTSYSAPI
WCHAR
NTAPI
RtlAnsiCharToUnicodeChar(
    IN OUT PUCHAR * SourceCharacter
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAnsiStringToUnicodeString(
    OUT PUNICODE_STRING DestinationString,
    IN  PANSI_STRING SourceString,
    IN  BOOLEAN AllocateDestinationString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToAnsiString(
    OUT PANSI_STRING DestinationString,
    IN  PUNICODE_STRING SourceString,
    IN  BOOLEAN AllocateDestinationString
    );

NTSYSAPI
VOID
NTAPI
RtlInitAnsiString(
    OUT PANSI_STRING DestinationString,
    IN  PCHAR SourceString
    );

NTSYSAPI
VOID
NTAPI
RtlFreeAnsiString(
    IN PANSI_STRING AnsiString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlFormatCurrentUserKeyPath(
    OUT PUNICODE_STRING CurrentUserKeyPath
    );

NTSYSAPI
VOID
NTAPI
RtlRaiseStatus(
    IN NTSTATUS Status
    );

NTSYSAPI
ULONG
NTAPI
RtlRandom(
    IN OUT PULONG Seed
);

//-----------------------------------------------------------------------------
// Debug functions

// Levels for DbgPrintEx
#define DPFLTR_ERROR_LEVEL      0
#define DPFLTR_WARNING_LEVEL    1
#define DPFLTR_TRACE_LEVEL      2
#define DPFLTR_INFO_LEVEL       3

// Component IDs for DbgPrintEx
typedef enum _DPFLTR_TYPE
{
    DPFLTR_SYSTEM_ID = 0,
    DPFLTR_SMSS_ID = 1,
    DPFLTR_SETUP_ID = 2,
    DPFLTR_NTFS_ID = 3,
    DPFLTR_FSTUB_ID = 4,
    DPFLTR_CRASHDUMP_ID = 5,
    DPFLTR_CDAUDIO_ID = 6,
    DPFLTR_CDROM_ID = 7,
    DPFLTR_CLASSPNP_ID = 8,
    DPFLTR_DISK_ID = 9,
    DPFLTR_REDBOOK_ID = 10,
    DPFLTR_STORPROP_ID = 11,
    DPFLTR_SCSIPORT_ID = 12,
    DPFLTR_SCSIMINIPORT_ID = 13,
    DPFLTR_CONFIG_ID = 14,
    DPFLTR_I8042PRT_ID = 15,
    DPFLTR_SERMOUSE_ID = 16,
    DPFLTR_LSERMOUS_ID = 17,
    DPFLTR_KBDHID_ID = 18,
    DPFLTR_MOUHID_ID = 19,
    DPFLTR_KBDCLASS_ID = 20,
    DPFLTR_MOUCLASS_ID = 21,
    DPFLTR_TWOTRACK_ID = 22,
    DPFLTR_WMILIB_ID = 23,
    DPFLTR_ACPI_ID = 24,
    DPFLTR_AMLI_ID = 25,
    DPFLTR_HALIA64_ID = 26,
    DPFLTR_VIDEO_ID = 27,
    DPFLTR_SVCHOST_ID = 28,
    DPFLTR_VIDEOPRT_ID = 29,
    DPFLTR_TCPIP_ID = 30,
    DPFLTR_DMSYNTH_ID = 31,
    DPFLTR_NTOSPNP_ID = 32,
    DPFLTR_FASTFAT_ID = 33,
    DPFLTR_SAMSS_ID = 34,
    DPFLTR_PNPMGR_ID = 35,
    DPFLTR_NETAPI_ID = 36,
    DPFLTR_SCSERVER_ID = 37,
    DPFLTR_SCCLIENT_ID = 38,
    DPFLTR_SERIAL_ID = 39,
    DPFLTR_SERENUM_ID = 40,
    DPFLTR_UHCD_ID = 41,
    DPFLTR_RPCPROXY_ID = 42,
    DPFLTR_AUTOCHK_ID = 43,
    DPFLTR_DCOMSS_ID = 44,
    DPFLTR_UNIMODEM_ID = 45,
    DPFLTR_SIS_ID = 46,
    DPFLTR_FLTMGR_ID = 47,
    DPFLTR_WMICORE_ID = 48,
    DPFLTR_BURNENG_ID = 49,
    DPFLTR_IMAPI_ID = 50,
    DPFLTR_SXS_ID = 51,
    DPFLTR_FUSION_ID = 52,
    DPFLTR_IDLETASK_ID = 53,
    DPFLTR_SOFTPCI_ID = 54,
    DPFLTR_TAPE_ID = 55,
    DPFLTR_MCHGR_ID = 56,
    DPFLTR_IDEP_ID = 57,
    DPFLTR_PCIIDE_ID = 58,
    DPFLTR_FLOPPY_ID = 59,
    DPFLTR_FDC_ID = 60,
    DPFLTR_TERMSRV_ID = 61,
    DPFLTR_W32TIME_ID = 62,
    DPFLTR_PREFETCHER_ID = 63,
    DPFLTR_RSFILTER_ID = 64,
    DPFLTR_FCPORT_ID = 65,
    DPFLTR_PCI_ID = 66,
    DPFLTR_DMIO_ID = 67,
    DPFLTR_DMCONFIG_ID = 68,
    DPFLTR_DMADMIN_ID = 69,
    DPFLTR_WSOCKTRANSPORT_ID = 70,
    DPFLTR_VSS_ID = 71,
    DPFLTR_PNPMEM_ID = 72,
    DPFLTR_PROCESSOR_ID = 73,
    DPFLTR_DMSERVER_ID = 74,
    DPFLTR_SR_ID = 75,
    DPFLTR_INFINIBAND_ID = 76,
    DPFLTR_IHVDRIVER_ID = 77,
    DPFLTR_IHVVIDEO_ID = 78,
    DPFLTR_IHVAUDIO_ID = 79,
    DPFLTR_IHVNETWORK_ID = 80,
    DPFLTR_IHVSTREAMING_ID = 81,
    DPFLTR_IHVBUS_ID = 82,
    DPFLTR_HPS_ID = 83,
    DPFLTR_RTLTHREADPOOL_ID = 84,
    DPFLTR_LDR_ID = 85,
    DPFLTR_TCPIP6_ID = 86,
    DPFLTR_ISAPNP_ID = 87,
    DPFLTR_SHPC_ID = 88,
    DPFLTR_STORPORT_ID = 89,
    DPFLTR_STORMINIPORT_ID = 90,
    DPFLTR_PRINTSPOOLER_ID = 91,
    DPFLTR_VSSDYNDISK_ID = 92,
    DPFLTR_VERIFIER_ID = 93,
    DPFLTR_VDS_ID = 94,
    DPFLTR_VDSBAS_ID = 95,
    DPFLTR_VDSDYNDR_ID = 96,
    DPFLTR_VDSUTIL_ID = 97,
    DPFLTR_DFRGIFC_ID = 98,
    DPFLTR_ENDOFTABLE_ID
} DPFLTR_TYPE;

NTSYSAPI
VOID
NTAPI
DbgBreakPoint(
    VOID
    );

NTSYSAPI
ULONG
_cdecl
DbgPrint(
    PCH Format,
    ...
    );

// Since Windows XP
NTSYSAPI
ULONG
_cdecl
DbgPrintEx(
    IN ULONG ComponentId,
    IN ULONG Level,
    IN PCSTR Format,
    ...
    );

//-----------------------------------------------------------------------------
// Critical section functions

NTSYSAPI
NTSTATUS
NTAPI
RtlInitializeCriticalSection(
    IN  PRTL_CRITICAL_SECTION CriticalSection
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlInitializeCriticalSectionAndSpinCount(
    IN  PRTL_CRITICAL_SECTION CriticalSection,
    IN  ULONG SpinCount
    );

NTSYSAPI
BOOL
NTAPI
RtlTryEnterCriticalSection(
    IN PRTL_CRITICAL_SECTION CriticalSection
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlEnterCriticalSection(
    IN PRTL_CRITICAL_SECTION CriticalSection
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlLeaveCriticalSection(
    IN PRTL_CRITICAL_SECTION CriticalSection
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDeleteCriticalSection(
    IN  PRTL_CRITICAL_SECTION CriticalSection
    );

//-----------------------------------------------------------------------------
// Compression and decompression

NTSYSAPI
NTSTATUS
NTAPI
RtlCompressBuffer(
    IN  USHORT CompressionFormatAndEngine,
    IN  PUCHAR UncompressedBuffer,
    IN  ULONG UncompressedBufferSize,
    OUT PUCHAR CompressedBuffer,
    IN  ULONG CompressedBufferSize,
    IN  ULONG UncompressedChunkSize,
    OUT PULONG FinalCompressedSize,
    IN  PVOID WorkSpace
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDecompressBuffer(
    IN  USHORT CompressionFormat,
    OUT PUCHAR UncompressedBuffer,
    IN  ULONG UncompressedBufferSize,
    IN  PUCHAR CompressedBuffer,
    IN  ULONG CompressedBufferSize,
    OUT PULONG FinalUncompressedSize
    );

//-----------------------------------------------------------------------------
// 8.3 name support

typedef struct _GENERATE_NAME_CONTEXT
{
    //
    //  The structure is divided into two strings.  The Name, and extension.
    //  Each part contains the value that was last inserted in the name.
    //  The length values are in terms of wchars and not bytes.  We also
    //  store the last index value used in the generation collision algorithm.
    //

    USHORT Checksum;
    BOOLEAN ChecksumInserted;

    UCHAR NameLength;                             // not including extension
    WCHAR NameBuffer[8];                          // e.g., "ntoskrnl"

    ULONG ExtensionLength;                        // including dot
    WCHAR ExtensionBuffer[4];                     // e.g., ".exe"

    ULONG LastIndexValue;

} GENERATE_NAME_CONTEXT, *PGENERATE_NAME_CONTEXT;

NTSYSAPI
BOOLEAN
NTAPI
RtlIsNameLegalDOS8Dot3(
    IN     PUNICODE_STRING Name,
    IN OUT POEM_STRING OemName OPTIONAL,
    OUT    PBOOLEAN NameContainsSpaces OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGenerate8dot3Name (
    IN     PCUNICODE_STRING Name,
    IN     BOOLEAN AllowExtendedCharacters,
    IN OUT PGENERATE_NAME_CONTEXT Context,
    IN OUT PUNICODE_STRING Name8dot3
    );

//-----------------------------------------------------------------------------
// Object functions

//
// Object Manager Directory Specific Access Rights.
//

#ifndef DIRECTORY_QUERY
#define DIRECTORY_QUERY                         0x0001
#define DIRECTORY_TRAVERSE                      0x0002
#define DIRECTORY_CREATE_OBJECT                 0x0004
#define DIRECTORY_CREATE_SUBDIRECTORY           0x0008
#define DIRECTORY_ALL_ACCESS                    (STANDARD_RIGHTS_REQUIRED | 0xF)
#endif

typedef enum _POOL_TYPE {
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType,
    NonPagedPoolCacheAligned,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS,
    MaxPoolType,
    NonPagedPoolSession = 32,
    PagedPoolSession,
    NonPagedPoolMustSucceedSession,
    DontUseThisTypeSession,
    NonPagedPoolCacheAlignedSession,
    PagedPoolCacheAlignedSession,
    NonPagedPoolCacheAlignedMustSSession

} POOL_TYPE;


//
// For NtQueryObject
//

typedef enum _OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation,
    ObjectNameInformation,
    ObjectTypeInformation,
    ObjectTypesInformation,
    ObjectHandleFlagInformation,
    ObjectSessionInformation,
    MaxObjectInfoClass

} OBJECT_INFORMATION_CLASS;

//
// NtQueryObject uses ObjectBasicInformation
//

typedef struct _OBJECT_BASIC_INFORMATION
{
    ULONG Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG HandleCount;
    ULONG PointerCount;
    ULONG PagedPoolUsage;
    ULONG NonPagedPoolUsage;
    ULONG Reserved[3];
    ULONG NameInformationLength;
    ULONG TypeInformationLength;
    ULONG SecurityDescriptorLength;
    LARGE_INTEGER CreateTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

//
// NtQueryObject uses ObjectNameInformation
//

typedef struct _OBJECT_NAME_INFORMATION
{
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;

//
// NtQueryObject uses ObjectTypeInformation
//

typedef struct _OBJECT_TYPE_INFORMATION
{
    UNICODE_STRING TypeName;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccessMask;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    ULONG PoolType;
    ULONG DefaultPagedPoolCharge;
    ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_TYPES_INFORMATION
{
    ULONG NumberOfTypes;
    OBJECT_TYPE_INFORMATION TypeInformation[1];
} OBJECT_TYPES_INFORMATION, *POBJECT_TYPES_INFORMATION;

//
// NtQueryObject uses ObjectHandleFlagInformation
// NtSetInformationObject uses ObjectHandleFlagInformation
//

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION
{
    BOOLEAN Inherit;
    BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

//
// NtQueryDirectoryObject uses this type
//

typedef struct _OBJECT_DIRECTORY_INFORMATION
{
    UNICODE_STRING Name;
    UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, *POBJECT_DIRECTORY_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
NtCreateDirectoryObject(
    OUT PHANDLE DirectoryHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenDirectoryObject(
    OUT PHANDLE DirectoryHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryObject(
    IN HANDLE DirectoryHandle,
    OUT    PVOID Buffer,
    IN     ULONG Length,
    IN     BOOLEAN ReturnSingleEntry,
    IN     BOOLEAN RestartScan,
    IN OUT PULONG Context,
    OUT    PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryObject (
    IN  HANDLE ObjectHandle,
    IN  OBJECT_INFORMATION_CLASS ObjectInformationClass,
    OUT PVOID ObjectInformation,
    IN  ULONG Length,
    OUT PULONG ResultLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationObject (
    IN HANDLE ObjectHandle,
    IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
    IN PVOID ObjectInformation,
    IN ULONG Length
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDuplicateObject (
    IN  HANDLE SourceProcessHandle,
    IN  HANDLE SourceHandle,
    IN  HANDLE TargetProcessHandle OPTIONAL,
    OUT PHANDLE TargetHandle OPTIONAL,
    IN  ACCESS_MASK DesiredAccess,
    IN  ULONG HandleAttributes,
    IN  ULONG Options
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySecurityObject (
    IN  HANDLE ObjectHandle,
    IN  SECURITY_INFORMATION SecurityInformation,
    OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN  ULONG DescriptorLength,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySecurityObject (
    IN  HANDLE ObjectHandle,
    IN  SECURITY_INFORMATION SecurityInformation,
    OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN  ULONG DescriptorLength,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetSecurityObject (
    IN HANDLE ObjectHandle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSecurityObject (
    IN HANDLE ObjectHandle,
    IN SECURITY_INFORMATION SecurityInformation,
    IN PSECURITY_DESCRIPTOR SecurityDescriptor
    );

NTSYSAPI
NTSTATUS
NTAPI
NtMakeTemporaryObject(
    IN HANDLE ObjectHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwMakeTemporaryObject(
    IN HANDLE ObjectHandle
    );

//-----------------------------------------------------------------------------
// Handle table RTL functions

#define LEVEL_HANDLE_ID         0x74000000
#define LEVEL_HANDLE_ID_MASK    0xFF000000
#define LEVEL_HANDLE_INDEX_MASK 0x00FFFFFF

typedef enum _RTL_GENERIC_COMPARE_RESULTS
{
    GenericLessThan,
    GenericGreaterThan,
    GenericEqual
} RTL_GENERIC_COMPARE_RESULTS;

typedef struct _RTL_SPLAY_LINKS
{
    struct _RTL_SPLAY_LINKS *Parent;
    struct _RTL_SPLAY_LINKS *LeftChild;
    struct _RTL_SPLAY_LINKS *RightChild;
} RTL_SPLAY_LINKS, *PRTL_SPLAY_LINKS;

struct _RTL_GENERIC_TABLE;

typedef
RTL_GENERIC_COMPARE_RESULTS
(NTAPI * PRTL_GENERIC_COMPARE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    PVOID FirstStruct,
    PVOID SecondStruct
    );

typedef
PVOID
(NTAPI *PRTL_GENERIC_ALLOCATE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    ULONG ByteSize
    );

typedef
VOID
(NTAPI *PRTL_GENERIC_FREE_ROUTINE) (
    struct _RTL_GENERIC_TABLE *Table,
    PVOID Buffer
    );

typedef struct _RTL_GENERIC_TABLE
{
    PRTL_SPLAY_LINKS TableRoot;
    LIST_ENTRY InsertOrderList;
    PLIST_ENTRY OrderedPointer;
    ULONG WhichOrderedElement;
    ULONG NumberGenericTableElements;
    PRTL_GENERIC_COMPARE_ROUTINE CompareRoutine;
    PRTL_GENERIC_ALLOCATE_ROUTINE AllocateRoutine;
    PRTL_GENERIC_FREE_ROUTINE FreeRoutine;
    PVOID TableContext;
} RTL_GENERIC_TABLE, *PRTL_GENERIC_TABLE;

typedef struct _RTL_HANDLE_TABLE_ENTRY
{
    ULONG Flags;
    struct _RTL_HANDLE_TABLE_ENTRY *NextFree;
} RTL_HANDLE_TABLE_ENTRY, *PRTL_HANDLE_TABLE_ENTRY;

typedef struct _RTL_HANDLE_TABLE
{
    ULONG MaximumNumberOfHandles;
    ULONG SizeOfHandleTableEntry;
    ULONG Reserved[2];
    PRTL_HANDLE_TABLE_ENTRY FreeHandles;
    PRTL_HANDLE_TABLE_ENTRY CommittedHandles;
    PRTL_HANDLE_TABLE_ENTRY UnCommittedHandles;
    PRTL_HANDLE_TABLE_ENTRY MaxReservedHandles;
} RTL_HANDLE_TABLE, *PRTL_HANDLE_TABLE;

NTSYSAPI
VOID
NTAPI
RtlInitializeGenericTable (
    IN PRTL_GENERIC_TABLE Table,
    IN PRTL_GENERIC_COMPARE_ROUTINE CompareRoutine,
    IN PRTL_GENERIC_ALLOCATE_ROUTINE AllocateRoutine,
    IN PRTL_GENERIC_FREE_ROUTINE FreeRoutine,
    IN PVOID TableContext
    );

NTSYSAPI
VOID
NTAPI
RtlInitializeHandleTable(
    IN  ULONG MaximumNumberOfHandles,
    IN  ULONG SizeOfHandleTableEntry,
    OUT PRTL_HANDLE_TABLE HandleTable
    );

NTSYSAPI
PRTL_HANDLE_TABLE_ENTRY
NTAPI
RtlAllocateHandle(
    IN  PRTL_HANDLE_TABLE HandleTable,
    OUT PULONG HandleIndex OPTIONAL
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHandle(
    IN PRTL_HANDLE_TABLE HandleTable,
    IN PRTL_HANDLE_TABLE_ENTRY Handle
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlIsValidIndexHandle(
    IN  PRTL_HANDLE_TABLE HandleTable,
    IN  ULONG HandleIndex,
    OUT PRTL_HANDLE_TABLE_ENTRY *Handle
    );

NTSYSAPI
PVOID
NTAPI
RtlInsertElementGenericTable (
    IN  PRTL_GENERIC_TABLE Table,
    IN  PVOID Buffer,
    IN  LONG BufferSize,
    OUT PBOOLEAN NewElement OPTIONAL
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlIsGenericTableEmpty (
    IN PRTL_GENERIC_TABLE Table
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlIsGenericTableEmpty (
    IN PRTL_GENERIC_TABLE Table
    );

NTSYSAPI
PVOID
NTAPI
RtlLookupElementGenericTable (
    IN PRTL_GENERIC_TABLE Table,
    IN PVOID Buffer
    );

NTSYSAPI
PVOID
NTAPI
RtlEnumerateGenericTableWithoutSplaying(
    IN  PRTL_GENERIC_TABLE Table,
    IN  PVOID *RestartKey
    );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
    IN  HANDLE Handle
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwClose(
    IN  HANDLE Handle
    );

//-----------------------------------------------------------------------------
// Environment functions

NTSYSAPI
NTSTATUS
NTAPI
RtlOpenCurrentUser(
    IN  ULONG DesiredAccess,
    OUT PHANDLE CurrentUserKey
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateEnvironment(
    BOOLEAN CloneCurrentEnvironment,
    PVOID *Environment
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlExpandEnvironmentStrings_U(
    IN  PVOID Environment OPTIONAL,
    IN  PUNICODE_STRING Source,
    OUT PUNICODE_STRING Destination,
    OUT PULONG ReturnedLength OPTIONAL
    );

// Since Windows Vista
NTSYSAPI
NTSTATUS
NTAPI
RtlQueryEnvironmentVariable(
    IN  PCWSTR Environment,
    IN  PCWSTR Name,
    IN  ULONG  NameLength,
    IN  PCWSTR Value,
    IN  ULONG  ValueLength,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlQueryEnvironmentVariable_U(
    IN  PVOID Environment,
    IN  PUNICODE_STRING Name,
    OUT PUNICODE_STRING Value
    );

NTSYSAPI
ULONG
NTAPI
RtlDosSearchPath_U(
    IN  PCWSTR lpPath,
    IN  PCWSTR lpFileName,
    IN  PCWSTR lpExtension OPTIONAL,
    IN  ULONG nBufferLength,
    OUT PWSTR lpBuffer,
    OUT PWSTR *lpFilePart
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetEnvironmentVariable(
    PVOID *Environment,
    PUNICODE_STRING Name,
    PUNICODE_STRING Value
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDestroyEnvironment(
    PVOID Environment
    );

//-----------------------------------------------------------------------------
// Registry functions

typedef enum _KEY_INFORMATION_CLASS
{
    KeyBasicInformation,
    KeyNodeInformation,
    KeyFullInformation,
    KeyNameInformation,
    KeyCachedInformation,
    KeyFlagsInformation

} KEY_INFORMATION_CLASS;

typedef enum _KEY_VALUE_INFORMATION_CLASS
{
    KeyValueBasicInformation = 0,
    KeyValueFullInformation,
    KeyValuePartialInformation,
    KeyValueFullInformationAlign64,
    KeyValuePartialInformationAlign64
} KEY_VALUE_INFORMATION_CLASS;

typedef enum _KEY_SET_INFORMATION_CLASS
{
    KeyWriteTimeInformation,
    KeyUserFlagsInformation,
    MaxKeySetInfoClass
} KEY_SET_INFORMATION_CLASS;

typedef struct _KEY_BASIC_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct _KEY_NODE_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG ClassOffset;
    ULONG ClassLength;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_NODE_INFORMATION, *PKEY_NODE_INFORMATION;

typedef struct _KEY_FULL_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG ClassOffset;
    ULONG ClassLength;
    ULONG SubKeys;
    ULONG MaxNameLen;
    ULONG MaxClassLen;
    ULONG Values;
    ULONG MaxValueNameLen;
    ULONG MaxValueDataLen;
    WCHAR Class[1];
} KEY_FULL_INFORMATION, *PKEY_FULL_INFORMATION;

typedef struct _KEY_NAME_INFORMATION
{
    WCHAR Name[1];
} KEY_NAME_INFORMATION, *PKEY_NAME_INFORMATION;

typedef struct _KEY_CACHED_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG SubKeys;
    ULONG MaxNameLen;
    ULONG Values;
    ULONG MaxValueNameLen;
    ULONG MaxValueDataLen;
    ULONG NameLength;
    WCHAR Name[1];            // Variable length string
} KEY_CACHED_INFORMATION, *PKEY_CACHED_INFORMATION;

typedef struct _KEY_FLAGS_INFORMATION
{
    ULONG   UserFlags;
} KEY_FLAGS_INFORMATION, *PKEY_FLAGS_INFORMATION;

typedef struct _KEY_VALUE_BASIC_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_BASIC_INFORMATION, *PKEY_VALUE_BASIC_INFORMATION;

typedef struct _KEY_VALUE_FULL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataOffset;
    ULONG DataLength;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_FULL_INFORMATION, *PKEY_VALUE_FULL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
NtLoadKey(
    IN POBJECT_ATTRIBUTES TargetKey,
    IN POBJECT_ATTRIBUTES SourceFile
    );

NTSYSAPI
NTSTATUS
NTAPI
NtLoadKey2(
    IN POBJECT_ATTRIBUTES TargetKey,
    IN POBJECT_ATTRIBUTES SourceFile,
    IN ULONG Flags
    );

// NtLoadKeyEx for Windows 2003 server
typedef NTSTATUS (NTAPI * NTLOADKEYEX_3790)(
    IN POBJECT_ATTRIBUTES TargetKey,
    IN POBJECT_ATTRIBUTES SourceFile,
    IN ULONG Flags,
    IN HANDLE TrustClassKey OPTIONAL
    );

// NtLoadKeyEx for Windows Vista to Windows 8.1
typedef NTSTATUS (NTAPI * NTLOADKEYEX)(
    IN POBJECT_ATTRIBUTES TargetKey,
    IN POBJECT_ATTRIBUTES SourceFile,
    IN ULONG Flags,
    IN HANDLE TrustClassKey OPTIONAL,
    IN PVOID Param5,
    IN PVOID Param6,
    IN PVOID Param7,
    IN PVOID Param8
    );

NTSYSAPI
NTSTATUS
NTAPI
NtUnloadKey(
    IN POBJECT_ATTRIBUTES TargetKey
    );

NTSYSAPI
NTSTATUS
NTAPI
NtCreateKey(
    OUT PHANDLE KeyHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  ULONG TitleIndex,
    IN  PUNICODE_STRING Class OPTIONAL,
    IN  ULONG CreateOptions,
    OUT PULONG Disposition OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenKey(
    OUT PHANDLE KeyHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtEnumerateKey(
    IN HANDLE KeyHandle,
    IN ULONG Index,
    IN KEY_INFORMATION_CLASS KeyInformationClass,
    IN PVOID KeyInformation,
    IN ULONG Length,
    IN PULONG ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwEnumerateKey(
    IN HANDLE KeyHandle,
    IN ULONG Index,
    IN KEY_INFORMATION_CLASS KeyInformationClass,
    IN PVOID KeyInformation,
    IN ULONG Length,
    IN PULONG ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtEnumerateValueKey(
    IN  HANDLE KeyHandle,
    IN  ULONG Index,
    IN  KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    OUT PVOID  KeyValueInformation,
    IN  ULONG  Length,
    OUT PULONG  ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwEnumerateValueKey(
    IN  HANDLE KeyHandle,
    IN  ULONG Index,
    IN  KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    OUT PVOID  KeyValueInformation,
    IN  ULONG  Length,
    OUT PULONG  ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteKey(
    IN HANDLE KeyHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryKey(
    IN  HANDLE KeyHandle,
    IN  KEY_INFORMATION_CLASS KeyInformationClass,
    OUT PVOID KeyInformation OPTIONAL,
    IN  ULONG Length,
    OUT PULONG ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryValueKey(
    IN  HANDLE KeyHandle,
    IN  PUNICODE_STRING ValueName,
    IN  KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    OUT PVOID KeyValueInformation,
    IN  ULONG Length,
    OUT PULONG ResultLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetValueKey(
    IN HANDLE KeyHandle,
    IN PUNICODE_STRING ValueName,
    IN ULONG TitleIndex OPTIONAL,
    IN ULONG Type,
    IN PVOID Data,
    IN ULONG DataSize
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteValueKey(
    IN HANDLE KeyHandle,
    IN PUNICODE_STRING ValueName
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushKey(
    IN HANDLE KeyHandle
    );

//-----------------------------------------------------------------------------
// RtlQueryRegistryValues

//
// The following flags specify how the Name field of a RTL_QUERY_REGISTRY_TABLE
// entry is interpreted.  A NULL name indicates the end of the table.
//

#define RTL_QUERY_REGISTRY_SUBKEY   0x00000001  // Name is a subkey and remainder of
                                                // table or until next subkey are value
                                                // names for that subkey to look at.

#define RTL_QUERY_REGISTRY_TOPKEY   0x00000002  // Reset current key to original key for
                                                // this and all following table entries.

#define RTL_QUERY_REGISTRY_REQUIRED 0x00000004  // Fail if no match found for this table
                                                // entry.

#define RTL_QUERY_REGISTRY_NOVALUE  0x00000008  // Used to mark a table entry that has no
                                                // value name, just wants a call out, not
                                                // an enumeration of all values.

#define RTL_QUERY_REGISTRY_NOEXPAND 0x00000010  // Used to suppress the expansion of
                                                // REG_MULTI_SZ into multiple callouts or
                                                // to prevent the expansion of environment
                                                // variable values in REG_EXPAND_SZ

#define RTL_QUERY_REGISTRY_DIRECT   0x00000020  // QueryRoutine field ignored.  EntryContext
                                                // field points to location to store value.
                                                // For null terminated strings, EntryContext
                                                // points to UNICODE_STRING structure that
                                                // that describes maximum size of buffer.
                                                // If .Buffer field is NULL then a buffer is
                                                // allocated.
                                                //

#define RTL_QUERY_REGISTRY_DELETE   0x00000040  // Used to delete value keys after they
                                                // are queried.


//
// The following values for the RelativeTo parameter determine what the
// Path parameter to RtlQueryRegistryValues is relative to.
//

#define RTL_REGISTRY_ABSOLUTE     0             // Path is an absolute registry path.
#define RTL_REGISTRY_SERVICES     1             // Path is relative to \Registry\Machine\System\CurrentControlSet\Services
#define RTL_REGISTRY_CONTROL      2             // Path is relative to \Registry\Machine\System\CurrentControlSet\Control
#define RTL_REGISTRY_WINDOWS_NT   3             // Path is relative to \Registry\Machine\Software\Microsoft\Windows NT\CurrentVersion
#define RTL_REGISTRY_DEVICEMAP    4             // Path is relative to \Registry\Machine\Hardware\DeviceMap
#define RTL_REGISTRY_USER         5             // Path is relative to \Registry\User\CurrentUser. (For a system process, this is \User\.Default)
#define RTL_REGISTRY_MAXIMUM      6
#define RTL_REGISTRY_HANDLE       0x40000000    // Specifies that the Path parameter is actually a registry handle to use
#define RTL_REGISTRY_OPTIONAL     0x80000000    // Specifies that the key referenced by this parameter and the Path parameter are optional

typedef NTSTATUS (NTAPI * PRTL_QUERY_REGISTRY_ROUTINE)(
    IN PWSTR ValueName,
    IN ULONG ValueType,
    IN PVOID ValueData,
    IN ULONG ValueLength,
    IN PVOID Context,
    IN PVOID EntryContext
    );

typedef struct _RTL_QUERY_REGISTRY_TABLE
{
    PRTL_QUERY_REGISTRY_ROUTINE QueryRoutine;
    ULONG Flags;
    PWSTR Name;
    PVOID EntryContext;
    ULONG DefaultType;
    PVOID DefaultData;
    ULONG DefaultLength;
} RTL_QUERY_REGISTRY_TABLE, *PRTL_QUERY_REGISTRY_TABLE;

NTSYSAPI
NTSTATUS
NTAPI
RtlQueryRegistryValues(
    IN ULONG  RelativeTo,
    IN PCWSTR  Path,
    IN PRTL_QUERY_REGISTRY_TABLE  QueryTable,
    IN PVOID  Context,
    IN PVOID  Environment OPTIONAL
    );

//-----------------------------------------------------------------------------
// Query system information
// Source: https://raw.githubusercontent.com/hfiref0x/KDU/master/Source/Shared/ntos/ntos.h

typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation = 0x0,
    SystemProcessorInformation = 0x1,
    SystemPerformanceInformation = 0x2,
    SystemTimeOfDayInformation = 0x3,
    SystemPathInformation = 0x4,
    SystemProcessInformation = 0x5,
    SystemCallCountInformation = 0x6,
    SystemDeviceInformation = 0x7,
    SystemProcessorPerformanceInformation = 0x8,
    SystemFlagsInformation = 0x9,
    SystemCallTimeInformation = 0xA,
    SystemModuleInformation = 0xB,
    SystemLocksInformation = 0xC,
    SystemStackTraceInformation = 0xD,
    SystemPagedPoolInformation = 0xE,
    SystemNonPagedPoolInformation = 0xF,
    SystemHandleInformation = 0x10,
    SystemObjectInformation = 0x11,
    SystemPageFileInformation = 0x12,
    SystemVdmInstemulInformation = 0x13,
    SystemVdmBopInformation = 0x14,
    SystemFileCacheInformation = 0x15,
    SystemPoolTagInformation = 0x16,
    SystemInterruptInformation = 0x17,
    SystemDpcBehaviorInformation = 0x18,
    SystemFullMemoryInformation = 0x19,
    SystemLoadGdiDriverInformation = 0x1A,
    SystemUnloadGdiDriverInformation = 0x1B,
    SystemTimeAdjustmentInformation = 0x1C,
    SystemSummaryMemoryInformation = 0x1D,
    SystemMirrorMemoryInformation = 0x1E,
    SystemPerformanceTraceInformation = 0x1F,
    SystemObsolete0 = 0x20,
    SystemExceptionInformation = 0x21,
    SystemCrashDumpStateInformation = 0x22,
    SystemKernelDebuggerInformation = 0x23,
    SystemContextSwitchInformation = 0x24,
    SystemRegistryQuotaInformation = 0x25,
    SystemExtendServiceTableInformation = 0x26,
    SystemPrioritySeperation = 0x27,
    SystemVerifierAddDriverInformation = 0x28,
    SystemVerifierRemoveDriverInformation = 0x29,
    SystemProcessorIdleInformation = 0x2A,
    SystemLegacyDriverInformation = 0x2B,
    SystemCurrentTimeZoneInformation = 0x2C,
    SystemLookasideInformation = 0x2D,
    SystemTimeSlipNotification = 0x2E,
    SystemSessionCreate = 0x2F,
    SystemSessionDetach = 0x30,
    SystemSessionInformation = 0x31,
    SystemRangeStartInformation = 0x32,
    SystemVerifierInformation = 0x33,
    SystemVerifierThunkExtend = 0x34,
    SystemSessionProcessInformation = 0x35,
    SystemLoadGdiDriverInSystemSpace = 0x36,
    SystemNumaProcessorMap = 0x37,
    SystemPrefetcherInformation = 0x38,
    SystemExtendedProcessInformation = 0x39,
    SystemRecommendedSharedDataAlignment = 0x3A,
    SystemComPlusPackage = 0x3B,
    SystemNumaAvailableMemory = 0x3C,
    SystemProcessorPowerInformation = 0x3D,
    SystemEmulationBasicInformation = 0x3E,
    SystemEmulationProcessorInformation = 0x3F,
    SystemExtendedHandleInformation = 0x40,
    SystemLostDelayedWriteInformation = 0x41,
    SystemBigPoolInformation = 0x42,
    SystemSessionPoolTagInformation = 0x43,
    SystemSessionMappedViewInformation = 0x44,
    SystemHotpatchInformation = 0x45,
    SystemObjectSecurityMode = 0x46,
    SystemWatchdogTimerHandler = 0x47,
    SystemWatchdogTimerInformation = 0x48,
    SystemLogicalProcessorInformation = 0x49,
    SystemWow64SharedInformationObsolete = 0x4A,
    SystemRegisterFirmwareTableInformationHandler = 0x4B,
    SystemFirmwareTableInformation = 0x4C,
    SystemModuleInformationEx = 0x4D,
    SystemVerifierTriageInformation = 0x4E,
    SystemSuperfetchInformation = 0x4F,
    SystemMemoryListInformation = 0x50,
    SystemFileCacheInformationEx = 0x51,
    SystemThreadPriorityClientIdInformation = 0x52,
    SystemProcessorIdleCycleTimeInformation = 0x53,
    SystemVerifierCancellationInformation = 0x54,
    SystemProcessorPowerInformationEx = 0x55,
    SystemRefTraceInformation = 0x56,
    SystemSpecialPoolInformation = 0x57,
    SystemProcessIdInformation = 0x58,
    SystemErrorPortInformation = 0x59,
    SystemBootEnvironmentInformation = 0x5A,
    SystemHypervisorInformation = 0x5B,
    SystemVerifierInformationEx = 0x5C,
    SystemTimeZoneInformation = 0x5D,
    SystemImageFileExecutionOptionsInformation = 0x5E,
    SystemCoverageInformation = 0x5F,
    SystemPrefetchPatchInformation = 0x60,
    SystemVerifierFaultsInformation = 0x61,
    SystemSystemPartitionInformation = 0x62,
    SystemSystemDiskInformation = 0x63,
    SystemProcessorPerformanceDistribution = 0x64,
    SystemNumaProximityNodeInformation = 0x65,
    SystemDynamicTimeZoneInformation = 0x66,
    SystemCodeIntegrityInformation = 0x67,
    SystemProcessorMicrocodeUpdateInformation = 0x68,
    SystemProcessorBrandString = 0x69,
    SystemVirtualAddressInformation = 0x6A,
    SystemLogicalProcessorAndGroupInformation = 0x6B,
    SystemProcessorCycleTimeInformation = 0x6C,
    SystemStoreInformation = 0x6D,
    SystemRegistryAppendString = 0x6E,
    SystemAitSamplingValue = 0x6F,
    SystemVhdBootInformation = 0x70,
    SystemCpuQuotaInformation = 0x71,
    SystemNativeBasicInformation = 0x72,
    SystemErrorPortTimeouts = 0x73,
    SystemLowPriorityIoInformation = 0x74,
    SystemBootEntropyInformation = 0x75,
    SystemVerifierCountersInformation = 0x76,
    SystemPagedPoolInformationEx = 0x77,
    SystemSystemPtesInformationEx = 0x78,
    SystemNodeDistanceInformation = 0x79,
    SystemAcpiAuditInformation = 0x7A,
    SystemBasicPerformanceInformation = 0x7B,
    SystemQueryPerformanceCounterInformation = 0x7C,
    SystemSessionBigPoolInformation = 0x7D,
    SystemBootGraphicsInformation = 0x7E,
    SystemScrubPhysicalMemoryInformation = 0x7F,
    SystemBadPageInformation = 0x80,
    SystemProcessorProfileControlArea = 0x81,
    SystemCombinePhysicalMemoryInformation = 0x82,
    SystemEntropyInterruptTimingInformation = 0x83,
    SystemConsoleInformation = 0x84,
    SystemPlatformBinaryInformation = 0x85,
    SystemPolicyInformation = 0x86,
    SystemHypervisorProcessorCountInformation = 0x87,
    SystemDeviceDataInformation = 0x88,
    SystemDeviceDataEnumerationInformation = 0x89,
    SystemMemoryTopologyInformation = 0x8A,
    SystemMemoryChannelInformation = 0x8B,
    SystemBootLogoInformation = 0x8C,
    SystemProcessorPerformanceInformationEx = 0x8D,
    SystemCriticalProcessErrorLogInformation = 0x8E,
    SystemSecureBootPolicyInformation = 0x8F,
    SystemPageFileInformationEx = 0x90,
    SystemSecureBootInformation = 0x91,
    SystemEntropyInterruptTimingRawInformation = 0x92,
    SystemPortableWorkspaceEfiLauncherInformation = 0x93,
    SystemFullProcessInformation = 0x94,
    SystemKernelDebuggerInformationEx = 0x95,
    SystemBootMetadataInformation = 0x96,
    SystemSoftRebootInformation = 0x97,
    SystemElamCertificateInformation = 0x98,
    SystemOfflineDumpConfigInformation = 0x99,
    SystemProcessorFeaturesInformation = 0x9A,
    SystemRegistryReconciliationInformation = 0x9B,
    SystemEdidInformation = 0x9C,
    SystemManufacturingInformation = 0x9D,
    SystemEnergyEstimationConfigInformation = 0x9E,
    SystemHypervisorDetailInformation = 0x9F,
    SystemProcessorCycleStatsInformation = 0xA0,
    SystemVmGenerationCountInformation = 0xA1,
    SystemTrustedPlatformModuleInformation = 0xA2,
    SystemKernelDebuggerFlags = 0xA3,
    SystemCodeIntegrityPolicyInformation = 0xA4,
    SystemIsolatedUserModeInformation = 0xA5,
    SystemHardwareSecurityTestInterfaceResultsInformation = 0xA6,
    SystemSingleModuleInformation = 0xA7,
    SystemAllowedCpuSetsInformation = 0xA8,
    SystemDmaProtectionInformation = 0xA9,
    SystemInterruptCpuSetsInformation = 0xAA,
    SystemSecureBootPolicyFullInformation = 0xAB,
    SystemCodeIntegrityPolicyFullInformation = 0xAC,
    SystemAffinitizedInterruptProcessorInformation = 0xAD,
    SystemRootSiloInformation = 0xAE,
    SystemCpuSetInformation = 0xAF,
    SystemCpuSetTagInformation = 0xB0,
    SystemWin32WerStartCallout = 0xB1,
    SystemSecureKernelProfileInformation = 0xB2,
    SystemCodeIntegrityPlatformManifestInformation = 0xB3,
    SystemInterruptSteeringInformation = 0xB4,
    SystemSupportedProcessorArchitectures = 0xB5,

    SystemMemoryUsageInformation = 182,
    SystemCodeIntegrityCertificateInformation = 183,
    SystemPhysicalMemoryInformation = 184,
    SystemControlFlowTransition = 185,
    SystemKernelDebuggingAllowed = 186,
    SystemActivityModerationExeState = 187,
    SystemActivityModerationUserSettings = 188,
    SystemCodeIntegrityPoliciesFullInformation = 189,
    SystemCodeIntegrityUnlockInformation = 190,
    SystemIntegrityQuotaInformation = 191,
    SystemFlushInformation = 192,
    SystemProcessorIdleMaskInformation = 193,
    SystemSecureDumpEncryptionInformation = 194,
    SystemWriteConstraintInformation = 195,
    SystemKernelVaShadowInformation = 196,
    SystemHypervisorSharedPageInformation = 197,
    SystemFirmwareBootPerformanceInformation = 198,
    SystemCodeIntegrityVerificationInformation = 199,
    SystemFirmwarePartitionInformation = 200,
    SystemSpeculationControlInformation = 201,
    SystemDmaGuardPolicyInformation = 202,
    SystemEnclaveLaunchControlInformation = 203,
    SystemWorkloadAllowedCpuSetsInformation = 204,
    SystemCodeIntegrityUnlockModeInformation = 205,
    SystemLeapSecondInformation = 206,
    SystemFlags2Information = 207,
    SystemSecurityModelInformation = 208,
    SystemCodeIntegritySyntheticCacheInformation = 209,
    SystemFeatureConfigurationInformation = 210,
    SystemFeatureConfigurationSectionInformation = 211,
    SystemFeatureUsageSubscriptionInformation = 212,
    SystemSecureSpeculationControlInformation = 213,
    SystemSpacesBootInformation = 214,
    SystemFwRamdiskInformation = 215,
    SystemWheaIpmiHardwareInformation = 216,
    SystemDifSetRuleClassInformation = 217,
    SystemDifClearRuleClassInformation = 218,
    SystemDifApplyPluginVerificationOnDriver = 219,
    SystemDifRemovePluginVerificationOnDriver = 220,
    SystemShadowStackInformation = 221,
    SystemBuildVersionInformation = 222,
    SystemPoolLimitInformation = 223,
    SystemCodeIntegrityAddDynamicStore = 224,
    SystemCodeIntegrityClearDynamicStores = 225,
    SystemDifPoolTrackingInformation = 226,
    SystemPoolZeroingInformation = 227,
    SystemDpcWatchdogInformation = 228,
    SystemDpcWatchdogInformation2 = 229,
    SystemSupportedProcessorArchitectures2 = 230,
    SystemSingleProcessorRelationshipInformation = 231,
    SystemXfgCheckFailureInformation = 232,
    SystemIommuStateInformation = 233,
    SystemHypervisorMinrootInformation = 234,
    SystemHypervisorBootPagesInformation = 235,
    SystemPointerAuthInformation = 236,
    SystemSecureKernelDebuggerInformation = 237,
    SystemOriginalImageFeatureInformation = 238,
    SystemMemoryNumaInformation = 239,
    SystemMemoryNumaPerformanceInformation = 240,
    SystemCodeIntegritySignedPoliciesFullInformation = 241,
    SystemSecureSecretsInformation = 242,
    SystemTrustedAppsRuntimeInformation = 243,
    SystemBadPageInformationEx = 244,
    SystemResourceDeadlockTimeout = 245,
    SystemBreakOnContextUnwindFailureInformation = 246,
    SystemOslRamdiskInformation = 247,
    SystemCodeIntegrityPolicyManagementInformation = 248,
    SystemMemoryNumaCacheInformation = 249,
    SystemProcessorFeaturesBitMapInformation = 250,
    SystemRefTraceInformationEx = 251,
    SystemBasicProcessInformation = 252,
    SystemHandleCountInformation = 253,
    MaxSystemInfoClass
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

//
// Thread priority
//

typedef LONG KPRIORITY;

// SYSTEM_INFORMATION_CLASS::SystemBasicInformation = 0
typedef struct _SYSTEM_BASIC_INFORMATION
{
    ULONG Reserved;
    ULONG TimerResolution;
    ULONG PageSize;
    ULONG NumberOfPhysicalPages;
    ULONG LowestPhysicalPageNumber;
    ULONG HighestPhysicalPageNumber;
    ULONG AllocationGranularity;
    ULONG_PTR MinimumUserModeAddress;
    ULONG_PTR MaximumUserModeAddress;
    ULONG_PTR ActiveProcessorsAffinityMask;
    CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemProcessorInformation = 1
typedef struct _SYSTEM_PROCESSOR_INFORMATION
{
    USHORT ProcessorArchitecture;
    USHORT ProcessorLevel;
    USHORT ProcessorRevision;
    USHORT Reserved;
    ULONG ProcessorFeatureBits;
} SYSTEM_PROCESSOR_INFORMATION, *PSYSTEM_PROCESSOR_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemPerformanceInformation = 2
typedef struct _SYSTEM_PERFORMANCE_INFORMATION
{
    LARGE_INTEGER IdleProcessTime;
    LARGE_INTEGER IoReadTransferCount;
    LARGE_INTEGER IoWriteTransferCount;
    LARGE_INTEGER IoOtherTransferCount;
    ULONG IoReadOperationCount;
    ULONG IoWriteOperationCount;
    ULONG IoOtherOperationCount;
    ULONG AvailablePages;
    ULONG CommittedPages;
    ULONG CommitLimit;
    ULONG PeakCommitment;
    ULONG PageFaultCount;
    ULONG CopyOnWriteCount;
    ULONG TransitionCount;
    ULONG CacheTransitionCount;
    ULONG DemandZeroCount;
    ULONG PageReadCount;
    ULONG PageReadIoCount;
    ULONG CacheReadCount;
    ULONG CacheIoCount;
    ULONG DirtyPagesWriteCount;
    ULONG DirtyWriteIoCount;
    ULONG MappedPagesWriteCount;
    ULONG MappedWriteIoCount;
    ULONG PagedPoolPages;
    ULONG NonPagedPoolPages;
    ULONG PagedPoolAllocs;
    ULONG PagedPoolFrees;
    ULONG NonPagedPoolAllocs;
    ULONG NonPagedPoolFrees;
    ULONG FreeSystemPtes;
    ULONG ResidentSystemCodePage;
    ULONG TotalSystemDriverPages;
    ULONG TotalSystemCodePages;
    ULONG NonPagedPoolLookasideHits;
    ULONG PagedPoolLookasideHits;
    ULONG Spare3Count;
    ULONG ResidentSystemCachePage;
    ULONG ResidentPagedPoolPage;
    ULONG ResidentSystemDriverPage;
    ULONG CcFastReadNoWait;
    ULONG CcFastReadWait;
    ULONG CcFastReadResourceMiss;
    ULONG CcFastReadNotPossible;
    ULONG CcFastMdlReadNoWait;
    ULONG CcFastMdlReadWait;
    ULONG CcFastMdlReadResourceMiss;
    ULONG CcFastMdlReadNotPossible;
    ULONG CcMapDataNoWait;
    ULONG CcMapDataWait;
    ULONG CcMapDataNoWaitMiss;
    ULONG CcMapDataWaitMiss;
    ULONG CcPinMappedDataCount;
    ULONG CcPinReadNoWait;
    ULONG CcPinReadWait;
    ULONG CcPinReadNoWaitMiss;
    ULONG CcPinReadWaitMiss;
    ULONG CcCopyReadNoWait;
    ULONG CcCopyReadWait;
    ULONG CcCopyReadNoWaitMiss;
    ULONG CcCopyReadWaitMiss;
    ULONG CcMdlReadNoWait;
    ULONG CcMdlReadWait;
    ULONG CcMdlReadNoWaitMiss;
    ULONG CcMdlReadWaitMiss;
    ULONG CcReadAheadIos;
    ULONG CcLazyWriteIos;
    ULONG CcLazyWritePages;
    ULONG CcDataFlushes;
    ULONG CcDataPages;
    ULONG ContextSwitches;
    ULONG FirstLevelTbFills;
    ULONG SecondLevelTbFills;
    ULONG SystemCalls;
} SYSTEM_PERFORMANCE_INFORMATION, *PSYSTEM_PERFORMANCE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemTimeOfDayInformation = 3
typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
{
    LARGE_INTEGER BootTime;
    LARGE_INTEGER CurrentTime;
    LARGE_INTEGER TimeZoneBias;
    ULONG TimeZoneId;
    ULONG Reserved;
    LARGE_INTEGER BootTimeBias;
    LARGE_INTEGER SleepTimeBias;
} SYSTEM_TIMEOFDAY_INFORMATION, *PSYSTEM_TIMEOFDAY_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemPathInformation = 4
// This class is obsolete, please use KUSER_SHARED_DATA instead

// SYSTEM_INFORMATION_CLASS::SystemPathInformation = 4
typedef struct _SYSTEM_THREAD_INFORMATION
{
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER CreateTime;
    ULONG WaitTime;
    PVOID StartAddress;
    CLIENT_ID ClientId;
    KPRIORITY Priority;
    LONG BasePriority;
    ULONG ContextSwitches;
    ULONG ThreadState;
    ULONG WaitReason;
} SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemProcessInformation = 5
typedef struct _SYSTEM_PROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER SpareLi1;
    LARGE_INTEGER SpareLi2;
    LARGE_INTEGER SpareLi3;
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    KPRIORITY BasePriority;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR PageDirectoryBase;

    //
    // This part corresponds to VM_COUNTERS_EX.
    // NOTE: *NOT* THE SAME AS VM_COUNTERS!
    //
    SIZE_T PeakVirtualSize;
    ULONG VirtualSize;
    SIZE_T PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;

    //
    // This part corresponds to IO_COUNTERS
    //
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;

    //SYSTEM_THREAD_INFORMATION TH[1];
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemCallCountInformation = 6
typedef struct _SYSTEM_CALL_COUNT_INFORMATION
{
    ULONG Length;
    ULONG NumberOfTables;
} SYSTEM_CALL_COUNT_INFORMATION, *PSYSTEM_CALL_COUNT_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemDeviceInformation = 7
typedef struct _SYSTEM_DEVICE_INFORMATION
{
    ULONG NumberOfDisks;
    ULONG NumberOfFloppies;
    ULONG NumberOfCdRoms;
    ULONG NumberOfTapes;
    ULONG NumberOfSerialPorts;
    ULONG NumberOfParallelPorts;
} SYSTEM_DEVICE_INFORMATION, *PSYSTEM_DEVICE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemProcessorPerformanceInformation = 8
typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
{
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER DpcTime;
    LARGE_INTEGER InterruptTime;
    ULONG InterruptCount;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemFlagsInformation = 9
typedef struct _SYSTEM_FLAGS_INFORMATION
{
    ULONG Flags;
} SYSTEM_FLAGS_INFORMATION, *PSYSTEM_FLAGS_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemCallTimeInformation = 10
typedef struct _SYSTEM_CALL_TIME_INFORMATION
{
    ULONG Length;
    ULONG TotalCalls;
    LARGE_INTEGER TimeOfCalls[1];
} SYSTEM_CALL_TIME_INFORMATION, *PSYSTEM_CALL_TIME_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemModuleInformation = 11
// Class 11 - See RTL_PROCESS_MODULES
typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    ULONG Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    CHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION, SYSTEM_MODULE, *PSYSTEM_MODULE;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES, SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemLocksInformation = 12
typedef struct _RTL_PROCESS_LOCK_INFORMATION
{
    PVOID Address;
    USHORT Type;
    USHORT CreatorBackTraceIndex;
    ULONG OwnerThreadId;
    ULONG ActiveCount;
    ULONG ContentionCount;
    ULONG EntryCount;
    ULONG RecursionCount;
    ULONG NumberOfSharedWaiters;
    ULONG NumberOfExclusiveWaiters;
} RTL_PROCESS_LOCK_INFORMATION, *PRTL_PROCESS_LOCK_INFORMATION;

typedef struct _RTL_PROCESS_LOCKS
{
    ULONG NumberOfLocks;
    RTL_PROCESS_LOCK_INFORMATION Locks[1];
} RTL_PROCESS_LOCKS, *PRTL_PROCESS_LOCKS;

// SYSTEM_INFORMATION_CLASS::SystemStackTraceInformation = 13
// - See RTL_PROCESS_BACKTRACES
typedef struct _RTL_PROCESS_BACKTRACE_INFORMATION
{
    PVOID SymbolicBackTrace;
    ULONG TraceCount;
    USHORT Index;
    USHORT Depth;
    PVOID BackTrace[16];
} RTL_PROCESS_BACKTRACE_INFORMATION, *PRTL_PROCESS_BACKTRACE_INFORMATION;

typedef struct _RTL_PROCESS_BACKTRACES
{
    ULONG CommittedMemory;
    ULONG ReservedMemory;
    ULONG NumberOfBackTraceLookups;
    ULONG NumberOfBackTraces;
    RTL_PROCESS_BACKTRACE_INFORMATION BackTraces[1];
} RTL_PROCESS_BACKTRACES, *PRTL_PROCESS_BACKTRACES;

// SYSTEM_INFORMATION_CLASS::SystemPagedPoolInformation = 14
// SYSTEM_INFORMATION_CLASS::SystemNonPagedPoolInformation = 15
typedef struct _SYSTEM_POOL_ENTRY
{
    BOOLEAN Allocated;
    BOOLEAN Spare0;
    USHORT AllocatorBackTraceIndex;
    ULONG Size;
    union
    {
        UCHAR Tag[4];
        ULONG TagUlong;
        PVOID ProcessChargedQuota;
    };
} SYSTEM_POOL_ENTRY, *PSYSTEM_POOL_ENTRY;

typedef struct _SYSTEM_POOL_INFORMATION
{
    ULONG TotalSize;
    PVOID FirstEntry;
    USHORT EntryOverhead;
    BOOLEAN PoolTagPresent;
    BOOLEAN Spare0;
    ULONG NumberOfEntries;
    SYSTEM_POOL_ENTRY Entries[1];
} SYSTEM_POOL_INFORMATION, *PSYSTEM_POOL_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemHandleInformation = 16
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
    ULONG UniqueProcessId;
    UCHAR ObjectTypeNumber;
    UCHAR HandleAttributes;
    USHORT HandleValue;
    PVOID Object;
    ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    ULONG NumberOfHandles;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemObjectInformation = 17
typedef struct _SYSTEM_OBJECTTYPE_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG NumberOfObjects;
    ULONG NumberOfHandles;
    ULONG TypeIndex;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccessMask;
    ULONG PoolType;
    BOOLEAN SecurityRequired;
    BOOLEAN WaitableObject;
    UNICODE_STRING TypeName;
} SYSTEM_OBJECTTYPE_INFORMATION, *PSYSTEM_OBJECTTYPE_INFORMATION;

typedef struct _SYSTEM_OBJECT_INFORMATION
{
    ULONG NextEntryOffset;
    PVOID Object;
    HANDLE CreatorUniqueProcess;
    USHORT CreatorBackTraceIndex;
    USHORT Flags;
    LONG PointerCount;
    LONG HandleCount;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    HANDLE ExclusiveProcessId;
    PVOID SecurityDescriptor;
    OBJECT_NAME_INFORMATION NameInfo;
} SYSTEM_OBJECT_INFORMATION, *PSYSTEM_OBJECT_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemPageFileInformation = 18
typedef struct _SYSTEM_PAGEFILE_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG TotalSize;
    ULONG TotalInUse;
    ULONG PeakUsage;
    UNICODE_STRING PageFileName;
} SYSTEM_PAGEFILE_INFORMATION, *PSYSTEM_PAGEFILE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemVdmInstemulInformation = 19
typedef struct _SYSTEM_VDM_INSTEMUL_INFO
{
    ULONG SegmentNotPresent;
    ULONG VdmOpcode0F;
    ULONG OpcodeESPrefix;
    ULONG OpcodeCSPrefix;
    ULONG OpcodeSSPrefix;
    ULONG OpcodeDSPrefix;
    ULONG OpcodeFSPrefix;
    ULONG OpcodeGSPrefix;
    ULONG OpcodeOPER32Prefix;
    ULONG OpcodeADDR32Prefix;
    ULONG OpcodeINSB;
    ULONG OpcodeINSW;
    ULONG OpcodeOUTSB;
    ULONG OpcodeOUTSW;
    ULONG OpcodePUSHF;
    ULONG OpcodePOPF;
    ULONG OpcodeINTnn;
    ULONG OpcodeINTO;
    ULONG OpcodeIRET;
    ULONG OpcodeINBimm;
    ULONG OpcodeINWimm;
    ULONG OpcodeOUTBimm;
    ULONG OpcodeOUTWimm ;
    ULONG OpcodeINB;
    ULONG OpcodeINW;
    ULONG OpcodeOUTB;
    ULONG OpcodeOUTW;
    ULONG OpcodeLOCKPrefix;
    ULONG OpcodeREPNEPrefix;
    ULONG OpcodeREPPrefix;
    ULONG OpcodeHLT;
    ULONG OpcodeCLI;
    ULONG OpcodeSTI;
    ULONG BopCount;
} SYSTEM_VDM_INSTEMUL_INFO, *PSYSTEM_VDM_INSTEMUL_INFO;

// SYSTEM_INFORMATION_CLASS::SystemVdmBopInformation = 20
// - ULONG VDMBOPINFO

// SYSTEM_INFORMATION_CLASS::SystemFileCacheInformation = 21
typedef struct _SYSTEM_FILECACHE_INFORMATION
{
    ULONG CurrentSize;
    ULONG PeakSize;
    ULONG PageFaultCount;
    ULONG MinimumWorkingSet;
    ULONG MaximumWorkingSet;
    ULONG CurrentSizeIncludingTransitionInPages;
    ULONG PeakSizeIncludingTransitionInPages;
    ULONG TransitionRePurposeCount;
    ULONG Flags;
} SYSTEM_FILECACHE_INFORMATION, *PSYSTEM_FILECACHE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemPoolTagInformation = 22
typedef struct _SYSTEM_POOLTAG
{
    union
    {
        UCHAR Tag[4];
        ULONG TagUlong;
    };
    ULONG PagedAllocs;
    ULONG PagedFrees;
    ULONG PagedUsed;
    ULONG NonPagedAllocs;
    ULONG NonPagedFrees;
    ULONG NonPagedUsed;
} SYSTEM_POOLTAG, *PSYSTEM_POOLTAG;
typedef struct _SYSTEM_POOLTAG_INFORMATION
{
    ULONG Count;
    SYSTEM_POOLTAG TagInfo[1];
} SYSTEM_POOLTAG_INFORMATION, *PSYSTEM_POOLTAG_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemInterruptInformation = 23
typedef struct _SYSTEM_INTERRUPT_INFORMATION
{
    ULONG ContextSwitches;
    ULONG DpcCount;
    ULONG DpcRate;
    ULONG TimeIncrement;
    ULONG DpcBypassCount;
    ULONG ApcBypassCount;
} SYSTEM_INTERRUPT_INFORMATION, *PSYSTEM_INTERRUPT_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemDpcBehaviorInformation = 24
typedef struct _SYSTEM_DPC_BEHAVIOR_INFORMATION
{
    ULONG Spare;
    ULONG DpcQueueDepth;
    ULONG MinimumDpcRate;
    ULONG AdjustDpcThreshold;
    ULONG IdealDpcRate;
} SYSTEM_DPC_BEHAVIOR_INFORMATION, *PSYSTEM_DPC_BEHAVIOR_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemFullMemoryInformation = 25
typedef struct _SYSTEM_MEMORY_INFO
{
    PUCHAR StringOffset;
    USHORT ValidCount;
    USHORT TransitionCount;
    USHORT ModifiedCount;
    USHORT PageTableCount;
} SYSTEM_MEMORY_INFO, *PSYSTEM_MEMORY_INFO;

typedef struct _SYSTEM_MEMORY_INFORMATION
{
    ULONG InfoSize;
    ULONG StringStart;
    SYSTEM_MEMORY_INFO Memory[1];
} SYSTEM_MEMORY_INFORMATION, *PSYSTEM_MEMORY_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemLoadGdiDriverInformation = 26
typedef struct _SYSTEM_GDI_DRIVER_INFORMATION
{
    UNICODE_STRING DriverName;
    PVOID ImageAddress;
    PVOID SectionPointer;
    PVOID EntryPoint;
    PIMAGE_EXPORT_DIRECTORY ExportSectionPointer;
    ULONG ImageLength;
} SYSTEM_GDI_DRIVER_INFORMATION, *PSYSTEM_GDI_DRIVER_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemUnloadGdiDriverInformation = 27
// Not an actually class, simply a PVOID to the ImageAddress

// SYSTEM_INFORMATION_CLASS::SystemTimeAdjustmentInformation = 28
typedef struct _SYSTEM_QUERY_TIME_ADJUST_INFORMATION
{
    ULONG TimeAdjustment;
    ULONG TimeIncrement;
    BOOLEAN Enable;
} SYSTEM_QUERY_TIME_ADJUST_INFORMATION, *PSYSTEM_QUERY_TIME_ADJUST_INFORMATION;

typedef struct _SYSTEM_SET_TIME_ADJUST_INFORMATION
{
    ULONG TimeAdjustment;
    BOOLEAN Enable;
} SYSTEM_SET_TIME_ADJUST_INFORMATION, *PSYSTEM_SET_TIME_ADJUST_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemSummaryMemoryInformation = 29
// - Same as 25

// FIXME: Class 30

// SYSTEM_INFORMATION_CLASS::SystemPerformanceTraceInformation = 31
typedef struct _SYSTEM_REF_TRACE_INFORMATION
{
   UCHAR TraceEnable;
   UCHAR TracePermanent;
   UNICODE_STRING TraceProcessName;
   UNICODE_STRING TracePoolTags;
} SYSTEM_REF_TRACE_INFORMATION, *PSYSTEM_REF_TRACE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemExceptionInformation = 33
typedef struct _SYSTEM_EXCEPTION_INFORMATION
{
    ULONG AlignmentFixupCount;
    ULONG ExceptionDispatchCount;
    ULONG FloatingEmulationCount;
    ULONG ByteWordEmulationCount;
} SYSTEM_EXCEPTION_INFORMATION, *PSYSTEM_EXCEPTION_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemCrashDumpStateInformation = 34
typedef struct _SYSTEM_CRASH_STATE_INFORMATION
{
    ULONG ValidCrashDump;
} SYSTEM_CRASH_STATE_INFORMATION, *PSYSTEM_CRASH_STATE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemKernelDebuggerInformation = 35
typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
{
    BOOLEAN KernelDebuggerEnabled;
    BOOLEAN KernelDebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, *PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemContextSwitchInformation = 36
typedef struct _SYSTEM_CONTEXT_SWITCH_INFORMATION
{
    ULONG ContextSwitches;
    ULONG FindAny;
    ULONG FindLast;
    ULONG FindIdeal;
    ULONG IdleAny;
    ULONG IdleCurrent;
    ULONG IdleLast;
    ULONG IdleIdeal;
    ULONG PreemptAny;
    ULONG PreemptCurrent;
    ULONG PreemptLast;
    ULONG SwitchToIdle;
} SYSTEM_CONTEXT_SWITCH_INFORMATION, *PSYSTEM_CONTEXT_SWITCH_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemRegistryQuotaInformation = 37
typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION
{
    ULONG RegistryQuotaAllowed;
    ULONG RegistryQuotaUsed;
    ULONG PagedPoolSize;
} SYSTEM_REGISTRY_QUOTA_INFORMATION, *PSYSTEM_REGISTRY_QUOTA_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemExtendServiceTableInformation = 38
// Not a structure, simply send the UNICODE_STRING

// SYSTEM_INFORMATION_CLASS::SystemPrioritySeperation = 39
// Not a structure, simply send a ULONG containing the new separation

// SYSTEM_INFORMATION_CLASS::SystemVerifierAddDriverInformation = 40
typedef struct _SYSTEM_PLUGPLAY_BUS_INFORMATION
{
    ULONG BusCount;
//  PLUGPLAY_BUS_INSTANCE BusInstance[1];
} SYSTEM_PLUGPLAY_BUS_INFORMATION, *PSYSTEM_PLUGPLAY_BUS_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemVerifierRemoveDriverInformation = 41
typedef enum _SYSTEM_DOCK_STATE
{
    SystemDockStateUnknown,
    SystemUndocked,
    SystemDocked
} SYSTEM_DOCK_STATE, *PSYSTEM_DOCK_STATE;

typedef enum _INTERFACE_TYPE
{
    InterfaceTypeUndefined = -1,
    Internal,
    Isa,
    Eisa,
    MicroChannel,
    TurboChannel,
    PCIBus,
    VMEBus,
    NuBus,
    PCMCIABus,
    CBus,
    MPIBus,
    MPSABus,
    ProcessorInternal,
    InternalPowerBus,
    PNPISABus,
    PNPBus,
    MaximumInterfaceType
}INTERFACE_TYPE, *PINTERFACE_TYPE;

typedef struct _SYSTEM_DOCK_INFORMATION
{
    SYSTEM_DOCK_STATE DockState;
    INTERFACE_TYPE DeviceBusType;
    ULONG DeviceBusNumber;
    ULONG SlotNumber;
} SYSTEM_DOCK_INFORMATION, *PSYSTEM_DOCK_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemProcessorIdleInformation = 42
typedef struct _SYSTEM_POWER_INFORMATION_NATIVE
{
    BOOLEAN SystemSuspendSupported;
    BOOLEAN SystemHibernateSupported;
    BOOLEAN ResumeTimerSupportsSuspend;
    BOOLEAN ResumeTimerSupportsHibernate;
    BOOLEAN LidSupported;
    BOOLEAN TurboSettingSupported;
    BOOLEAN TurboMode;
    BOOLEAN SystemAcOrDc;
    BOOLEAN PowerDownDisabled;
    LARGE_INTEGER SpindownDrives;
} SYSTEM_POWER_INFORMATION_NATIVE, *PSYSTEM_POWER_INFORMATION_NATIVE;

// SYSTEM_INFORMATION_CLASS::SystemLegacyDriverInformation = 43
typedef struct _SYSTEM_LEGACY_DRIVER_INFORMATION
{
//  PNP_VETO_TYPE VetoType;
    UNICODE_STRING VetoDriver;
    // CHAR Buffer[0];
} SYSTEM_LEGACY_DRIVER_INFORMATION, *PSYSTEM_LEGACY_DRIVER_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemCurrentTimeZoneInformation = 44
//typedef struct _TIME_ZONE_INFORMATION RTL_TIME_ZONE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemLookasideInformation = 45
typedef struct _SYSTEM_LOOKASIDE_INFORMATION
{
    USHORT CurrentDepth;
    USHORT MaximumDepth;
    ULONG TotalAllocates;
    ULONG AllocateMisses;
    ULONG TotalFrees;
    ULONG FreeMisses;
    ULONG Type;
    ULONG Tag;
    ULONG Size;
} SYSTEM_LOOKASIDE_INFORMATION, *PSYSTEM_LOOKASIDE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemTimeSlipNotification = 46
// Not a structure. Only a HANDLE for the SlipEvent;

// SYSTEM_INFORMATION_CLASS::SystemSessionCreate = 47
// Not a structure. Only a ULONG for the SessionId;

// SYSTEM_INFORMATION_CLASS::SystemSessionDetach = 48
// Not a structure. Only a ULONG for the SessionId;

// FIXME: Class 49

// SYSTEM_INFORMATION_CLASS::SystemRangeStartInformation = 50
// Not a structure. Only a ULONG_PTR for the SystemRangeStart

// SYSTEM_INFORMATION_CLASS::SystemVerifierInformation = 51
typedef struct _SYSTEM_VERIFIER_INFORMATION
{
   ULONG NextEntryOffset;
   ULONG Level;
   UNICODE_STRING DriverName;
   ULONG RaiseIrqls;
   ULONG AcquireSpinLocks;
   ULONG SynchronizeExecutions;
   ULONG AllocationsAttempted;
   ULONG AllocationsSucceeded;
   ULONG AllocationsSucceededSpecialPool;
   ULONG AllocationsWithNoTag;
   ULONG TrimRequests;
   ULONG Trims;
   ULONG AllocationsFailed;
   ULONG AllocationsFailedDeliberately;
   ULONG Loads;
   ULONG Unloads;
   ULONG UnTrackedPool;
   ULONG CurrentPagedPoolAllocations;
   ULONG CurrentNonPagedPoolAllocations;
   ULONG PeakPagedPoolAllocations;
   ULONG PeakNonPagedPoolAllocations;
   ULONG PagedPoolUsageInBytes;
   ULONG NonPagedPoolUsageInBytes;
   ULONG PeakPagedPoolUsageInBytes;
   ULONG PeakNonPagedPoolUsageInBytes;
} SYSTEM_VERIFIER_INFORMATION, *PSYSTEM_VERIFIER_INFORMATION;

// FIXME: Class 52

// SYSTEM_INFORMATION_CLASS::SystemSessionProcessInformation = 53
typedef struct _SYSTEM_SESSION_PROCESS_INFORMATION
{
    ULONG SessionId;
    ULONG SizeOfBuf;
    PVOID Buffer; // Same format as in SystemProcessInformation
} SYSTEM_SESSION_PROCESS_INFORMATION, *PSYSTEM_SESSION_PROCESS_INFORMATION;

// FIXME: Classes 54-97

//
// Hotpatch flags
//
#define RTL_HOTPATCH_SUPPORTED_FLAG         0x01
#define RTL_HOTPATCH_SWAP_OBJECT_NAMES      0x08 << 24
#define RTL_HOTPATCH_SYNC_RENAME_FILES      0x10 << 24
#define RTL_HOTPATCH_PATCH_USER_MODE        0x20 << 24
#define RTL_HOTPATCH_REMAP_SYSTEM_DLL       0x40 << 24
#define RTL_HOTPATCH_PATCH_KERNEL_MODE      0x80 << 24

// SYSTEM_INFORMATION_CLASS::SystemExtendedHandleInformation = 64
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
{
    PVOID Object;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR HandleValue;
    ULONG GrantedAccess;
    USHORT CreatorBackTraceIndex;
    USHORT ObjectTypeIndex;
    ULONG  HandleAttributes;
    ULONG  Reserved;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
    ULONG_PTR NumberOfHandles;
    ULONG_PTR Reserved;
    SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
} SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

// SYSTEM_INFORMATION_CLASS::SystemHotpatchInformation = 69
typedef struct _SYSTEM_HOTPATCH_CODE_INFORMATION
{
    ULONG Flags;
    ULONG InfoSize;
    union
    {
        struct
        {
            ULONG Foo;
        } CodeInfo;
        struct
        {
            USHORT NameOffset;
            USHORT NameLength;
        } KernelInfo;
        struct
        {
            USHORT NameOffset;
            USHORT NameLength;
            USHORT TargetNameOffset;
            USHORT TargetNameLength;
            UCHAR PatchingFinished;
        } UserModeInfo;
        struct
        {
            USHORT NameOffset;
            USHORT NameLength;
            USHORT TargetNameOffset;
            USHORT TargetNameLength;
            UCHAR PatchingFinished;
            NTSTATUS ReturnCode;
            HANDLE TargetProcess;
        } InjectionInfo;
        struct
        {
            HANDLE FileHandle1;
            PIO_STATUS_BLOCK IoStatusBlock1;
            PVOID RenameInformation1;
            PVOID RenameInformationLength1;
            HANDLE FileHandle2;
            PIO_STATUS_BLOCK IoStatusBlock2;
            PVOID RenameInformation2;
            PVOID RenameInformationLength2;
        } RenameInfo;
        struct
        {
            HANDLE ParentDirectory;
            HANDLE ObjectHandle1;
            HANDLE ObjectHandle2;
        } AtomicSwap;
    };
} SYSTEM_HOTPATCH_CODE_INFORMATION, *PSYSTEM_HOTPATCH_CODE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemRegisterFirmwareTableInformationHandler = 75
struct _SYSTEM_FIRMWARE_TABLE_INFORMATION;
typedef NTSTATUS (_cdecl *PFNFTH)(
    IN struct _SYSTEM_FIRMWARE_TABLE_INFORMATION *FirmwareTableInformation
    );

typedef enum _SYSTEM_FIRMWARE_TABLE_ACTION
{
    SystemFirmwareTable_Enumerate = 0,
    SystemFirmwareTable_Get = 1,
} SYSTEM_FIRMWARE_TABLE_ACTION, *PSYSTEM_FIRMWARE_TABLE_ACTION;

typedef struct _SYSTEM_FIRMWARE_TABLE_HANDLER
{
    ULONG ProviderSignature;
    BOOLEAN Register;
    PFNFTH FirmwareTableHandler;
    PVOID DriverObject;
} SYSTEM_FIRMWARE_TABLE_HANDLER, *PSYSTEM_FIRMWARE_TABLE_HANDLER;

// SYSTEM_INFORMATION_CLASS::SystemFirmwareTableInformation = 76
typedef struct _SYSTEM_FIRMWARE_TABLE_INFORMATION
{
    ULONG ProviderSignature;
    SYSTEM_FIRMWARE_TABLE_ACTION Action;
    ULONG TableID;
    ULONG TableBufferLength;
    UCHAR TableBuffer[1];
} SYSTEM_FIRMWARE_TABLE_INFORMATION, *PSYSTEM_FIRMWARE_TABLE_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemFileCacheInformationEx = 81
typedef struct _SYSTEM_MEMORY_LIST_INFORMATION
{
   SIZE_T ZeroPageCount;
   SIZE_T FreePageCount;
   SIZE_T ModifiedPageCount;
   SIZE_T ModifiedNoWritePageCount;
   SIZE_T BadPageCount;
   SIZE_T PageCountByPriority[8];
   SIZE_T RepurposedPagesByPriority[8];
} SYSTEM_MEMORY_LIST_INFORMATION, *PSYSTEM_MEMORY_LIST_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemProcessIdInformation = 88
typedef struct _SYSTEM_PROCESS_ID_INFORMATION
{
    HANDLE UniqueProcessId;                         // On input, set this to Process ID
    UNICODE_STRING ImageName;                       // On input, initialize to an allocated buffer
} SYSTEM_PROCESS_ID_INFORMATION, *PSYSTEM_PROCESS_ID_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemBootEnvironmentInformation = 90
typedef struct _SYSTEM_BOOT_ENVIRONMENT_INFORMATION
{
    GUID  CurrentBootGuid;
    ULONG Unknown;
} SYSTEM_BOOT_ENVIRONMENT_INFORMATION, *PSYSTEM_BOOT_ENVIRONMENT_INFORMATION;

// SYSTEM_INFORMATION_CLASS::SystemBasicProcessInformation = 252
typedef struct _SYSTEM_BASICPROCESS_INFORMATION
{
    ULONG NextEntryOffset;
    HANDLE UniqueProcessId;
    HANDLE InheritedFromUniqueProcessId;
    ULONG64 SequenceNumber;
    UNICODE_STRING ImageName;
} SYSTEM_BASICPROCESS_INFORMATION, *PSYSTEM_BASICPROCESS_INFORMATION;

typedef NTSTATUS (NTAPI * NTQUERYSYSTEMINFORMATION)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

typedef NTSTATUS (NTAPI * RTLGETNATIVESYSTEMINFORMATION)(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN  ULONG SystemInformationLength,
    OUT PULONG ReturnLength
    );

typedef BOOLEAN (NTAPI * RTLGETPRODUCTINFO)(
    IN  ULONG OSMajorVersion,
    IN  ULONG OSMinorVersion,
    IN  ULONG SpMajorVersion,
    IN  ULONG SpMinorVersion,
    OUT PULONG ReturnedProductType
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySystemInformation(
    IN  SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN  ULONG SystemInformationLength,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN  ULONG SystemInformationLength,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetSystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    IN PVOID SystemInformation,
    IN ULONG SystemInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetSystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    IN PVOID SystemInformation,
    IN ULONG SystemInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetNativeSystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN  ULONG SystemInformationLength,
    OUT PULONG ReturnLength
    );

//------------------------------------------------------------------------------
// Shutdown system

typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff

} SHUTDOWN_ACTION, *PSHUTDOWN_ACTION;

NTSYSAPI
NTSTATUS
NTAPI
NtShutdownSystem(
    IN SHUTDOWN_ACTION Action
    );

//-----------------------------------------------------------------------------
// File functions

#ifndef OLD_DOS_VOLID
#define OLD_DOS_VOLID   0x00000008
#endif

#ifndef FILE_SUPERSEDE
#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005
#endif  // File create flags


// Define the create/open option flags
#ifndef FILE_DIRECTORY_FILE
#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008
#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080
#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800
#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000
#define FILE_OPEN_REQUIRING_OPLOCK              0x00010000
#define FILE_DISALLOW_EXCLUSIVE                 0x00020000
#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000
#endif // FILE_DIRECTORY_FILE


//
// Define the I/O status information return values for NtCreateFile/NtOpenFile
//

#ifndef FILE_SUPERSEDED
#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005
#endif

#ifndef FILE_REMOVABLE_MEDIA
#define FILE_REMOVABLE_MEDIA                        0x00000001
#define FILE_READ_ONLY_DEVICE                       0x00000002
#define FILE_FLOPPY_DISKETTE                        0x00000004
#define FILE_WRITE_ONCE_MEDIA                       0x00000008
#define FILE_REMOTE_DEVICE                          0x00000010
#define FILE_DEVICE_IS_MOUNTED                      0x00000020
#define FILE_VIRTUAL_VOLUME                         0x00000040
#define FILE_AUTOGENERATED_DEVICE_NAME              0x00000080
#define FILE_DEVICE_SECURE_OPEN                     0x00000100
#define FILE_CHARACTERISTIC_PNP_DEVICE              0x00000800
#define FILE_CHARACTERISTIC_TS_DEVICE               0x00001000
#define FILE_CHARACTERISTIC_WEBDAV_DEVICE           0x00002000
#define FILE_CHARACTERISTIC_CSV                     0x00010000
#define FILE_DEVICE_ALLOW_APPCONTAINER_TRAVERSAL    0x00020000
#define FILE_PORTABLE_DEVICE                        0x00040000
#define FILE_REMOTE_DEVICE_VSMB                     0x00080000
#define FILE_DEVICE_REQUIRE_SECURITY_CHECK          0x00100000
#endif

#ifndef PIO_APC_ROUTINE_DEFINED
typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved
    );
#define PIO_APC_ROUTINE_DEFINED
#endif  // PIO_APC_ROUTINE_DEFINED


typedef enum _FILE_INFORMATION_CLASS
{
    FileDirectoryInformation                 = 1,
    FileFullDirectoryInformation,            // 2
    FileBothDirectoryInformation,            // 3
    FileBasicInformation,                    // 4
    FileStandardInformation,                 // 5
    FileInternalInformation,                 // 6
    FileEaInformation,                       // 7
    FileAccessInformation,                   // 8
    FileNameInformation,                     // 9
    FileRenameInformation,                   // 10
    FileLinkInformation,                     // 11
    FileNamesInformation,                    // 12
    FileDispositionInformation,              // 13
    FilePositionInformation,                 // 14
    FileFullEaInformation,                   // 15
    FileModeInformation,                     // 16
    FileAlignmentInformation,                // 17
    FileAllInformation,                      // 18
    FileAllocationInformation,               // 19
    FileEndOfFileInformation,                // 20
    FileAlternateNameInformation,            // 21
    FileStreamInformation,                   // 22
    FilePipeInformation,                     // 23
    FilePipeLocalInformation,                // 24
    FilePipeRemoteInformation,               // 25
    FileMailslotQueryInformation,            // 26
    FileMailslotSetInformation,              // 27
    FileCompressionInformation,              // 28
    FileObjectIdInformation,                 // 29
    FileCompletionInformation,               // 30
    FileMoveClusterInformation,              // 31
    FileQuotaInformation,                    // 32
    FileReparsePointInformation,             // 33
    FileNetworkOpenInformation,              // 34
    FileAttributeTagInformation,             // 35
    FileTrackingInformation,                 // 36
    FileIdBothDirectoryInformation,          // 37
    FileIdFullDirectoryInformation,          // 38
    FileValidDataLengthInformation,          // 39
    FileShortNameInformation,                // 40
    FileIoCompletionNotificationInformation, // 41
    FileIoStatusBlockRangeInformation,       // 42
    FileIoPriorityHintInformation,           // 43
    FileSfioReserveInformation,              // 44
    FileSfioVolumeInformation,               // 45
    FileHardLinkInformation,                 // 46
    FileProcessIdsUsingFileInformation,      // 47
    FileNormalizedNameInformation,           // 48
    FileNetworkPhysicalNameInformation,      // 49

    // Windows 7+
    FileIdGlobalTxDirectoryInformation,      // 50
    FileIsRemoteDeviceInformation,           // 51
    FileAttributeCacheInformation,           // 52
    FileNumaNodeInformation,                 // 53
    FileStandardLinkInformation,             // 54
    FileRemoteProtocolInformation,           // 55

    // Windows 8+
    FileRenameInformationBypassAccessCheck,  // 56 (kernel-mode only)
    FileLinkInformationBypassAccessCheck,    // 57 (kernel-mode only)
    FileVolumeNameInformation,               // 58
    FileIdInformation,                       // 59
    FileIdExtdDirectoryInformation,          // 60

    // WinBlue+
    FileReplaceCompletionInformation,        // 61
    FileHardLinkFullIdInformation,           // 62

    // Windows 10 THRESHOLD+
    FileIdExtdBothDirectoryInformation,      // 63
    FileDispositionInformationEx,            // 64
    FileRenameInformationEx,                 // 65
    FileRenameInformationExBypassAccessCheck,// 66 (kernel-mode only)

    // Windows 10 REDSTONE+
    FileDesiredStorageClassInformation,      // 67
    FileStatInformation,                     // 68
    FileMemoryPartitionInformation,          // 69

    // Windows 10 April 2018 Update+
    FileStatLxInformation,                   // 70
    FileCaseSensitiveInformation,            // 71

    // Windows 10 Fall 2018 Update+
    FileLinkInformationEx,                          // 72
    FileLinkInformationExBypassAccessCheck,         // 73
    FileStorageReserveIdInformation,                // 74
    FileCaseSensitiveInformationForceAccessCheck,   // 75

    // Windows 11
    FileKnownFolderInformation,                     // 76

    // WDK 22600
    FileStatBasicInformation,                       // 77
    FileId64ExtdDirectoryInformation,               // 78
    FileId64ExtdBothDirectoryInformation,           // 79
    FileIdAllExtdDirectoryInformation,              // 80
    FileIdAllExtdBothDirectoryInformation,          // 81

    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;


typedef struct _FILE_DIRECTORY_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_DIRECTORY_INFORMATION, *PFILE_DIRECTORY_INFORMATION;


typedef struct _FILE_FULL_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    WCHAR FileName[1];
} FILE_FULL_DIR_INFORMATION, *PFILE_FULL_DIR_INFORMATION;


typedef struct _FILE_BOTH_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;


typedef struct _FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;


typedef struct _FILE_STANDARD_INFORMATION {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_INFORMATION, *PFILE_STANDARD_INFORMATION;

// Windows 10 THRESHOLD+
typedef struct _FILE_STANDARD_INFORMATION_EX {
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG NumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
    BOOLEAN AlternateStream;
    BOOLEAN MetadataAttribute;
} FILE_STANDARD_INFORMATION_EX, *PFILE_STANDARD_INFORMATION_EX;

typedef struct _FILE_INTERNAL_INFORMATION {
    LARGE_INTEGER IndexNumber;
} FILE_INTERNAL_INFORMATION, *PFILE_INTERNAL_INFORMATION;


typedef struct _FILE_EA_INFORMATION {
    ULONG EaSize;
} FILE_EA_INFORMATION, *PFILE_EA_INFORMATION;


typedef struct _FILE_ACCESS_INFORMATION {
    ACCESS_MASK AccessFlags;
} FILE_ACCESS_INFORMATION, *PFILE_ACCESS_INFORMATION;


typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;


typedef struct _FILE_RENAME_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

#define FILE_RENAME_REPLACE_IF_EXISTS                     0x00000001
#define FILE_RENAME_POSIX_SEMANTICS                       0x00000002
#define FILE_RENAME_SUPPRESS_PIN_STATE_INHERITANCE        0x00000004
#define FILE_RENAME_SUPPRESS_STORAGE_RESERVE_INHERITANCE  0x00000008
#define FILE_RENAME_NO_INCREASE_AVAILABLE_SPACE           0x00000010
#define FILE_RENAME_NO_DECREASE_AVAILABLE_SPACE           0x00000020
#define FILE_RENAME_PRESERVE_AVAILABLE_SPACE              0x00000030
#define FILE_RENAME_IGNORE_READONLY_ATTRIBUTE             0x00000040
#define FILE_RENAME_FORCE_RESIZE_TARGET_SR                0x00000080
#define FILE_RENAME_FORCE_RESIZE_SOURCE_SR                0x00000100
#define FILE_RENAME_FORCE_RESIZE_SR                       0x00000180

// Renamed from original FILE_RENAME_INFORMATION in RS1+
typedef struct _FILE_RENAME_INFORMATION_EX {
    ULONG Flags;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION_EX, *PFILE_RENAME_INFORMATION_EX;

typedef struct _FILE_NAMES_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_NAMES_INFORMATION, *PFILE_NAMES_INFORMATION;


typedef struct _FILE_DISPOSITION_INFORMATION {
    BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION, *PFILE_DISPOSITION_INFORMATION;


typedef struct _FILE_POSITION_INFORMATION {
    LARGE_INTEGER CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;


typedef struct _FILE_FULL_EA_INFORMATION {
    ULONG NextEntryOffset;
    UCHAR Flags;
    UCHAR EaNameLength;
    USHORT EaValueLength;
    CHAR EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;


typedef struct _FILE_MODE_INFORMATION {
    ULONG Mode;
} FILE_MODE_INFORMATION, *PFILE_MODE_INFORMATION;

#define FILE_BYTE_ALIGNMENT             0x00000000
#define FILE_WORD_ALIGNMENT             0x00000001
#define FILE_LONG_ALIGNMENT             0x00000003
#define FILE_QUAD_ALIGNMENT             0x00000007
#define FILE_OCTA_ALIGNMENT             0x0000000f
#define FILE_32_BYTE_ALIGNMENT          0x0000001f
#define FILE_64_BYTE_ALIGNMENT          0x0000003f
#define FILE_128_BYTE_ALIGNMENT         0x0000007f
#define FILE_256_BYTE_ALIGNMENT         0x000000ff
#define FILE_512_BYTE_ALIGNMENT         0x000001ff

typedef struct _FILE_ALIGNMENT_INFORMATION {
    ULONG AlignmentRequirement;
} FILE_ALIGNMENT_INFORMATION, *PFILE_ALIGNMENT_INFORMATION;


typedef struct _FILE_ALL_INFORMATION {
    FILE_BASIC_INFORMATION BasicInformation;
    FILE_STANDARD_INFORMATION StandardInformation;
    FILE_INTERNAL_INFORMATION InternalInformation;
    FILE_EA_INFORMATION EaInformation;
    FILE_ACCESS_INFORMATION AccessInformation;
    FILE_POSITION_INFORMATION PositionInformation;
    FILE_MODE_INFORMATION ModeInformation;
    FILE_ALIGNMENT_INFORMATION AlignmentInformation;
    FILE_NAME_INFORMATION NameInformation;
} FILE_ALL_INFORMATION, *PFILE_ALL_INFORMATION;


typedef struct _FILE_ALLOCATION_INFORMATION {
    LARGE_INTEGER AllocationSize;
} FILE_ALLOCATION_INFORMATION, *PFILE_ALLOCATION_INFORMATION;


typedef struct _FILE_END_OF_FILE_INFORMATION {
    LARGE_INTEGER EndOfFile;
} FILE_END_OF_FILE_INFORMATION, *PFILE_END_OF_FILE_INFORMATION;


typedef struct _FILE_STREAM_INFORMATION {
    ULONG NextEntryOffset;
    ULONG StreamNameLength;
    LARGE_INTEGER StreamSize;
    LARGE_INTEGER StreamAllocationSize;
    WCHAR StreamName[1];
} FILE_STREAM_INFORMATION, *PFILE_STREAM_INFORMATION;

// ReadMode
#define FILE_PIPE_BYTE_STREAM_MODE      0x00000000
#define FILE_PIPE_MESSAGE_MODE          0x00000001

// CompletionMode
#define FILE_PIPE_QUEUE_OPERATION       0x00000000
#define FILE_PIPE_COMPLETE_OPERATION    0x00000001

typedef struct _FILE_PIPE_INFORMATION {
     ULONG ReadMode;
     ULONG CompletionMode;
} FILE_PIPE_INFORMATION, *PFILE_PIPE_INFORMATION;

// NamedPipeType
#define FILE_PIPE_BYTE_STREAM_TYPE      0x00000000
#define FILE_PIPE_MESSAGE_TYPE          0x00000001
#define FILE_PIPE_ACCEPT_REMOTE_CLIENTS 0x00000000
#define FILE_PIPE_REJECT_REMOTE_CLIENTS 0x00000002
#define FILE_PIPE_TYPE_VALID_MASK       0x00000003

// NamedPipeConfiguration
#define FILE_PIPE_INBOUND               0x00000000
#define FILE_PIPE_OUTBOUND              0x00000001
#define FILE_PIPE_FULL_DUPLEX           0x00000002

// NamedPipeState
#define FILE_PIPE_DISCONNECTED_STATE    0x00000001
#define FILE_PIPE_LISTENING_STATE       0x00000002
#define FILE_PIPE_CONNECTED_STATE       0x00000003
#define FILE_PIPE_CLOSING_STATE         0x00000004

// NamedPipeEnd
#define FILE_PIPE_CLIENT_END            0x00000000
#define FILE_PIPE_SERVER_END            0x00000001

typedef struct _FILE_PIPE_LOCAL_INFORMATION {
     ULONG NamedPipeType;
     ULONG NamedPipeConfiguration;
     ULONG MaximumInstances;
     ULONG CurrentInstances;
     ULONG InboundQuota;
     ULONG ReadDataAvailable;
     ULONG OutboundQuota;
     ULONG WriteQuotaAvailable;
     ULONG NamedPipeState;
     ULONG NamedPipeEnd;
} FILE_PIPE_LOCAL_INFORMATION, *PFILE_PIPE_LOCAL_INFORMATION;


typedef struct _FILE_PIPE_REMOTE_INFORMATION {
     LARGE_INTEGER CollectDataTime;
     ULONG MaximumCollectionCount;
} FILE_PIPE_REMOTE_INFORMATION, *PFILE_PIPE_REMOTE_INFORMATION;


typedef struct _FILE_MAILSLOT_QUERY_INFORMATION {
    ULONG MaximumMessageSize;
    ULONG MailslotQuota;
    ULONG NextMessageSize;
    ULONG MessagesAvailable;
    LARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_QUERY_INFORMATION, *PFILE_MAILSLOT_QUERY_INFORMATION;


typedef struct _FILE_MAILSLOT_SET_INFORMATION {
    PLARGE_INTEGER ReadTimeout;
} FILE_MAILSLOT_SET_INFORMATION, *PFILE_MAILSLOT_SET_INFORMATION;


typedef struct _FILE_COMPRESSION_INFORMATION {
    LARGE_INTEGER CompressedFileSize;
    USHORT CompressionFormat;
    UCHAR CompressionUnitShift;
    UCHAR ChunkShift;
    UCHAR ClusterShift;
    UCHAR Reserved[3];
} FILE_COMPRESSION_INFORMATION, *PFILE_COMPRESSION_INFORMATION;


typedef struct _FILE_LINK_INFORMATION {
    BOOLEAN ReplaceIfExists;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_INFORMATION, *PFILE_LINK_INFORMATION;

#define FILE_LINK_REPLACE_IF_EXISTS                     0x00000001      // If a file with the given name already exists, it should be replaced with the new link. Equivalent to the ReplaceIfExists field used with the FileLinkInformation information class.
#define FILE_LINK_POSIX_SEMANTICS                       0x00000002      // If FILE_LINK_REPLACE_IF_EXISTS is also specified, allow replacing a file even if there are existing handles to it. Existing handles to the replaced file continue to be valid for operations such as read and write. Any subsequent opens of the target name will open the new link, not the replaced file.
#define FILE_LINK_SUPPRESS_STORAGE_RESERVE_INHERITANCE  0x00000008      // When creating a link in a new directory, suppress any inheritance rules related to the storage reserve ID property of the file.
#define FILE_LINK_NO_INCREASE_AVAILABLE_SPACE           0x00000010      // If FILE_LINK_SUPPRESS_STORAGE_RESERVE_INHERITANCE is not also specified, when creating a link in a new directory, automatically resize affected storage reserve areas to prevent the user visible free space on the volume from increasing. Requires manage volume access.
#define FILE_LINK_NO_DECREASE_AVAILABLE_SPACE           0x00000020      // If FILE_LINK_SUPPRESS_STORAGE_RESERVE_INHERITANCE is not also specified, when creating a link in a new directory, automatically resize affected storage reserve areas to prevent the user visible free space on the volume from decreasing. Requires manage volume access.
#define FILE_LINK_PRESERVE_AVAILABLE_SPACE              0x00000030      // Equivalent to specifying both FILE_LINK_NO_INCREASE_AVAILABLE_SPACE and FILE_LINK_NO_DECREASE_AVAILABLE_SPACE.
#define FILE_LINK_IGNORE_READONLY_ATTRIBUTE             0x00000040
#define FILE_LINK_FORCE_RESIZE_TARGET_SR                0x00000080
#define FILE_LINK_FORCE_RESIZE_SOURCE_SR                0x00000100
#define FILE_LINK_FORCE_RESIZE_SR                       0x00000180

// Renamed from original FILE_LINK_INFORMATION in RS5+
typedef struct _FILE_LINK_INFORMATION_EX {
    ULONG Flags;              // FileLinkInformationEx
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_INFORMATION_EX, *PFILE_LINK_INFORMATION_EX;


typedef struct _FILE_OBJECTID_INFORMATION
{
    LONGLONG FileReference;
    UCHAR ObjectId[16];
    union {
        struct {
            UCHAR BirthVolumeId[16];
            UCHAR BirthObjectId[16];
            UCHAR DomainId[16];
        } ;
        UCHAR ExtendedInfo[48];
    };
} FILE_OBJECTID_INFORMATION, *PFILE_OBJECTID_INFORMATION;


typedef struct _FILE_COMPLETION_INFORMATION {
    HANDLE Port;
    PVOID Key;
} FILE_COMPLETION_INFORMATION, *PFILE_COMPLETION_INFORMATION;


typedef struct _FILE_MOVE_CLUSTER_INFORMATION {
    ULONG ClusterCount;
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_MOVE_CLUSTER_INFORMATION, *PFILE_MOVE_CLUSTER_INFORMATION;


typedef struct _FILE_NETWORK_OPEN_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;


typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION {
    ULONG FileAttributes;
    ULONG ReparseTag;
} FILE_ATTRIBUTE_TAG_INFORMATION, *PFILE_ATTRIBUTE_TAG_INFORMATION;


typedef struct _FILE_TRACKING_INFORMATION {
    HANDLE DestinationFile;
    ULONG ObjectInformationLength;
    CHAR ObjectInformation[1];
} FILE_TRACKING_INFORMATION, *PFILE_TRACKING_INFORMATION;


typedef struct _FILE_REPARSE_POINT_INFORMATION {
    LONGLONG FileReference;
    ULONG Tag;
} FILE_REPARSE_POINT_INFORMATION, *PFILE_REPARSE_POINT_INFORMATION;


typedef struct _FILE_QUOTA_INFORMATION {
    ULONG NextEntryOffset;
    ULONG SidLength;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER QuotaUsed;
    LARGE_INTEGER QuotaThreshold;
    LARGE_INTEGER QuotaLimit;
    SID Sid;
} FILE_QUOTA_INFORMATION, *PFILE_QUOTA_INFORMATION;


typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;


typedef struct _FILE_ID_FULL_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    LARGE_INTEGER FileId;
    WCHAR FileName[1];
} FILE_ID_FULL_DIR_INFORMATION, *PFILE_ID_FULL_DIR_INFORMATION;


typedef struct _FILE_VALID_DATA_LENGTH_INFORMATION {
    LARGE_INTEGER ValidDataLength;
} FILE_VALID_DATA_LENGTH_INFORMATION, *PFILE_VALID_DATA_LENGTH_INFORMATION;

//
// Don't queue an entry to an associated completion port if returning success
// synchronously.
//
#define FILE_SKIP_COMPLETION_PORT_ON_SUCCESS    0x1

//
// Don't set the file handle event on IO completion.
//
#define FILE_SKIP_SET_EVENT_ON_HANDLE           0x2

//
// Don't set user supplied event on successful fast-path IO completion.
//
#define FILE_SKIP_SET_USER_EVENT_ON_FAST_IO     0x4

typedef  struct _FILE_IO_COMPLETION_NOTIFICATION_INFORMATION {
    ULONG Flags;
} FILE_IO_COMPLETION_NOTIFICATION_INFORMATION, *PFILE_IO_COMPLETION_NOTIFICATION_INFORMATION;


typedef  struct _FILE_PROCESS_IDS_USING_FILE_INFORMATION {
    ULONG NumberOfProcessIdsInList;
    ULONG_PTR ProcessIdList[1];
} FILE_PROCESS_IDS_USING_FILE_INFORMATION, *PFILE_PROCESS_IDS_USING_FILE_INFORMATION;


typedef struct _FILE_IOSTATUSBLOCK_RANGE_INFORMATION {
    PUCHAR       IoStatusBlockRange;
    ULONG        Length;
} FILE_IOSTATUSBLOCK_RANGE_INFORMATION, *PFILE_IOSTATUSBLOCK_RANGE_INFORMATION;


typedef enum _IO_PRIORITY_HINT {
    IoPriorityVeryLow = 0,    // Winfs promotion, defragging, content indexing and other background I/Os
    IoPriorityLow,            // Prefetching for applications.
    IoPriorityNormal,         // Normal I/Os
    IoPriorityHigh,           // Used by filesystems for checkpoint I/O
    IoPriorityCritical,       // Used by memory manager. Not available for applications.
    MaxIoPriorityTypes
} IO_PRIORITY_HINT;


typedef struct _FILE_IO_PRIORITY_HINT_INFORMATION {
    IO_PRIORITY_HINT   PriorityHint;
} FILE_IO_PRIORITY_HINT_INFORMATION, *PFILE_IO_PRIORITY_HINT_INFORMATION;


//
// Support to reserve bandwidth for a file handle.
//

typedef struct _FILE_SFIO_RESERVE_INFORMATION {
    ULONG RequestsPerPeriod;
    ULONG Period;
    BOOLEAN RetryFailures;
    BOOLEAN Discardable;
    ULONG RequestSize;
    ULONG NumOutstandingRequests;
} FILE_SFIO_RESERVE_INFORMATION, *PFILE_SFIO_RESERVE_INFORMATION;

//
// Support to query bandwidth properties of a volume.
//

typedef struct _FILE_SFIO_VOLUME_INFORMATION {
    ULONG MaximumRequestsPerPeriod;
    ULONG MinimumPeriod;
    ULONG MinimumTransferSize;
} FILE_SFIO_VOLUME_INFORMATION, *PFILE_SFIO_VOLUME_INFORMATION;


typedef struct _FILE_LINK_ENTRY_INFORMATION {
    ULONG NextEntryOffset;
    LONGLONG ParentFileId;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_ENTRY_INFORMATION, *PFILE_LINK_ENTRY_INFORMATION;


typedef struct _FILE_LINKS_INFORMATION
{
    ULONG BytesNeeded;
    ULONG EntriesReturned;
    FILE_LINK_ENTRY_INFORMATION Entry;
} FILE_LINKS_INFORMATION, *PFILE_LINKS_INFORMATION;

#ifndef FILE_SUPPORTS_INTEGRITY_STREAMS             // In newer Microsoft SDKs only. Consistent with CLANG and GCC
typedef struct _FILE_ID_128
{
    BYTE Identifier[16];
} FILE_ID_128, *PFILE_ID_128;
#endif

typedef struct _FILE_LINK_ENTRY_FULL_ID_INFORMATION
{
    ULONG NextEntryOffset;
    FILE_ID_128 ParentFileId;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_LINK_ENTRY_FULL_ID_INFORMATION, *PFILE_LINK_ENTRY_FULL_ID_INFORMATION;

#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_WRITELOCKED         0x00000001
#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_VISIBLE_TO_TX       0x00000002
#define FILE_ID_GLOBAL_TX_DIR_INFO_FLAG_VISIBLE_OUTSIDE_TX  0x00000004

typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION
{
  ULONG          NextEntryOffset;
  ULONG          FileIndex;
  LARGE_INTEGER  CreationTime;
  LARGE_INTEGER  LastAccessTime;
  LARGE_INTEGER  LastWriteTime;
  LARGE_INTEGER  ChangeTime;
  LARGE_INTEGER  EndOfFile;
  LARGE_INTEGER  AllocationSize;
  ULONG          FileAttributes;
  ULONG          FileNameLength;
  LARGE_INTEGER  FileId;
  GUID           LockingTransactionId;
  ULONG          TxInfoFlags;
  WCHAR          FileName[1];
} FILE_ID_GLOBAL_TX_DIR_INFORMATION, *PFILE_ID_GLOBAL_TX_DIR_INFORMATION;


typedef struct _FILE_IS_REMOTE_DEVICE_INFORMATION
{
    BOOLEAN IsRemote;
} FILE_IS_REMOTE_DEVICE_INFORMATION, *PFILE_IS_REMOTE_DEVICE_INFORMATION;

typedef struct _FILE_NUMA_NODE_INFORMATION {
    USHORT NodeNumber;
} FILE_NUMA_NODE_INFORMATION, *PFILE_NUMA_NODE_INFORMATION;

typedef struct _FILE_STANDARD_LINK_INFORMATION
{
    ULONG NumberOfAccessibleLinks;
    ULONG TotalNumberOfLinks;
    BOOLEAN DeletePending;
    BOOLEAN Directory;
} FILE_STANDARD_LINK_INFORMATION, *PFILE_STANDARD_LINK_INFORMATION;

typedef struct _FILE_VOLUME_NAME_INFORMATION
{
    ULONG DeviceNameLength;
    WCHAR DeviceName[1];
} FILE_VOLUME_NAME_INFORMATION, *PFILE_VOLUME_NAME_INFORMATION;

typedef struct _FILE_ID_INFORMATION
{
    ULONGLONG VolumeSerialNumber;
    FILE_ID_128 FileId;
} FILE_ID_INFORMATION, *PFILE_ID_INFORMATION;

typedef struct _FILE_ID_EXTD_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    WCHAR FileName[1];
} FILE_ID_EXTD_DIR_INFORMATION, *PFILE_ID_EXTD_DIR_INFORMATION;

typedef struct _FILE_ID_EXTD_BOTH_DIR_INFORMATION
{
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    ULONG ReparsePointTag;
    FILE_ID_128 FileId;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_ID_EXTD_BOTH_DIR_INFORMATION, *PFILE_ID_EXTD_BOTH_DIR_INFORMATION;

#define FILE_DISPOSITION_DO_NOT_DELETE              0x00000000
#define FILE_DISPOSITION_DELETE                     0x00000001
#define FILE_DISPOSITION_POSIX_SEMANTICS            0x00000002
#define FILE_DISPOSITION_FORCE_IMAGE_SECTION_CHECK  0x00000004
#define FILE_DISPOSITION_ON_CLOSE                   0x00000008
#define FILE_DISPOSITION_IGNORE_READONLY_ATTRIBUTE  0x00000010

typedef struct _FILE_DISPOSITION_INFORMATION_EX
{
    ULONG Flags;
} FILE_DISPOSITION_INFORMATION_EX, *PFILE_DISPOSITION_INFORMATION_EX;

#ifndef FILE_STORAGE_TIER_FLAG_SMR
typedef enum _FILE_STORAGE_TIER_CLASS {

    FileStorageTierClassUnspecified = 0,
    FileStorageTierClassCapacity,
    FileStorageTierClassPerformance,
    FileStorageTierClassMax

} FILE_STORAGE_TIER_CLASS, *PFILE_STORAGE_TIER_CLASS;

typedef struct _FILE_DESIRED_STORAGE_CLASS_INFORMATION
{
    FILE_STORAGE_TIER_CLASS Class;      // Class type of the tier
    ULONG Flags;                        // Flags

} FILE_DESIRED_STORAGE_CLASS_INFORMATION, *PFILE_DESIRED_STORAGE_CLASS_INFORMATION;
#endif

#ifndef LX_FILE_METADATA_HAS_UID        // In Windows 11 SDK, this is already defined
typedef struct _FILE_STAT_INFORMATION
{
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    DWORD FileAttributes;
    DWORD ReparseTag;
    DWORD NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
} FILE_STAT_INFORMATION, *PFILE_STAT_INFORMATION;
#endif

typedef struct _FILE_MEMORY_PARTITION_INFORMATION
{
    ULONG_PTR OwnerPartitionHandle;
    union {
        struct {
            UCHAR NoCrossPartitionAccess;
            UCHAR Spare[3];
        } DUMMYSTRUCTNAME;

        ULONG AllFlags;
    } Flags;
} FILE_MEMORY_PARTITION_INFORMATION, *PFILE_MEMORY_PARTITION_INFORMATION;

#ifndef LX_FILE_METADATA_HAS_UID
#define LX_FILE_METADATA_HAS_UID       0x00000001
#define LX_FILE_METADATA_HAS_GID       0x00000002
#define LX_FILE_METADATA_HAS_MODE      0x00000004
#define LX_FILE_METADATA_HAS_DEVICE_ID 0x00000008
#define LX_FILE_CASE_SENSITIVE_DIR     0x00000010

typedef struct _FILE_STAT_LX_INFORMATION {
    LARGE_INTEGER FileId;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER AllocationSize;
    LARGE_INTEGER EndOfFile;
    ULONG FileAttributes;
    ULONG ReparseTag;
    ULONG NumberOfLinks;
    ACCESS_MASK EffectiveAccess;
    ULONG LxFlags;
    ULONG LxUid;
    ULONG LxGid;
    ULONG LxMode;
    ULONG LxDeviceIdMajor;
    ULONG LxDeviceIdMinor;
} FILE_STAT_LX_INFORMATION, *PFILE_STAT_LX_INFORMATION;
#endif

#ifndef FILE_CS_FLAG_CASE_SENSITIVE_DIR
#define FILE_CS_FLAG_CASE_SENSITIVE_DIR     0x00000001

typedef struct _FILE_CASE_SENSITIVE_INFORMATION
{
    DWORD Flags;
} FILE_CASE_SENSITIVE_INFORMATION, *PFILE_CASE_SENSITIVE_INFORMATION;
#endif

#ifndef FSCTL_REARRANGE_FILE
typedef enum _STORAGE_RESERVE_ID
{

    StorageReserveIdNone = 0,
    StorageReserveIdHard,
    StorageReserveIdSoft,
    StorageReserveIdUpdateScratch,

    StorageReserveIdMax

} STORAGE_RESERVE_ID, * PSTORAGE_RESERVE_ID;
#endif

typedef struct _FILE_SET_STORAGE_RESERVE_ID_INFORMATION
{
    STORAGE_RESERVE_ID StorageReserveId;
} FILE_SET_STORAGE_RESERVE_ID_INFORMATION, *PFILE_SET_STORAGE_RESERVE_ID_INFORMATION;

typedef enum _FILE_KNOWN_FOLDER_TYPE
{
    KnownFolderNone,
    KnownFolderDesktop,
    KnownFolderDocuments,
    KnownFolderDownloads,
    KnownFolderMusic,
    KnownFolderPictures,
    KnownFolderVideos,
    KnownFolderMax
} FILE_KNOWN_FOLDER_TYPE;

typedef struct _FILE_KNOWN_FOLDER_INFORMATION
{
    FILE_KNOWN_FOLDER_TYPE Type;
} FILE_KNOWN_FOLDER_INFORMATION, *PFILE_KNOWN_FOLDER_INFORMATION;



typedef enum _FSINFOCLASS {
    FileFsVolumeInformation         = 1,
    FileFsLabelInformation,        // 2
    FileFsSizeInformation,         // 3
    FileFsDeviceInformation,       // 4
    FileFsAttributeInformation,    // 5
    FileFsControlInformation,      // 6
    FileFsFullSizeInformation,     // 7
    FileFsObjectIdInformation,     // 8
    FileFsDriverPathInformation,   // 9
    FileFsVolumeFlagsInformation,  // 10
    FileFsSectorSizeInformation,   // 11
    FileFsDataCopyInformation,     // 12
    FileFsMetadataSizeInformation, // 13
    FileFsFullSizeInformationEx,   // 14
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;


typedef struct _FILE_FS_VOLUME_INFORMATION {
    LARGE_INTEGER VolumeCreationTime;
    ULONG VolumeSerialNumber;
    ULONG VolumeLabelLength;
    BOOLEAN SupportsObjects;
    WCHAR VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;


typedef struct _FILE_FS_LABEL_INFORMATION {
    ULONG VolumeLabelLength;
    WCHAR VolumeLabel[1];
} FILE_FS_LABEL_INFORMATION, *PFILE_FS_LABEL_INFORMATION;


typedef struct _FILE_FS_SIZE_INFORMATION {
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER AvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_SIZE_INFORMATION, *PFILE_FS_SIZE_INFORMATION;


typedef struct _FILE_FS_DEVICE_INFORMATION {
    DEVICE_TYPE DeviceType;
    ULONG Characteristics;
} FILE_FS_DEVICE_INFORMATION, *PFILE_FS_DEVICE_INFORMATION;


typedef struct _FILE_FS_ATTRIBUTE_INFORMATION {
    ULONG FileSystemAttributes;
    LONG MaximumComponentNameLength;
    ULONG FileSystemNameLength;
    WCHAR FileSystemName[1];
} FILE_FS_ATTRIBUTE_INFORMATION, *PFILE_FS_ATTRIBUTE_INFORMATION;

#define FILE_VC_QUOTA_NONE                  0x00000000
#define FILE_VC_QUOTA_TRACK                 0x00000001
#define FILE_VC_QUOTA_ENFORCE               0x00000002
#define FILE_VC_QUOTA_MASK                  0x00000003
#define FILE_VC_CONTENT_INDEX_DISABLED      0x00000008
#define FILE_VC_LOG_QUOTA_THRESHOLD         0x00000010
#define FILE_VC_LOG_QUOTA_LIMIT             0x00000020
#define FILE_VC_LOG_VOLUME_THRESHOLD        0x00000040
#define FILE_VC_LOG_VOLUME_LIMIT            0x00000080
#define FILE_VC_QUOTAS_INCOMPLETE           0x00000100
#define FILE_VC_QUOTAS_REBUILDING           0x00000200

typedef struct _FILE_FS_CONTROL_INFORMATION {
    LARGE_INTEGER FreeSpaceStartFiltering;
    LARGE_INTEGER FreeSpaceThreshold;
    LARGE_INTEGER FreeSpaceStopFiltering;
    LARGE_INTEGER DefaultQuotaThreshold;
    LARGE_INTEGER DefaultQuotaLimit;
    ULONG FileSystemControlFlags;
} FILE_FS_CONTROL_INFORMATION, *PFILE_FS_CONTROL_INFORMATION;


typedef struct _FILE_FS_FULL_SIZE_INFORMATION {
    LARGE_INTEGER TotalAllocationUnits;
    LARGE_INTEGER CallerAvailableAllocationUnits;
    LARGE_INTEGER ActualAvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION, *PFILE_FS_FULL_SIZE_INFORMATION;


typedef struct _FILE_FS_OBJECTID_INFORMATION {
    UCHAR ObjectId[16];
    UCHAR ExtendedInfo[48];
} FILE_FS_OBJECTID_INFORMATION, *PFILE_FS_OBJECTID_INFORMATION;


typedef struct _FILE_FS_DRIVER_PATH_INFORMATION {
    BOOLEAN DriverInPath;
    ULONG   DriverNameLength;
    WCHAR   DriverName[1];
} FILE_FS_DRIVER_PATH_INFORMATION, *PFILE_FS_DRIVER_PATH_INFORMATION;


typedef struct _FILE_FS_VOLUME_FLAGS_INFORMATION {
    ULONG Flags;
} FILE_FS_VOLUME_FLAGS_INFORMATION, *PFILE_FS_VOLUME_FLAGS_INFORMATION;

#define SSINFO_FLAGS_ALIGNED_DEVICE                 0x00000001
#define SSINFO_FLAGS_PARTITION_ALIGNED_ON_DEVICE    0x00000002
#define SSINFO_FLAGS_NO_SEEK_PENALTY                0x00000004
#define SSINFO_FLAGS_TRIM_ENABLED                   0x00000008
#define SSINFO_FLAGS_BYTE_ADDRESSABLE               0x00000010

typedef struct _FILE_FS_SECTOR_SIZE_INFORMATION {
    ULONG LogicalBytesPerSector;
    ULONG PhysicalBytesPerSectorForAtomicity;
    ULONG PhysicalBytesPerSectorForPerformance;
    ULONG FileSystemEffectivePhysicalBytesPerSectorForAtomicity;
    ULONG Flags;
    ULONG ByteOffsetForSectorAlignment;
    ULONG ByteOffsetForPartitionAlignment;
} FILE_FS_SECTOR_SIZE_INFORMATION, *PFILE_FS_SECTOR_SIZE_INFORMATION;


typedef struct _FILE_FS_DATA_COPY_INFORMATION
{
    ULONG NumberOfCopies;
} FILE_FS_DATA_COPY_INFORMATION, * PFILE_FS_DATA_COPY_INFORMATION;


typedef struct _FILE_FS_METADATA_SIZE_INFORMATION
{
    LARGE_INTEGER TotalMetadataAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_METADATA_SIZE_INFORMATION, * PFILE_FS_METADATA_SIZE_INFORMATION;


typedef struct _FILE_FS_FULL_SIZE_INFORMATION_EX
{
    LARGE_INTEGER ActualTotalAllocationUnits;
    LARGE_INTEGER ActualAvailableAllocationUnits;
    LARGE_INTEGER ActualPoolUnavailableAllocationUnits;
    LARGE_INTEGER CallerTotalAllocationUnits;
    LARGE_INTEGER CallerAvailableAllocationUnits;
    LARGE_INTEGER CallerPoolUnavailableAllocationUnits;
    LARGE_INTEGER UsedAllocationUnits;
    LARGE_INTEGER TotalReservedAllocationUnits;
    LARGE_INTEGER VolumeStorageReserveAllocationUnits;
    LARGE_INTEGER AvailableCommittedAllocationUnits;
    LARGE_INTEGER PoolAvailableAllocationUnits;
    ULONG SectorsPerAllocationUnit;
    ULONG BytesPerSector;
} FILE_FS_FULL_SIZE_INFORMATION_EX, * PFILE_FS_FULL_SIZE_INFORMATION_EX;

NTSYSAPI
NTSTATUS
NTAPI
NtCreateFile(
    OUT PHANDLE FileHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER AllocationSize,
    IN  ULONG FileAttributes,
    IN  ULONG ShareAccess,
    IN  ULONG CreateDisposition,
    IN  ULONG CreateOptions,
    IN  PVOID EaBuffer,
    IN  ULONG EaLength);

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateFile(
    OUT PHANDLE FileHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER AllocationSize,
    IN  ULONG FileAttributes,
    IN  ULONG ShareAccess,
    IN  ULONG CreateDisposition,
    IN  ULONG CreateOptions,
    IN  PVOID EaBuffer,
    IN  ULONG EaLength);

NTSYSAPI
NTSTATUS
NTAPI
NtOpenFile(
    OUT PHANDLE FileHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG ShareAccess,
    IN  ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtCreateNamedPipeFile(
     OUT PHANDLE FileHandle,
     IN  ULONG DesiredAccess,
     IN  POBJECT_ATTRIBUTES ObjectAttributes,
     OUT PIO_STATUS_BLOCK IoStatusBlock,
     IN  ULONG ShareAccess,
     IN  ULONG CreateDisposition,
     IN  ULONG CreateOptions,
     IN  ULONG NamedPipeType,
     IN  ULONG ReadMode,
     IN  ULONG CompletionMode,
     IN  ULONG MaximumInstances,
     IN  ULONG InboundQuota,
     IN  ULONG OutboundQuota,
     IN  PLARGE_INTEGER DefaultTimeout OPTIONAL
     );

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenFile(
    OUT PHANDLE FileHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG ShareAccess,
    IN  ULONG OpenOptions
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryAttributesFile(
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PFILE_BASIC_INFORMATION FileInformation
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryAttributesFile(
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    OUT PFILE_BASIC_INFORMATION FileInformation
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryDirectoryFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass,
    IN  BOOLEAN ReturnSingleEntry,
    IN  PUNICODE_STRING FileName OPTIONAL,
    IN  BOOLEAN RestartScan
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryDirectoryFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass,
    IN  BOOLEAN ReturnSingleEntry,
    IN  PUNICODE_STRING FileName OPTIONAL,
    IN  BOOLEAN RestartScan
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryVolumeInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FsInformation,
    IN  ULONG Length,
    IN  FS_INFORMATION_CLASS FsInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVolumeInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FsInformation,
    IN  ULONG Length,
    IN  FS_INFORMATION_CLASS FsInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID FileInformation,
    IN  ULONG Length,
    IN  FILE_INFORMATION_CLASS FileInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetVolumeInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FsInformation,
    IN  ULONG Length,
    IN  FS_INFORMATION_CLASS FsInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetVolumeInformationFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FsInformation,
    IN  ULONG Length,
    IN  FS_INFORMATION_CLASS FsInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryEaFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN  ULONG Length,
    IN  BOOLEAN ReturnSingleEntry,
    IN  PVOID EaList OPTIONAL,
    IN  ULONG EaListLength,
    IN  PULONG EaIndex OPTIONAL,
    IN  BOOLEAN RestartScan
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryEaFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN  ULONG Length,
    IN  BOOLEAN ReturnSingleEntry,
    IN  PVOID EaList OPTIONAL,
    IN  ULONG EaListLength,
    IN  PULONG EaIndex OPTIONAL,
    IN  BOOLEAN RestartScan
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetEaFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID Buffer,
    IN  ULONG Length
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEaFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID Buffer,
    IN  ULONG Length
    );

NTSYSAPI
NTSTATUS
NTAPI
NtReadFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN  ULONG Length,
    IN  PLARGE_INTEGER ByteOffset OPTIONAL,
    IN  PULONG Key OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtReadFileScatter(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PFILE_SEGMENT_ELEMENT SegmentArray,
    IN  ULONG Length,
    IN  PLARGE_INTEGER ByteOffset OPTIONAL,
    IN  PULONG Key OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwReadFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID Buffer,
    IN  ULONG Length,
    IN  PLARGE_INTEGER ByteOffset OPTIONAL,
    IN  PULONG Key OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID Buffer,
    IN  ULONG Length,
    IN  PLARGE_INTEGER ByteOffset OPTIONAL,
    IN  PULONG Key OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwWriteFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PVOID Buffer,
    IN  ULONG Length,
    IN  PLARGE_INTEGER ByteOffset OPTIONAL,
    IN  PULONG Key OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeleteFile(
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwDeleteFile(
    IN POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushBuffersFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushBuffersFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDeviceIoControlFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event,
    IN  PIO_APC_ROUTINE ApcRoutine,
    IN  PVOID ApcContext,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG IoControlCode,
    IN  PVOID InputBuffer,
    IN  ULONG InputBufferLength,
    IN  PVOID OutputBuffer,
    IN  ULONG OutputBufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwDeviceIoControlFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event,
    IN  PIO_APC_ROUTINE ApcRoutine,
    IN  PVOID ApcContext,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG IoControlCode,
    IN  PVOID InputBuffer,
    IN  ULONG InputBufferLength,
    IN  PVOID OutputBuffer,
    IN  ULONG OutputBufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFsControlFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event,
    IN  PIO_APC_ROUTINE ApcRoutine,
    IN  PVOID ApcContext,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG FsControlCode,
    IN  PVOID InputBuffer,
    IN  ULONG InputBufferLength,
    IN  PVOID OutputBuffer,
    IN  ULONG OutputBufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwFsControlFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event,
    IN  PIO_APC_ROUTINE ApcRoutine,
    IN  PVOID ApcContext,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  ULONG FsControlCode,
    IN  PVOID InputBuffer,
    IN  ULONG InputBufferLength,
    IN  PVOID OutputBuffer,
    IN  ULONG OutputBufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtCancelIoFile(
    IN  HANDLE Filehandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCancelIoFile(
    IN  HANDLE Filehandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock
    );

NTSYSAPI
NTSTATUS
NTAPI
NtLockFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER ByteOffset,
    IN  PLARGE_INTEGER Length,
    IN  ULONG Key,
    IN  BOOLEAN FailImmediately,
    IN  BOOLEAN ExclusiveLock
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwLockFile(
    IN  HANDLE FileHandle,
    IN  HANDLE Event OPTIONAL,
    IN  PIO_APC_ROUTINE ApcRoutine OPTIONAL,
    IN  PVOID ApcContext OPTIONAL,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER ByteOffset,
    IN  PLARGE_INTEGER Length,
    IN  ULONG Key,
    IN  BOOLEAN FailImmediately,
    IN  BOOLEAN ExclusiveLock
    );

NTSYSAPI
NTSTATUS
NTAPI
NtUnlockFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER ByteOffset,
    IN  PLARGE_INTEGER Length,
    IN  ULONG Key
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwUnlockFile(
    IN  HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN  PLARGE_INTEGER ByteOffset,
    IN  PLARGE_INTEGER Length,
    IN  ULONG Key
    );


//-----------------------------------------------------------------------------
// DOS <-> NT path functions

// OUT Disposition values for RtlNtPathNameToDosPathName
#define RTL_NT_PATH_NAME_TO_DOS_PATH_NAME_AMBIGUOUS   (0x00000001)
#define RTL_NT_PATH_NAME_TO_DOS_PATH_NAME_UNC         (0x00000002)
#define RTL_NT_PATH_NAME_TO_DOS_PATH_NAME_DRIVE       (0x00000003)
#define RTL_NT_PATH_NAME_TO_DOS_PATH_NAME_ALREADY_DOS (0x00000004)

typedef enum _RTL_PATH_TYPE
{
    RtlPathTypeUnknown,         // 0
    RtlPathTypeUncAbsolute,     // 1
    RtlPathTypeDriveAbsolute,   // 2
    RtlPathTypeDriveRelative,   // 3
    RtlPathTypeRooted,          // 4
    RtlPathTypeRelative,        // 5
    RtlPathTypeLocalDevice,     // 6
    RtlPathTypeRootLocalDevice  // 7
} RTL_PATH_TYPE, *PRTL_PATH_TYPE;

NTSYSAPI
ULONG
NTAPI
RtlIsDosDeviceName_U(
    IN PWSTR DosFileName
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlDosPathNameToNtPathName_U (
    IN  PCWSTR DosPathName,
    OUT PUNICODE_STRING NtPathName,
    OUT PWSTR * NtFileNamePart OPTIONAL,
    OUT PCURDIR DirectoryInfo OPTIONAL
    );

NTSYSAPI
ULONG
NTAPI
RtlGetFullPathName_U(
    PCWSTR lpFileName,
    ULONG nBufferLength,
    PWSTR lpBuffer,
    PWSTR *lpFilePart OPTIONAL
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlDoesFileExists_U(
    IN PCWSTR FileName
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlNtPathNameToDosPathName(                     // Available in Windows XP or newer
    IN     ULONG Flags,
    IN OUT PRTL_UNICODE_STRING_BUFFER Path,
    OUT    PULONG Disposition OPTIONAL,
    IN OUT PWSTR * FilePart OPTIONAL
    );

//-----------------------------------------------------------------------------
// Process functions

//
// RTL_USER_PROCESS_PARAMETERS::Flags
//

#define RTL_USER_PROC_PARAMS_NORMALIZED         0x00000001
#define RTL_USER_PROC_PROFILE_USER              0x00000002
#define RTL_USER_PROC_PROFILE_KERNEL            0x00000004
#define RTL_USER_PROC_PROFILE_SERVER            0x00000008
#define RTL_USER_PROC_RESERVE_1MB               0x00000020
#define RTL_USER_PROC_RESERVE_16MB              0x00000040
#define RTL_USER_PROC_CASE_SENSITIVE            0x00000080
#define RTL_USER_PROC_DISABLE_HEAP_DECOMMIT     0x00000100
#define RTL_USER_PROC_DLL_REDIRECTION_LOCAL     0x00001000
#define RTL_USER_PROC_APP_MANIFEST_PRESENT      0x00002000
#define RTL_USER_PROC_IMAGE_KEY_MISSING         0x00004000
#define RTL_USER_PROC_ADDRESS_SPACE_PRESENT     0x00008000          // Guessed name, flag is set by PspSetupUserProcessAddressSpace
#define RTL_USER_PROC_OPTIN_PROCESS             0x00020000
#define RTL_USER_PROC_SECURE_PROCESS            0x80000000

//
// Global flags that can be set to control system behavior.
// PEB::NtGlobalFlag
//

#define FLG_STOP_ON_EXCEPTION           0x00000001      // user and kernel mode
#define FLG_SHOW_LDR_SNAPS              0x00000002      // user and kernel mode
#define FLG_DEBUG_INITIAL_COMMAND       0x00000004      // kernel mode only up until WINLOGON started
#define FLG_STOP_ON_HUNG_GUI            0x00000008      // kernel mode only while running

#define FLG_HEAP_ENABLE_TAIL_CHECK      0x00000010      // user mode only
#define FLG_HEAP_ENABLE_FREE_CHECK      0x00000020      // user mode only
#define FLG_HEAP_VALIDATE_PARAMETERS    0x00000040      // user mode only
#define FLG_HEAP_VALIDATE_ALL           0x00000080      // user mode only

#define FLG_APPLICATION_VERIFIER        0x00000100      // user mode only
#define FLG_MONITOR_SILENT_PROCESS_EXIT 0x00000200      // Undocumented??
#define FLG_POOL_ENABLE_TAGGING         0x00000400      // kernel mode only
#define FLG_HEAP_ENABLE_TAGGING         0x00000800      // user mode only

#define FLG_USER_STACK_TRACE_DB         0x00001000      // x86 user mode only
#define FLG_KERNEL_STACK_TRACE_DB       0x00002000      // x86 kernel mode only at boot time
#define FLG_MAINTAIN_OBJECT_TYPELIST    0x00004000      // kernel mode only at boot time
#define FLG_HEAP_ENABLE_TAG_BY_DLL      0x00008000      // user mode only

#define FLG_DISABLE_STACK_EXTENSION     0x00010000      // user mode only
#define FLG_ENABLE_CSRDEBUG             0x00020000      // kernel mode only at boot time
#define FLG_ENABLE_KDEBUG_SYMBOL_LOAD   0x00040000      // kernel mode only
#define FLG_DISABLE_PAGE_KERNEL_STACKS  0x00080000      // kernel mode only at boot time

#define FLG_ENABLE_SYSTEM_CRIT_BREAKS   0x00100000      // user mode only
#define FLG_HEAP_DISABLE_COALESCING     0x00200000      // user mode only
#define FLG_ENABLE_CLOSE_EXCEPTIONS     0x00400000      // kernel mode only
#define FLG_ENABLE_EXCEPTION_LOGGING    0x00800000      // kernel mode only

#define FLG_ENABLE_HANDLE_TYPE_TAGGING  0x01000000      // kernel mode only
#define FLG_HEAP_PAGE_ALLOCS            0x02000000      // user mode only
#define FLG_DEBUG_INITIAL_COMMAND_EX    0x04000000      // kernel mode only up until WINLOGON started
#define FLG_DISABLE_DBGPRINT            0x08000000      // kernel mode only

#define FLG_CRITSEC_EVENT_CREATION      0x10000000      // user mode only, Force early creation of resource events
#define FLG_LDR_TOP_DOWN                0x20000000      // user mode only, win64 only
#define FLG_ENABLE_HANDLE_EXCEPTIONS    0x40000000      // kernel mode only
#define FLG_DISABLE_PROTDLLS            0x80000000      // user mode only (smss/winlogon)

#define FLG_VALID_BITS                  0xFFFFFDFF

#define FLG_USERMODE_VALID_BITS        (FLG_STOP_ON_EXCEPTION           | \
                                        FLG_SHOW_LDR_SNAPS              | \
                                        FLG_HEAP_ENABLE_TAIL_CHECK      | \
                                        FLG_HEAP_ENABLE_FREE_CHECK      | \
                                        FLG_HEAP_VALIDATE_PARAMETERS    | \
                                        FLG_HEAP_VALIDATE_ALL           | \
                                        FLG_APPLICATION_VERIFIER        | \
                                        FLG_HEAP_ENABLE_TAGGING         | \
                                        FLG_USER_STACK_TRACE_DB         | \
                                        FLG_HEAP_ENABLE_TAG_BY_DLL      | \
                                        FLG_DISABLE_STACK_EXTENSION     | \
                                        FLG_ENABLE_SYSTEM_CRIT_BREAKS   | \
                                        FLG_HEAP_DISABLE_COALESCING     | \
                                        FLG_DISABLE_PROTDLLS            | \
                                        FLG_HEAP_PAGE_ALLOCS            | \
                                        FLG_CRITSEC_EVENT_CREATION      | \
                                        FLG_LDR_TOP_DOWN)

#define FLG_BOOTONLY_VALID_BITS        (FLG_KERNEL_STACK_TRACE_DB       | \
                                        FLG_MAINTAIN_OBJECT_TYPELIST    | \
                                        FLG_ENABLE_CSRDEBUG             | \
                                        FLG_DEBUG_INITIAL_COMMAND       | \
                                        FLG_DEBUG_INITIAL_COMMAND_EX    | \
                                        FLG_DISABLE_PAGE_KERNEL_STACKS)

#define FLG_KERNELMODE_VALID_BITS      (FLG_STOP_ON_EXCEPTION           | \
                                        FLG_SHOW_LDR_SNAPS              | \
                                        FLG_STOP_ON_HUNG_GUI            | \
                                        FLG_POOL_ENABLE_TAGGING         | \
                                        FLG_ENABLE_KDEBUG_SYMBOL_LOAD   | \
                                        FLG_ENABLE_CLOSE_EXCEPTIONS     | \
                                        FLG_ENABLE_EXCEPTION_LOGGING    | \
                                        FLG_ENABLE_HANDLE_TYPE_TAGGING  | \
                                        FLG_DISABLE_DBGPRINT            | \
                                        FLG_ENABLE_HANDLE_EXCEPTIONS      \
                                       )

// For ProcessExecuteFlags
#define MEM_EXECUTE_OPTION_DISABLE                          0x1
#define MEM_EXECUTE_OPTION_ENABLE                           0x2
#define MEM_EXECUTE_OPTION_DISABLE_THUNK_EMULATION          0x4
#define MEM_EXECUTE_OPTION_PERMANENT                        0x8
#define MEM_EXECUTE_OPTION_EXECUTE_DISPATCH_ENABLE          0x10
#define MEM_EXECUTE_OPTION_IMAGE_DISPATCH_ENABLE            0x20
#define MEM_EXECUTE_OPTION_DISABLE_EXCEPTION_CHAIN_VALIDATION	0x40
#define MEM_EXECUTE_OPTION_VALID_FLAGS                      0x3F

//
// Process Information Classes
//

typedef enum _PROCESSINFOCLASS {
    ProcessBasicInformation                      = 0,
    ProcessQuotaLimits                           = 1,
    ProcessIoCounters                            = 2,
    ProcessVmCounters                            = 3,
    ProcessTimes                                 = 4,
    ProcessBasePriority                          = 5,
    ProcessRaisePriority                         = 6,
    ProcessDebugPort                             = 7,
    ProcessExceptionPort                         = 8,
    ProcessAccessToken                           = 9,
    ProcessLdtInformation                        = 10,
    ProcessLdtSize                               = 11,
    ProcessDefaultHardErrorMode                  = 12,
    ProcessIoPortHandlers                        = 13,  // Note: this is kernel mode only
    ProcessPooledUsageAndLimits                  = 14,
    ProcessWorkingSetWatch                       = 15,
    ProcessUserModeIOPL                          = 16,
    ProcessEnableAlignmentFaultFixup             = 17,
    ProcessPriorityClass                         = 18,
    ProcessWx86Information                       = 19,
    ProcessHandleCount                           = 20,
    ProcessAffinityMask                          = 21,
    ProcessPriorityBoost                         = 22,
    ProcessDeviceMap                             = 23,
    ProcessSessionInformation                    = 24,
    ProcessForegroundInformation                 = 25,
    ProcessWow64Information                      = 26,  // Returns pointer to Peb32(UINT_PTR)
    ProcessImageFileName                         = 27,
    ProcessLUIDDeviceMapsEnabled                 = 28,
    ProcessBreakOnTermination                    = 29,
    ProcessDebugObjectHandle                     = 30,
    ProcessDebugFlags                            = 31,
    ProcessHandleTracing                         = 32,
    ProcessIoPriority                            = 33,
    ProcessExecuteFlags                          = 34,
    ProcessTlsInformation                        = 35,
    ProcessCookie                                = 36,
    ProcessImageInformation                      = 37,
    ProcessCycleTime                             = 38,
    ProcessPagePriority                          = 39,
    ProcessInstrumentationCallback               = 40,
    ProcessThreadStackAllocation                 = 41,
    ProcessWorkingSetWatchEx                     = 42,
    ProcessImageFileNameWin32                    = 43,
    ProcessImageFileMapping                      = 44,
    ProcessAffinityUpdateMode                    = 45,
    ProcessMemoryAllocationMode                  = 46,
    ProcessGroupInformation                      = 47,
    ProcessTokenVirtualizationEnabled            = 48,
    ProcessOwnerInformation                      = 49,
    ProcessWindowInformation                     = 50,
    ProcessHandleInformation                     = 51,
    ProcessMitigationPolicy                      = 52,
    ProcessDynamicFunctionTableInformation       = 53,
    ProcessHandleCheckingMode                    = 54,
    ProcessKeepAliveCount                        = 55,
    ProcessRevokeFileHandles                     = 56,
    ProcessWorkingSetControl                     = 57,
    ProcessHandleTable                           = 58,
    ProcessCheckStackExtentsMode                 = 59,
    ProcessCommandLineInformation                = 60,
    ProcessProtectionInformation                 = 61,
    ProcessMemoryExhaustion                      = 62,
    ProcessFaultInformation                      = 63,
    ProcessTelemetryIdInformation                = 64,
    ProcessCommitReleaseInformation              = 65,
    ProcessReserved1Information                  = 66,
    ProcessReserved2Information                  = 67,
    ProcessSubsystemProcess                      = 68,
    ProcessInPrivate                             = 70,
    ProcessRaiseUMExceptionOnInvalidHandleClose  = 71,
    ProcessSubsystemInformation                  = 75,
    ProcessWin32kSyscallFilterInformation        = 79,
    ProcessEnergyTrackingState                   = 82,
    MaxProcessInfoClass // MaxProcessInfoClass should always be the last enum
} PROCESSINFOCLASS;

//
// Thread Information Classes
//

typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,                 // 0x00
    ThreadTimes,                            // 0x01
    ThreadPriority,                         // 0x02
    ThreadBasePriority,                     // 0x03
    ThreadAffinityMask,                     // 0x04
    ThreadImpersonationToken,               // 0x05  HANDLE
    ThreadDescriptorTableEntry,             // 0x06  ULONG Selector + LDT_ENTRY
    ThreadEnableAlignmentFaultFixup,        // 0x07
    ThreadEventPair,                        // 0x08
    ThreadQuerySetWin32StartAddress,        // 0x09
    ThreadZeroTlsCell,                      // 0x0A
    ThreadPerformanceCount,                 // 0x0B
    ThreadAmILastThread,                    // 0x0C  ULONG
    ThreadIdealProcessor,                   // 0x0D
    ThreadPriorityBoost,                    // 0x0E
    ThreadSetTlsArrayAddress,               // 0x0F
    ThreadIsIoPending,                      // 0x10
    ThreadHideFromDebugger,                 // 0x11
    MaxThreadInfoClass
} THREADINFOCLASS;

typedef struct _SECTION_IMAGE_INFORMATION
{
    PVOID TransferAddress;
    ULONG ZeroBits;
    SIZE_T MaximumStackSize;
    SIZE_T CommittedStackSize;
    ULONG SubSystemType;
    union {
        struct {
            USHORT SubSystemMinorVersion;
            USHORT SubSystemMajorVersion;
        };
        ULONG SubSystemVersion;
    };
    ULONG GpValue;
    USHORT ImageCharacteristics;
    USHORT DllCharacteristics;
    USHORT Machine;
    BOOLEAN ImageContainsCode;
    BOOLEAN Spare1;
    ULONG LoaderFlags;
    ULONG ImageFileSize;                    // Reserved[0] for NT 4.0 and 2000
    ULONG CheckSum;                         // Reserved[1] until Vista
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;


typedef struct _RTL_USER_PROCESS_INFORMATION
{
    ULONG Length;
    HANDLE ProcessHandle;
    HANDLE ThreadHandle;
    CLIENT_ID ClientId;
    SECTION_IMAGE_INFORMATION ImageInformation;

} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;

typedef struct _PROCESS_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;

} PROCESS_BASIC_INFORMATION,*PPROCESS_BASIC_INFORMATION;

typedef struct _PROCESS_BASIC_INFORMATION_WOW64
{
    NTSTATUS ExitStatus;
    ULONG64  PebBaseAddress;
    ULONG64  AffinityMask;
    KPRIORITY BasePriority;
    ULONG64  UniqueProcessId;
    ULONG64  InheritedFromUniqueProcessId;

} PROCESS_BASIC_INFORMATION_WOW64, *PPROCESS_BASIC_INFORMATION_WOW64;

typedef struct _PROCESS_HANDLE_TABLE_ENTRY_INFO
{
    HANDLE HandleValue;
    ULONG_PTR HandleCount;
    ULONG_PTR PointerCount;
    ACCESS_MASK GrantedAccess;
    ULONG ObjectTypeIndex;
    ULONG HandleAttributes;
    ULONG Reserved;
} PROCESS_HANDLE_TABLE_ENTRY_INFO, * PPROCESS_HANDLE_TABLE_ENTRY_INFO;

typedef struct _PROCESS_HANDLE_SNAPSHOT_INFORMATION
{
    ULONG_PTR NumberOfHandles;
    ULONG_PTR Reserved;
    PROCESS_HANDLE_TABLE_ENTRY_INFO Handles[ANYSIZE_ARRAY];
} PROCESS_HANDLE_SNAPSHOT_INFORMATION, *PPROCESS_HANDLE_SNAPSHOT_INFORMATION;

typedef BOOLEAN (*PDLL_INIT_ROUTINE)(
    IN PVOID DllHandle,
    IN ULONG Reason,
    IN PCONTEXT Context OPTIONAL
    );

typedef VOID (NTAPI *PUSER_THREAD_START_ROUTINE)(
    IN PVOID ApcArgument1
    );

typedef VOID (*PPS_APC_ROUTINE) (
    IN PVOID ApcArgument1 OPTIONAL,
    IN PVOID ApcArgument2 OPTIONAL,
    IN PVOID ApcArgument3 OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateUserThread(
    IN  HANDLE Process,
    IN  PSECURITY_DESCRIPTOR ThreadSecurityDescriptor OPTIONAL,
    IN  BOOLEAN CreateSuspended,
    IN  ULONG_PTR ZeroBits OPTIONAL,
    IN  SIZE_T MaximumStackSize OPTIONAL,
    IN  SIZE_T CommittedStackSize OPTIONAL,
    IN  PUSER_THREAD_START_ROUTINE StartAddress,
    IN  PVOID Parameter OPTIONAL,
    OUT PHANDLE Thread OPTIONAL,
    OUT PCLIENT_ID ClientId OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenThread (
    OUT PHANDLE ThreadHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  PCLIENT_ID ClientId OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenThread (
    OUT PHANDLE ThreadHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  PCLIENT_ID ClientId OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationThread(
    IN  HANDLE ThreadHandle,
    IN  THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN  ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationThread(
    IN  HANDLE ThreadHandle,
    IN  THREADINFOCLASS ThreadInformationClass,
    OUT PVOID ThreadInformation,
    IN  ULONG ThreadInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationThread(
    IN HANDLE          ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    IN PVOID           ThreadInformation,
    IN ULONG           ThreadInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationThread(
    IN HANDLE          ThreadHandle,
    IN THREADINFOCLASS ThreadInformationClass,
    IN PVOID           ThreadInformation,
    IN ULONG           ThreadInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSuspendThread(
    IN  HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtResumeThread(
    IN  HANDLE ThreadHandle,
    OUT PULONG PreviousSuspendCount OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtTerminateThread(
    HANDLE Thread,
    NTSTATUS ExitStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwTerminateThread(
    HANDLE Thread,
    NTSTATUS ExitStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetThreadExecutionState(
    ULONG EsFlags,
    PULONG PreviousFlags
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueueApcThread(
    HANDLE ThreadHandle,
    PPS_APC_ROUTINE ApcRoutine,
    PVOID ApcArgument1,
    PVOID ApcArgument2,
    PVOID ApcArgument3
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueueApcThread(
    HANDLE ThreadHandle,
    PPS_APC_ROUTINE ApcRoutine,
    PVOID ApcArgument1,
    PVOID ApcArgument2,
    PVOID ApcArgument3
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAdjustPrivilege(
    ULONG    Privilege,
    BOOLEAN  Enable,
    BOOLEAN  CurrentThread,
    PBOOLEAN Enabled
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateProcessParameters(
    PRTL_USER_PROCESS_PARAMETERS *ProcessParameters,
    PUNICODE_STRING ImagePathName,
    PUNICODE_STRING DllPath,
    PUNICODE_STRING CurrentDirectory,
    PUNICODE_STRING CommandLine,
    PVOID Environment,
    PUNICODE_STRING WindowTitle,
    PUNICODE_STRING DesktopInfo,
    PUNICODE_STRING ShellInfo,
    PUNICODE_STRING RuntimeData
    );

NTSYSAPI
PRTL_USER_PROCESS_PARAMETERS
NTAPI
RtlNormalizeProcessParams(
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlDestroyProcessParameters(
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateUserProcess(
    PUNICODE_STRING NtImagePathName,
    ULONG Attributes,
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    PSECURITY_DESCRIPTOR ProcessSecurityDescriptor,
    PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
    HANDLE ParentProcess,
    BOOLEAN InheritHandles,
    HANDLE DebugPort,
    HANDLE ExceptionPort,
    PRTL_USER_PROCESS_INFORMATION ProcessInformation
    );

#define NtCurrentProcess() ((HANDLE) -1)
#define NtCurrentThread()  ((HANDLE) -2)
#define NtCurrentPeb()     ((PPEB)(NtCurrentTeb()->ProcessEnvironmentBlock))

NTSYSAPI
NTSTATUS
NTAPI
NtCreateProcess(
    OUT PHANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  HANDLE ParentProcess,
    IN  BOOLEAN InheritObjectTable,
    IN  HANDLE SectionHandle OPTIONAL,
    IN  HANDLE DebugPort OPTIONAL,
    IN  HANDLE ExceptionPort OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateProcess(
    OUT PHANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  HANDLE ParentProcess,
    IN  BOOLEAN InheritObjectTable,
    IN  HANDLE SectionHandle OPTIONAL,
    IN  HANDLE DebugPort OPTIONAL,
    IN  HANDLE ExceptionPort OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenProcess (
    OUT PHANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  PCLIENT_ID ClientId OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenProcess (
    OUT PHANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  PCLIENT_ID ClientId OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationProcess(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

typedef NTSTATUS (NTAPI * NTWOW64QUERYINFORMATIONPROCESS64)(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN ULONG ProcessInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationProcess (
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    IN PVOID ProcessInformation,
    IN ULONG ProcessInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationProcess(
    IN HANDLE ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    IN PVOID ProcessInformation,
    IN ULONG ProcessInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtTerminateProcess(
    HANDLE Process,
    NTSTATUS ExitStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwTerminateProcess(
    HANDLE Process,
    NTSTATUS ExitStatus
    );

//------------------------------------------------------------------------------
// Worker factory functions

#define WORKER_FACTORY_RELEASE_WORKER 0x0001
#define WORKER_FACTORY_WAIT 0x0002
#define WORKER_FACTORY_SET_INFORMATION 0x0004
#define WORKER_FACTORY_QUERY_INFORMATION 0x0008
#define WORKER_FACTORY_READY_WORKER 0x0010
#define WORKER_FACTORY_SHUTDOWN 0x0020

#define WORKER_FACTORY_ALL_ACCESS ( \
       STANDARD_RIGHTS_REQUIRED | \
       WORKER_FACTORY_RELEASE_WORKER | \
       WORKER_FACTORY_WAIT | \
       WORKER_FACTORY_SET_INFORMATION | \
       WORKER_FACTORY_QUERY_INFORMATION | \
       WORKER_FACTORY_READY_WORKER | \
       WORKER_FACTORY_SHUTDOWN \
       )

typedef enum _WORKERFACTORYINFOCLASS
{
    WorkerFactoryTimeout = 0,
    WorkerFactoryRetryTimeout = 1,
    WorkerFactoryIdleTimeout = 2,
    WorkerFactoryBindingCount = 3,
    WorkerFactoryThreadMinimum = 4,
    WorkerFactoryThreadMaximum = 5,
    WorkerFactoryPaused = 6,
    WorkerFactoryBasicInformation = 7,
    WorkerFactoryAdjustThreadGoal = 8,
    WorkerFactoryCallbackType = 9,
    WorkerFactoryStackInformation = 10,
    WorkerFactoryThreadBasePriority = 11,
    WorkerFactoryTimeoutWaiters = 12,
    WorkerFactoryFlags = 13,
    WorkerFactoryThreadSoftMaximum = 14,
    WorkerFactoryMaxInfoClass = 15 /* Not implemented */
} WORKERFACTORYINFOCLASS, *PWORKERFACTORYINFOCLASS;

typedef struct _WORKER_FACTORY_BASIC_INFORMATION
{
    LARGE_INTEGER Timeout;
    LARGE_INTEGER RetryTimeout;
    LARGE_INTEGER IdleTimeout;
    BOOLEAN Paused;
    BOOLEAN TimerSet;
    BOOLEAN QueuedToExWorker;
    BOOLEAN MayCreate;
    BOOLEAN CreateInProgress;
    BOOLEAN InsertedIntoQueue;
    BOOLEAN Shutdown;
    ULONG BindingCount;
    ULONG ThreadMinimum;
    ULONG ThreadMaximum;
    ULONG PendingWorkerCount;
    ULONG WaitingWorkerCount;
    ULONG TotalWorkerCount;
    ULONG ReleaseCount;
    LONGLONG InfiniteWaitGoal;
    PVOID StartRoutine;
    PVOID StartParameter;
    HANDLE ProcessId;
    SIZE_T StackReserve;
    SIZE_T StackCommit;
    NTSTATUS LastThreadCreationStatus;
} WORKER_FACTORY_BASIC_INFORMATION, * PWORKER_FACTORY_BASIC_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
NtCreateWorkerFactory(
    OUT PHANDLE WorkerFactoryHandleReturn,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  HANDLE CompletionPortHandle,
    IN  HANDLE WorkerProcessHandle,
    IN  PVOID StartRoutine,
    IN  PVOID StartParameter OPTIONAL,
    IN  ULONG MaxThreadCount OPTIONAL,
    IN  SIZE_T StackReserve OPTIONAL,
    IN  SIZE_T StackCommit OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationWorkerFactory(
    IN  HANDLE WorkerFactoryHandle,
    IN  WORKERFACTORYINFOCLASS WorkerFactoryInformationClass,
    IN  PVOID WorkerFactoryInformation,
    IN  ULONG WorkerFactoryInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationWorkerFactory(
    IN  HANDLE WorkerFactoryHandle,
    IN  WORKERFACTORYINFOCLASS WorkerFactoryInformationClass,
    IN  PVOID WorkerFactoryInformation,
    IN  ULONG WorkerFactoryInformationLength
    );

//------------------------------------------------------------------------------
// LPC Functions

#define MAX_LPC_DATA 0x130    // Maximum number of bytes that can be copied through LPC

// Valid values for PORT_MESSAGE::u2::s2::Type
#define LPC_REQUEST                  1
#define LPC_REPLY                    2
#define LPC_DATAGRAM                 3
#define LPC_LOST_REPLY               4
#define LPC_PORT_CLOSED              5
#define LPC_CLIENT_DIED              6
#define LPC_EXCEPTION                7
#define LPC_DEBUG_EVENT              8
#define LPC_ERROR_EVENT              9
#define LPC_CONNECTION_REQUEST      10

#define ALPC_REQUEST            0x2000 | LPC_REQUEST
#define ALPC_CONNECTION_REQUEST 0x2000 | LPC_CONNECTION_REQUEST

#define LPC_MESSAGE_BASE_SIZE	24

//
// Define header for Port Message
//

typedef struct _PORT_MESSAGE
{
    union
    {
        struct
        {
            USHORT DataLength;          // Length of data following the header (bytes)
            USHORT TotalLength;         // Length of data + sizeof(PORT_MESSAGE)
        } s1;
        ULONG Length;
    } u1;

    union
    {
        struct
        {
            USHORT Type;
            USHORT DataInfoOffset;
        } s2;
        ULONG ZeroInit;
    } u2;

    union
    {
        CLIENT_ID ClientId;
        double   DoNotUseThisField;     // Force quadword alignment
    };

    ULONG  MessageId;                   // Identifier of the particular message instance

    union
    {
        ULONG_PTR ClientViewSize;       // Size of section created by the sender (in bytes)
        ULONG  CallbackId;              //
    };

} PORT_MESSAGE, *PPORT_MESSAGE, LPC_MESSAGE, *PLPC_MESSAGE;

//
// Define structure for initializing shared memory on the caller's side of the port
//

typedef struct _PORT_VIEW {

    ULONG  Length;                      // Size of this structure
    HANDLE SectionHandle;               // Handle to section object with
                                        // SECTION_MAP_WRITE and SECTION_MAP_READ
    ULONG  SectionOffset;               // The offset in the section to map a view for
                                        // the port data area. The offset must be aligned
                                        // with the allocation granularity of the system.
    SIZE_T ViewSize;                    // The size of the view (in bytes)
    PVOID  ViewBase;                    // The base address of the view in the creator
                                        //
    PVOID  ViewRemoteBase;              // The base address of the view in the process
                                        // connected to the port.
} PORT_VIEW, *PPORT_VIEW;

//
// Define structure for shared memory coming from remote side of the port
//

typedef struct _REMOTE_PORT_VIEW {

    ULONG  Length;                      // Size of this structure
    SIZE_T ViewSize;                    // The size of the view (bytes)
    PVOID  ViewBase;                    // Base address of the view

} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;

//
// Macro for initializing the message header
//

#ifndef InitializeMessageHeader
#define InitializeMessageHeader(ph, l, t)                              \
{                                                                      \
    (ph)->u1.s1.TotalLength      = (USHORT)(l);                        \
    (ph)->u1.s1.DataLength       = (USHORT)(l - sizeof(PORT_MESSAGE)); \
    (ph)->u2.s2.Type             = (USHORT)(t);                        \
    (ph)->u2.s2.DataInfoOffset   = 0;                                  \
    (ph)->ClientId.UniqueProcess = NULL;                               \
    (ph)->ClientId.UniqueThread  = NULL;                               \
    (ph)->MessageId              = 0;                                  \
    (ph)->ClientViewSize         = 0;                                  \
}
#endif

/*++

    NtCreatePort
    ============

    Creates a LPC port object. The creator of the LPC port becomes a server
    of LPC communication

    PortHandle - Points to a variable that will receive the
        port object handle if the call is successful.

    ObjectAttributes - Points to a structure that specifies the object's
        attributes. OBJ_KERNEL_HANDLE, OBJ_OPENLINK, OBJ_OPENIF, OBJ_EXCLUSIVE,
        OBJ_PERMANENT, and OBJ_INHERIT are not valid attributes for a port object.

    MaxConnectionInfoLength - The maximum size, in bytes, of data that can
        be sent through the port.

    MaxMessageLength - The maximum size, in bytes, of a message
        that can be sent through the port.

    MaxPoolUsage - Specifies the maximum amount of NonPaged pool that can be used for
        message storage. Zero means default value.

    ZwCreatePort verifies that (MaxDataSize <= 0x104) and (MaxMessageSize <= 0x148).

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtCreatePort(
    OUT PHANDLE PortHandle,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  ULONG MaxConnectionInfoLength,
    IN  ULONG MaxMessageLength,
    IN  ULONG MaxPoolUsage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCreatePort(
    OUT PHANDLE PortHandle,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  ULONG MaxConnectionInfoLength,
    IN  ULONG MaxMessageLength,
    IN  ULONG MaxPoolUsage
    );


/*++

    NtConnectPort
    =============

    Creates a port connected to a named port (cliend side).

    PortHandle - A pointer to a variable that will receive the client
        communication port object handle value.

    PortName - Points to a structure that specifies the name
        of the port to connect to.

    SecurityQos - Points to a structure that specifies the level
        of impersonation available to the port listener.

    ClientView - Optionally points to a structure describing
        the shared memory region used to send large amounts of data
        to the listener; if the call is successful, this will be updated.

    ServerView - Optionally points to a caller-allocated buffer
        or variable that receives information on the shared memory region
        used by the listener to send large amounts of data to the
        caller.

    MaxMessageLength - Optionally points to a variable that receives the size,
        in bytes, of the largest message that can be sent through the port.

    ConnectionInformation - Optionally points to a caller-allocated
        buffer or variable that specifies connect data to send to the listener,
        and receives connect data sent by the listener.

    ConnectionInformationLength - Optionally points to a variable that
        specifies the size, in bytes, of the connect data to send
        to the listener, and receives the size of the connect data
        sent by the listener.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtConnectPort(
    OUT    PHANDLE PortHandle,
    IN     PUNICODE_STRING PortName,
    IN     PSECURITY_QUALITY_OF_SERVICE SecurityQos,
    IN OUT PPORT_VIEW ClientView OPTIONAL,
    OUT    PREMOTE_PORT_VIEW ServerView OPTIONAL,
    OUT    PULONG MaxMessageLength OPTIONAL,
    IN OUT PVOID ConnectionInformation OPTIONAL,
    IN OUT PULONG ConnectionInformationLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwConnectPort(
    OUT    PHANDLE PortHandle,
    IN     PUNICODE_STRING PortName,
    IN     PSECURITY_QUALITY_OF_SERVICE SecurityQos,
    IN OUT PPORT_VIEW ClientView OPTIONAL,
    OUT    PREMOTE_PORT_VIEW ServerView OPTIONAL,
    OUT    PULONG MaxMessageLength OPTIONAL,
    IN OUT PVOID ConnectionInformation OPTIONAL,
    IN OUT PULONG ConnectionInformationLength OPTIONAL
    );


/*++

    NtListenPort
    ============

    Listens on a port for a connection request message on the server side.

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    ConnectionRequest - Points to a caller-allocated buffer
        or variable that receives the connect message sent to
        the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtListenPort(
    IN  HANDLE PortHandle,
    OUT PPORT_MESSAGE RequestMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwListenPort(
    IN  HANDLE PortHandle,
    OUT PPORT_MESSAGE RequestMessage
    );

/*++

    NtAcceptConnectPort
    ===================

    Accepts or rejects a connection request on the server side.

    PortHandle - Points to a variable that will receive the port object
        handle if the call is successful.

    PortContext - A numeric identifier to be associated with the port.

    ConnectionRequest - Points to a caller-allocated buffer or variable
        that identifies the connection request and contains any connect
        data that should be returned to requestor of the connection

    AcceptConnection - Specifies whether the connection should
        be accepted or not

    ServerView - Optionally points to a structure describing
        the shared memory region used to send large amounts of data to the
        requestor; if the call is successful, this will be updated

    ClientView - Optionally points to a caller-allocated buffer
        or variable that receives information on the shared memory
        region used by the requestor to send large amounts of data to the
        caller

--*/


NTSYSAPI
NTSTATUS
NTAPI
NtAcceptConnectPort(
    OUT    PHANDLE PortHandle,
    IN     PVOID PortContext OPTIONAL,
    IN     PPORT_MESSAGE ConnectionRequest,
    IN     BOOLEAN AcceptConnection,
    IN OUT PPORT_VIEW ServerView OPTIONAL,
    OUT    PREMOTE_PORT_VIEW ClientView OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwAcceptConnectPort(
    OUT    PHANDLE PortHandle,
    IN     PVOID PortContext OPTIONAL,
    IN     PPORT_MESSAGE ConnectionRequest,
    IN     BOOLEAN AcceptConnection,
    IN OUT PPORT_VIEW ServerView OPTIONAL,
    OUT    PREMOTE_PORT_VIEW ClientView OPTIONAL
    );


/*++

    NtCompleteConnectPort
    =====================

    Completes the port connection process on the server side.

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtCompleteConnectPort(
    IN  HANDLE PortHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCompleteConnectPort(
    IN  HANDLE PortHandle
    );


/*++

    NtRequestPort
    =============

    Sends a request message to a port (client side)

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    RequestMessage - Points to a caller-allocated buffer or variable
        that specifies the request message to send to the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtRequestPort (
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE RequestMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwRequestPort (
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE RequestMessage
    );

/*++

    NtRequestWaitReplyPort
    ======================

    Sends a request message to a port and waits for a reply (client side)

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    RequestMessage - Points to a caller-allocated buffer or variable
        that specifies the request message to send to the port.

    ReplyMessage - Points to a caller-allocated buffer or variable
        that receives the reply message sent to the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtRequestWaitReplyPort(
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE RequestMessage,
    OUT PPORT_MESSAGE ReplyMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwRequestWaitReplyPort(
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE RequestMessage,
    OUT PPORT_MESSAGE ReplyMessage
    );


/*++

    NtReplyPort
    ===========

    Sends a reply message to a port (Server side)

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    ReplyMessage - Points to a caller-allocated buffer or variable
        that specifies the reply message to send to the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtReplyPort(
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE ReplyMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyPort(
    IN  HANDLE PortHandle,
    IN  PPORT_MESSAGE ReplyMessage
    );

/*++

    NtReplyWaitReplyPort
    ====================

    Sends a reply message to a port and waits for a reply message

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    ReplyMessage - Points to a caller-allocated buffer or variable
        that specifies the reply message to send to the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtReplyWaitReplyPort(
    IN     HANDLE PortHandle,
    IN OUT PPORT_MESSAGE ReplyMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyWaitReplyPort(
    IN     HANDLE PortHandle,
    IN OUT PPORT_MESSAGE ReplyMessage
    );

/*++

    NtReplyWaitReceivePort
    ======================

    Optionally sends a reply message to a port and waits for a
    message

    PortHandle - A handle to a port object. The handle doesn't need
        to grant any specific access.

    PortContext - Optionally points to a variable that receives
        a numeric identifier associated with the port.

    ReplyMessage - Optionally points to a caller-allocated buffer
        or variable that specifies the reply message to send to the port.

    ReceiveMessage - Points to a caller-allocated buffer or variable
        that receives the message sent to the port.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtReplyWaitReceivePort(
    IN  HANDLE PortHandle,
    OUT PVOID *PortContext OPTIONAL,
    IN  PPORT_MESSAGE ReplyMessage OPTIONAL,
    OUT PPORT_MESSAGE ReceiveMessage
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwReplyWaitReceivePort(
    IN  HANDLE PortHandle,
    OUT PVOID *PortContext OPTIONAL,
    IN  PPORT_MESSAGE ReplyMessage OPTIONAL,
    OUT PPORT_MESSAGE ReceiveMessage
    );

//-----------------------------------------------------------------------------
// Heap functions

#define HEAP_NO_SERIALIZE               0x00000001
#define HEAP_GROWABLE                   0x00000002
#define HEAP_GENERATE_EXCEPTIONS        0x00000004
#define HEAP_ZERO_MEMORY                0x00000008
#define HEAP_REALLOC_IN_PLACE_ONLY      0x00000010
#define HEAP_TAIL_CHECKING_ENABLED      0x00000020
#define HEAP_FREE_CHECKING_ENABLED      0x00000040
#define HEAP_DISABLE_COALESCE_ON_FREE   0x00000080
#define HEAP_CREATE_ALIGN_16            0x00010000
#define HEAP_CREATE_ENABLE_TRACING      0x00020000
#define HEAP_MAXIMUM_TAG                0x0FFF
#define HEAP_PSEUDO_TAG_FLAG            0x8000

//
// Data structure for heap definition. This includes various
// sizing parameters and callback routines, which, if left NULL,
// result in default behavior
//

typedef struct RTL_HEAP_PARAMETERS {
    ULONG Length;        //sizeof(RTL_HEAP_PARAMETERS)
    ULONG SegmentReserve;
    ULONG SegmentCommit;
    SIZE_T DeCommitFreeBlockThreshold;
    SIZE_T DeCommitTotalFreeThreshold;
    SIZE_T MaximumAllocationSize;
    SIZE_T VirtualMemoryThreshold;
    ULONG InitialCommit;
    ULONG InitialReserve;
    PVOID CommitRoutine;
    ULONG Reserved[2];
} RTL_HEAP_PARAMETERS, *PRTL_HEAP_PARAMETERS;

typedef struct _RTL_HEAP_INFORMATION
{
    PVOID BaseAddress;
    ULONG Flags;
    USHORT EntryOverhead;
    USHORT CreatorBackTraceIndex;
    unsigned int BytesAllocated;
    unsigned int BytesCommitted;
    unsigned int NumberOfTags;
    unsigned int NumberOfEntries;
    unsigned int NumberOfPseudoTags;
    unsigned int PseudoTagGranularity;
    unsigned int Reserved[5];
    struct _RTL_HEAP_TAG * Tags;
    struct _RTL_HEAP_ENTRY * Entries;
} RTL_HEAP_INFORMATION, *PRTL_HEAP_INFORMATION;

typedef struct _RTL_PROCESS_HEAPS
{
    ULONG NumberOfHeaps;
    RTL_HEAP_INFORMATION Heaps[1];
} RTL_PROCESS_HEAPS, *PRTL_PROCESS_HEAPS;

#define RtlProcessHeap() (HANDLE)(NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap)

NTSYSAPI
HANDLE
NTAPI
RtlCreateHeap (
    IN ULONG Flags,
    IN PVOID BaseAddress OPTIONAL,
    IN SIZE_T SizeToReserve,
    IN SIZE_T SizeToCommit,
    IN PVOID Lock OPTIONAL,
    IN PRTL_HEAP_PARAMETERS Definition OPTIONAL
    );

NTSYSAPI
ULONG
NTAPI
RtlCreateTagHeap(
    IN PVOID HeapHandle,
    IN ULONG Flags,
    IN PWSTR TagPrefix OPTIONAL,
    IN PWSTR TagNames
    );

NTSYSAPI
PVOID
NTAPI
RtlDestroyHeap (
    IN HANDLE HeapHandle
    );

NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags,
    IN SIZE_T Size
    );

NTSYSAPI
PVOID
NTAPI
RtlReAllocateHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags,
    IN LPVOID Address,
    IN SIZE_T Size
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags,
    IN PVOID Address
    );

NTSYSAPI
ULONG
NTAPI
RtlCompactHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlLockHeap (
    IN HANDLE HeapHandle
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlUnlockHeap (
    IN HANDLE HeapHandle
    );

NTSYSAPI
SIZE_T
NTAPI
RtlSizeHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags,
    IN PVOID Address
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlValidateHeap (
    IN HANDLE HeapHandle,
    IN ULONG Flags,
    IN PVOID Address OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlQueryHeapInformation (
    IN  PVOID HeapHandle,
    IN  HEAP_INFORMATION_CLASS HeapInformationClass,
    OUT PVOID HeapInformation OPTIONAL,
    IN  SIZE_T HeapInformationLength OPTIONAL,
    OUT PSIZE_T ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetHeapInformation (
    IN PVOID HeapHandle,
    IN HEAP_INFORMATION_CLASS HeapInformationClass,
    IN PVOID HeapInformation OPTIONAL,
    IN SIZE_T HeapInformationLength OPTIONAL
    );

//-----------------------------------------------------------------------------
// Virtual memory functions

typedef enum _MEMORY_INFORMATION_CLASS
{
    MemoryBasicInformation          = 0x0,
    MemoryWorkingSetInformation     = 0x1,
    MemoryMappedFilenameInformation = 0x2,
    MemoryRegionInformation         = 0x3,
    MemoryWorkingSetExInformation   = 0x4,
    MemorySharedCommitInformation   = 0x5,
    MemoryImageInformation          = 0x6,
    MemoryRegionInformationEx       = 0x7,
} MEMORY_INFORMATION_CLASS, *PMEMORY_INFORMATION_CLASS;

typedef struct _MEMORY_WORKING_SET_EX_BLOCK
{
    union
    {
        struct
        {
            ULONG Valid : 1;
            ULONG ShareCount : 3;
            ULONG Win32Protection : 11;
            ULONG Shared : 1;
            ULONG Node : 6;
            ULONG Locked : 1;
            ULONG LargePage : 1;
            ULONG Priority : 3;
            ULONG Reserved : 3;
            ULONG SharedOriginal : 1;
            ULONG Bad : 1;
        };
        ULONG Invalid;
    };

} MEMORY_WORKING_SET_EX_BLOCK, *PMEMORY_WORKING_SET_EX_BLOCK;

typedef struct _MEMORY_WORKING_SET_EX_INFORMATION
{
    void * VirtualAddress;
    union
    {
        MEMORY_WORKING_SET_EX_BLOCK VirtualAttributes;
        ULONG Long;
    };
} MEMORY_WORKING_SET_EX_INFORMATION, *PMEMORY_WORKING_SET_EX_INFORMATION;

typedef struct _MEMORY_REGION_INFORMATION
{
    void * AllocationBase;
    ULONG AllocationProtect;
    ULONG RegionType;
    ULONG RegionSize;
    ULONG CommitSize;
} MEMORY_REGION_INFORMATION, *PMEMORY_REGION_INFORMATION;

typedef struct _MEMORY_IMAGE_INFORMATION
{
    PVOID ImageBase;
    ULONG SizeOfImage;

    union
    {
        ULONG ImageFlags;
        struct
        {
            ULONG ImagePartialMap:1;
            ULONG ImageNotExecutable:1;
            ULONG Reserved:30;
        };
    };
} MEMORY_IMAGE_INFORMATION, *PMEMORY_IMAGE_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
NtAllocateVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN     ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG AllocationType,
    IN     ULONG Protect
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwAllocateVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN     ULONG_PTR ZeroBits,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG AllocationType,
    IN     ULONG Protect
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFreeVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG FreeType
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwFreeVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG FreeType
    );

NTSYSAPI
NTSTATUS
NTAPI
NtProtectVirtualMemory(
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG NewProtect,
    OUT    PULONG OldProtect
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    IN     ULONG NewProtect,
    OUT    PULONG OldProtect
    );

NTSYSAPI
NTSTATUS
NTAPI
NtReadVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    OUT PVOID Buffer,
    IN  SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesRead OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwReadVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    OUT PVOID Buffer,
    IN  SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesRead OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWriteVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    IN  PVOID Buffer,
    IN  SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesWritten OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwWriteVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    IN  PVOID Buffer,
    IN  SIZE_T BufferSize,
    OUT PSIZE_T NumberOfBytesWritten OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtFlushVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    OUT    PIO_STATUS_BLOCK IoStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwFlushVirtualMemory (
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN OUT PSIZE_T RegionSize,
    OUT    PIO_STATUS_BLOCK IoStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    IN  MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation,
    IN  SIZE_T MemoryInformationLength,
    OUT PSIZE_T ReturnLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
    IN  HANDLE ProcessHandle,
    IN  PVOID BaseAddress,
    IN  MEMORY_INFORMATION_CLASS MemoryInformationClass,
    OUT PVOID MemoryInformation,
    IN  SIZE_T MemoryInformationLength,
    OUT PSIZE_T ReturnLength OPTIONAL
    );

//-----------------------------------------------------------------------------
// Section functions

typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2

} SECTION_INHERIT;


typedef enum _SECTION_INFORMATION_CLASS
{
    SectionBasicInformation = 0x0,
    SectionImageInformation = 0x1,
    SectionRelocationInformation = 0x2,
    SectionOriginalBaseInformation = 0x3,
    MaxSectionInfoClass = 0x4,

} SECTION_INFORMATION_CLASS, *PSECTION_INFORMATION_CLASS;


/*++

    NtCreateSection
    ===============

    Creates a section object.

    SectionHandle - Points to a variable that will receive the section
        object handle if the call is successful.

    DesiredAccess - Specifies the type of access that the caller requires
        to the section object. This parameter can be zero, or any combination
        of the following flags:

        SECTION_QUERY       - Query access
        SECTION_MAP_WRITE   - Can be written when mapped
        SECTION_MAP_READ    - Can be read when mapped
        SECTION_MAP_EXECUTE - Can be executed when mapped
        SECTION_EXTEND_SIZE - Extend access
        SECTION_ALL_ACCESS  - All of the preceding +
                              STANDARD_RIGHTS_REQUIRED

    ObjectAttributes - Points to a structure that specifies the object's attributes.
        OBJ_OPENLINK is not a valid attribute for a section object.

    MaximumSize - Optionally points to a variable that specifies the size,
        in bytes, of the section. If FileHandle is zero, the size must be
        specified; otherwise, it can be defaulted from the size of the file
        referred to by FileHandle.

    SectionPageProtection - The protection desired for the pages
        of the section when the section is mapped. This parameter can take
        one of the following values:

        PAGE_READONLY
        PAGE_READWRITE
        PAGE_WRITECOPY
        PAGE_EXECUTE
        PAGE_EXECUTE_READ
        PAGE_EXECUTE_READWRITE
        PAGE_EXECUTE_WRITECOPY

    AllocationAttributes - The attributes for the section. This parameter must
        be a combination of the following values:

        SEC_BASED     0x00200000    // Map section at same address in each process
        SEC_NO_CHANGE 0x00400000    // Disable changes to protection of pages
        SEC_IMAGE     0x01000000    // Map section as an image
        SEC_VLM       0x02000000    // Map section in VLM region
        SEC_RESERVE   0x04000000    // Reserve without allocating pagefile storage
        SEC_COMMIT    0x08000000    // Commit pages; the default behavior
        SEC_NOCACHE   0x10000000    // Mark pages as non-cacheable

    FileHandle - Identifies the file from which to create the section object.
        The file must be opened with an access mode compatible with the protection
        flags specified by the Protect parameter. If FileHandle is zero,
        the function creates a section object of the specified size backed
        by the paging file rather than by a named file in the file system.

--*/

NTSYSAPI
NTSTATUS
NTAPI
NtCreateSection(
    OUT PHANDLE SectionHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  PLARGE_INTEGER MaximumSize OPTIONAL,
    IN  ULONG SectionPageProtection,
    IN  ULONG AllocationAttributes,
    IN  HANDLE FileHandle OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateSection(
    OUT PHANDLE SectionHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  PLARGE_INTEGER MaximumSize OPTIONAL,
    IN  ULONG SectionPageProtection,
    IN  ULONG AllocationAttributes,
    IN  HANDLE FileHandle OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSection (
    OUT PHANDLE SectionHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenSection (
    OUT PHANDLE SectionHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtMapViewOfSection (
    IN     HANDLE SectionHandle,
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN     ULONG_PTR ZeroBits,
    IN     SIZE_T CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PSIZE_T ViewSize,
    IN     SECTION_INHERIT InheritDisposition,
    IN     ULONG AllocationType,
    IN     ULONG Protect
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwMapViewOfSection (
    IN     HANDLE SectionHandle,
    IN     HANDLE ProcessHandle,
    IN OUT PVOID *BaseAddress,
    IN     ULONG_PTR ZeroBits,
    IN     ULONG CommitSize,
    IN OUT PLARGE_INTEGER SectionOffset OPTIONAL,
    IN OUT PULONG ViewSize,
    IN     SECTION_INHERIT InheritDisposition,
    IN     ULONG AllocationType,
    IN     ULONG Protect
    );

NTSYSAPI
NTSTATUS
NTAPI
NtUnmapViewOfSection (
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwUnmapViewOfSection (
    IN HANDLE ProcessHandle,
    IN PVOID BaseAddress
    );

NTSYSAPI
NTSTATUS
NTAPI
NtExtendSection (
    IN     HANDLE SectionHandle,
    IN OUT PLARGE_INTEGER SectionSize
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwExtendSection (
    IN     HANDLE SectionHandle,
    IN OUT PLARGE_INTEGER SectionSize
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySection (
    IN  HANDLE SectionHandle,
    IN  SECTION_INFORMATION_CLASS SectionInformationClass,
    OUT PVOID SectionInformation,
    IN  SIZE_T Length,
    OUT PSIZE_T ResultLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySection (
    IN  HANDLE SectionHandle,
    IN  SECTION_INFORMATION_CLASS SectionInformationClass,
    OUT PVOID SectionInformation,
    IN  SIZE_T Length,
    OUT PSIZE_T ResultLength OPTIONAL
    );


//-----------------------------------------------------------------------------
// Synchronization

//
// Wait type
//

typedef enum _WAIT_TYPE {
    WaitAll,
    WaitAny
    } WAIT_TYPE;

NTSYSAPI
NTSTATUS
NTAPI
NtWaitForSingleObject (
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForSingleObject (
    IN HANDLE Handle,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtWaitForMultipleObjects (
    IN ULONG Count,
    IN HANDLE Handle[],
    IN WAIT_TYPE WaitType,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwWaitForMultipleObjects (
    IN ULONG Count,
    IN HANDLE Handle[],
    IN WAIT_TYPE WaitType,
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER Timeout OPTIONAL
    );


//-----------------------------------------------------------------------------
// Event support

typedef enum _EVENT_INFORMATION_CLASS {
    EventBasicInformation    // = 0
} EVENT_INFORMATION_CLASS;

typedef struct _EVENT_BASIC_INFORMATION {
    EVENT_TYPE EventType;
    LONG EventState;
} EVENT_BASIC_INFORMATION, *PEVENT_BASIC_INFORMATION;

//
// Event handling routines
//

NTSYSAPI
NTSTATUS
NTAPI
NtCreateEvent (
    OUT PHANDLE EventHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  EVENT_TYPE EventType,
    IN  BOOLEAN InitialState
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwCreateEvent (
    OUT PHANDLE EventHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  EVENT_TYPE EventType,
    IN  BOOLEAN InitialState
    );

NTSYSAPI
NTSTATUS
NTAPI
NtClearEvent (
    IN HANDLE Handle
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwClearEvent (
    IN HANDLE Handle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtPulseEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwPulseEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtResetEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwResetEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwSetEvent (
    IN  HANDLE Handle,
    OUT PLONG PreviousState OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenEvent (
    OUT PHANDLE EventHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwOpenEvent (
    OUT PHANDLE EventHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryEvent (
    IN  HANDLE EventHandle,
    IN  EVENT_INFORMATION_CLASS EventInfoClass,
    OUT PVOID EventInfo,
    IN  ULONG Length,
    OUT PULONG ResultLength OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryEvent (
    IN  HANDLE EventHandle,
    IN  EVENT_INFORMATION_CLASS EventInfoClass,
    OUT PVOID EventInfo,
    IN  ULONG Length,
    OUT PULONG ResultLength OPTIONAL
    );

//-----------------------------------------------------------------------------
// Mutant support

NTSYSAPI
NTSTATUS
NTAPI
NtCreateMutant(
    OUT PHANDLE MutantHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  BOOLEAN InitialOwner
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenMutant(
    OUT PHANDLE MutantHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtReleaseMutant(
    IN HANDLE MutantHandle,
    IN PLONG PreviousCount OPTIONAL
    );

//-----------------------------------------------------------------------------
// Semaphore support

NTSYSAPI
NTSTATUS
NTAPI
NtCreateSemaphore(
    OUT PHANDLE SemaphoreHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
    IN  ULONG InitialCount,
    IN  ULONG MaximumCount
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSemaphore(
    OUT PHANDLE SemaphoreHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

//-----------------------------------------------------------------------------
// EventPair support

#define EVENT_PAIR_ALL_ACCESS ( STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE )

NTSYSAPI
NTSTATUS
NTAPI
NtCreateEventPair(
    OUT PHANDLE EventPairHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenEventPair(
    OUT PHANDLE EventPairHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );


//-----------------------------------------------------------------------------
// Security descriptor functions

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateSecurityDescriptor (
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN ULONG Revision
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAbsoluteToSelfRelativeSD(
    IN     PSECURITY_DESCRIPTOR AbsoluteSecurityDescriptor,
    IN OUT PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor,
    IN OUT PULONG BufferLength
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSelfRelativeToAbsoluteSD(
    IN OUT PSECURITY_DESCRIPTOR SelfRelativeSecurityDescriptor,
    OUT    PSECURITY_DESCRIPTOR AbsoluteSecurityDescriptor,
    IN OUT PULONG AbsoluteSecurityDescriptorSize,
    IN OUT PACL Dacl,
    IN OUT PULONG DaclSize,
    IN OUT PACL Sacl,
    IN OUT PULONG SaclSize,
    IN OUT PSID Owner,
    IN OUT PULONG OwnerSize,
    IN OUT PSID PrimaryGroup,
    IN OUT PULONG PrimaryGroupSize
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetOwnerSecurityDescriptor(
    IN  PSECURITY_DESCRIPTOR SecurityDescriptor,
    OUT PSID *Owner,
    OUT PBOOLEAN OwnerDefaulted
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetOwnerSecurityDescriptor(
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN PSID Owner OPTIONAL,
    IN BOOLEAN OwnerDefaulted OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetGroupSecurityDescriptor(
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    OUT PSID *Group,
    OUT PBOOLEAN GroupDefaulted
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetGroupSecurityDescriptor(
    IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN     PSID Group OPTIONAL,
    IN     BOOLEAN GroupDefaulted OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetDaclSecurityDescriptor(
    IN  PSECURITY_DESCRIPTOR  SecurityDescriptor,
    OUT PBOOLEAN  DaclPresent,
    OUT PACL  *Dacl,
    OUT PBOOLEAN  DaclDefaulted
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetDaclSecurityDescriptor(
    IN PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN BOOLEAN DaclPresent,
    IN PACL Dacl OPTIONAL,
    IN BOOLEAN DaclDefaulted OPTIONAL
    );


NTSYSAPI
NTSTATUS
NTAPI
RtlGetSaclSecurityDescriptor(
    IN  PSECURITY_DESCRIPTOR SecurityDescriptor,
    OUT PBOOLEAN SaclPresent,
    OUT PACL *Sacl,
    OUT PBOOLEAN SaclDefaulted
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlSetSaclSecurityDescriptor(
    IN OUT PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN     BOOLEAN SaclPresent,
    IN     PACL Sacl,
    IN     BOOLEAN SaclDefaulted
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAllocateAndInitializeSid(
    IN  PSID_IDENTIFIER_AUTHORITY IdentifierAuthority,
    IN  UCHAR SubAuthorityCount,
    IN  ULONG SubAuthority0,
    IN  ULONG SubAuthority1,
    IN  ULONG SubAuthority2,
    IN  ULONG SubAuthority3,
    IN  ULONG SubAuthority4,
    IN  ULONG SubAuthority5,
    IN  ULONG SubAuthority6,
    IN  ULONG SubAuthority7,
    OUT PSID *Sid
    );

NTSYSAPI
ULONG
NTAPI
RtlLengthSid(
    IN PSID Sid
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualSid(
    IN PSID Sid1,
    IN PSID Sid2
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlConvertSidToUnicodeString(
    OUT PUNICODE_STRING UnicodeString,
    IN  PSID Sid,
    IN  BOOLEAN AllocateDestinationString
    );

NTSYSAPI
PVOID
NTAPI
RtlFreeSid(
    IN PSID Sid
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlCreateAcl(
    IN PACL Acl,
    IN ULONG AclLength,
    IN ULONG AclRevision
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetAce(
    IN  PACL Acl,
    IN  ULONG AceIndex,
    OUT PVOID  *Ace
    );

NTSYSAPI
NTSTATUS
RtlAddAce(
    IN OUT PACL Acl,
    IN ULONG AceRevision,
    IN ULONG StartingAceIndex,
    IN PVOID AceList,
    IN ULONG AceListLength
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAddAccessAllowedAce(
    IN OUT PACL Acl,
    IN     ULONG AceRevision,
    IN     ACCESS_MASK AccessMask,
    IN     PSID Sid
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAddAccessAllowedAceEx(
    IN OUT PACL Acl,
    IN     ULONG AceRevision,
    IN     ULONG AceFlags,
    IN     ULONG AccessMask,
    IN     PSID Sid
    );

//-----------------------------------------------------------------------------
// Token functions

NTSYSAPI
NTSTATUS
NTAPI
NtAccessCheck (
    IN     PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN     HANDLE ClientToken,
    IN     ACCESS_MASK DesiredAccess,
    IN     PGENERIC_MAPPING GenericMapping,
    OUT    PPRIVILEGE_SET PrivilegeSet,
    IN OUT PULONG PrivilegeSetLength,
    OUT    PACCESS_MASK GrantedAccess,
    OUT    NTSTATUS * AccessStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwAccessCheck (
    IN     PSECURITY_DESCRIPTOR SecurityDescriptor,
    IN     HANDLE ClientToken,
    IN     ACCESS_MASK DesiredAccess,
    IN     PGENERIC_MAPPING GenericMapping,
    OUT    PPRIVILEGE_SET PrivilegeSet,
    IN OUT PULONG PrivilegeSetLength,
    OUT    PACCESS_MASK GrantedAccess,
    OUT    NTSTATUS * AccessStatus
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenProcessToken(
    IN  HANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    OUT PHANDLE TokenHandle
    );

// Windows XP or newer
NTSYSAPI
NTSTATUS
NTAPI
NtOpenProcessTokenEx(
    IN  HANDLE ProcessHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  ULONG HandleAttributes,
    OUT PHANDLE TokenHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenThreadToken(
    IN  HANDLE ThreadHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  BOOLEAN OpenAsSelf,
    OUT PHANDLE TokenHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryInformationToken(
    IN  HANDLE  TokenHandle,
    IN  TOKEN_INFORMATION_CLASS  TokenInformationClass,
    OUT PVOID  TokenInformation,
    IN  ULONG  TokenInformationLength,
    OUT PULONG  ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationToken(
    IN  HANDLE  TokenHandle,
    IN  TOKEN_INFORMATION_CLASS  TokenInformationClass,
    OUT PVOID  TokenInformation,
    IN  ULONG  TokenInformationLength,
    OUT PULONG  ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtSetInformationToken(
    IN HANDLE  TokenHandle,
    IN TOKEN_INFORMATION_CLASS  TokenInformationClass,
    IN PVOID  TokenInformation,
    IN ULONG  TokenInformationLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtAdjustPrivilegesToken(
    IN  HANDLE TokenHandle,
    IN  BOOLEAN DisableAllPrivileges,
    IN  PTOKEN_PRIVILEGES NewState OPTIONAL,
    IN  ULONG BufferLength OPTIONAL,
    IN  PTOKEN_PRIVILEGES PreviousState OPTIONAL,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDuplicateToken(
    IN  HANDLE ExistingTokenHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  BOOLEAN EffectiveOnly,
    IN  TOKEN_TYPE TokenType,
    OUT PHANDLE NewTokenHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
NtCompareTokens(
    IN  HANDLE FirstTokenHandle,
    IN  HANDLE SecondTokenHandle,
    OUT PBOOLEAN IdenticalTokens
    );

//-----------------------------------------------------------------------------
// Token functions available from Windows 8.1

// Token security attribute type
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_INVALID 0x00
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_INT64 0x01
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_UINT64 0x02
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_STRING 0x03
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_FQBN 0x04
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_SID 0x05
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_BOOLEAN 0x06
#define TOKEN_SECURITY_ATTRIBUTE_TYPE_OCTET_STRING 0x10

// Token security attribute flags
#define TOKEN_SECURITY_ATTRIBUTE_NON_INHERITABLE 0x0001
#define TOKEN_SECURITY_ATTRIBUTE_VALUE_CASE_SENSITIVE 0x0002
#define TOKEN_SECURITY_ATTRIBUTE_USE_FOR_DENY_ONLY 0x0004
#define TOKEN_SECURITY_ATTRIBUTE_DISABLED_BY_DEFAULT 0x0008
#define TOKEN_SECURITY_ATTRIBUTE_DISABLED 0x0010
#define TOKEN_SECURITY_ATTRIBUTE_MANDATORY 0x0020

typedef struct _TOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE
{
    ULONG64 Version;
    UNICODE_STRING Name;

} TOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE, *PTOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE;

typedef struct _TOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE
{
    PVOID pValue;
    ULONG ValueLength;

} TOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE, *PTOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE;

typedef struct _TOKEN_SECURITY_ATTRIBUTE_V1
{
    UNICODE_STRING Name;
    USHORT ValueType;
    USHORT Reserved;
    ULONG  Flags;
    ULONG  ValueCount;

    union
    {
        PLONG64 pInt64;
        PULONG64 pUint64;
        PUNICODE_STRING pString;
        PTOKEN_SECURITY_ATTRIBUTE_FQBN_VALUE pFqbn;
        PTOKEN_SECURITY_ATTRIBUTE_OCTET_STRING_VALUE pOctetString;
    } Values;

} TOKEN_SECURITY_ATTRIBUTE_V1, *PTOKEN_SECURITY_ATTRIBUTE_V1;

typedef struct _TOKEN_SECURITY_ATTRIBUTES_INFORMATION
{
    USHORT Version;
    USHORT Reserved;
    ULONG AttributeCount;

    union
    {
        PTOKEN_SECURITY_ATTRIBUTE_V1 pAttributeV1;
    } Attribute;

} TOKEN_SECURITY_ATTRIBUTES_INFORMATION, *PTOKEN_SECURITY_ATTRIBUTES_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySecurityAttributesToken(
    IN  HANDLE TokenHandle,
    IN  PUNICODE_STRING Attributes,
    IN  ULONG NumberOfAttributes,
    OUT PVOID Buffer,
    IN  ULONG Length,
    OUT PULONG ReturnLength
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySecurityAttributesToken(
    IN  HANDLE TokenHandle,
    IN  PUNICODE_STRING Attributes,
    IN  ULONG NumberOfAttributes,
    OUT PVOID Buffer,
    IN  ULONG Length,
    OUT PULONG ReturnLength
    );

//-----------------------------------------------------------------------------
// Symbolic links

//
// Object Manager Symbolic Link Specific Access Rights.
//

#ifndef SYMBOLIC_LINK_QUERY
#define SYMBOLIC_LINK_QUERY (0x0001)
#define SYMBOLIC_LINK_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0x1)
#endif

NTSYSAPI
NTSTATUS
NTAPI
NtCreateSymbolicLinkObject(
    OUT PHANDLE SymbolicLinkHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes,
    IN  PUNICODE_STRING DestinationName
    );

NTSYSAPI
NTSTATUS
NTAPI
NtOpenSymbolicLinkObject (
    OUT PHANDLE SymbolicLinkHandle,
    IN  ACCESS_MASK DesiredAccess,
    IN  POBJECT_ATTRIBUTES ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQuerySymbolicLinkObject (
    IN  HANDLE SymbolicLinkHandle,
    OUT PUNICODE_STRING NameString,
    OUT PULONG ResultLength OPTIONAL
    );

//-----------------------------------------------------------------------------
// Loader functions

#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK  (0x00000001)
#define RTL_IMAGE_NT_HEADER_EX_FLAG_UNKNOWN_W10     (0x00000002)     // Used to call RtlImageNtHeaderEx in W10, but never checked

#define LDRP_FIND_RESOURCE_DATA                     (0x00000000)
#define LDRP_FIND_RESOURCE_DIRECTORY                (0x00000002)
#define LDR_FIND_RESOURCE_LANGUAGE_EXACT            (0x00000004)
#define LDR_FIND_RESOURCE_LANGUAGE_REDIRECT_VERSION (0x00000008)

NTSYSAPI
PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(
    IN PVOID Base
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlImageNtHeaderEx(
    IN  ULONG Flags,
    IN  PVOID Base,
    IN  ULONG64 Size,
    OUT PIMAGE_NT_HEADERS * OutHeaders
    );

NTSYSAPI
PVOID
NTAPI
RtlImageDirectoryEntryToData(
    IN  PVOID Base,
    IN  BOOLEAN MappedAsImage,
    IN  USHORT DirectoryEntry,
    OUT PULONG Size
    );

NTSYSAPI
PVOID
NTAPI
RtlImageRvaToVa(
    IN     PIMAGE_NT_HEADERS NtHeaders,
    IN     PVOID Base,
    IN     ULONG Rva,
    IN OUT PIMAGE_SECTION_HEADER *LastRvaSection OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrLoadDll(
    IN  PWSTR DllPath OPTIONAL,
    IN  PULONG DllCharacteristics OPTIONAL,
    IN  PUNICODE_STRING DllName,
    OUT PVOID *DllHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrGetDllHandle(
    IN  PWSTR DllPath OPTIONAL,
    IN  PULONG DllCharacteristics OPTIONAL,
    IN  PUNICODE_STRING DllName,
    OUT PVOID * DllHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrUnloadDll(
    IN PVOID DllHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrGetProcedureAddress(
    IN  PVOID DllHandle,
    IN  PANSI_STRING ProcedureName OPTIONAL,
    IN  ULONG ProcedureNumber OPTIONAL,
    OUT PVOID *ProcedureAddress
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrFindResource_U(
    IN  PVOID DllHandle,
    IN  PULONG_PTR ResourceIdPath,
    IN  ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    );

// Windows XP+
NTSYSAPI
NTSTATUS
NTAPI
LdrFindResourceEx_U(
    IN  ULONG Flags,
    IN  PVOID DllHandle,
    IN  PULONG_PTR ResourceIdPath,
    IN  ULONG ResourceIdPathLength,
    OUT PIMAGE_RESOURCE_DATA_ENTRY *ResourceDataEntry
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrAccessResource(
    IN  PVOID DllHandle,
    IN  PIMAGE_RESOURCE_DATA_ENTRY ResourceDataEntry,
    OUT PVOID *Address OPTIONAL,
    OUT PULONG Size OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrDisableThreadCalloutsForDll(
    IN PVOID DllHandle
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileKeyOption(
    IN HANDLE KeyHandle,
    IN PCWSTR OptionName,
    IN ULONG Type,
    OUT PVOID Buffer,
    IN ULONG BufferSize,
    OUT PULONG ResultSize OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileExecutionOptions(
    IN PCUNICODE_STRING ImagePathName,
    IN PCWSTR OptionName,
    IN DWORD Type,
    OUT PVOID Buffer,
    IN ULONG BufferSize,
    OUT PULONG ResultSize OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlFormatMessage(
    IN  PWSTR Message,
    IN  ULONG MaxWidth OPTIONAL,
    IN  BOOLEAN IgnoreInserts,
    IN  BOOLEAN ArgumentsAreAnsi,
    IN  BOOLEAN ArgumentsAreAnArray,
    IN  va_list* Arguments,
    OUT PWSTR Buffer,
    IN  ULONG BufferSize,
    OUT PULONG ReturnLength OPTIONAL
    );

//-----------------------------------------------------------------------------
// Driver functions

NTSYSAPI
NTSTATUS
NTAPI
NtLoadDriver(
    PUNICODE_STRING DriverServiceName
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver(
    PUNICODE_STRING DriverServiceName
    );

NTSYSAPI
NTSTATUS
NTAPI
NtUnloadDriver(
    PUNICODE_STRING DriverServiceName
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver(
    PUNICODE_STRING DriverServiceName
    );

//-----------------------------------------------------------------------------
// Functions dealing with NTSTATUS and Win32 error

typedef enum _HARDERROR_RESPONSE_OPTION
{
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem

} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE
{
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes

} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosError(
    NTSTATUS Status
    );

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosErrorNoTeb(
    NTSTATUS Status
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetLastNtStatus(
    );

NTSYSAPI
ULONG
NTAPI
RtlGetLastWin32Error(
    );

NTSYSAPI
VOID
NTAPI
RtlSetLastWin32Error(
    ULONG WinError
    );

NTSYSAPI
VOID
NTAPI
RtlSetLastWin32ErrorAndNtStatusFromNtStatus(
    NTSTATUS Status
    );

NTSYSAPI
NTSTATUS
NTAPI
NtRaiseHardError(
    IN  NTSTATUS ErrorStatus,
    IN  ULONG NumberOfParameters,
    IN  PUNICODE_STRING UnicodeStringParameterMask OPTIONAL,
    IN  PVOID * Parameters,
    IN  HARDERROR_RESPONSE_OPTION ResponseOption,
    OUT PHARDERROR_RESPONSE Response
    );

//-----------------------------------------------------------------------------
// Other functions

NTSYSAPI
NTSTATUS
NTAPI
NtContinue(
    IN PCONTEXT Context,
    IN BOOLEAN Alertable
    );

NTSYSAPI
NTSTATUS
NTAPI
NtTestAlert(
    );

NTSYSAPI
NTSTATUS
NTAPI
NtQueryPerformanceCounter(
    OUT PLARGE_INTEGER PerformanceCounter,
    OUT PLARGE_INTEGER PerformanceFrequency OPTIONAL
    );

NTSYSAPI
NTSTATUS
NTAPI
NtAllocateLocallyUniqueId(
    OUT PLUID LocallyUniqueId
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDelayExecution(
    IN BOOLEAN Alertable,
    IN PLARGE_INTEGER DelayInterval
    );

NTSYSAPI
NTSTATUS
NTAPI
NtDisplayString(
    IN PUNICODE_STRING String
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetVersion(
    PRTL_OSVERSIONINFOW lpVersionInformation
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlGetCurrentDirectory_U(
    ULONG nBufferLength,
    PWSTR lpBuffer
    );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __NTDLL_H__
