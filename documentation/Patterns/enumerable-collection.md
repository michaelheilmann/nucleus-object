# Enumerable Collection
An Enumerable [Collection](Collection.md) `[C]` with Values `[V]` provides means to
create en [Enumerator](Enumerator.md) to enumerate the elements of the collection.
For this purpose it provides a function `[C]_getEnumerator`.


## `[C]_getEnumerator`
*Get an enumerator for for the elements of the collection.*

## C Signature
```
Nucleus_Status
[C]_getEnumerator
    (
        [C] *collection,
        [E] *enumerator
    )
```
### Parameters
- `self` a pointer to an `[E]` object
- `hasValue` a pointer to a `Nucleus_Boolean` variable

### Descriptions
This function gets if an enumerator `[E]` object has a value available.

If this function succeeds, then `*hasValue* is assigned `Nucleus_Boolean_True` if the enumerator
has a value available and is assigned `Nucleus_Boolean_False` if the enumerator has no value
available. `Nucleus_Status_Success` is returned to indicate success.

This function fails if and only if `self` or `hasValue` are null pointers.
To indicate failure, a non-zero status code is returned and `value` is not dereferenced.

