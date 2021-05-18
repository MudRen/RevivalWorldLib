/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : champion_ring.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-08-26
 * Note	  : �a�x�٫�
 * Update :
 *  o 2000-00-00  

 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <buff.h>
#include <feature.h>
#include <equipment.h>

inherit	STANDARD_OBJECT;
inherit EQUIPMENT;

int is_baseball_equipment()
{
	return 1;	
}

void create(string option)
{
	int year;
	
	if( !stringp(option) ) return;

	sscanf(option, "%d", year);
	
	set_idname(HIY"champion "NOR YEL"ring "HIC+year+NOR, NOR CYN"�� "+HIC+year+NOR CYN" �y�u"HIY"�@��"NOR YEL"�a�x�٫�"NOR);
	set_temp("status", HIG"��"NOR GRN"�_");
	
	set("year", year);
	set("long", "�� "+year+" �y�u�βy���ɤ���o���@�ɫa�x�٫��A�˳Ʀb�y�����W�N�i�H���ɲy����{�C");
	set("unit", "�u");
	set("value", 100000000);
	set("mass", 100);
	set("special", 1);
	set("flag/no_amount", 1);

	set(BUFF_BATTER_POWER, 5);
	set(BUFF_PITCHER_POWER, 5);
	set(BUFF_FIELDER_POWER, 5);
	set(BUFF_CHA, 10);

	::setup_equipment(EQ_FINGER, HIG"��"NOR GRN"��"NOR);
}
