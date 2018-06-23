#pragma once

#define DECLARE_MODULE(name) \
    Nucleus_Object_Library_Export Nucleus_Status \
    name##_initialize \
        ( \
        ); \
\
    Nucleus_Object_Library_Export Nucleus_Status \
    name##_uninitialize \
        ( \
        );

#define DEFINE_MODULE(name) \
    Nucleus_Status \
    name##_initialize \
        ( \
        ) \
    { \
        name##_Mutex_lock(&g_mutex); \
        if (NULL != g_singleton) \
        { \
            g_singleton->referenceCount++; \
            name##_Mutex_unlock(&g_mutex); \
            return Nucleus_Status_Success; \
        } \
        else \
        { \
            Nucleus_Status status = __##name##_create(&g_singleton); \
            name##_Mutex_unlock(&g_mutex); \
            return status; \
        } \
    } \
\
    Nucleus_Status \
    name##_uninitialize \
        ( \
        ) \
    { \
        name##_Mutex_lock(&g_mutex); \
        if (NULL == g_singleton) \
        { \
            name##_Mutex_unlock(&g_mutex); \
            return Nucleus_Status_NotExists; \
        } \
        if (0 == --g_singleton->referenceCount) \
        { \
            __##name##_destroy(g_singleton); \
            g_singleton = NULL; \
            name##_Mutex_unlock(&g_mutex); \
            return Nucleus_Status_Success; \
        } \
        return Nucleus_Status_Success; \
    }

#define DECLARE_MODULE_PRIVATE(name) \
    typedef struct name name; \
\
    static name##_Mutex g_mutex = name##_Mutex_Initializer; \
    static name *g_singleton = NULL; \
\
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_initialize \
        ( \
            name *module \
        ); \
\
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_uninitialize \
        ( \
            name *module \
        ); \
\
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_create \
        ( \
            name **module \
        ); \
\
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_destroy \
        ( \
            name *module \
        );

#define DEFINE_MODULE_PRIVATE(name) \
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_create \
        ( \
            name **module \
        ) \
    { \
        if (Nucleus_Unlikely(!module)) \
        { return Nucleus_Status_InvalidArgument; } \
        Nucleus_Status status; \
        name *temporary = NULL; \
        status = Nucleus_allocateMemory((void **)&temporary, sizeof(name)); \
        if (Nucleus_Unlikely(status)) \
        { return status; } \
        status = __##name##_initialize(temporary); \
        if (Nucleus_Unlikely(status)) \
        { \
            Nucleus_deallocateMemory(temporary); \
            return status; \
        } \
        *module = temporary; \
        return Nucleus_Status_Success; \
    } \
\
    Nucleus_NonNull() static Nucleus_Status \
    __##name##_destroy \
        ( \
            name *module \
        ) \
    { \
        if (Nucleus_Unlikely(!module)) \
        { return Nucleus_Status_InvalidArgument; } \
        __##name##_uninitialize(module); \
        Nucleus_deallocateMemory(module); \
        return Nucleus_Status_Success; \
    }
