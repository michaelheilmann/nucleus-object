# Enumerator
An enumerator `E` provides means to enumerate a finite or infinite sequence of values of type `V`.
It provides three functions `[E]_hasValue`, `[E]_getValue`, and `[E]_next`.


## `[E]_hasValue`
*Get if the enumerator has a value.*

## C Signature
```
Nucleus_Status
[E]_hasValue
    (
        [E] *enumerator,
        Nucleus_Boolean *hasValue
    )
```
### Parameters
- `enumerator` a pointer to an `[E]` object
- `hasValue` a pointer to a `Nucleus_Boolean` variable

### Descriptions
This function gets if an enumerator `[E]` object has a value available.

If this function succeeds, then `*hasValue* is assigned `Nucleus_Boolean_True` if the enumerator
has a value available and is assigned `Nucleus_Boolean_False` if the enumerator has no value
available. `Nucleus_Status_Success` is returned to indicate success.

This function fails if and only if `enumerator` or `hasValue` are null pointers.
To indicate failure, a non-zero status code is returned and `value` is not dereferenced.


## `[E]_hasValue`
*Gets the element.*

### C Signature
```
Nucleus_Status
[E]_getValue
    (
        [E] *enumerator,
        [V] *value
    )
```

## Description
This function gets the value of an enumerator `[E]` object.

If this function succeeds, then `*value* is assigned the value and `Nucleus_Status_Success` is returned.
Otherwise a non-zero status code is returned and 

`Nucleus_Boolean_True` if the enumerator
has a value and is assigned `Nucleus_Boolean_False` if the enumerator if the enumerator does not
have a value. The `Nucleus_Status_Success` is returned to indicate success.

This function fails if and only if `enumerator` or `value` are null pointers.


## `[E]_next`
*Increment the enumerator.*

### C Signature
``` 
Nucleus_Status
[E]_next
    (
        [E] *enumerator
    )
```

### Parameters
- `enumerator` a pointer to an `[E]` object

### Description
This function fails if `enumerator` is a null pointer.
If the enumerator has no more elements to enumerate, it retains its position.
