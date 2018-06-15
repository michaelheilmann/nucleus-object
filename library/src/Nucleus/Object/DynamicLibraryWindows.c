// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibraryWindows.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)

#include "Nucleus/Memory.h"
#include "Nucleus/transcodeString.h"

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            "Nucleus.DynamicLibraryWindows",
                            Nucleus_DynamicLibraryWindows,
                            Nucleus_DynamicLibrary)

Nucleus_NonNull() static Nucleus_Status
Nucleus_DynamicLibraryWindows_load
    (
        Nucleus_DynamicLibraryWindows *self
    );

Nucleus_NonNull() static Nucleus_Status
Nucleus_DynamicLibraryWindows_getSymbol
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *symbolName,
        void **symbol
    );

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_DynamicLibraryWindows_Class *dispatch
    )
{
    NUCLEUS_DYNAMICLIBRARY_CLASS(dispatch)->load = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&Nucleus_DynamicLibraryWindows_load;
    NUCLEUS_DYNAMICLIBRARY_CLASS(dispatch)->getSymbol = (Nucleus_Status (*)(Nucleus_DynamicLibrary *, const char *, void **))&Nucleus_DynamicLibraryWindows_getSymbol;
    return Nucleus_Status_Success;
}
   
Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryWindows_construct
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    Nucleus_Type *type;
    Nucleus_Status status;
    status = Nucleus_DynamicLibraryWindows_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibrary_construct(NUCLEUS_DYNAMICLIBRARY(self), pathname);
    if (Nucleus_Unlikely(status)) return status;
    self->handle = NULL;
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
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
    status = Nucleus_Object_allocate((Nucleus_Object **)&temporary,
                                     sizeof(Nucleus_DynamicLibraryWindows));
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibraryWindows_construct(temporary, pathname);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary));
        return status;
    }
    *self = temporary;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_DynamicLibraryWindows *self
    )
{
    if (NULL != self->handle)
    {
        FreeLibrary(self->handle);
        self->handle = NULL;
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Nucleus_DynamicLibraryWindows_load
    (
        Nucleus_DynamicLibraryWindows *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->handle)
    {
        Nucleus_UCS16 *pathnameInternal;
        Nucleus_Status status;
        status = Nucleus_UTF8_to_UCS16(&pathnameInternal, NUCLEUS_DYNAMICLIBRARY(self)->pathname);
        if (Nucleus_Unlikely(status)) return status;
        self->handle = LoadLibraryEx(pathnameInternal, NULL, 0);
        Nucleus_deallocateMemory(pathnameInternal);
        if (NULL == self->handle)
        {
            return Nucleus_Status_NotFound;
        }
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
Nucleus_DynamicLibraryWindows_getSymbol
    (
        Nucleus_DynamicLibraryWindows *self,
        const char *symbolName,
        void **symbol
    )
{
    if (Nucleus_Unlikely(!self || !symbolName || !symbol)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->handle) return Nucleus_Status_NotOpen;
    Nucleus_Status status;
    char *symbolNameAscii;
    // Convert to ASCII.
    status = Nucleus_UTF8_to_ASCII(&symbolNameAscii, symbolName);
    if (Nucleus_Unlikely(status)) return status;
    void *temporary = GetProcAddress(self->handle, symbolNameAscii);
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
