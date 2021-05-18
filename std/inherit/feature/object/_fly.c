/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _fly.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-14
 * Note   :
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

varargs int is_flying(int fly_type)
{
	if( undefinedp(fly_type) )
		return query_temp("is_flying");
	else
		return fly_type == query_temp("is_flying");
}

void start_fly(int fly_type)
{
	set_temp("is_flying", fly_type);
}

void stop_fly()
{
	delete_temp("is_flying");
}
