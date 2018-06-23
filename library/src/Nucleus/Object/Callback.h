#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"

#define NUCLEUS_CALLBACK(p) ((Nucleus_Callback *)(p))
typedef Nucleus_Status (Nucleus_Callback)(void);
