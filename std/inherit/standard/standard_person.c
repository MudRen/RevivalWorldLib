/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : standard_person.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-19
 * Note   : 標準人形 NPC 繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
#include <inherit.h>

inherit STANDARD_NPC;

int is_person()
{
	return 1;
}
