// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Status.h"
#include "Nucleus/Configuration.h"

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS)
    #define Nucleus_PluginExport() __declspec(dllexport)
#else
    #define Nucleus_PluginExport()
#endif

Nucleus_PluginExport() Nucleus_Status
f
    (
    )
{
    return Nucleus_Status_Success;
}
