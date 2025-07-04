#ifndef BASICFLAGS_H
#define BASICFLAGS_H

/*
////////////////////////////////////////////////////
					File modes
////////////////////////////////////////////////////
*/

#define FILE_BINARY 0
#define FILE_TEXT   1

/*
////////////////////////////////////////////////////
					File indentificators
////////////////////////////////////////////////////
*/



#define MODEL_MAGIC "MODL"
#define MODEL_MAGIC_END "MEND"

/*
////////////////////////////////////////////////////
					Error Codes
////////////////////////////////////////////////////
*/

#define ERROR_LOAD_FILE 0x0001
#define ERROR_LOAD_FILE 0x0001

/*
////////////////////////////////////////////////////
				Polygon properties
////////////////////////////////////////////////////
*/

#define POLYGON_NONE			(0x0000)   // default polygon state, no selected
#define POLYGON_IS_VISIBLE		(1u << 0)  // polygon has collision, but not render
#define POLYGON_HAS_SHADOW		(1u << 1)  // polygon has shadow
#define POLYGON_IS_PASSABLE		(1u << 2)  // polygon doesn't have collision, but can be rendering
#define POLYGON_FULL_BRIGHT		(1u << 3)  // polygon dont have shadows or light
#define POLYGON_HAS_LIGHTMAP	(1u << 4)  // polygon can have lightmap (old-style lighting)
#define POLYGON_BULLET_PASSABLE (1u << 5)  // polygon passable only for raycasting (bullets, rails)
#define POLYGON_HAS_DETAIL		(1u << 6)  // polygon has detail texture
#define POLYGON_HAS_BLOOM		(1u << 7)  // polygon can have bloom effect
#define POLYGON_HAS_BUMP		(1u << 8)  // polygon can have bump texture
#define POLYGON_HAS_REFLECTION	(1u << 9)  // polygon can have reflection
#define POLYGON_INVISIBLE		(1u << 10) // polygon to not render

#define POLYGON_ALL	1023 // all properties


#define COLLISION_STATIC     (1u << 0)
#define COLLISION_NPC        (1u << 1)
#define COLLISION_ITEM       (1u << 2)
#define COLLISION_PROJECTILE (1u << 3)
#define COLLISION_ALL ( COLLISION_STATIC )| (COLLISION_NPC )|( COLLISION_ITEM )|( COLLISION_PROJECTILE)


/*
////////////////////////////////////////////////////
				Entity flags
////////////////////////////////////////////////////
*/




#define ENTITY_LIVE   (1 << 0)
#define ENTITY_EDITOR (1 << 1)
#define ENTITY_BRUSH  (1 << 2)
#define ENTITY_MODEL  (1 << 3)

#define PHYS_SPHERE    (1u << 12) // entity who initializes like sphere objects
#define PHYS_ENTITY    (1u << 13) // entity who presents items, powerups, bullets, projectiles
#define PHYS_CHARACTER (1u << 14) // entity who presents enemy, player
#define PHYS_CINEMATIC (1u << 15) // entity can apply real physics
#define PHYS_STATIC    (1u << 16) // entity can apply physics only for collision (player, enemies, etc.)

#define RENDER_LINES			(1<<0)
#define RENDER_DYNAMIC_SHADOWS  (1<<1)
#define RENDER_BASE				(1<<2)


#define TEXTURE_CONST   (1<<0) // texture doesn't generate mipmaps
#define TEXTURE_REPEAT  (1<<1) // texture sets repeat
#define TEXTURE_CLAMP   (1<<2) // texture sets clamp


#define TEX_TRANSPARENT   0
#define TEX_DEFAULT 1


#define MODEL_WEAPON (1<<0)

#endif