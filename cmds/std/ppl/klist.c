/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : klist.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-06-10
 * Note   : klist ���O
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */
#include <ansi.h>
#include <feature.h>
#include <daemon.h>

inherit COMMAND;

string help = @HELP
�d��������

�ϥΦ����O�i�d�ߦU�إͪ��������`�ơC
HELP;

private void do_command(object me, string arg)
{
	object target = me;
	mapping klist;
	string *msg = allocate(0);
	object *npcs;
	
	if( wizardp(me) && stringp(arg) )
	{
		if( !objectp(target = find_player(arg)) )
			return tell(me, "�ثe�u�W�S�� "+arg+" �o�쪱�a�C\n");
	}
	
	klist = query("combat/defeat", target);
	
	if( sizeof(klist) == 0 )
		return tell(me, (target == me ? pnoun(2, me) : target->query_idname())+"�L����ͪ����������C\n");

	msg += ({ (target == me ? pnoun(2, me) : target->query_idname())+"���ͪ������ƦC��p�U�G" });
	
	msg += ({ NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
	msg += ({ sprintf("%-50s %22s", "�ͪ��W��", "�����`��") });
	msg += ({ NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
	
	npcs = map(keys(klist), (: load_object(replace_string($1,"#","/")) :));

	foreach(object npc in sort_array(npcs, (: COMBAT_D->total_value($1) - COMBAT_D->total_value($2) :)))
		msg += ({ sprintf("%-50s %22s", npc->query_idname(), NUMBER_D->number_symbol(klist[replace_string(base_name(npc), "/", "#")])) });
	
	//foreach(string npc, int count in klist)
	//	msg += ({ sprintf("%-50s %22s", load_object(replace_string(npc,"#","/"))->query_idname(), NUMBER_D->number_symbol(count)) });
		
	msg += ({ NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w"NOR });
	
	me->more(implode(msg, "\n"));
}