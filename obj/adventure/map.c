/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : map.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-08-26
 * Note	  : ���_��
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <daemon.h>

inherit	STANDARD_OBJECT;

void create(string option)
{
	int level;
	
	if( !stringp(option) ) return;

	sscanf(option, "%d", level);
	
	set_idname(HIW"lv"+level+NOR WHT" treasure map", HIW+CHINESE_D->chinese_number(level)+"��"NOR WHT"���_��"NOR);
	
	if( level >= 5 )
		set_temp("status", HIW"�v"NOR WHT"��");
	else if( level >= 3 )
		set_temp("status", HIG"��"NOR GRN"�_");
	else
		set_temp("status", HIC"�}"NOR CYN"��");

	set("level", level);

	//if( this_object()->set_shadow_ob() ) return;

	set("long", "�N�~����o�����_�ϡA�ϤW���G�yø�۵}���_�����a�I�C");
	set("unit", "�i");
	set("value", 100);
	set("mass", 100);
	set("special", 1);
}
