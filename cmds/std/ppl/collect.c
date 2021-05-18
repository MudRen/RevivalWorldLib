/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : collect.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-2-19
 * Note   : collect ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define COLLECT_STR_COST	40

#define MATERIAL_PATH		"/obj/materials/"

#include <buff.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <material.h>
#include <location.h>
#include <citydata.h>

inherit COMMAND;

string help = @HELP

�Ķ����O

�o�ӫ��O�i�b�a�ϤW�Ķ��U�ح�ơA��Ʀ@�����ءC

metal	���� - ��s�ϨϥΤQ�r�����u��Ķ�
stone	��� - ��s�Ϩϥ��p�Y���u��Ķ�
water	�M�� - ��e�y�ϥΤ������u��Ķ�
wood	��� - ��˪L�ϥΤ�����u��Ķ�
fuel	�U�� - ��˪L�ϥ���l���u��Ķ�

���O�榡:
collect metal			- �Ķ��@�Ӫ���(�Ķ����W�������Q�r�����u��)
collect 10 metal		- �@���Ķ��Q�Ӫ���
collect warehouse city N X,Y	- �]�w�귽�Ķ��ܮw�a�I(ex: collect warehouse wizard 1 50,50)
collect warehouse -d		- ������ƭܮw�]�w
�������O: info

HELP;

private int cost_stamina(object me, int amount, int skilllevel, int quality)
{
	int collect_buff = me->query_all_buff(BUFF_COLLECT_ABILITY);
	int stamina_cost = (amount - amount * collect_buff / 100)* (COLLECT_STR_COST - skilllevel/4);

	stamina_cost = to_int(stamina_cost * 100. / quality);

	return me->cost_stamina(stamina_cost);	
}

