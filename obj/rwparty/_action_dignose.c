/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _action_dignose.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-05-16
 * Note   : dignose action
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <condition.h>
#include <daemon.h>

void do_dignose(object me, string arg)
{
	object target;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���˩��֪����W�u�L�h�H\n");
	
	if( !objectp(target = present(arg)) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӪF��C\n");
		
	if( !target->is_living() || target->is_module_npc() )
		return tell(me, pnoun(2, me)+"�L�k��o�ǹ�H�u��ˡC\n");
		
	if( target == me )
		return tell(me, pnoun(2, me)+"�L�k��ۤv���ˡC\n");
		
	if( userp(target) && !BATTLEFIELD_D->in_battle(me) )
		return tell(me, pnoun(2, me)+"�u���b�Գ����i�H�缾�a���ˡC\n");

	if( !me->cost_stamina(500) )
		return tell(me, pnoun(2, me)+"�S���O��A����դF�C\n");

	if( query_temp("dignose", me) > time() - 30 )
		return tell(me, pnoun(2, me)+"�C 30 ��~����@����աC\n");
		
	set_temp("dignose", time(), me);
		
	msg("$ME�ΤO�a���F�X�U��աA�u������Ҥ��t�ê��ƪT�@�����ɬ�M�ۥ|���K�詹$YOU�s��u�g�I�I"HIY"���� "NOR YEL"�ѢݢۢТ�"NOR"�I�I\n", me, target, 1);

	target->start_condition(DIGNOSE, 60, 5, me);
	
	if( !random(10) )
	{
		msg("$ME����զb������R�ݤU�����t���A����z�F�I\n", me, 0, 1);
		me->faint();
	}
}