# `ReferenceCountedObjects`
A `ReferenceCountedObject` is an `Object` which lifetime is managed by
reference-counting. When a `ReferenceCountedObject` is created, it has
a reference count of `1`.

The reference count of a `ReferenceCountedObject` `o` can be increment (decrement) by `1` by
call `Nucleus_ReferenceCountedObject_increment(o)` (`Nucleus_ReferenceCountedObject_decrement(o)`) .

If the reference count of a ``ReferenceCountedObject` reaches 0, the object is destroyed.
