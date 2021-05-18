/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : church.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-14
 * Note   : �а�
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

void confirm_pray(object me, string type, string arg)
{
	arg = lower_case(arg);
	
	if( arg != "yes" && arg != "y" )
		return tell(me, pnoun(2, me)+"��󴣤��ݩʯ�O�C\n");

	switch(type)
	{
		case "strength":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 50 �I�C\n");
				return me->finish_input();
			}
	
			me->add_str(1);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I���O�q�ݩʡC\n"NOR);
			break;
		case "physique":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 50 �I�C\n");
				return me->finish_input();
			}
	
			me->add_phy(1);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I������ݩʡC\n"NOR);
			break;
		case "intelligence":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 50 �I�C\n");
				return me->finish_input();
			}
	
			me->add_int(1);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I�������ݩʡC\n"NOR);
			break;
		case "agility":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 50 �I�C\n");
				return me->finish_input();
			}
	
			me->add_agi(1);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I���ӱ��ݩʡC\n"NOR);
			break;
		case "charisma":
			if( !me->cost_prestige(50) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 50 �I�C\n");
				return me->finish_input();
			}
	
			me->add_cha(1);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I���y�O�ݩʡC\n"NOR);
			break;
		case "stamina":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 1 �I�C\n");
				return me->finish_input();
			}
	
			addn("abi/stamina/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I����O�̤j�ȡC\n"NOR);
			break;
		case "health":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 1 �I�C\n");
				return me->finish_input();
			}
	
			addn("abi/health/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I���ͩR�̤j�ȡC\n"NOR);
			break;
		case "energy":
			if( !me->cost_prestige(1) )
			{
				tell(me, pnoun(2, me)+"���n�椣�� 1 �I�C\n");
				return me->finish_input();
			}
	
			addn("abi/energy/max", 1, me);
			tell(me, HIY+pnoun(2, me)+"�W�[�F 1 �I���믫�̤j�ȡC\n"NOR);
			break;
	}	
	
	me->save();
	me->finish_input();
}

// ��ë
void do_pray(object me, string arg)
{
	if( !arg )
	{
		if( !me->in_condition(DIE) )
			return tell(me, pnoun(2, me)+"���ݭn�b�ơC\n");

       		msg("$ME���W�����}�l��ë�A"HIW"�b"NOR WHT"��"NOR"���W����©�C\n", me, 0, 1);

	       	me->stop_condition(DIE);
      	
	       	return;
	}
	
	if( !sscanf(arg, "for %s", arg) )
		return tell(me, pnoun(2, me)+"�Q�n��ë����H\n");
	
	arg = lower_case(arg);	

	switch(arg)
	{
		case "strength":
			tell(me, pnoun(2, me)+"���ɤO�q�ݭn 50 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "physique":
			tell(me, pnoun(2, me)+"�������ݭn 50 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "intelligence":
			tell(me, pnoun(2, me)+"���ɴ��ӻݭn 50 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "agility":
			tell(me, pnoun(2, me)+"���ɱӱ��ݭn 50 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "charisma":
			tell(me, pnoun(2, me)+"���ɾy�O�ݭn 50 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "stamina":
			tell(me, pnoun(2, me)+"������O�ݭn 1 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "health":
			tell(me, pnoun(2, me)+"���ɥͩR�ݭn 1 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		case "energy":
			tell(me, pnoun(2, me)+"���ɺ믫�ݭn 1 �I�n��A�O�_�T�w���ɡH(Y/N)");
			input_to( (: confirm_pray, me, arg :) );
			break;
		default:
			tell(me, "�S�� "+arg+" �o�Ӷ��ءA�аѦ� help pray�C\n");
			break;
	}
}


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"�а�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �а�j�U�C
HELP,

"pray":
@HELP
�i�H�V����ë�C
pray			�V����D�b��
pray for strength	�V����ë�O�q����(�C 1 �I�� 50 �n��)
pray for physique	�V����ë��洣��(�C 1 �I�� 50 �n��)
pray for intelligence	�V����ë���Ӵ���(�C 1 �I�� 50 �n��)
pray for agility	�V����ë�ӱ�����(�C 1 �I�� 50 �n��)
pray for charisma	�V����ë�y�O����(�C 1 �I�� 50 �n��)
pray for stamina	�V����ë��O����(�C 1 �I�� 1 �n��)
pray for health		�V����ë�ͩR����(�C 1 �I�� 1 �n��)
pray for energy		�V����ë�믫����(�C 1 �I�� 1 �n��)
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
                "action":
                        ([
                                "pray"		: (: do_pray :),
                        ]),
	
	]),	

]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIW"�а�"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,5000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,10
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
