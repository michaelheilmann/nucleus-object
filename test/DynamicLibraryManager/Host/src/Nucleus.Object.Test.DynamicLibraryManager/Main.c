// Copyright (c) 2018 Michael Heilmann
#include "Nucleus/Object/DynamicLibrary.h"
#include "Nucleus/Object/Types.h"
#include "Nucleus/Memory.h"
#include "Nucleus/getExecutableDirectoryPathname.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if (Nucleus_OperatingSystem == Nucleus_OperatingSystem_WINDOWS) || \
    (Nucleus_OperatingSystem == Nucleus_OperatingSystem_CYGWIN)
    static const char *dllExtension = u8"dll";
#elif (Nucleus_OperatingSystem == Nucleus_OperatingSystem_LINUX) || \
      (Nucleus_OperatingSystem == Nucleus_OperatingSystem_MACOS)
    static const char *dllExtension = u8"so";
#else
    #error("operating system not supported")
#endif

Nucleus_NonNull() Nucleus_Status
getDynamicLoadableLibrarySearchPath
    (
        char **searchPath
    );

Nucleus_Status
load
    (
        const char *dllFilename
    )
{
    Nucleus_Status status;
    Nucleus_DynamicLibrary *dynamicLibrary;
    
    // Get the DLL search path.
    char *dllSearchPathPathname;
    status = getDynamicLoadableLibrarySearchPath(&dllSearchPathPathname);
    if (status) { fprintf(stderr, "%s:%d: %s failed\n", __FILE__, __LINE__,
                          "getDynamicLoadableLibrarySearchPath");
                  return status; }

    // Concatenate DLL search path, DLL file name, '.', and DLL extension.
    char *dllPathname;
    status = Nucleus_allocateArrayMemory((void **)&dllPathname, strlen(dllSearchPathPathname) +
                                                                strlen(dllFilename) + 
                                                                strlen(".") + 
                                                                strlen(dllExtension) + 1, sizeof(char));
    if (status) { fprintf(stderr, u8"%s:%d: %s failed\n", __FILE__, __LINE__,
                          u8"Nucleus_allocateMemory");
                  Nucleus_deallocateMemory(dllSearchPathPathname);
                  return status; }
    strcpy(dllPathname, dllSearchPathPathname);
    strcat(dllPathname, dllFilename);
    strcat(dllPathname, ".");
    strcat(dllPathname, dllExtension);
    Nucleus_deallocateMemory(dllSearchPathPathname);
    
    // Load the DLL.
    status = Nucleus_Types_loadDynamicLibrary(dllPathname, &dynamicLibrary);
    if (Nucleus_Unlikely(status))
    {
        fprintf(stderr, u8"unable to load dynamically loadable library '%s'\n", dllPathname);
        Nucleus_deallocateMemory(dllPathname);
        return status;
    }
    Nucleus_deallocateMemory(dllPathname);
    Nucleus_DynamicLibrary_unlock(dynamicLibrary);
    return Nucleus_Status_Success;
}

Nucleus_NonNull() Nucleus_Status
getDynamicLoadableLibrarySearchPath
    (
        char **searchPath
    )
{
    char *temporary;
    Nucleus_Status status;
    status = Nucleus_getExecutableDirectoryPathname(&temporary);
    if (Nucleus_Unlikely(status)) return status;
    fprintf(stdout, u8"executable path: %s\n", temporary);
    *searchPath = temporary;
    return Nucleus_Status_Success;
}

int
main
    (
        int argc,
        char **argv
    )
{
    Nucleus_Status status;
    int exitCode = EXIT_SUCCESS;

    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status)) { return EXIT_FAILURE; }
    //
    status = load(u8"Nucleus.Object.Test.DynamicLibraryManager.ClientA");
    if (status) { exitCode = EXIT_FAILURE; goto End; }
    //
    status = load(u8"Nucleus.Object.Test.DynamicLibraryManager.ClientB");
    if (status) { exitCode = EXIT_FAILURE; goto End; }
End:
    Nucleus_Types_uninitialize();
    return exitCode;
}
