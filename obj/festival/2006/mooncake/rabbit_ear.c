/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : rabbit_ear.c
 * Author : Kyo@RevivalWorld
 * Date   : 2006-10-07
 * Note   : ����`§�� - �ߦ�
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

void create()
{
	set_idname("rabbit ear", HIC"���"NOR+CYN"�ߦ�"HIW"�v��"NOR);
	set_temp("status", HIB"20"NOR BLU"06 "HIB"��"NOR BLU"��");

	if( this_object()->set_shadow_database() ) return;

	set("long", "���ͪ��@�ɤ���`��§���C\n");
	
	set("unit", "��");
	set("mass", 0);
	set("value", 10);
	set("badsell", 1);
	set("flag/no_amount", 1);

	set(BUFF_INT, 1);

	::setup_equipment(EQ_ETC, HIG"�Y"NOR GRN"��"NOR);
}
