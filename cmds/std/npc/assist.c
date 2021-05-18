/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : assist.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-10
 * Note   : assist ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <npc.h>

inherit COMMAND;

string help = @HELP
���U�L�H�i������C

assist [�Y�H]		- �}�l���U�Y�H����
assist -d		- ����U����

�Y�ѭ��u���U�����A�Ǫ����������ǡG���u -> �U�F�������O���a
�Y�Ѫ��a���U�����A�ĤH���������ǡG�U�F�������O���a -> ���U�������a
HELP;

private void do_command(object me, string arg)
{
	string target_id;
	object target_ob;
	
	if( !arg )
	{
		if( me->is_npc() )
			return me->do_command("say ����Ʊ�ڻ��U�֧����H\n");
		else
			return tell(me, pnoun(2, me)+"�Q�n���U�֧����H\n");
	}

	//���L������U���O
	if( me->is_delaying() )
	{
		if( me->is_npc() )
			return me->do_command("say "+me->query_delay_msg()+"\n");

		tell(me, me->query_delay_msg());
		return me->show_prompt();
	}

	if( arg == "-d" )
	{
		target_ob = query_temp("assisting", me);
		
		if( !objectp(target_ob) )
		{
			delete_temp("assisting", me);

			if( me->is_npc() )
				return me->do_command("say �ڭ쥻�K�S�����U����H�����C\n");
			else
				return tell(me, pnoun(2, me)+"�쥻�K�S�����U����H�����C\n");
		}

		delete_temp("assisting", me);

		msg("$ME����U$YOU�����C\n", me, target_ob, 1);
		
		return;
	}

	if( me->is_module_npc() && query("job/type", me) != SOLDIER )
	{
		me->do_command("say �ڤ��O�x�H�A�L�k�԰�");
		return;
	}

	if( BATTLEFIELD_D->inside_battlefield(me) )
		return tell(me, "�Գ����L�k�ϥλ��U�������O�C\n");
		
	target_id = arg;
	target_ob = present(target_id);

	if( !objectp(target_ob) )
	{
		if( me->is_npc() )
		{
			if( target_id == "me" )
				target_ob = this_player(1);
			else
				return me->do_command("say �o�̨S�� "+arg+" �o�ӪF��C\n");
		}
		else
			return tell(me, "�o�̨S�� "+arg+" �o�ӪF��C\n");
	}

	if( !target_ob->is_living() )
	{
		if( me->is_npc() )
			return me->do_command("say "+target_ob->query_idname()+"���O�ͪ��A�L�k���U�����C\n");
		else
			return tell(me, target_ob->query_idname()+"���O�ͪ��A�L�k���U�����C\n");
	}
		
	if( target_ob == me )
	{
		if( me->is_npc() )
			return me->do_command("say �ڵL�k���U�ڦۤv�i������C\n");
		else
			return tell(me, pnoun(2, me)+"�L�k���U"+pnoun(2, me)+"�ۤv�i������C\n");
	}

	if( (me->is_module_npc() || userp(me)) && !me->is_in_my_group(target_ob) )
	{
		if( me->is_npc() )
			return me->do_command("say �ڤ��b"+target_ob->query_idname()+"������A�L�k��U"+pnoun(3, target_ob)+"�i������C\n");
		else
			return tell(me, pnoun(2, me)+"���b"+target_ob->query_idname()+"������A�L�k��U"+pnoun(3, target_ob)+"�i������C\n");
	}

	if( query_temp("assisting", target_ob) )
	{
		if( me->is_npc() )
			return me->do_command("say "+target_ob->query_idname()+"���b��U��L�H�����A�ڵL�k��U"+pnoun(3, target_ob)+"�C\n");
		else
			return tell(me, target_ob->query_idname()+"���b��U��L�H�����A"+pnoun(2, me)+"�L�k��U"+pnoun(3, target_ob)+"�C\n");
	}
	
	set_temp("assisting", target_ob, me);

	msg("$ME�}�l���U$YOU�i������C\n", me, target_ob, 1);	
}
