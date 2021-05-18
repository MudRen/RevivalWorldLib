/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _stuck_liv.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-02
 * Note   : 移動延遲
 * Update :
 *  o 2003-00-00 
 -----------------------------------------
 */

#include <stuck.h>

/*
({
	second,
	message,
	finish message,
	function point,
})
*/

private nosave mapping stuckinfo = allocate_mapping(0);

varargs int is_stucking(string key)
{
	if( !key )
		return sizeof(stuckinfo);
	else
		return !undefinedp(stuckinfo[key]);
}

varargs int start_stuck(string key, int second, string msg, string fmsg, function fp)
{
	if( second < 1 )
		return 0;

	stuckinfo[key] = ({ second, msg, fmsg, fp });
	
	this_object()->startup_heartbeat();
}

varargs void remove_stuck(string key)
{
	if( !key )
		stuckinfo = allocate_mapping(0);
	else
		map_delete(stuckinfo, key);
}

array query_stuck(string key)
{
	return stuckinfo[key];
}

string query_stuck_msg(string key)
{
	if( !key )
	{
		if( is_stucking() )
			return values(stuckinfo)[0][MESSAGE]+"...仍需 "+values(stuckinfo)[0][SECOND]+" 秒\n";
		else
			return 0;
	}
	else
	{
		if( is_stucking(key) )
			return stuckinfo[key][MESSAGE]+"...仍需 "+values(stuckinfo)[0][SECOND]+" 秒\n";
		else
			return 0;
	}
}

private void stuck_decay()
{
	foreach( string key, array info in stuckinfo )
	{
		if( --stuckinfo[key][SECOND] <= 0 )
		{
			if( functionp(info[FUNCTION]) )
				catch( evaluate(info[FUNCTION]) );
			
			if( stringp(info[FMESSAGE]) )
				tell(this_object(), info[FMESSAGE]);

			map_delete(stuckinfo, key);
		}
	}
}
