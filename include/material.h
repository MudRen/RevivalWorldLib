/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : material.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define MATERIAL_CHINESE	([ \
METAL	:	"[1;37mª÷ÄÝ[0m",	\
STONE	:	"[0;1;30m­ì¥Û[0m",	\
WATER	:	"[1;36m²M¤ô[0m", 	\
WOOD	:	"[0;33m­ì¤ì[0m", 	\
FUEL	:	"[1;34m¿U®Æ[0m", 	\
])

/* °ò¥»­ì®Æ */
#define METAL		"metal"		//ª÷ÄÝ
#define WATER		"water"		//¤ô
#define WOOD		"wood"		//­ì¤ì
#define STONE		"stone"		//­ì¥Û
#define FUEL		"fuel"		//¤Æ¥Û¿U®Æ

#define MATERIAL(x)	("/obj/materials/"+x)


