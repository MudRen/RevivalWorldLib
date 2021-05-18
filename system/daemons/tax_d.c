/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tax_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-08-14
 * Note   : 
 * Update :
 *  o 2006-02-23 Clode ���s��g
 *
 -----------------------------------------
 */
 
#include <ansi.h>
#include <daemon.h>
#include <estate.h>
#include <message.h>
#include <time.h>
#include <money.h>
#include <login.h>

int query_player_tax(string id, int estatetax)
{
	int floors = ESTATE_D->query_total_estate_floors(id);
	
	return floors * estatetax;
}

void game_month_process()
{
	int owetax, *gametime = TIME_D->query_gametime_array();
	int estatetax;
	string city, citizen, money_unit, msg, cityidname;
	object player;
	int totalmoney;

	msg = "���ͪ��@��"+CHINESE_D->chinese_number(gametime[YEAR])+"�~"+CHINESE_D->chinese_number(gametime[MON]+1)+"��A";
	
	foreach( city in CITY_D->query_cities() )
	{
		estatetax = CITY_D->query_city_info(city, "estatetax");

		money_unit = MONEY_D->city_to_money_unit(city);
		cityidname = CITY_D->query_city_idname(city);

		foreach( citizen in CITY_D->query_citizens(city) )
		{
			reset_eval_cost();

			totalmoney = query_player_tax(citizen, estatetax);

			catch(player = load_user(citizen));

			if( !objectp(player) )
			{
				CHANNEL_D->channel_broadcast("sys", "�|�ȡG�L�k���J "+citizen+" �������");
				continue;
			}

			// �W�u���� 15 �Ѫ����K�|
		        if( query("total_online_time", player) < 86400*15 )
			{
				if( userp(player) )
					tell(player, "\n\n"HIY+pnoun(2, player)+"�W�u���� 15 �ѡA�K������|�ڡC\n\n\n"NOR);
				else
					destruct(player);
				
				continue;
			}

			owetax = query("owetax", player);
	
			if( owetax > 0 && userp(player) )
				tell(player, pnoun(2, player)+"�@��| "+owetax+" ���A�����N�@�H "+(owetax*3)+" ���|�ڡC");

			if( !undefinedp(query("taxrate", player)) )
				totalmoney *= copy(query("taxrate", player));

			if( totalmoney > 0 && !MONEY_D->spend_money(citizen, money_unit, owetax ? owetax * 3 * totalmoney : totalmoney, FORCED_PAYMENT) )
			{
				owetax = addn("owetax", 1, player);
				
				if( owetax == 1 )
					CHANNEL_D->channel_broadcast("city", "����"+player->query_idname()+"�� 1 ���I���_�|�ڡA�U�����|�N�@�H 3 �����B�A�s��ֿn 7 ����|�h�x���Ҧ��Цa���ö}�����y�C", player);
				else if( owetax >= 7 )
				{
					if( CITY_D->is_mayor(city, player) && owetax <= 50 )
					{
						CHANNEL_D->channel_broadcast("city", "����"+player->query_idname()+"�@��| "+owetax+" ���C", player);
						tell(player, HIY+pnoun(2, player)+"�@��|�W�L "+owetax+" ���C\n"NOR);
					}
					else
					{
						player->move(STARTROOM);
						CHANNEL_D->channel_broadcast("city", "����"+player->query_idname()+"�@��| "+owetax+" ���A�x���Ҧ��Цa���ö}�����y�C", player);
						delete("owetax", player);
						delete("city", player);
						ESTATE_D->remove_estate(citizen);
						CITY_D->remove_citizen(citizen);
						tell(player, HIY+pnoun(2, player)+"�@��|�W�L "+owetax+" ���A�x���Ҧ��Цa���ö}�����y�C\n"NOR);
					}
					player->save();
					
					if( !userp(player) )
						destruct(player);
	
					continue;	
				}
				else
					CHANNEL_D->channel_broadcast("city", "����"+player->query_idname()+"�s�� "+owetax+" ���I���_�|�ڡA�U�����|�N�@�H "+(owetax*3)+" �����B�A�s��ֿn 7 ����|�h�x���Ҧ��Цa���ö}�����y�C", player);
			}
			else
				delete("owetax", player);			
			
			CITY_D->change_assets(city, totalmoney);

			player->save();

			if( userp(player) )
				tell(player, "\n\n"HIY+pnoun(2, player)+"�`�@�Q�x���|�� $"+money_unit+" "+NUMBER_D->number_symbol(owetax ? owetax * 3 * totalmoney : totalmoney)+"\n"NOR YEL"�Բӵ|�����ؽЧQ�� tax ���O�d�ߡC\n\n\n"NOR);
			else
				destruct(player);
		}
	}
}

int query_city_tax(string city)
{
	int estatetax;
	string citizen;
	int totalmoney;
	object player;
	int money;

	if( !CITY_D->city_exist(city) ) return 0;

	estatetax = CITY_D->query_city_info(city, "estatetax");

	foreach( citizen in CITY_D->query_citizens(city) )
	{
		player = load_user(citizen);

		if( !objectp(player) ) continue;
		
		money = query_player_tax(citizen, estatetax);
		
		if( !undefinedp(query("taxrate", player)) )
			money *= copy(query("taxrate", player));

		totalmoney += money;
		
		if( !userp(player) )
			destruct(player);
	}
	
	return totalmoney;
}

void create()
{


}
string query_name()
{
	return "�|���t��(TAX_D)";
}
