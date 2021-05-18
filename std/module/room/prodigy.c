
/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : prodigy.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-07
 * Note   : �@�ɩ_�[�ʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <condition.h>

inherit ROOM_ACTION_MOD;

void do_pray(object me, string arg)
{
	object env = environment(me);

	if( env->query_city() != query("city", me) )
		return tell(me, pnoun(2, me)+"���O�o�ӫ����������A�L�k�b�o�̬�D�_�ݡC\n");
	
    msg("$ME���b�_�[�����������n��ë�A�@�ѯ��t���O�q��M��X�A���������w�w���X"HIR"��"NOR RED"�B"HIB"��"NOR BLU"�B"HIG"��"NOR GRN"�B"HIY"��"NOR"�|���C�⪺�H�ϡA�N$ME�]�а_��...\n", me, 0, 1);

	me->start_condition(LIFE_MIRACLE);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"naos"	:
	([
		"short"	: HIR"�_�[����"NOR,
		"help"	:
			([
"topics":
@HELP
    �_�[�����C
HELP,

"pray":
@HELP
��D�_�ݪ����{�C
HELP,

			]),
		"action":
			([
				"pray"	: (: do_pray :),
			]),
	]),

]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIR"�@��"NOR RED"�_�["NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,1

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,1000000000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,1000

	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});

