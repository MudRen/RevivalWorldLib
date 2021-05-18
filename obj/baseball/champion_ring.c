/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : champion_ring.c
 * Author : Clode@RevivalWorld
 * Date	  : 2010-08-26
 * Note	  : 冠軍戒指
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
	
	set_idname(HIY"champion "NOR YEL"ring "HIC+year+NOR, NOR CYN"第 "+HIC+year+NOR CYN" 球季"HIY"世界"NOR YEL"冠軍戒指"NOR);
	set_temp("status", HIG"傳"NOR GRN"奇");
	
	set("year", year);
	set("long", "第 "+year+" 球季棒球比賽中獲得的世界冠軍戒指，裝備在球員身上將可以提升球員表現。");
	set("unit", "只");
	set("value", 100000000);
	set("mass", 100);
	set("special", 1);
	set("flag/no_amount", 1);

	set(BUFF_BATTER_POWER, 5);
	set(BUFF_PITCHER_POWER, 5);
	set(BUFF_FIELDER_POWER, 5);
	set(BUFF_CHA, 10);

	::setup_equipment(EQ_FINGER, HIG"手"NOR GRN"指"NOR);
}
