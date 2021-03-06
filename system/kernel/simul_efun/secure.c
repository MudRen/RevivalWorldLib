/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : secure.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-04-28
 * Note   : included by Simul_Efun Object
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */
 
varargs void shutdown(int how)
{
	if( !is_command(previous_object()) ) 
		error("使用 shutdown() 權限不足");
	
	if( undefinedp(how) )
		efun::shutdown();
	else
		efun::shutdown(how);
}

int exec(object to, object from)
{
	if( !is_daemon(previous_object()) )
		error("使用 exec() 權限不足");
	
	return efun::exec(to, from);
}
