# `Nucleus_ObjectEnumerator_getObject`
*Gets the object of an enumerator.*

### C Signature
```
Nucleus_Status
Nucleus_ObjectEnumerator_getObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Object **object
    )
```

## Parameters
- `self` a pointer to this enumerator
- `object` a pointer to a `Nucleus_Object *` variable

## Description
This function gets the object of this enumerator.

If this function succeeds, then `*object* is assigned the object and `Nucleus_Status_Success` is
returned. Furthermore, the reference count of the object was incremented by `1`. On failure, a
non-zeros status code is returned and `object` is not dereferenced.
