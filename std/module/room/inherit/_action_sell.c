/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _sell_action.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-23
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <message.h>
#include <npc.h>
#include <money.h>

int over_capacity(object env, string database, int amount, int capacity);
void input_object(object env, string database, string basename, int amount);
void output_object(object env, string database, string basename, int amount);

// �p�۳c��ӫ~�ʧ@
void do_sell(object me, string arg, string database, int capacity)
{
	int percent;
	int value;
	mapping sell;
	mapping sell_setup;
	int amount;
	string option;
	string basename, unit;
	object ob, env, boss, master;
	
	env = environment(me);
	master = env->query_master();

	sell = query("setup/sell", master)||allocate_mapping(0);

	if( !arg )
	{
		int number;
		string msg, *allowed_sell_products = keys(sell);
			
		msg  = "���\���ʰӫ~�C��\n";
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		msg += "�s�� ���~                            �����ʼƶq ���ʻ�              ���ʤ�\n";
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		foreach(basename in allowed_sell_products)
		{
			if( basename == "all" || !objectp(ob = load_object(replace_string(basename,"#","/"))) ) continue;
					
			sell_setup = sell[replace_string(basename, "/", "#")]||sell["all"];

			if( !mapp(sell_setup) || sell_setup["nosell"] )
				continue;

			msg += sprintf("%s%-5s"NOR"%-34s%s%8s %-20s%6s\n"NOR, 
				!(number % 2) ? "" : WHT,
				++number+".", 
				ob->query_idname(),
				number % 2 ? "" : WHT,
				(sell_setup["amount"]||"�L��")+"",
				NUMBER_D->number_symbol(count(count(query("value", ob), "*", sell_setup["percent"]), "/", 100)),
				sell_setup["percent"]+"%",
				);
		}
		if( member_array("all", allowed_sell_products) != -1 )
		{
			msg += sprintf("     %-34s%s%8s %26s\n"NOR, 
				"��L�Ҧ����~",
				number % 2 ? "" : WHT,
				(sell["all"]["amount"]||"�L��")+"",
				sell["all"]["percent"]+"%",
				);
			
		}
				
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		return me->more(msg+"\n");
	}

	if( query("mode", master) )
		return tell(me, env->query_room_name()+"�ثe���A���u�޲z�Ҧ��v�A����ʪ��~�C\n");

	if( query("owner", env) == me->query_id(1) )
		return tell(me, "�����W�[�ӫ~���O����ֶܡH\n");

	unit = env->query_money_unit();


	if( lower_case(arg) == "all" )
	{
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			if( !objectp(boss = load_user(query("owner", env)) ) )
				return tell(me, "�o���ө����󪺳s��\��w�g�l�a�A�L�k�V"+pnoun(2, me)+"���ʰӫ~�C\n");
/*
			if( query("city", boss) != query("city", me) )
			{
				if( !userp(boss) )
					destruct(boss);
			
				return tell(me, "�L�k�󫰳c��ӫ~�C\n");
			}
*/
			foreach(ob in all_ob)
			{
				amount = ob->query_amount();
				
				if( ob->is_keeping() || query("flag/no_import", ob) || query("flag/no_give", ob) || query("flag/no_drop", ob) || count(query("value", ob),"<",1) || query_temp("endurance", ob) < 0 ) continue;
				
				basename = replace_string(base_name(ob), "/", "#");
				
				sell_setup = sell[basename]||sell["all"];
				
				if( !mapp(sell_setup) || sell_setup["nosell"] )
				{
					tell(me, "�o�̨ä�����"+ob->query_idname()+"�o�ت��~�C\n");
					continue;
				}
					
				percent = sell_setup["percent"];
				
				if( sell_setup["amount"] && sell_setup["amount"] < amount )
				{
					tell(me, pnoun(2, me)+"���c�檺"+ob->query_idname()+"�ƶq�w�W�L���\�����ʼơC\n");
					continue;
				}
				
				if( over_capacity(master, database, amount, capacity) )
				{
					if( !userp(boss) )
					{
						boss->save();
						destruct(boss);
					}
					return tell(me, env->query_room_name()+"�L�k�A�e�ǳo��h�����~�F("+capacity+")�C\n");
				}

				value = to_int(query("value", ob) * amount * percent / 100.);
				
				if( value != 0 && !boss->spend_money(unit, value, BANK_PAYMENT) )
				{
					if( !userp(boss) )
					{
						boss->save();
						destruct(boss);
					}
					return tell(me, "�ө�����w�g�S������h�������i�H����"+pnoun(2, me)+"���ӫ~�F�C\n");
				}
				else
					tell(boss, env->query_room_name()+"��F"HIY+QUANTITY_D->money_info(unit, value)+NOR" ���V"+me->query_idname()+"���ʤF"+ob->query_idname()+"�C\n");
				
				
				me->earn_money(unit, value);
				
				input_object(master, database, replace_string(basename, "#", "/") , amount);

				if( to_int(sell_setup["amount"]) )
				{
					if( sell[basename] )
						set("setup/sell/"+basename+"/amount", to_int(sell_setup["amount"]) - amount, master);
					else
						set("setup/sell/all/amount", to_int(sell_setup["amount"]) - amount, master);
				}

				msg("$ME�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"��X�A�o��F"HIY+QUANTITY_D->money_info(unit, value)+NOR"�C\n",me,0,1);
				
				if( value > 10000000 )
					log_file("command/sell", me->query_idname()+"�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"�浹 "+query("owner", env)+"�A�o��F"HIY+QUANTITY_D->money_info(unit, value)+NOR"�C");
				
				destruct(ob, amount);
				me->delay_save(300);
			}
			
			if( !userp(boss) )
			{
				boss->save();
				destruct(boss);
			}

			return msg("$ME�⨭�W����檺�F����汼�F�C\n"NOR,me,0,1);
		}
		return tell(me, pnoun(2, me)+"���W�S���F��i�H��I\n");
	}

	if( sscanf(arg, "%s %s", option, arg) == 2 && (!arg || (option != "all" && !big_number_check(option))) )
	{
		arg = option+" "+arg;
		amount = 1;
	}

	if( !objectp(ob = present(arg, me) || present(arg, environment(me))) )
		return tell(me, "�o����èS�� "+arg+" �o�˪F��C\n");

	if( ob->is_keeping() )
		return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");

	if( query("flag/no_import", ob) || query("flag/no_give", ob) || query("flag/no_drop", ob) )
		return tell(me, ob->query_idname()+"�����\�c��C\n");

	if( option == "all" )
		amount = ob->query_amount();
	else if( !(amount = to_int(option)) || amount < 1 )
		amount = 1;

	if( amount > ob->query_amount() )
		return tell(me, "�o����èS������h"+(query("unit", ob)||"��")+ob->query_idname()+"�C\n");
	
	if( query_temp("endurance", ob) < 0 )
		return tell(me, ob->query_idname()+"�w�g�l�a�F�A�L�k�V"+pnoun(2, me)+"���ʡC\n");

	value = query("value", ob);
	
	if( value < 1 )
		return tell(me, "��p�ڭ̤��A�ѳo�Ӱӫ~���Ȧh�֡A�L�k�V"+pnoun(2, me)+"���ʡC\n");
	
	
	basename = replace_string(base_name(ob), "/", "#");

	sell_setup = sell[basename]||sell["all"];
	
	if( !mapp(sell_setup) || sell_setup["nosell"] )
		return tell(me, "�o�̨ä����⦬�ʳo�ذӫ~�C\n");
	
	if( to_int(sell_setup["amount"]) && to_int(sell_setup["amount"]) < amount )
		return tell(me, pnoun(2, me)+"���c�檺"+ob->query_idname()+"�ƶq�w�W�L���\�����ʼơC\n");

	percent = sell_setup["percent"];
	
	value = to_int(value * amount * percent / 100.);
	
	if( over_capacity(master, database, amount, capacity) )
		return tell(me, env->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");

	boss = load_user(query("owner", env));
/*
	if( query("city", boss) != query("city", me) )
	{
		if( !userp(boss) )
			destruct(boss);
			
		return tell(me, "�L�k�󫰳c��ӫ~�C\n");
	}
*/
	if( value != 0 && !boss->spend_money(unit, value, BANK_PAYMENT) )
	{
		if( !userp(boss) )
		{
			boss->save();
			destruct(boss);
		}
		return tell(me, "�ө�����w�g�S������h�������i�H����"+pnoun(2, me)+"���ӫ~�F�C\n");
	}
	else if( !userp(boss) )
	{
		boss->save();
		destruct(boss);
	}
	else
		tell(boss, env->query_room_name()+"��F"HIY+QUANTITY_D->money_info(unit, value)+NOR" ���V"+me->query_idname()+"���ʤF"+ob->query_idname()+"�C\n");

	input_object(master, database, replace_string(basename, "#", "/") , amount);

	me->earn_money(unit, value);
	
	if( to_int(sell_setup["amount"]) )
	{
		if( sell[basename] )
			set("setup/sell/"+basename+"/amount", to_int(sell_setup["amount"]) - amount, master);
		else
			set("setup/sell/all/amount", to_int(sell_setup["amount"]) - amount, master);
	}
	msg("$ME�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"��X�A�o��F"HIY+QUANTITY_D->money_info(unit, value)+NOR"�C\n",me,0,1);
	
	if( value > 10000000 )
		log_file("command/sell", me->query_idname()+"�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"�浹 "+query("owner", env)+"�A�o��F"HIY+QUANTITY_D->money_info(unit, value)+NOR"�C");
	
	destruct(ob, amount);
	me->delay_save(300);
}

// ��X�ӫ~
void do_trading_post_sell(object me, string arg, int percent)
{
	int value;
	string option;
	int amount, env_city;
	object ob, env;
	
	env = environment(me)->query_master();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�椰��F��H\n");

	env_city = env->query_city();
/*
	if( query("city", me) != env_city )
		return tell(me, "�Ȯɰ���󫰳c��ӫ~���\��C\n");
*/
	if( lower_case(arg) == "all" )
	{
		object *all_ob = all_inventory(me);
		
		if( sizeof(all_ob) )
		{
			foreach(ob in all_ob)
			{
				amount = ob->query_amount();
				
				if( ob->is_keeping() || count(query("value", ob),"<",1) || query_temp("endurance", ob) < 0 ) continue;
				
				if( query("badsell", ob) )
					percent = 50;
				else
					percent = 50+me->query_skill_level("price")/5;

				value = to_int(query("value", ob) * amount * percent / 100.);
				
				me->earn_money(MONEY_D->city_to_money_unit(env_city), value);
				msg("$ME�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"��X�A�o��F"HIY+QUANTITY_D->money_info(env_city, value)+NOR"�C\n",me,0,1);

				destruct(ob, amount);
			}
			return msg("$ME�⨭�W����檺�F����汼�F�C\n"NOR, me, 0, 1);
		}
		return tell(me, pnoun(2, me)+"���W�S���F��i�H��I\n");
	}

	if( sscanf(arg, "%s %s", option, arg) == 2 && (!arg || (option != "all" && !big_number_check(option))) )
	{
		arg = option+" "+arg;
		amount = 1;
	}

	if( !objectp(ob = present(arg, me) || present(arg, environment(me))) )
		return tell(me, "�o����èS�� "+arg+" �o�˪F��C\n");
	
	if( ob->is_keeping() )
		return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
			
	if( option == "all" )
		amount = ob->query_amount();
	else if( !(amount = to_int(option)) || amount < 1 )
		amount = 1;

	if( amount > ob->query_amount() )
		return tell(me, "�o����èS������h"+(query("unit", ob)||"��")+ob->query_idname()+"�C\n");
	
	if( query_temp("endurance", ob) < 0 )
		return tell(me, ob->query_idname()+"�w�g�l�a�F�A�L�k�V"+pnoun(2, me)+"���ʡC\n");
	
	value = query("value", ob);
	
	if( value < 1 )
		return tell(me, "��p�ڭ̤��A�ѳo�Ӱӫ~���Ȧh�֡A�L�k�V"+pnoun(2, me)+"���ʡC\n");

	if( query("badsell", ob) )
		percent = 50;
	else
		percent = 50+me->query_skill_level("price")/5;
					
	value = to_int(value * amount * percent / 100.);
	
	//CITY_D->set_city_info(env_city, "trading_post_gain", count(CITY_D->query_city_info(env_city, "trading_post_gain"), "-", count(value, "/", 5)));
	me->earn_money(MONEY_D->city_to_money_unit(env_city), value);
	
	msg("$ME�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"��X�A�o��F"HIY+QUANTITY_D->money_info(env_city, value)+NOR"�C\n",me,0,1);
	//log_file("command/sell", me->query_idname()+"�H "+percent+"% ������N"+QUANTITY_D->obj(ob, amount)+"��X�A�o��F"HIY+QUANTITY_D->money_info(env_city, value)+NOR"�C");
	destruct(ob, amount);
}