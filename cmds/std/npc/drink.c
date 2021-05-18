/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : drink.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-03-04
 * Note   : drink ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>

inherit COMMAND;

string help = @HELP
���ζ��ƪ����O
drink '���~'		- �N�Y�Ӫ��~�ܨ�{�l��

HELP;

private void do_command(object me, string arg)
{
	int capacity;
	object ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Q�n�ڳܨǤ���H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n�ܨǤ���H\n");
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
		if( me->is_npc() )
			return me->do_command("say ����èS�� "+arg+" �o�ض��ơC\n");
		else
			return tell(me, "����èS�� "+arg+" �o�ض��ơC\n");
	}

	if( !ob->is_drink() )
	{
		if( me->is_npc() )
			return me->do_command("say "+ob->query_idname()+"���O���ơA�L�k���ΡC\n");
		else
			return tell(me, ob->query_idname()+"���O���ơA�L�k���ΡC\n");
	}

	if( query_temp("decorated", ob) )
		return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"�����C�C\n");

	if( query_temp("coldtime/drink", me) > time() )
		return tell(me, pnoun(2, me)+"�ٻݭn���� "+(query_temp("coldtime/drink", me)-time())+" ���~�඼��"+ob->query_idname()+"�C\n");

	capacity = query("capacity", ob);

	if( capacity > 0 && !me->add_drink(capacity) )
	{
		if( me->is_npc() )
			return me->do_command("say �ڳܤ��U"+ob->query_idname()+"�F�I\n");
		else
			return tell(me, pnoun(2, me)+"�ܤ��U"+ob->query_idname()+"�F�I\n");
	}

	ob->do_drink(me);

	if( query("coldtime", ob) )
		set_temp("coldtime/drink", time() + query("coldtime", ob), me);

	destruct(ob, 1);
}