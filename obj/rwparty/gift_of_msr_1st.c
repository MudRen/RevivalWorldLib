/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : gift_of_msr_1st.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-05-14
 * Note   : Msr ���s�ޫ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <equipment.h>
#include <buff.h>

inherit STANDARD_OBJECT;
inherit EQUIPMENT;
inherit __DIR__"_action_dignose.c";

mapping actions;

void do_dignose(object me, string arg);


void create()
{
	set_idname("gift of msr", HIM"Msr ��"NOR MAG"�s�ޫ���"NOR);
	set_temp("status", HIC"���E"NOR CYN"������");

	if( this_object()->set_shadow_database() ) return;

	set("long", "�Ĥ@�����ͪ��@�ɺ��E(2006/05/13)�����~�C\n");
	
	set("unit", "��");
	set("mass", 0);
	set("value", 10);
	set("badsell", 1);
	set("flag/no_amount", 1);
	set("flag/no_drop", 1);
	set("flag/no_give", 1);
	set("flag/no_import", 1);

        set(BUFF_STR, 2);
        set(BUFF_LOADING_MAX, 5);
        set("buff/status", NOR BLINK WHT"ʨ"NOR);
        ::setup_equipment(EQ_NOSE, HIG"��"NOR GRN"��"NOR);
        	
        actions = ([ "dignose" : (: do_dignose :) ]);
}
