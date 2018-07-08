// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibrary.h"

#include "Nucleus/Memory.h"
#include "Nucleus/C/strdup.h"
#include <stdlib.h>
#include <string.h>

#if defined(Nucleus_Platform_Linux) || defined(Nucleus_Platform_Osx)
  #include "Nucleus/Object/DynamicLibraryLinux.h"
#elif defined(Nucleus_Platform_Windows)
  #include "Nucleus/Object/DynamicLibraryWindows.h"
#endif

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_construct
    (
        Nucleus_DynamicLibrary *self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self || !pathname)) return Nucleus_Status_InvalidArgument;
     self->pathname = C_strdup(pathname);
    if (Nucleus_Unlikely(!self->pathname))
    {
        return Nucleus_Status_AllocationFailed;
    }
    self->referenceCount = 1;
    self->load = NULL;
    self->getSymbol = NULL;
    self->destruct = (Nucleus_Status (*)(Nucleus_DynamicLibrary *))&Nucleus_DynamicLibrary_destruct;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_destruct
    (
        Nucleus_DynamicLibrary *self
    )
{
    free(self->pathname);
    self->pathname = NULL;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getName
    (
        Nucleus_DynamicLibrary *self,
        const char **pathname
    )
{
    if (Nucleus_Unlikely(!self || !pathname)) return Nucleus_Status_InvalidArgument;
    *pathname = self->pathname;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_load
    (
        Nucleus_DynamicLibrary *self
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    return self->load(self);
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_getSymbol
    (
        Nucleus_DynamicLibrary *self,
        const char *symbolName,
        void **symbol
    )
{
    if (Nucleus_Unlikely(!self)) return Nucleus_Status_InvalidArgument;
    return self->getSymbol(self, symbolName, symbol);
}

Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_lock
    (
        Nucleus_DynamicLibrary *self
    )
{
    self->referenceCount++;
    return Nucleus_Status_Success;
}
    
Nucleus_Object_Library_Export Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_unlock
    (
        Nucleus_DynamicLibrary *self
    )
{
    if (0 == --self->referenceCount)
    {
        self->destruct(self);
        Nucleus_deallocateMemory(self);
    }
    return Nucleus_Status_Success;
}
