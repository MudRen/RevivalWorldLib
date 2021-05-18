/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : home.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-3-27
 * Note   : home ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit COMMAND;

string help = @HELP
�@�@�^�a�����O�A�������سy��v�ؿv�æb���U�i�� sethome ���]�w�ʧ@�C
HELP;

#define HOME_STAMINA		500

private void do_command(object me, string arg)
{
	string file;
	object home;

	if( wizardp(me) )
	{
		if( arg )
			file = wiz_home(arg)+"/workroom";
		else
			file = query("home", me) || (wiz_home(me->query_id(1))+"/workroom");

		//intact_file_name(ref file);
		
		if( !file_exists(file) )
			return tell(me, "�o�ӤH�S���u�@�ǡC\n");
	
		home = load_object(file);
		
		if( environment(me) == home ) 
			return tell(me, pnoun(2, me)+"�w�g�b"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǤF�C\n");
			
		msg("$ME�Ƨ@�@�}"HIG"��"NOR GRN"��"NOR"��"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǭ��F�L�h�C\n",me, 0, 1, ENVMSG);
	
		me->move(home);
	
		msg("$ME�Ƨ@�@�}"HIG"��"NOR GRN"��"NOR"��"+(arg?" "+capitalize(arg)+" ":"�ۤv")+"���u�@�ǭ��F�L�ӡC\n",me, 0, 1, ENVMSG, ({me}));
	}
	else
	{
		if( !me->valid_move() ) 
			return;

		file = query("home", me);
		
		if( !stringp(file) || !file_exists(file) || !objectp(home = load_object(file)) || query("owner", home) != me->query_id(1) || query("function", home) != "livingroom")
		{
			delete("home", me);
			return tell(me, pnoun(2, me)+"�|���]�w"+pnoun(2, me)+"�a����}(�����سy��v)�C\n");
		}

		if( environment(me) == home ) 
			return tell(me, pnoun(2, me)+"�w�g�b�a�̤F�C\n");

		if( BATTLEFIELD_D->in_battle(me) )
			return tell(me, pnoun(2, me)+"���b�Գ����A�Y�T�w�n���}�Գ��Шϥ� battle cancel ���O�C\n");

		if( !me->cost_stamina(HOME_STAMINA) )
			return tell(me, pnoun(2, me)+"�S����������O�^��a�̡C\n");

		msg("$ME�k�ߦ��b�A�ӶO "+HOME_STAMINA+" �I����O��������a�e���u"+query("short", home)+"�v�C\n",me, 0, 1, ENVMSG);
	
		me->move(home);
	
		msg("$ME�k�ߦ��b�A�ӶO "+HOME_STAMINA+" �I����O��������a�^��u"+query("short", home)+"�v�C\n",me, 0, 1, ENVMSG, ({me}));
	}
}