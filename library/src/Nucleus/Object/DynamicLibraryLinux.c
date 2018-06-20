// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibraryLinux.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX)  || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)

#include "Nucleus/Memory.h"
#include <dlfcn.h>
#include <stdio.h>

Nucleus_NonNull() static Nucleus_Status
load
    (
        Nucleus_DynamicLibraryLinux *self
    );

Nucleus_NonNull() static Nucleus_Status
getSymbol
    (
        Nucleus_DynamicLibraryLinux *self,
        const char *symbolName,
        void **symbol
    );

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_construct
    (
        Nucleus_DynamicLibraryLinux *self,
        const char *pathname  
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    //Nucleus_Type *type;
    Nucleus_Status status;
    //status = Nucleus_DynamicLibraryLinux_getType(&type);
    //if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibrary_construct(NUCLEUS_DYNAMICLIBRARY(self), pathname);
    if (Nucleus_Unlikely(status)) return status;
    self->handle = NULL;
    NUCLEUS_DYNAMICLIBRARY(self)->load = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&load;
    NUCLEUS_DYNAMICLIBRARY(self)->getSymbol = (Nucleus_Status (*)(Nucleus_DynamicLibrary *, const char *, void **))&getSymbol;
    NUCLEUS_DYNAMICLIBRARY(self)->destruct = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&Nucleus_DynamicLibraryLinux_destruct;
    //NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_destruct
    (
        Nucleus_DynamicLibraryLinux *self
    )
{
    if (NULL != self->handle)
    {
        dlclose(self->handle);
        self->handle = NULL;
    }
    return Nucleus_DynamicLibrary_destruct(NUCLEUS_DYNAMICLIBRARY(self));
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibraryLinux_create
    (
        Nucleus_DynamicLibraryLinux **self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self || !pathname)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    Nucleus_DynamicLibraryLinux *temporary;
    status = Nucleus_allocateMemory((void **)&temporary,
                                    sizeof(Nucleus_DynamicLibraryLinux));
    if (Nucleus_Unlikely(status)) return status;
    status = Nucleus_DynamicLibraryLinux_construct(temporary, pathname);
    if (Nucleus_Unlikely(status))
    {
        Nucleus_deallocateMemory(temporary);// Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary));
        return status;
    }
    *self = temporary;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
load
    (
        Nucleus_DynamicLibraryLinux *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->handle)
    {
        self->handle = dlopen(NUCLEUS_DYNAMICLIBRARY(self)->pathname, RTLD_LAZY | RTLD_GLOBAL);
        if (NULL == self->handle)
        {
            fprintf(stderr, "unable to load dynamically loadable library %s: %s\n",
                            NUCLEUS_DYNAMICLIBRARY(self)->pathname, dlerror());
            return Nucleus_Status_NotFound;
        }
    }
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
getSymbol
    (
        Nucleus_DynamicLibraryLinux *self,
        const char *symbolName,
        void **symbol
    )
{
    if (Nucleus_Unlikely(!self || !symbolName || !symbol)) return Nucleus_Status_InvalidArgument;
    if (NULL == self->handle) return Nucleus_Status_NotOpen;
    void *temporary = dlsym(self->handle, symbolName);
    if (NULL == temporary)
    {
        return Nucleus_Status_NotFound;
    }
    *symbol = temporary;
    return Nucleus_Status_Success;
}

#endif