private void do_command(object me, string arg)
{
	array loc;
	object ob, env = environment(me);
	mapping coorrangetype;
	object *tools;
	int skilllevel, endurance, amount;
	string arg2;
	string area, city;
	int num;
	int quality;
	int x, y;
	string mycity = query("city", me);
	object warehouse;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�Ķ����ظ귽�H("HIW"���� metal"NOR"�B"WHT"��� stone"NOR"�B"HIC"�M�� water"NOR"�B"YEL"��� wood"NOR"�B"HIM"�U�� fuel"NOR")\n");
	
	if( arg == "warehouse" )
	{
		if( !objectp(warehouse = load_module(query("collect_warehouse", me))) )
			return tell(me, pnoun(2, me)+"�|���]�w�귽�ܮw�C\n");
		else
			return tell(me, pnoun(2, me)+"�ثe���귽�ܮw�]�w�b"+warehouse->query_room_name()+"�C\n");
	}
	else if( arg == "warehouse -d" )
	{
		if( !query("collect_warehouse", me) )
			return tell(me, pnoun(2, me)+"�쥻�K���]�w�귽�ܮw�C\n");
			
		delete("collect_warehouse", me);
		
		return tell(me, pnoun(2, me)+"�����F�귽�ܮw���]�w�C\n");
	}
	else if( sscanf(arg, "warehouse '%s' %d %d,%d", city, num, x, y) == 4 || sscanf(arg, "warehouse %s %d %d,%d", city, num, x, y) == 4 )
	{
		string file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");
		
		if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
			return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS���ܮw�C\n");

		if( query("owner", warehouse) != me->query_id(1) )
			return tell(me, warehouse->query_room_name()+"�ä��O"+pnoun(2, me)+"���ܮw�C\n");

		if( warehouse != warehouse->query_master() )
		{
			file = base_name(warehouse->query_master());
			warehouse = warehouse->query_master();
		}
		
		set("collect_warehouse", file, me);
		
		msg("$ME�]�w"+warehouse->query_room_name()+"���귽�Ķ����ܮw�C\n", me, 0, 1);
		return;
	}

	if( !env || !env->is_maproom() )
		return tell(me, "�L�k�b�o�̱Ķ��귽�C\n");
	
	//���L������U���O
	if( me->is_delaying() )
	{
		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}
	
	if( !mycity )
		return tell(me, pnoun(2, me)+"�����������Y�ӫ�������������~�i�H�}�l�Ķ��귽�C\n");

	loc = query_temp("location", me);
	
	if( (MAP_D->query_map_system(loc)) != AREA_D )
		return tell(me, "�L�k�b�������Ķ��귽�C\n");

	warehouse = load_module(query("collect_warehouse", me));
		
	if( objectp(warehouse) && query("owner", warehouse) != me->query_id(1) )
		return tell(me, warehouse->query_room_name()+"�ä��O"+pnoun(2, me)+"���ܮw�C\n");
 
 	area = loc[AREA];
	num = loc[NUM];
	
	if( AREA_D->query_lead_city(area, num) != mycity )
		return tell(me, pnoun(2, me)+"������"+CITY_D->query_city_idname(mycity)+"�|������o�����ϩΦ����ƶq���� 50000�A"+pnoun(2, me)+"�L�k�b�o�̱Ķ��귽�C\n");

 	if( me->is_fatigue_full() )
		return tell(me, pnoun(2, me)+"��b�O�ӯh�ҤF�A�L�k�A�Ķ��귽�C\n");

	coorrangetype = AREA_D->query_coor_range(loc, TYPE, 1);

	arg = lower_case(arg);

	if( sscanf(arg, "%d %s", amount, arg2) == 2 )
		arg = arg2;
	
	if( amount < 1 )
		amount = 1;
	
	if( amount > 1000 )
		return tell(me, "�̦h�@���u��Ķ� 1000 �Ӹ귽�C\n");

	switch(arg)
	{
		case "metal":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���s�ϡA�S��k�}�Ī��ݡC\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�Ī��ݪ��u��C\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME��ݼN�N�A�w�g�S�O��A�}�Ī��ݤF...�C\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�Ī��ݤF...�C\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);

			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME�|�_$YOU��O�a�q�q�ۤ��w�X�F"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("metal") < 20 )
				me->add_skill_exp("metal", amount*(12+random(11)));
			else if( me->query_skill_level("metal") == 20 )
				tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

			me->add_social_exp(to_int(amount*(30+random(31))));

			if( !random(1000) )
				me->add_fatigue(1);

			break;	

		case "stone":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == MOUNTAIN :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���s�ϡA�S��k�}�ĥۧ��C\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�ĥۧ����u��C\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME��ݼN�N�A�w�g�S�O��A�}�ĥۧ��F...�C\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�ĥۧ��F...�C\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);

			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME�|�_$YOU��O�a�q���h�w�X�F"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("stone") < 20 )
				me->add_skill_exp("stone", amount*(12+random(11)));
			else if( me->query_skill_level("stone") == 20 )
				tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

			me->add_social_exp(to_int(amount*(30+random(31))));
			
			if( !random(1000) )
				me->add_fatigue(1);

			break;	

		case "water":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == POOL || $2 == RIVER :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS�������A�S��k�Ķ��M���C\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ������u��C\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME��ݼN�N�A�w�g�S�O��A����F...�C\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A�������F�@�Ӥj�}�A��M�O�S��k�A�~��ΨӸˤ��F...�C\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}

				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);
			
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME����$YOU�ΤO�a�q�e����_�F"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("water") < 20 )
				me->add_skill_exp("water", amount*(12+random(11)));
			else if( me->query_skill_level("water") == 20 )
				tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

			me->add_social_exp(to_int(amount*(30+random(31))));
			
			if( !random(1000) )
				me->add_fatigue(1);

			break;
			
		case "wood":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS����L�A�S��k���C\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���Ψӥ�쪺�u��C\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME��ݼN�N�A�w�g�S�O��A���F...�C\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��Ψӥ��F...�C\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
	
			ob = new(MATERIAL_PATH+arg);
		
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME�ⴤ$YOU�V�O�a�q����W���U�F"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("wood") < 20 )
				me->add_skill_exp("wood", amount*(12+random(11)));
			else if( me->query_skill_level("wood") == 20 )
				tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

			me->add_social_exp(to_int(amount*(30+random(31))));

			if( !random(1000) )
				me->add_fatigue(1);

			break;

		case "fuel":
			skilllevel = me->query_skill_level(arg);
			
			coorrangetype = filter(coorrangetype, (: $2 == FOREST :));

			if( !sizeof(coorrangetype) )
				return tell(me, "���񪺰ϰ줤�èS���˪L�A�S��k�}�ĿU�ơC\n");
			
			tools = filter_array(all_inventory(me), (: query("collection_tool", $1) == $(arg) :));
			
			if( !sizeof(tools) )
				return tell(me, pnoun(2, me)+"���W�S���ΨӶ}�ĿU�ƪ��u��C\n");
			
			quality = query("quality", tools[0]);

			if( !cost_stamina(me, amount, skilllevel, quality) )
				return msg("$ME��ݼN�N�A�w�g�S�O��A�}�ĿU�ƤF...�C\n", me, 0, 1);
			
			endurance = query("endurance", tools[0]);

			// �u��l�a(�l�a���v�P�u��@�[�שM�ϥΪ̧ޯ����)
			if( !random(endurance + to_int(skilllevel*endurance/100.)) )
			{
				msg("$ME��W��"+tools[0]->query_idname()+"��M�y�ԡz���@�n�A��`�B���F�}�ӡA��M�O�S��k�A�~��ΨӶ}�ĿU�ƤF...�C\n", me, 0, 1);
				
				if( !random(2) )
				{
					tell(me, pnoun(2, me)+"�q�o�Өƥ���o��h�����|�g��C\n");
					me->add_social_exp(endurance + to_int(skilllevel*endurance/100.) - 50 + random(101));
				}
				
				destruct(tools[0], 1);
				return;
			}
			
			ob = new(MATERIAL_PATH+arg);
			
			if( amount > 1 )
				ob->set_amount(amount);

			msg("$ME�|�_$YOU��O�a�q�g�a�̫��X�F"+ob->short(1)+"\n", me, tools[0], 1);
			
			if( objectp(warehouse) )
			{
				warehouse->query_module_file()->input_object(warehouse, "products", MATERIAL_PATH+arg, amount);
				destruct(ob);
			}
			else if( !me->available_get(ob, amount) )
			{
				msg("$ME�w�g������"+ob->short(1)+"�F�I�u�n����b�a�W�C\n", me, 0, 1);
				ob->move_to_environment(me);
			}
			else
				ob->move(me);

			if( me->query_skill_level("fuel") < 20 )
				me->add_skill_exp("fuel", amount*(12+random(11)));
			else if( me->query_skill_level("fuel") == 20 )
				tell(me, pnoun(2, me)+"���Ķ��ޯ൥�Ťw�g��F 20�A�L�k�A�ۧک��W���ɡA�󰪲`���ǰݥ����}�l��j�ǾǲߡC\n");

			me->add_social_exp(amount*(30+random(31)));

			if( !random(1000) )
				me->add_fatigue(1);

			break;

		default:
			return tell(me, pnoun(2, me)+"�Q�n�������ح�ơH("HIW"���� metal"NOR", "WHT"��� stone"NOR", "HIC"�M�� water"NOR", "YEL"��� wood"NOR", "HIM"�U�� fuel"NOR")\n");
			break;
	}
	
	me->remove_status(query_temp("last_time_collect_status", me));
	me->add_status(set_temp("last_time_collect_status", HIM"��"NOR MAG"��"NOR, me), 30);
}
	
