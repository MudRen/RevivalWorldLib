/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battlefield_glory_statue.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-16
 * Note   : �Ԫ��aģ�۸O
 * Update :
 *  o 2003-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <condition.h>
#include <feature.h>
#include <daemon.h>

inherit STANDARD_OBJECT;

void delay_destruct()
{
	destruct(this_object());
}

void init(object ob)
{
	if( !userp(ob) )
		return;

	if( !BATTLEFIELD_D->in_battle(ob) )
		return;

	if( query_temp("used") )
		return tell(ob, this_object()->query_idname()+"���j�j��O�w�g�Q�l���C\n");
	
	ob->start_condition(BATTLEFIELD_GLORY);
	BATTLEFIELD_D->add_bonus_score(ob, 40);
	tell(ob, pnoun(2, ob)+"�]���IĲ"+this_object()->query_idname()+"����o "HIY"40"NOR" �I���Z���Q�C\n");

	call_out((: delay_destruct :), 0);
	
	set_temp("used", 1);
}

void create()
{
	set_idname(HIW"battlefield "NOR WHT"glory statue"NOR, HIW"���ͪ�"NOR WHT"�Ԫ��aģ�۸O"NOR);
	
	if( this_object()->set_shadow_database() ) return;

	set("long", "�@�Ĥ@�Ӭݨ�o�ӥ۸O���H�i�H��o���j�j����O�C");
	set("unit", "��");
	set("capacity", 0);
	set("mass", -1);
	set("value", 100000000);
	set("badsell", 1);
}
