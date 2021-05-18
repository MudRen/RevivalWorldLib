/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mayor.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-14
 * Note   : ������T
 * Update :
 *  o 2003-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <citycondition.h>
#include <money.h>

inherit COMMAND;

string help = @HELP
	��ܫ�����T�C
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

int total_area_influence(string city)
{
	int i;
	string area, *sort_city;
	mapping area_section_influence;
	int total_area_nums;

	foreach(area in AREA_D->query_areas())
	{	
		for(i = 0;i<AREA_D->query_area_nums(area);++i)
		{
			area_section_influence = AREA_D->query_section_info(area, i, "area_section_influence");
			
			if( !sizeof(area_section_influence) )
				continue;	

			sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));

			if( !sizeof(sort_city) || sort_city[0] != city || area_section_influence[city]< 50000)
				continue;

			total_area_nums++;
		}
	}	
	
	return total_area_nums;
}

private void do_command(object me, string arg)
{
	int i, j, totalresident, totalflourish;
	string str;
	string *cities, city, mayor, *emblem;
	float rate;
	float levelup;
	string GNP, NNP, oldGNP;
	int age;
	mapping area_section_influence;
	mapping level_up_resource;
	int last_resource_needed;

	if(!arg)
	{
		cities = sort_array(CITY_D->query_cities() - ({ "wizard" }), (: CITY_D->query_city_info($1, "totalflourish") < CITY_D->query_city_info($2, "totalflourish") ? 1 : -1 :));
	
		str = HIW"����             �ìP���� ���� ����        �`�c�a�� ���� ���U ��� �ɯ� ����  �����v\n";
		str +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		foreach(city in cities)
		{
			age = CITY_D->query_city_info(city, "age");
			if( age > 0 )
				last_resource_needed = (25000000 + 20000000*(CITY_D->query_city_nums(city)-1))*to_int(pow(age-1, 2));
			else
				last_resource_needed = 0;

			mayor = CITY_D->query_city_info(city, "government/mayor");
			level_up_resource = CITY_D->query_city_info(city, "level_up_resource");
			
			levelup = 0.;

			foreach(string option in ({ "metal", "stone", "water", "wood", "fuel"}))
			{
				if( level_up_resource[option+"_curr"] > level_up_resource[option+"_need"] )
					levelup += 100.;
				else
					levelup += (level_up_resource[option+"_curr"]-last_resource_needed) * 100. / ((level_up_resource[option+"_need"]-last_resource_needed)||1);
			}
			
			levelup /= 5.;

			GNP = CITY_D->query_city_info(city, "GNP");
			NNP = CITY_D->query_city_info(city, "NNP");
			oldGNP = CITY_D->query_city_info(city, "old_GNP");
			
			if( count(oldGNP, "<=", 0) )
				rate = 0.;
			else
				rate = to_float(count(count(NNP, "*", 10000), "/", oldGNP))/100.;

			for(i=0;CITY_D->city_exist(city, i);i++);
			
			if( CITY_D->query_city_info(city, "fallen") ) rate = 0.;
			
			str += sprintf("%-24s"HIM"%d %3s  "HIW"%-12s"HIG"%-9s"HIC"%-5d"HIY"%-5s%-5s"HIW"%3d"NOR WHT"%%"HIG" %4d"NOR+(rate>0?HIC:HIR)+"%7.2f%%\n"NOR,
				CITY_D->query_city_idname(city),
				i,
				CITY_D->query_city_info(city, "fallen") ? HIR"��"NOR : HIG"��"NOR,
				mayor ? capitalize(mayor) : "",
				NUMBER_D->number_symbol(CITY_D->query_city_info(city, "totalflourish")),
				sizeof(CITY_D->query_city_info(city, "citizens")),
				CITY_D->query_city_info(city, "fallen") ? "":CITY_D->query_city_info(city, "register_open")?HIG"�}��"NOR:HIR"�ʳ�"NOR,
				CITY_D->query_age_name(city, 2),
				to_int(levelup),
				total_area_influence(city),
				rate,
			);
		}
		str +=NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		str +=NOR WHT"mayor '����ID' �d�߲Ӹ`���e\n"NOR;
	}
	else
	{
		int shopping_power;
		string shopping_data;
		string city_idname, moneyunit, area, city_scale_mayor;
		string *city_description = allocate(0);
		int count;
		mapping officer;
		int shift;
		string *sort_city;
		int city_occupy_time;
		int flourish_bonus;
		int shopping_bonus;

		city = arg;

		if( !CITY_D->city_exist(city) )
			return tell(me, "�S�� "+city+" �o�ӫ����C\n");
		
		//if( CITY_D->query_city_info(city, "fallen") )
		//	return tell(me, "�L�k�d��"+CITY_D->query_city_idname(city)+"����ơC\n");

		emblem = CITY_D->query_city_info(city, "emblem");
		mayor = CITY_D->query_city_info(city, "government/mayor");
		officer = CITY_D->query_city_info(city, "government/officer");
		city_occupy_time = CITY_D->query_city_info(city, "time");
		age = CITY_D->query_city_info(city, "age");

		if( age > 0 )
			last_resource_needed = (25000000 + 20000000*(CITY_D->query_city_nums(city)-1))*to_int(pow(age-1, 2));
		else
			last_resource_needed = 0;

		totalresident = 0;
		totalflourish = CITY_D->query_city_info(city, "totalflourish");

		for(i=0;CITY_D->city_exist(city, i);i++)
			totalresident += CITY_D->query_section_info(city, i, "resident");
		
		city_idname = CITY_D->query_city_idname(city);
		str = sprintf("%-24s"NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR"\n", city_idname);

		str += sprintf("��������    %-30s                      %10s\n"NOR, HIW+(mayor ? capitalize(mayor) : "�L")+NOR, emblem[0]);
		str += sprintf("����ɥN    %-30s                      %10s\n"NOR, CITY_D->query_age_name(city)+" "HIY+repeat_string("*", CITY_D->query_city_info(city, "level"))+NOR, emblem[1]);
		str += sprintf("�ث��ɶ�    %-30s                      %10s\n"NOR, HIW+(city_occupy_time==0?"���i��":TIME_D->replace_ctime(city_occupy_time))+NOR, emblem[2]);
		
		moneyunit = MONEY_D->city_to_money_unit(city);
		GNP = CITY_D->query_city_info(city, "GNP");
		NNP = CITY_D->query_city_info(city, "NNP");
		oldGNP = CITY_D->query_city_info(city, "old_GNP");
		
		if( count(oldGNP, "<=", 0) )
			rate = 0.;
		else
			rate = to_float(count(count(NNP, "*", 10000), "/", oldGNP))/100.;

		if( CITY_D->query_city_info(city, "fallen") ) rate = 0.;
		str += sprintf("\n%-38s%-38s\n", "�`������    "WHT+sizeof(CITY_D->query_city_info(city, "citizens"))+NOR, "�`�c�a��    "WHT+NUMBER_D->number_symbol(totalflourish)+NOR);
		str += sprintf("%-38s%-38s\n", "�`�~����    "WHT+NUMBER_D->number_symbol(totalresident)+NOR, "�g�٦����v  "+(rate>0?HIC:HIR)+sprintf("%-.2f%%", rate)+NOR);
		str += sprintf("%-38s%-38s\n", "������    "WHT+(moneyunit ? "$"+moneyunit:"���w")+NOR, "�F���겣    "WHT+money(moneyunit, CITY_D->query_city_info(city, "assets"))+NOR);
		str += sprintf("%-38s%-38s\n", "��ڶײv    "WHT+EXCHANGE_D->query_exchange_data(moneyunit||"0")+NOR, "�a���|      "WHT+CITY_D->query_city_info(city, "estatetax")+NOR);
		str += sprintf("%-38s%-38s\n", "�����`�겣  "WHT+money(MONEY_D->query_default_money_unit(), GNP)+NOR, "������y    "WHT+money(moneyunit, CITY_D->query_city_info(city, "area_influence_reward"))+NOR);
		str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"BYEL HIW"�x�����\n"NOR;
		if( !sizeof(officer) )
			str += "     �L\n";
		else
		{
			count = 0;
			foreach( string officerid, string title in officer )
				str += sprintf("%-12s%-26s"+(count++%2?"\n":""), HIW+capitalize(officerid)+NOR, title);
			if( count%2 == 1 )
				str += "\n";
		}
		str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"BMAG HIW"������T\n"NOR;
		count = 0;
		for(j=0;j<i;j++)
		{
			switch(CITY_D->query_section_info(city, j, "level"))
			{
				case 0..2: city_scale_mayor = "����"; break;
				case 3..5: city_scale_mayor = "�m���"; break;
				case 6..8: city_scale_mayor = "�ƥ���"; break;
				case 9..11: city_scale_mayor = "�ƭ���"; break;
				case 12..999: city_scale_mayor = "�Ƴ���"; break;
			}
				
			if( undefinedp(SHOPPING_D->query_shopping_info(city, j, "shopping_power")) )
				shopping_power = -1;
			else
				shopping_power = SHOPPING_D->query_shopping_info(city, j, "shopping_power");

			switch(shopping_power)
			{
				case -1:	shopping_data = HIR"��Ʃ|����s"NOR; break;
				case 0..20:	shopping_data = HIR + shopping_power + NOR RED" %"NOR;	break;
				case 21..50:	shopping_data = HIY + shopping_power + NOR YEL" %"NOR;	break;
				case 51..100:	shopping_data = HIG + shopping_power + NOR GRN" %"NOR;	break;
				case 101..MAX_INT:	shopping_data = HIC + shopping_power + NOR CYN" %"NOR;	break;
			}
			
			if( count%2 == 0 )
				city_description += allocate(7, "");
			
			shift = (count/2)*7;
			
			flourish_bonus = 0;

			if( CITY_D->query_section_info(city, j, FLOURISH_BUFF) > time() )
				flourish_bonus += 20;
			if( CITY_D->query_city_info(city, BATTLEFIELD_FLOURISH_BUFF) > time() )
				flourish_bonus += 20;
			if( CITY_D->has_building("prodigy", city, j) )
				flourish_bonus += 10;

			shopping_bonus = 0;
			
			if( BOSS_D->poseidon_exists() )
				shopping_bonus -= 50;
			
			if( CITY_D->query_section_info(city, j, SHOPPING_BUFF) > time() )
				shopping_bonus += 10;
				
			if( CITY_D->query_city_info(city, BATTLEFIELD_SHOPPING_BUFF) > time() )
				shopping_bonus += 30;
				
			city_description[shift] += sprintf("%-38s", (CITY_D->query_city_idname(city, j)||"�� "+(j+1)+" �ìP����"));
			city_description[shift+1] += sprintf("%-12s%-26s", city_scale_mayor, HIW+(capitalize(CITY_D->query_section_info(city, j, "vicemayor"))||"")+NOR);
			city_description[shift+2] += sprintf("%-12s%-26s", "�c�a��", 	WHT+NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "flourish"))+NOR+(flourish_bonus > 0 ? "("HIG"+"+flourish_bonus+NOR GRN"%"NOR")":""));
			city_description[shift+3] += sprintf("%-12s%-26s", "�~����", 	WHT+NUMBER_D->number_symbol(CITY_D->query_section_info(city, j, "resident"))+NOR);
			city_description[shift+4] += sprintf("%-12s%-26s", "�W��", 		CITY_D->query_city_scale(city, j));
			city_description[shift+5] += sprintf("%-12s%-26s", "���O�`�B/��", 	WHT+money(moneyunit, 24 * SHOPPING_D->query_shopping_info(city, j, "totalmoney"))+NOR +(shopping_bonus == 0 ? "" : (shopping_bonus > 0 ? "("NOR GRN"+"HIG+shopping_bonus+"%"NOR")" : "("NOR RED"-"HIR+(-shopping_bonus)+"%"NOR")")));
			city_description[shift+6] += sprintf("%-12s%-26s"+(count%2?"\n":""), "���O��O", shopping_data);
			
			count++;
		}
		str += implode(city_description, "\n");
		if( count%2 == 1 )
			str += "\n";
	
		str += "\n�@�ɫe����O�l�B���p "HIY+money(DEFAULT_MONEY_UNIT, 24 * SHOPPING_D->query_world_money())+NOR"\n";
		str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"BCYN HIW"�����ɯ�\n"NOR;
		level_up_resource = CITY_D->query_city_info(city, "level_up_resource");
		str += sprintf(HIW"%-12s%-7.2f%%                  "HIW"%-12d"WHT"/ %d\n"NOR, HIW"���ݸ귽"NOR, ((level_up_resource["metal_curr"]>level_up_resource["metal_need"]?level_up_resource["metal_need"]:level_up_resource["metal_curr"])-last_resource_needed)*100./((level_up_resource["metal_need"]-last_resource_needed)||1), level_up_resource["metal_curr"], level_up_resource["metal_need"]);
		str += sprintf(HIW"%-12s%-7.2f%%                  "HIW"%-12d"WHT"/ %d\n"NOR, WHT"��۸귽"NOR, ((level_up_resource["stone_curr"]>level_up_resource["stone_need"]?level_up_resource["stone_need"]:level_up_resource["stone_curr"])-last_resource_needed)*100./((level_up_resource["stone_need"]-last_resource_needed)||1), level_up_resource["stone_curr"], level_up_resource["stone_need"]);
		str += sprintf(HIW"%-12s%-7.2f%%                  "HIW"%-12d"WHT"/ %d\n"NOR, HIC"�M���귽"NOR, ((level_up_resource["water_curr"]>level_up_resource["water_need"]?level_up_resource["water_need"]:level_up_resource["water_curr"])-last_resource_needed)*100./((level_up_resource["water_need"]-last_resource_needed)||1), level_up_resource["water_curr"], level_up_resource["water_need"]);
		str += sprintf(HIW"%-12s%-7.2f%%                  "HIW"%-12d"WHT"/ %d\n"NOR, NOR YEL"���귽"NOR, ((level_up_resource["wood_curr"]>level_up_resource["wood_need"]?level_up_resource["wood_need"]:level_up_resource["wood_curr"])-last_resource_needed)*100./((level_up_resource["wood_need"]-last_resource_needed)||1), level_up_resource["wood_curr"], level_up_resource["wood_need"]);
		str += sprintf(HIW"%-12s%-7.2f%%                  "HIW"%-12d"WHT"/ %d\n"NOR, HIM"�U�Ƹ귽"NOR, ((level_up_resource["fuel_curr"]>level_up_resource["fuel_need"]?level_up_resource["fuel_need"]:level_up_resource["fuel_curr"])-last_resource_needed)*100./((level_up_resource["fuel_need"]-last_resource_needed)||1), level_up_resource["fuel_curr"], level_up_resource["fuel_need"]);
		str += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"BGRN HIW"���Ϧ���\n"NOR;
		
		foreach(area in AREA_D->query_areas())
		{
			for(i = 0;i<AREA_D->query_area_nums(area);++i)
			{
				area_section_influence = AREA_D->query_section_info(area, i, "area_section_influence");
				
				if( !sizeof(area_section_influence) )
					continue;
	
				sort_city = sort_array(keys(area_section_influence), (: $(area_section_influence)[$1] < $(area_section_influence)[$2] ? 1 : -1 :));
					
				if( !sizeof(sort_city) || sort_city[0] != city || area_section_influence[city] < 50000 )
					continue;

				str += sprintf("%-38s%-38s\n", AREA_D->query_area_idname(area, i), GRN"�����      "HIG+NUMBER_D->number_symbol(area_section_influence[city])+NOR);
			}
		}
		str += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	}
	me->more(str+"\n\n");
}
