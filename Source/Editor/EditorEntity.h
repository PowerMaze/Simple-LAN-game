#pragma once

/*
///////////////////////////////////////////////////////////////

Basic entity representation for the level editor. The general idea 
is that the level editor loads entities from a file that contains 
entity descriptions from the game library (Game.dll).

The nuance is that you can describe any entity in the file and 
save it to the level. But the game will try to find this entity 
in the library, which may cause an error, so the description of 
the entity must match the definition in the library.

The first fields are for the generic CEntityMain class located 
in Engine\EntityMain.h. Next is a list of properties that can be 
edited through the editor. 

IMPORTANT!!! As mentioned above, the type and name must match 
for the fields from the classes that are presented for editing 
in Game.dll

///////////////////////////////////////////////////////////////
*/

//void ParseEntities(FILE* file);




