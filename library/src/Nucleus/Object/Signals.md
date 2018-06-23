
# Connections
The signal singleton (Nucleus_Signal) maintains a map from object addresses to object connection
sets (ObjectConnectionSet). When connecting to some signal S of some object O, then we add first
check if the mapping from the address of O to it object connection set OCS exists. If not the
mapping O -> OCS is added.

Afterwards an object connection OC is added to the OCS.
An OC coonsists merely of the signal S and the CALLBACK CB.
