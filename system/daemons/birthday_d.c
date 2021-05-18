/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : birthday_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-12-23
 * Note   : 生日精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

void grow_up(object user)
{
	addn("age", 1, user);
	delete("age_hour", user);
	
	if( userp(user) )
		tell(user, pnoun(2, user)+"渡過了 "+query("age", user)+" 歲的生日。\n");

	user->earn_prestige(range_random(2,4));
	user->save();
}

string query_name()
{
	return "生日系統(BIRTHDAY_D)";
}
