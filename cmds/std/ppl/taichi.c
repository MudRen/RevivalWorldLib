/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : taichi.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : �ӷ����O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <skill.h>

inherit COMMAND;

string help = @HELP
   �����ӷ��A�O�ͨ���C����ͥ|�H�A�|�H�ͤK���C

   taichi 100 [��O��] to [��O��]	- �ഫ 100 �I��O�Ȧܥt�@�ӯ�O��
   				          ��O�]�t stamina�Bhealth�Benergy
   ��: taichi 300 energy to stamina	- �ഫ 300 �I�믫����O
   
   �ഫ�Ĳv�H�ޯ൥�Ŵ��ɡC
HELP;

private void do_command(object me, string arg)
{
	int amount;
	string from, to;
	string from_name;
	int level;
	int to_amount;

	if( me->is_delaying() )
		return tell(me, me->query_delay_msg());

	if( !arg || sscanf(arg, "%d %s to %s", amount, from, to) != 3 )
		return tell(me, "�п�J���T�����O�榡(help taichi)�C\n");

 	if( me->is_fatigue_full() )
		return tell(me, pnoun(2, me)+"��b�O�ӯh�ҤF�A�L�k�A�i���ഫ�C\n");

	if( amount <= 0 )
		return tell(me, "�ഫ�ƶq���i�p�� 0�C\n");

	if( COMBAT_D->in_fight(me) && time() - query_temp("taichi_time", me) < 5 )
		return tell(me, "�ٻݭn "+(5 - (time() - query_temp("taichi_time", me)))+" ���~����A�׬I�i���ͤӷ��C\n");

	if( !random(10000) )
		me->add_fatigue(1);
			
	level = me->query_skill_level("taichi");
	
	if( level < 1 )
		return tell(me, pnoun(2, me)+"�����o�p��I�i�o�ӧޯ�C\n");

	switch(from)
	{
		case "stamina":
			if( !me->cost_stamina(amount) )
				return tell(me, pnoun(2, me)+"�S���o��h����O�C\n");
				
			from_name = "��O";
			break;
		case "health":
			if( !me->cost_health(amount) )
				return tell(me, pnoun(2, me)+"�S���o��h���ͩR�C\n");
				
			from_name = "�ͩR";
			break;
		case "energy":
			if( !me->cost_energy(amount) )
				return tell(me, pnoun(2, me)+"�S���o��h���믫�C\n");
				
			from_name = "�믫";
			break;
		default:
			return tell(me, "�п�J���T���ഫ�ӷ������C\n");
	}
	
	to_amount = to_int(amount * level / 100.);

	switch(to)
	{
		case "stamina":
			me->earn_stamina(to_amount);
			msg("$ME�ϥX�u"HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR"�v�A�ഫ "+amount+" �I��"+from_name+"�� "+to_amount+" �I����O�C\n", me, 0, 1);
			break;
		case "health":
			me->earn_health(to_amount);
			msg("$ME�ϥX�u"HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR"�v�A�ഫ "+amount+" �I��"+from_name+"�� "+to_amount+" �I���ͩR�C\n", me, 0, 1);
			break;
		case "energy":
			me->earn_energy(to_amount);
			msg("$ME�ϥX�u"HIW"��"NOR WHT"��"HIW"��"NOR WHT"��"NOR"�v�A�ഫ "+amount+" �I��"+from_name+"�� "+to_amount+" �I���믫�C\n", me, 0, 1);
			break;
		default:
			return tell(me, "�п�J���T���ഫ�ؼк����C\n");
	}
	
	set_temp("taichi_time", time(), me);
}
