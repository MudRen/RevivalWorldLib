/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : defendtower.c
 * Author : Clode@RevivalWorld
 * Date   : 2008-04-15
 * Note   : ���m��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>

inherit ROOM_ACTION_MOD;

void do_arrow_fire(object me, string arg)
{
	
	
	
}

void do_stone_fire(object me, string arg)
{
	
	
	
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"arrow"	:
	([
		"short"	: HIG"�}�b"NOR GRN"��"NOR,
		"heartbeat":5,	// ��ڮɶ� 1 �����
		"master":1,
		"help"	:
			([
"topics":
@HELP
	�}�b��i�����j�d��i��}�b�g���C
HELP,

"fire":
@HELP
�}�b�g��������O�A�Ϊk�p�U�G
	fire			- �i��j�d��}�b�g��
HELP,
			]),
		"action":
			([
				"fire":	(: do_arrow_fire :),
			]),
	]),
	
	"stone"	:
	([
		"short"	: HIW"��"NOR WHT"�۶�"NOR,
		"heartbeat":5,	// ��ڮɶ� 1 �����
		"master":1,
		"help"	:
			([
"topics":
@HELP
	��۶�i�����p�d��i���ۧ����C
HELP,

"fire":
@HELP
��ۧ���������O�A�Ϊk�p�U�G
	fire			- �i��p�d���ۧ���
HELP,
			]),
		"action":
			([
				"fire":	(: do_stone_fire :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"���m"NOR YEL"��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AREA_REGION

	// �}�i�����O��
	,3000000

	// �ؿv���������ռаO
	,1

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,2
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,3
});
