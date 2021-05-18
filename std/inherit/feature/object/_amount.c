/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _amount.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-21
 * Note   :
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

int query_amount()
{
	return to_int(query_temp("amount")) || 1;
}

int set_amount(int amount)
{
	return set_temp("amount", amount);
}

int add_amount(int amount)
{
	return set_temp("amount", query_amount() + amount);
}

int no_amount()
{
	return query("flag/no_amount");
}

int set_no_amount()
{
	set("flag/no_amount", 1);	
}

