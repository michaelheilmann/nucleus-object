# `Nucleus_ObjectEnumerator_nextObject`
*Increment the enumerator.*

### C Signature
``` 
Nucleus_Status
Nucleus_ObjectEnumerator_nextObject
    (
        Nucleus_ObjectEnumerator *self
    )
```

### Parameters
- `self` a pointer to this enumerator

### Description
This function fails if `enumerator` is a null pointer.
If the enumerator has no more elements to enumerate, it retains its position.
