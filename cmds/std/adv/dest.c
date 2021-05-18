/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : dest.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-7-5
 * Note   : dest ���O
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
	�з� dest ���O�C
HELP;

private void command(object me, string arg)
{
	int dest_all, amount, total_amount;
	string str, ob_idname, location, option;
	object ob, env;
	
	if( !is_command() ) return;

	if( !arg ) return tell(me, "���O�榡�Gdest <����W||�ɮצW>�C\n");
	
	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		// �p�G option �ä��O�Ʀr���
		if( to_float(option) == 0. )
		{	
			if( lower_case(option) == "all" )
				dest_all = 1;
			else
			{
				arg = option + " " + arg;
				amount = 1;
			}	
		}	
		else if( to_int(option) < 1 )
			amount = 1;
		else
			amount = to_int(option);
	}
	else amount = 1;
	
	ob = present(arg, me) || present(arg, environment(me));

	if( !objectp(ob) )
	{
		str = resolve_path(me, arg);
		ob = find_object(str) || find_object(str+".c") || find_object(str+".o");
	}

	if( !objectp(ob) )
		return tell(me, "�S�� "+arg+" �o�Ӫ�����ɮסC\n");
	
	ob_idname = ob->query_idname()||file_name(ob);
	
	if( userp(ob) )
		return tell(me, pnoun(2, me)+"�L�k�R���@�ӨϥΪ̪���C\n");
	
	total_amount = ob->query_amount();
	
	env = environment(ob);
	
	if( env == me )
		location = "���W";
	else if( env == environment(me) )
		location = "�a�W";
	else
		location = "�O���餤";
	
	if( dest_all || total_amount <= amount )
	{	
		if( destruct(ob) )
			return msg("$ME�ϥX"HIW"��"NOR WHT"��"HIW"�x"NOR"�N"+location+"�������u"+ob_idname+"�v�����F�H���C\n", me, 0, 1);
			
		return tell(me, "�R������ "+arg+" ���ѡC\n");
	}
	else
	{
		msg("$ME�ϥX"HIW"��"NOR WHT"��"HIW"�x"NOR"�N"+location+"�� "+NUMBER_D->number_symbol(amount)+" "+(query("unit",ob)||"��")+"�u"+ob_idname+"�v�����F�H���C\n", me, 0, 1);

		ob->set_amount(total_amount - amount);
		return;
	}
}