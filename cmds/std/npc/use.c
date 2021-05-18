/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : use.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : use ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <use.h>

inherit COMMAND;

string help = @HELP
�ϥΪ��~�����O
use '���~'		- �ϥάY�Ӫ��~

HELP;

private void do_command(object me, string arg)
{
	string args;
	object ob;
	string basename;
	int coldtime;

	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڨϥΤ��򪫫~�H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�ϥΤ��򪫫~�H\n");
	}
	
	//���L������U���O
	if( me->is_delaying() )
	{
		if( me->is_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	ob = present(arg, me) || present(arg, environment(me));

	if( !objectp(ob) )
	{
		sscanf(arg, "%s to %s", arg, args);
		ob = present(arg, me) || present(arg, environment(me));
	}

	if( !objectp(ob) )
	{
		if( me->is_npc() )
			return me->do_command("say ����èS�� "+arg+" �o�ت��~�C\n");
		else
			return tell(me, "����èS�� "+arg+" �o�ت��~�C\n");
	}

	if( !function_exists("do_use", ob) )
	{
		if( me->is_npc() )
			return me->do_command("say �ڤ����D�p��ϥ�"+ob->query_idname()+"�C\n");
		else
			return tell(me, pnoun(2, me)+"�����D�p��ϥ�"+ob->query_idname()+"�C\n");
	}

	if( query_temp("decorated", ob) )
		return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"�����C�C\n");

	basename = base_name(ob);

	if( (coldtime = query("coldtime/"+basename, me)) > time() )
		return tell(me, pnoun(2, me)+"�ٻݭn���� "+(coldtime - time())+" ���~��ϥ�"+ob->query_idname()+"�C\n");
	
	if( (coldtime = query("coldtime", ob)) )
		set("coldtime/"+basename, time() + coldtime, me);

	switch( ob->do_use(me, args) )
	{
		case 0:
			delete("coldtime/"+basename, me);
			break;
		case DESTRUCT_ONE:
			destruct(ob, 1);
			break;
		case DESTRUCT_ALL:
			destruct(ob);
			break;
	}
}
