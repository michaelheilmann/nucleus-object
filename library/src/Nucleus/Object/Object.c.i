#include "Nucleus/Object/Object.h.i"

DEFINE_MODULE_PRIVATE(Nucleus_Objects)

Nucleus_NonNull() static Nucleus_Status
__Nucleus_Objects_initialize
    (
        Nucleus_Objects *module
    )
{
    if (Nucleus_Unlikely(!module)) return Nucleus_Status_InvalidArgument;
    Nucleus_Status status;
    //
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status)) return status;
    //
    module->referenceCount = 1;
    //
    return Nucleus_Status_Success;
}

Nucleus_NonNull() static Nucleus_Status
__Nucleus_Objects_uninitialize
    (
        Nucleus_Objects *module
    )
{
    Nucleus_Types_uninitialize();
    return Nucleus_Status_Success;
}
