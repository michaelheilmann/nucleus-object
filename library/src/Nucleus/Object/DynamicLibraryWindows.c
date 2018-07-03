// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibraryWindows.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)

#include "Nucleus/Memory.h"
#include "Nucleus/transcodeString.h"
#include "Nucleus/IncludesWindows.h"

struct Nucleus_DynamicLibraryWindowsImpl
{
	HINSTANCE handle;
};

Nucleus_NonNull() static Nucleus_Status
load
    (
        Nucleus_DynamicLibraryWindows *self
    );

Nucleus_NonNull() static Nucleus_Status
getSymbol
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *symbolName,
        void **symbol
    );

Nucleus_NonNull() static Nucleus_Status
destruct
    (
    );
   
Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_construct
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    
	Nucleus_Status status;
    
	status = Nucleus_DynamicLibrary_construct(NUCLEUS_DYNAMICLIBRARY(self), pathname);
    if (Nucleus_Unlikely(status)) return status;

	status = Nucleus_allocateMemory(&self->pimpl, sizeof(Nucleus_DynamicLibraryWindowsImpl));
	if (Nucleus_Unlikely(status))
	{
		Nucleus_DynamicLibrary_destruct(NUCLEUS_DYNAMICLIBRARY(self));
		return status;
	}

	self->pimpl->handle = NULL;

    NUCLEUS_DYNAMICLIBRARY(self)->load = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&load;
    NUCLEUS_DYNAMICLIBRARY(self)->getSymbol = (Nucleus_Status (*)(Nucleus_DynamicLibrary *, const char *, void **))&getSymbol;
    NUCLEUS_DYNAMICLIBRARY(self)->destruct = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&Nucleus_DynamicLibraryWindows_destruct;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_destruct
    (
        Nucleus_DynamicLibraryWindows *self
    )
{
    if (NULL != self->pimpl->handle)
    {
        FreeLibrary(self->pimpl->handle);
        self->pimpl->handle = NULL;
    }
	Nucleus_deallocateMemory(self->pimpl);
	self->pimpl = NULL;
    return Nucleus_DynamicLibrary_destruct(NUCLEUS_DYNAMICLIBRARY(self));
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_create
    (
        Nucleus_DynamicLibraryWindows **self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_DynamicLibraryWindows *temporary;
    status = Nucleus_allocateMemory((void **)&temporary,
                                     sizeof(Nucleus_DynamicLibraryWindows));
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibraryWindows_construct(temporary, pathname);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);
        return status;
    }
    *self = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
load
    (
        Nucleus_DynamicLibraryWindows *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->pimpl->handle)
    {
        Nucleus_UCS16 *pathnameInternal;
        Nucleus_Status status;
        status = Nucleus_UTF8_to_UCS16(&pathnameInternal, NUCLEUS_DYNAMICLIBRARY(self)->pathname);
        if (Nucleus_Unlikely(status)) return status;
        self->pimpl->handle = LoadLibraryEx(pathnameInternal, NULL, 0);
        Nucleus_deallocateMemory(pathnameInternal);
        if (NULL == self->pimpl->handle)
        {
            return Nucleus_Status_NotFound;
        }
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
getSymbol
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *symbolName,
        void **symbol
    )
{
    if (Nucleus_Unlikely(!self || !symbolName || !symbol)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->pimpl->handle) return Nucleus_Status_NotOpen;
    Nucleus_Status status;
    char *symbolNameAscii;
    // Convert to ASCII.
    status = Nucleus_UTF8_to_ASCII(&symbolNameAscii, symbolName);
    if (Nucleus_Unlikely(status)) return status;
    void *temporary = GetProcAddress(self->pimpl->handle, symbolNameAscii);
    Nucleus_deallocateMemory(symbolNameAscii);
    if (NULL == temporary)
    {
        DWORD dwLastError = GetLastError();
        switch (dwLastError)
        {
        case ERROR_PROC_NOT_FOUND:
        default:
            return Nucleus_Status_NotFound;
        };
    }
    *symbol = temporary;
    return Nucleus_Status_Success;
}

#endif
