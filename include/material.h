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
METAL	:	"[1;37m����[0m",	\
STONE	:	"[0;1;30m���[0m",	\
WATER	:	"[1;36m�M��[0m", 	\
WOOD	:	"[0;33m���[0m", 	\
FUEL	:	"[1;34m�U��[0m", 	\
])

/* �򥻭�� */
#define METAL		"metal"		//����
#define WATER		"water"		//��
#define WOOD		"wood"		//���
#define STONE		"stone"		//���
#define FUEL		"fuel"		//�ƥۿU��

#define MATERIAL(x)	("/obj/materials/"+x)


