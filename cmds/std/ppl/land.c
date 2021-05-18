/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : land.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-08
 * Note   : land ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <map.h>
#include <location.h>

inherit COMMAND;

#define LEVEL_UP_RESOURCE_COST		1

string help = @HELP
1. �g�a��اl�ǡA�i�Ω��ಾ�g�a����
2. ��������ɯ�

land			- �d�ߥثe�w�g�l�Ǫ��g�a���
land absorb		- �l�Ǥg�a���(�ݭn�S���~)
land release '�Ʀr'	- �N�g�a��ئC���� N �ӧl�����g�a��������ثe�Ҧb���g�a
land '�ƶq' '�귽�W��'	- �N�۵M�귽�Q�]�b�����g�a�W�A���ɫ����������
land info		- �d�߫����ɯŰ^�m��

HELP;

varargs void query_resource_amount(mapping ref result, object ob, float percent, int need)
{
	object temp_ob;
	mapping product_info = ob->query_product_info();
	
	if( !mapp(product_info) ) return;

	if( !percent )
		percent = 1.1;
	
	if( !need )
		need = 1;
	


	foreach(string material, int count in product_info["material"])
	{
		if( objectp(temp_ob = load_object("/std/module/product/"+material+"_module") ) )
			query_resource_amount(ref result, temp_ob, percent*1.1, count * need);

		else switch(material)
		{
			case "/obj/materials/metal":	result["metal"] += to_int(need * count * percent);	break;
			case "/obj/materials/stone":	result["stone"] += to_int(need * count * percent);	break;
			case "/obj/materials/water":	result["water"] += to_int(need * count * percent);	break;
			case "/obj/materials/wood":	result["wood"] += to_int(need * count * percent);	break;	
			case "/obj/materials/fuel":	result["fuel"] += to_int(need * count * percent);	break;
		}
	}
}

