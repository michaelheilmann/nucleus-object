#pragma once

#include "Nucleus/Object/Object.h"

#include "Nucleus/Object/Types.h"
#include "Nucleus/Memory.h"
#include "Nucleus/Hash/Pointer.h"
#include "Nucleus/Object/Type.h"
#include "Nucleus/Object/Module.h"

#define Nucleus_Objects_Mutex int
#define Nucleus_Objects_Mutex_Initializer (0)
#define Nucleus_Objects_Mutex_lock(mutex) /* - */
#define Nucleus_Objects_Mutex_unlock(mutex) /* - */
DECLARE_MODULE_PRIVATE(Nucleus_Objects)

struct Nucleus_Objects
{
    int referenceCount;
}; // struct Nucleus_Objects
