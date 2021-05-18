/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : lost_shard_5.c
 * Author : Clode@RevivalWorld
 * Date	  : 2007-08-16
 * Note	  : 遺失的遠古武器碎片
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>

inherit	STANDARD_OBJECT;

void destruct_self()
{
	msg("脆弱的$ME禁不起長時間的震動，裂開化為粉末。\n", this_object(), 0, 1);
	
	destruct(this_object());
}

void create()
{
	set_idname(NOR YEL"lost"HIY" shard"NOR, HIY"遠古武器碎片"NOR YEL"「伍」"NOR);

	if( clonep() )
		call_out( (: destruct_self :), 300 );

	if( this_object()->set_shadow_ob() ) return;

	set("disassemble", ([ 
		"/obj/gem/gem4" : 1,
		"/obj/gem/gem5" : 1,
	]));

	set("long", "遺失的遠古武器碎片─「伍」，此碎片非常脆弱，若放在身上太久可能會損壞。");
	set("unit", "塊");
	set("value", 1000);
	set("mass", 100);
	set("flag/no_amount", 1);
}
