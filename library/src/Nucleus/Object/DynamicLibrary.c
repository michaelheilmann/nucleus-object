// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibrary.h"

#include "Nucleus/Memory.h"
#include <stdlib.h>
#include <string.h>

#if defined(Nucleus_Platform_Linux) || defined(Nucleus_Platform_Osx)
  #include "Nucleus/Object/DynamicLibraryLinux.h"
#elif defined(Nucleus_Platform_Windows)
  #include "Nucleus/Object/DynamicLibraryWindows.h"
#endif

Nucleus_ClassTypeDefinition(Nucleus_Object_Library_Export,
                            "Nucleus_DynamicLibrary",
                            Nucleus_DynamicLibrary,
                            Nucleus_Object)

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
constructDispatch
    (
        Nucleus_DynamicLibrary_Class *dispatch
    )
{
    dispatch->load = NULL;
    dispatch->getSymbol = NULL;
    return Nucleus_Status_Success;
}

Nucleus_AlwaysSucceed() Nucleus_NonNull() static Nucleus_Status
destruct
    (
        Nucleus_DynamicLibrary *self
    )
{
    free(self->pathname);
    self->pathname = NULL;
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
Nucleus_DynamicLibrary_construct
    (
        Nucleus_DynamicLibrary *self,
        const char *pathname
    )
{
    if (Nucleus_Unlikely(!self || !pathname)) return Nucleus_Status_InvalidArgument;
    Nucleus_Type *type;
    Nucleus_Status status;
    status = Nucleus_DynamicLibrary_getType(&type);
    if (Nucleus_Unlikely(status)) return status;
    Nucleus_Object_construct(NUCLEUS_OBJECT(self));
    self->pathname = strdup(pathname);
    if (Nucleus_Unlikely(!self->pathname))
    {
        return Nucleus_Status_AllocationFailed;
    }
    NUCLEUS_OBJECT(self)->type = type;
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
    return NUCLEUS_DYNAMICLIBRARY_CLASS(NUCLEUS_OBJECT(self)->type->dispatch)->load(self);
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
    return NUCLEUS_DYNAMICLIBRARY_CLASS(NUCLEUS_OBJECT(self)->type->dispatch)->getSymbol(self, symbolName, symbol);
}