private void do_command(object me, string arg)
{
	int i;
	string city;
	int num;

	if( !arg )
	{
		string list = "";
		array landdata = query("landdata", me);
		
		if( arrayp(landdata) && sizeof(landdata) )
		{
			list += HIY"�g�a"NOR YEL"���"NOR"\n";
			foreach(array elem in landdata)
			{
				i++;
				switch(elem[0])
				{
					case FARM: 
						list += sprintf("%-3d%-20s "NOR GRN"�g�a���� "HIG"%s"NOR GRN"/%d\n"NOR, i, HIG"�A��"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case PASTURE: 
						list += sprintf("%-3d%-20s "NOR YEL"�g�a���� "HIY"%s"NOR YEL"/%d\n"NOR, i, NOR YEL"����"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
					case FISHFARM: 
						list += sprintf("%-3d%-20s "NOR BLU"�g�a���� "HIB"%s"NOR BLU"/%d\n"NOR, i, HIB"�i�޳�"NOR, repeat_string("*", elem[1]), elem[2]);
						break;
				}
			}
			return tell(me, list+"\n");
		}
		else
			return tell(me, "�ثe�S������g�a�l�Ǻ�ءC\n");
	}
	else if( (wizardp(me) && sscanf(arg, "info %s", city) == 1) || arg == "info" )
	{
		string str;
		string id;
		mapping land_record;
		mapping total_record = allocate_mapping(0);
		mapping data;

		if( !city )
			city = query("city", me);
		
		if( !city )
			return tell(me, pnoun(2, me)+"���ݩ����@�y�����C\n");

		if( !CITY_D->city_exist(city) )
			return tell(me, "�S�� "+city+" �o�y�����C\n");
		
		str = CITY_D->query_city_idname(city)+"�귽�^�m�ƶq�p�U�G\n";
		str += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"NOR;
		
		for(num=0;CITY_D->city_exist(city, num);num++)
		{
			land_record = copy(CITY_D->query_section_info(city, num, "material_land_record"));
			
			if( sizeof(land_record) )
			foreach(id, data in land_record)
			{
				if( undefinedp(total_record[id]) )
					total_record[id] = allocate_mapping(0);

				total_record[id]["metal"] += data["metal"];
				total_record[id]["stone"] += data["stone"];
				total_record[id]["water"] += data["water"];
				total_record[id]["wood"] += data["wood"];
				total_record[id]["fuel"] += data["fuel"];
			}
		}
		
		foreach(id, mapping data2 in total_record)
		{
			str += sprintf("  %-15s%-10s%-10s%-10s%-10s%-10s\n",
				capitalize(id),
				HIW+data2["metal"]+NOR,
				WHT+data2["stone"]+NOR,
				HIC+data2["water"]+NOR,
				YEL+data2["wood"]+NOR,
				HIM+data2["fuel"]+NOR);
		}
		
		str += WHT"\n�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		str += HIW"���� metal"NOR"�B"WHT"��� stone"NOR"�B"HIC"�M�� water"NOR"�B"YEL"��� wood"NOR"�B"HIM"�U�� fuel"NOR"\n\n";
		return me->more(str);
	}
	else if( arg == "absorb" )
	{
		int type;
		int growth_level;
		int maintain_time;
		
		object media;
		object env = environment(me);
		array loc = query_temp("location", me);
		array landdata = query("landdata", me) || allocate(0);
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "�L�k�l�ǳo���g�a����ءC\n");
		
		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o���g�a�ä��O"+pnoun(2, me)+"���C\n");
			
		foreach(object ob in all_inventory(me))
		{
			if( query("land_absorb", ob) )	
			{
				media = ob;
				break;
			}
		}
		
		if( !objectp(media) )
			return tell(me, pnoun(2, me)+"���W�ʥF�l�Ǥg�a��ت��C�����~�C\n");
		
		type = CITY_D->query_coor_data(loc, TYPE);
		growth_level = CITY_D->query_coor_data(loc, "growth_level");
		maintain_time = CITY_D->query_coor_data(loc, "maintain_time");
	
		if( type != FARM && type != FISHFARM && type != PASTURE )
			return tell(me, "�L�k�l�ǳo�ؤg�a����ءC\n");
	
		if( maintain_time < 1 )
			return tell(me, "�o���g�a�S�������إi�H�l���C\n");

		msg("$ME�N$YOU��m�b�g�a�������A�u��$YOU�o�X�벴��"HIW"�ե�"NOR"�N�o���g�a����ا����l�ǰ_�ӡC\n", me, media, 1);
		msg("�H�ۥե��v�����h�A$YOU�]��ۮ����F�C\n", me, media, 1);
		destruct(media, 1);

		CITY_D->delete_coor_data(loc, "growth_level");
		CITY_D->delete_coor_data(loc, "maintain_time");
		CITY_D->delete_coor_data(loc, "growth");
		CITY_D->delete_coor_data(loc, "status");
		TOP_D->delete_top("maintain", save_variable(loc));

		landdata += ({ ({ type, growth_level, maintain_time }) });
		set("landdata", landdata, me);
		me->save();
		CITY_D->save_city(loc[CITY], loc[NUM]);
	}
	else if( sscanf(arg, "release %d", i) == 1 )
	{
		object env = environment(me);
		array landdata = query("landdata", me);
		array loc = query_temp("location", me);
		array elem;
		
		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "�L�k�N�g�a��������o���g�a�W�C\n");

		if( CITY_D->query_coor_data(loc, "owner") != me->query_id(1) )
			return tell(me, "�o���g�a�ä��O"+pnoun(2, me)+"���C\n");
			
		if( !arrayp(landdata) )
			return tell(me, "�ثe�S�����󪺤g�a��ءC\n");

		if( i < 1 || i > sizeof(landdata) )
			return tell(me, "�п�J���T���Ʀr�C\n");

		elem = landdata[i-1];
		
		if( CITY_D->query_coor_data(loc, TYPE) != elem[0] )
			return tell(me, "�L�k�N�o���������g�a��������o���g�a�W�C\n");
			
		if( CITY_D->query_coor_data(loc, "maintain_time") )
			return tell(me, "�o���g�a�w�g���{�s���g�a��ءA�L�k�A�N�g�a��������o���g�a�W�C\n");

		CITY_D->set_coor_data(loc, "growth_level", elem[1]);
		CITY_D->set_coor_data(loc, "maintain_time", elem[2]);
		GROWTH_D->set_status(loc);
		
		msg("$ME�N�j�q���g�a��������o���g�a�W�A�u���g�a�W�x�_�@�}"HIW"��ծ���~"NOR"�C\n", me, 0, 1);
		TOP_D->update_top("maintain", save_variable(loc), elem[1], me->query_idname(), elem[0]);

		landdata = landdata[0..i-2] + landdata[i..<1];
		
		if( !sizeof(landdata) )
			delete("landdata", me);
		else
			set("landdata", landdata, me);
		
		me->save();
	}
	else if( sscanf(arg, "%d %s", i, arg) == 2 )
	{
		object ob = present(arg);
		object env = environment(me);
		array loc = query_temp("location", me);
		mapping level_up_resource;
		mapping ob_resources;
		string str = "";
		int age, last_resource_needed;

		if( !objectp(ob) )
			return tell(me, "�S�� "+arg+" �o�Ӫ��~�C\n");

		//���L������U���O
		if( me->is_delaying() )
		{
			tell(me, me->query_delay_msg());
			return me->show_prompt();
		}

		if( i <= 0 || i > 10000 )
			return tell(me, "�п�J���T���ƶq(1~10000)�C\n");

		if( ob->is_keeping() )
			return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d���A�C\n");

		ob_resources = query("material", ob) || allocate_mapping(0);

		query_resource_amount(ref ob_resources, ob);

		if( !sizeof(ob_resources) )
			return tell(me, ob->query_idname()+"�L�k�ΨӶi�櫰������ɯšC\n");

		if( !env->is_maproom() || !arrayp(loc) || !CITY_D->city_exist(loc[CITY], loc[NUM]) )
			return tell(me, "�L�k�N�귽�Q�]�b�o���g�a�W�C\n");

		if( i > ob->query_amount() )
			return tell(me, "�o�̨S���o��h��"+ob->query_idname()+"�C\n");

		if( CITY_D->query_city_info(loc[CITY], "age") >= 3 )
			return tell(me, "�ثe�C���u�}���u"+CITY_D->query_age_name(3)+"�v�C\n");

		if( !me->cost_stamina(LEVEL_UP_RESOURCE_COST*i) )
			return tell(me, pnoun(2, me)+"����O�����F�C\n");

		level_up_resource = copy(CITY_D->query_city_info(loc[CITY], "level_up_resource"));
		
		str = "$ME�ӶO "+(LEVEL_UP_RESOURCE_COST*i)+" �I��O�N"+QUANTITY_D->obj(ob, i)+"�O�쫰�����a��U("HIG"������ "NOR GRN;

		age = CITY_D->query_city_info(loc[CITY], "age");

		if( age > 0 )
			last_resource_needed = (25000000 + 20000000*(CITY_D->query_city_nums(loc[CITY])-1))*to_int(pow(age-1, 2));
		else
			last_resource_needed = 0;

		foreach(string resource_name, int count in ob_resources)
		{
			if( level_up_resource[resource_name+"_curr"] + count * i > level_up_resource[resource_name+"_need"] )
			{
				tell(me, capitalize(resource_name) +" �귽�ƶq�L�k�A���W���ɤF�C\n");
				level_up_resource[resource_name+"_curr"] = level_up_resource[resource_name+"_need"];
			}
			else
				level_up_resource[resource_name+"_curr"] += count * i;
			
			str += capitalize(resource_name) +" "+sprintf("%.5f%%", (level_up_resource[resource_name+"_curr"]-last_resource_needed)*100./(level_up_resource[resource_name+"_need"]-last_resource_needed))+" ";

			// �����ƶq��T
			CITY_D->add_land_record(loc[CITY], loc[NUM], me->query_id(1), resource_name, count * i);
		}

		str += NOR")�C\n";
		
		msg(str, me, 0, 1);
		
		destruct(ob, i);

		CITY_D->set_city_info(loc[CITY], "level_up_resource", level_up_resource);
		CITY_D->city_level_up(loc[CITY]);
	}

	else return tell(me, "�п�J���T�����O�Ѽ�(help land)�C\n");
}
