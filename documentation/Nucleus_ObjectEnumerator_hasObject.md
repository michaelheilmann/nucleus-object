# `Nucleus_ObjectEnumerator_hasObject`
*Get if the enumerator has an object.*

## C Signature
```
Nucleus_Status
Nucleus_ObjectEnumerator_hasObject
    (
        Nucleus_ObjectEnumerator *self,
        Nucleus_Boolean *hasObject
    )
```
### Parameters
- `enumerator` a pointer to this enumerator
- `hasObject` a pointer to a `Nucleus_Boolean` variable

### Descriptions
This function gets if this enumerator has an object available.

If this function succeeds, then `*hasObject* is assigned `Nucleus_Boolean_True` if this enumerator
has a value available and is assigned `Nucleus_Boolean_False` if this enumerator has no value
available. `Nucleus_Status_Success` is returned to indicate success.

This function fails if and only if `self` or `hasObject` are null pointers. To indicate failure,
a non-zero status code is returned and `hasObject` is not dereferenced.
