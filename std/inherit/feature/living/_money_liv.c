/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _money.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-02-01
 * Note   : 金錢處理繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <bank.h>
#include <daemon.h>

varargs int spend_money(string unit, int money, int autotransfer)
{
	if( !this_object()->is_user_ob() ) return 0;

	return MONEY_D->spend_money(this_object()->query_id(1), unit, money, autotransfer);
}

varargs string earn_money(string unit, int money, int autotransfer)
{
	if( !this_object()->is_user_ob() ) return 0;

	return MONEY_D->earn_money(this_object()->query_id(1), unit, money, autotransfer);
}

int drop_money(string unit, int money)
{
	string myid = this_object()->query_id(1);
	mapping moneydata = MONEY_D->query_moneydata(myid);
	
	if( !money || !unit || !mapp(moneydata) ) return 0;
	
	unit = upper_case(unit);
	
	if( money <= 0 ) return 0;

	if( MONEY_D->spend_money(myid, unit, money) )
	{
		new("/obj/etc/money", unit, money)->move_to_environment(this_object());
	
		return 1;
	}
	return 0;
}

string current_money_unit()
{
	object env = environment(this_object());

	if( !env ) return 0;

	return MONEY_D->city_to_money_unit(query("city", env) || env->query_city() || query("city"));
}

string my_money_unit()
{
	return MONEY_D->city_to_money_unit(query("city")) || MONEY_D->query_default_money_unit();
}

int valid_money_unit(string unit)
{
	if( !unit || !MONEY_D->money_unit_exist(unit) )
		return 0;

	return unit == MONEY_D->city_to_money_unit(query("city")) || unit == MONEY_D->query_default_money_unit();
}
