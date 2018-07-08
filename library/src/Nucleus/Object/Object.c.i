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
#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals
    status = Nucleus_Signals_initialize();
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Types_uninitialize();
        return status;
    }
#endif
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
#if defined(Nucleus_WithSignals) && 1 == Nucleus_WithSignals
    Nucleus_Signals_uninitialize();
#endif
    Nucleus_Types_uninitialize();
    return Nucleus_Status_Success;
}
