# `Nucleus_addSignal`
*Add a signal to a type.*

## C Signature
```
Nucleus_Status
Nucleus_addSignal
    (
       Nucleus_TypeSystem *typeSystem,
       Nucleus_Signal **signal,
       const char *name,
       Nucleus_Type *type,
       Nucleus_Status (*notifyShutdown)()
    )
```

## Parameters
- `signal` signal a pointer to a (Nucleus_Signal *) variable
- `name` a pointer to a zero-terminated UTF-8 string, the name of the signal
- `type` a pointer to a (Nucleus_Type)) object, the type to add the signal to
- `notifyShutdown` a pointer to a function invoked if the type system shuts down

## Description
This function adds a signal to a type.

This function fails under three conditions.
Each condition returns a status code distinguishible from the status codes of the other conditions.
The rows in the following table denote the condition (in the left column) and the status code (in the right column).

| Condition                                                                                     | Status Code                       |
|-----------------------------------------------------------------------------------------------|-----------------------------------|
| an allocation failed                                                                          | `Nucleus_Status_AllocationFailed` |
| `typeSystem`, `signal`, `name`, or `type` is a null pointer                                   | `Nucleus_Status_InvalidArgument`  |
| a signal of the same name was already registered with the type or any of its ancestor types   | `Nucleus_Status_Exists`           |
