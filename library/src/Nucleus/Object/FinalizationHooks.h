#pragma once

#include "Nucleus/Annotations.h"
#include "Nucleus/Status.h"
#include "Nucleus/Object/Exports.h"
#include "Nucleus/Object/Module.h"
#include "Nucleus/Object/Callback.h"

DECLARE_MODULE(Nucleus_FinalizationHooks)

Nucleus_Status
Nucleus_FinalizationHooks_add
    (
        void *pointer,
		Nucleus_Callback *callback
    );

Nucleus_Status
Nucleus_FinalizationHooks_invoke
    (
        void *pointer
    );

Nucleus_Status
Nucleus_FinalizationHooks_removeAll
	(
		void *pointer
	);
