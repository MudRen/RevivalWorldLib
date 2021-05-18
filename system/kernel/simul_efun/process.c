/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : process.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-07-23
 * Note   : included by Simul_Efun Object
 * Update :
 *
 *
 -----------------------------------------
 */

void process_function(array fp)
{
	int second;

	if( !sizeof(fp) ) return;
	
	if( functionp(fp[0]) )
	{
		if( !objectp(function_owner(fp[0])) || evaluate(fp[0]) == -1 )
			return;
	}
	
	if( intp(fp[0]) )
		second = fp[0];

	call_out((: process_function :), second, fp[1..]);
}
