/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : info.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-01-31
 * Note   : ��T�d�߫��O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <location.h>
#include <material.h>

inherit COMMAND;

string help = @HELP
info		- �d�߷�a��T
HELP;

string influence_color(int i)
{
	switch(i)
	{
		case 0..20: return WHT; break;
		case 21..40: return HIY; break;
		case 41..60: return HIG; break;
		case 61..80: return HIC; break;
		case 81..100: return HIW; break;
	}
}

private void do_command(object me, string arg)
{
	object env = environment(me);
	mapping data;
	string msg;
	array loc = query_temp("location", me) || env->query_location();

	if( !arrayp(loc) ) 
		return tell(me, "�L�k�d�ߦ��a��ơC\n");

	if( !arg || arg == "here" )
	{
		msg = pnoun(2, me)+"�{�b�Ҧb��m��������T�p�U�G\n";
	
		if( CITY_D->is_city_location(loc) )
		{
			data = CITY_D->query_coor_data(loc);
		
			if( !data["owner"] )
				msg += "�֦���  �G�L\n";
			else if( belong_to_government(data["owner"]) )
				msg += "�֦���  �G���F�� "+CITY_D->query_city_idname(loc[CITY])+"\n";
			else if( belong_to_enterprise(data["owner"]) )
				msg += "�֦���  �G���~���� "+ENTERPRISE_D->query_enterprise_color_id(data["owner"][11..])+"\n";
			else
				msg += "�֦���  �G"+(find_player(data["owner"]) ? find_player(data["owner"])->query_idname() +HIC" �u�W"NOR : capitalize(data["owner"])+HIR" ���u"NOR)+"\n";
	
			switch( data["region"] )
			{
				case AGRICULTURE_REGION:
					msg += "�g�a�W���G"HIG"�A�~��\n"NOR;
					break;
				case INDUSTRY_REGION:
					msg += "�g�a�W���G"HIY"�u�~��\n"NOR;
					break;
				case COMMERCE_REGION:
					msg += "�g�a�W���G"HIC"�ӷ~��\n"NOR;
					break;
				default:
					msg += "�g�a�W���G�L\n"NOR;
					break;
			}
				
			msg += "�g�a���ȡG$"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(data[VALUE])+"\n";
			msg += "�a�����ȡG$"+MONEY_D->city_to_money_unit(loc[CITY])+" "+NUMBER_D->number_symbol(ESTATE_D->query_estate_value(loc))+"\n";
	

		}
		else if( AREA_D->is_area_location(loc) )
		{
			mapping area_section_influence;
			float rate;
			int total_count;
			string* sort_city;
			string city;
			
			if( !env->is_maproom() || !arrayp(loc) || !AREA_D->area_exist(loc[AREA], loc[NUM]) )
				return tell(me, "���a�L�k�d�߳o�ظ�ơC\n");
		
			area_section_influence = AREA_D->query_section_info(loc[AREA], loc[NUM], "area_section_influence");

			msg = AREA_D->query_area_idname(loc[AREA], loc[NUM])+"���귽�����v��Ʀp�U�G\n";
			
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			
			total_count = implode(values(area_section_influence), (: $1+$2 :));
			msg += HIW BRED"�����W��                    �귽�����v    �����ƶq                        \n"NOR;
			
			sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));
				
			foreach(city in sort_city)
			{
				if( !CITY_D->city_exist(city) ) continue;
				
				rate = area_section_influence[city] * 100. / total_count;
				
				msg += sprintf("%-30s%s%7.2f%%    "HIW"%d\n"NOR, CITY_D->query_city_idname(city), influence_color(to_int(rate)), rate, area_section_influence[city]);
			}
		
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			
			return me->more(msg);
			
		}
	}
	else
		return tell(me, "�ثe�u��d�ߤg�a��ơC\n");

	return tell(me, msg);
}
	
