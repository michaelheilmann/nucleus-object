# `Nucleus_ClassTypeState`
*Enumeration denoting the states of a class type*
## C Signature
```
typedef enum Nucleus_ClassTypeState
{
	Nucleus_ClassTypeState_Uninitialized,
	Nucleus_ClassTypeState_DispatchInitialized,
	Nucleus_ClassTypeState_SignalsInitialized,	
} Nucleus_ClassTypeState;
```
## Members
- `Nucleus_ClassTypeState_Uninitialized`
   Enumeration element denoting that a class type is uninitialized.
- `Nucleus_ClassTypeState_DispatchInitialized`
   Enumeration element denoting that a class type had its dispatch initialized.
- `Nucleus_ClassTypeState_SignalsInitialized`
   Enumeration element denoting that a class type had its signals initialized.