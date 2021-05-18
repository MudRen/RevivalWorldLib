/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : airport.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : ��ھ���
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
#include <condition.h>
#include <delay.h>

inherit ROOM_ACTION_MOD;


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"airstrip"	:
	([
		"short"	: HIC"�����]�D"NOR,
		"help"	:
			([
"topics":
@HELP
    �ȫȷf�����Ծ��ǡC
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"master": 0,
		"action":
			([

			]),
	
	]),	
	"lobby"	:
	([
		"short"	: HIC"����"NOR CYN"�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �ʶR�������j�U�C
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"master": 0,
		"action":
			([

			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"���"NOR CYN"����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,12

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,180000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,100
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,3
});
