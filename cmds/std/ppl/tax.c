/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : tax.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-09-04
 * Note   : tax ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
    �d�ߵ|�����p

tax			- �d�ߦۤv���|�����p
tax '���aID'    	- �d�߬Y�쥫�����|�����p(�����x�����O)
tax city '����ID'	- �d�߾�y�������|�����p(�����x�����O)
    
HELP;

private void do_command(object me, string arg)
{
	int estatetax;
	string city, money_unit, msg, id, query_city;
	object player;
	int money;

	city = query("city", me);

	if( !city )
		return tell(me, pnoun(2, me)+"���[�J���󫰥��A�L�|����ơC\n");

	if( !arg )
		id = me->query_id(1);
	else if( sscanf(arg, "city %s", query_city) == 1 )
	{
		int total_money;
		
		money_unit = MONEY_D->city_to_money_unit(query_city);
		estatetax = CITY_D->query_city_info(query_city, "estatetax");

		if( !wizardp(me) && !CITY_D->is_mayor_or_officer(query_city, me) )
			return tell(me, "�u�������Ωx���i�H�d�߳o�y�������|�����ΡC\n");
		
		msg = CITY_D->query_city_idname(query_city)+"���Ҧ��|�����p�p�U�G\n";
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		msg += "�W��                   �|�����v         �|��\n";
		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

		foreach(string citizen in CITY_D->query_citizens(query_city))
		{
			money = TAX_D->query_player_tax(citizen, estatetax);
			
			player = load_user(citizen);
			
			if( !undefinedp(query("taxrate", player)) )
				money *= copy(query("taxrate", player));

			total_money += money;

			msg += sprintf("%-30s%-10d"HIY"%s"NOR"\n", player->query_idname(), undefinedp(query("taxrate", player))?1:query("taxrate", player), money(money_unit, money));
			
			if( !userp(player) )
				destruct(player);
		}

		msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
		msg += sprintf("%-40s"HIY"%s"NOR"\n", CITY_D->query_city_idname(city)+"�|���`���J", money(money_unit, total_money));

		return me->more(msg);
	}
	else if( wizardp(me) )
		id = arg;
	else if( CITY_D->is_mayor_or_officer(city, me) )
	{
		if( !CITY_D->is_citizen(arg, city) )
			return tell(me, capitalize(arg)+" ���O�������������C\n");
		
		id = arg;
	}
	else
		return tell(me, pnoun(2, me)+"�S���v�Q�d�ߨ�L�H���|����ơC\n");
	
	if( !objectp(player = load_user(id)) )
		return tell(me, "�S�� "+id+" �o�쪱�a�C\n");

	city = query("city", player);

	money_unit = MONEY_D->city_to_money_unit(city);
	estatetax = CITY_D->query_city_info(city, "estatetax");
	money = TAX_D->query_player_tax(id, estatetax);

	if( !undefinedp(query("taxrate", player)) )
		money *= copy(query("taxrate", player));
		
	msg = (player == me ? pnoun(2, me):player->query_idname())+"���|����Ʀp�U�G\n";	
	msg += sprintf("�|������      �g�a+�Ыμ� x �|�v   x �|�����v   =   �|���`�B\n"
	NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
	NOR HIY"�a���|        %-11d x %-6d x %-10d =   %s\n"
	NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"NOR
	, ESTATE_D->query_total_estate_floors(id)
	, estatetax
	, undefinedp(query("taxrate", player)) ? 1 : query("taxrate", player)
	, money(money_unit, money));
	
	if( !userp(player) )
		destruct(player);

	tell(me, msg);
}