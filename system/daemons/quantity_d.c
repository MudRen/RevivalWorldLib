/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : quantity_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-16
 * Note   : 量詞精靈
 * Update :
 *
 -----------------------------------------
 */
 
#include <daemon.h>

varargs string obj(object ob, mixed amount, int no_blank)
{
	if( undefinedp(amount) )
		amount = ob->query_amount();
	
	amount = to_int(amount);
	
	if( amount == 1 )
		return ob->short();
	
	return (no_blank?"":" ")+NUMBER_D->number_symbol(amount)+" "+(query("unit",ob)||"個")+ob->short();
}

varargs string obj_info(string idname, string unit, mixed amount, int no_blank)
{
	if( !unit ) unit = "個";
	
	amount = to_int(amount);
	if( amount == 1 )
		return idname;

	return (no_blank?"":" ")+NUMBER_D->number_symbol(amount)+" "+unit+idname;		
}

varargs string money(object ob, mixed amount, int no_blank)
{
	string unit = query("unit", ob);
	
	if( undefinedp(amount) )
		amount = query("money", ob) || 1;

	return (no_blank?"":" ")+"$"+unit+" "+NUMBER_D->number_symbol(amount)+"("+unit+" money)";
}

varargs string money_info(string city_or_money_unit, mixed amount, int no_blank)
{
	if( !MONEY_D->money_unit_exist(city_or_money_unit) )
		city_or_money_unit = MONEY_D->city_to_money_unit(city_or_money_unit);
	
	if( !city_or_money_unit )
		return 0;
		
	return (no_blank?"":" ")+"$"+city_or_money_unit+" "+NUMBER_D->number_symbol(amount);
}
string query_name()
{
	return "量詞系統(QUANTITY_D)";
}
