# Nucleus: Object Tutorial
The following tutorial will help you to get started with Nucleus: Object.

# Starting-up and shutting-down the library.
Nucleus: Object is a library. 
The library needs to be explicitly initialized before it is used
and needs to explicitly uninitialized when it is no longer required.

```
#include "Nucleus/Object.h"

int main(int argc, char **argv)
{
    Nucleus_Status status;
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status))
    { return EXIT_FAILURE; }
    /* Do something with the library. */
    Nucleus_Types_uninitialize();
    return EXIT_SUCCESS;
}
```


The Nucleus_Types singleton is reference counted to facilate integration into libraries
agnostic of each other.

The initial reference count of the type system is `0`.
A successfull call to `Nucleus_Types_initialize` will increment the reference count by `1`.
In the step from `0` to `1`, the type system will is initialized.
Likewise:
A successfull call to `Nucleus_Types_uninitialize` will decrement the reference count by `1`.
In the stop from `1` to `0`, the type system will be uninitialized.

That is, as long as each library balances its calls to `Nucleus_Types_initialize` and
`Nucleus_Types_uninitialize` properly, it is not relevant which library and in what
order the libraries (un)initialize the singleton.

# Types and Objects
The Nucleus: Object type system has a root type called "Object".
Sub-types of the "Object" type are either "CLASSES" or "INTERFACES".

The following program demonstrates how to implement a CLASSED type of name "X" in the files "x.h"
and "x.c" and initialize the library in the file "main.c", create and destroy a value of "X",
and uninitialize the library.

"x.h"
```
#pragam once

#include "Nucleus/Object.h"

#define Visibility

Nucleus_ClassTypeDeclaration(Visibility,
                             "X",
                             X,
                             Nucleus_Object)
#define X(p) ((X *)(p))
#define X_CLASS(p) ((X_Class *)(p))

Nucleus_Status
X_construct
    (
        X *self
    );

Nucleus_Status
X_create
    (
        X **value
    );
```

"x.c"
```
#include "x.h"

Nucleus_ClassTypeDefinition(Visibility,
                            "X",
                            X,
                            Nucleus_Object)

Nucleus_Status
X_construct
    (
        X *self
    )
{
    Nucleus_Type *type;
    Nucleus_Status status;
    status = X_getType(&t);
    if (Nucleus_Unlikely(status)) return status;
    NUCLEUS_OBJECT(self)->type = type;
    return Nucleus_Status_Success;
}

Nucleus_Status
X_create
    (
        X **value
    )
{
    X *temporary;
    Nucleus_Status status;
    status = Nucleus_allocateObject((Nucleus_Object **)&temporary, sizeof(X));
    if (Nucleus_Unlikely(status)) return status;
    status = X_construct(temporary);
    if (Nucleus_Unlikely(status))
    { Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(temporary)); return status; }
    *value = temporary;
    return Nucleus_Status_Success;
}

```

`main.c`
```
#include "x.h"

Nucleus_Status run()
{
    Nucleus_Status status;
    X *p;
    status = X_create(&p);
    if (Nucleus_Unlikely(status)) return status;
    { return status; }
    Nucleus_Object_decrementReferenceCount(NUCLEUS_OBJECT(p));
    return status;
}

int main(int argc, char **argv)
{
    Nucleus_Status status;
    status = Nucleus_Types_initialize();
    if (Nucleus_Unlikely(status))
    { return EXIT_FAILURE; }
    status = run();
    if (Nucleus_Unlikely(status))
    {
        Nucleus_Types_uninitialize();
        return EXIT_FAILURE;
    }
    /* Do something with the library. */
    Nucleus_Types_uninitialize();
    return EXIT_SUCCESS;
}
```

Values of type "Object" are always passed around by pointers.

At runtime, one creates values of "CLASSES". Values of "CLASSES" are always passed around by pointer.

For example, in the following fragment a value of a class X which provides a default constructor
(i.e. a constructor without arguments) is created.




# Upcasting
Given an value of a class X and a pointer X *p to that value.

If Y is the parent class of X,
then it is safe to cast p into a pointer of type Y *.

If I is an interface implemented by X or any of its ancestor types,
then it is safe to cast p into a pointer of type I *.

