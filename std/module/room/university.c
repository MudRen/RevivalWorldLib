/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : university.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-31
 * Note   : �j��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>
inherit ROOM_ACTION_MOD;

#include <ansi.h>
#include <map.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <skill.h>
#include <condition.h>

#define CLASS_UNIT	10000
#define MSGTITLE	HIM"�W��-"NOR


void do_list(object me, string arg)
{
	string skill_type;
	int type;
	string list="";
	object skillob, env = environment(me);
	string *skills = SKILL_D->query_sorted_skills();
	string course = query("course", env);
	
	list += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	if( course )
		list += "�ثe�ҵ{�G"HIY+(SKILL(course))->query_idname()+"\n"NOR;
	else
		list += "�ثe�S���}�]����ҵ{\n";

	list += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	list += "�����ҵ{���                 �ǲ����� �W�� ���a ���u �g�� ��� �ҵ{²��\n";
	
	foreach(string skill in skills)
	{
		if( !SKILL_D->skill_exists(skill) )
		{
			skill_type = skill;		
			continue;
		}
		
		skillob = load_object(SKILL(skill));
		
		if( !objectp(skillob) ) continue;
	
		if( skillob->self_learning() ) continue;

		if( skill_type )
		{
			list += "- "+skill_type+" -\n";
			skill_type = 0;
		}
		type = query("type", skillob);

		list += sprintf("%-33s"WHT"%4.1f"HIY" %4d "NOR" %2s   %2s  %2s %2s %s"NOR"\n"NOR
			    ,skillob->query_idname()
			    ,1/(skillob->exp_multiple(me))
			    ,skillob->max_level(me)
			    ,skillob->allowable_learn() & PLAYER_SKILL ? ( skillob->allowable_learn(me) ? HIW : HIR)+"��"NOR : WHT"��"NOR
			    ,skillob->allowable_learn() & NPC_SKILL ? HIW"��"NOR : WHT"��"NOR
			    ,type==1?HIG"���|"NOR:type==2?HIR"�԰�"NOR:type==3?HIW"��X"NOR:HIY"���~"NOR
			    ,CITY_D->query_age_name(query("age", skillob), 1)
			    ,skillob->query_skill_note()||"�L����");
	}
	list += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	list += "                                      "HIW"��"NOR"�G�i�ǲߡB"HIR"��"NOR"�G�i�ǲߦ��{�ץ���B"WHT"��"NOR"�G�L�k�ǲ�\n\n"NOR;
	
	me->more(list);
}



void do_setup(object me, string arg)
{
	object env = environment(me);
	string course = query("course", env);
	string city = env->query_city();
	object skillob;
	int age;

	if( query("owner", env)[11..] != query("enterprise", me) )
		return tell(me, pnoun(2, me)+"�ä��O�����~�������C\n");

	if( !arg )
		return tell(me, "�п�J���T�����O�榡�C\n");

	if( sscanf(arg, "course %s", course) == 1 )
	{
		if( course == "-d" )
		{
			msg("$ME�����o���ЫǪ��ҵ{�]�w�C\n", me, 0, 1);
			delete("course", env);
			env->save();
			return;
		}
		
		if( !SKILL_D->skill_exists(course) || !objectp(skillob = load_object(SKILL(course))) || skillob->self_learning() )
			return tell(me, "�L�k�|�� "+course+" �o�ؽҵ{�C\n");
		
		age = query("age", skillob);

		if( age > CITY_D->query_city_info(city, "age") )
			return tell(me, "�������N����������ɦܡu"+CITY_D->query_age_name(age)+"�v�~���|�즹�ҵ{�C\n");

		msg("$ME�N�o���ЫǪ��ҵ{�]�w���u"+(SKILL(course))->query_idname()+"�v�C\n", me, 0, 1);
		set("course", course, env);
		env->save();
	}
	else
		return tell(me, "�п�J���T�����O�榡�C\n");
}


void confirm_learn(object me, string skill, int exp, string exp_type, string exp_type_name, string input)
{
	int bonus;
	string msg = "";

	if( input && (lower_case(input) == "y" || lower_case(input) == "yes") )
	{
		switch(random(50))
		{
			case 0:
			{
				bonus = exp/100;
				msg = MSGTITLE"�Ѯv���ۯ����b�ªO�W�V�g�۽������ƾǤ���...�C\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 1% ��"+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				
				break;
			}
			case 1:
			{
				bonus = exp/50;
				msg = MSGTITLE"�Ѯv���}��M�q���O�W��Ŭ����a��F�@�U�A�o���ˤ���Ƥ]�S�o��...�C\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 2% ��"+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				
				break;
			}
			case 2:
			{
				msg = MSGTITLE"$ME�������F�Ӥp�ȹΩ��Ѯv�I�ᰪ�t�g�h�A�u���Ѯv���⻴������@�ۡA��ŧ����L��...�C\n";
				break;
			}
			case 3:
			{
				bonus = exp/33;
				msg = MSGTITLE"�Ѯv�N�N�a���D�G�u"+me->query_idname()+pnoun(2, me)+"�Ӧ^���o�Ӱ��D�C�v\n";
				msg += MSGTITLE"$ME�y��@�ܰ��W���D�G�u��...�Ѯv�藍�_�A�ڨ��餣�ΪA�Q�R...�C�v\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 3% "+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				
				break;
			}
			case 4:
			{
				msg = MSGTITLE"$ME�Φ��b��W�A�ѤW�y���F�f���G�u�I...�I...�I...�v�C\n";
				break;
			}
			case 5:
			{
				msg = MSGTITLE"$ME���b�Ĥ@�ƪ��y��W���ɦa�յ۰��ݤk�Ѯv���ȩ��C\n";
				break;
			}
			case 6:
			{
				msg = MSGTITLE"�Ыǥ~���ɦa�ǨӡG�u�N�׺�...�n�Y���N�׺���o...�v�C\n";
				break;
			}
			case 7:
			{
				msg = MSGTITLE"�ЫǸ̶ǨӰ}�}���死�A�u��$ME�C���Y�����D�b�F��...�C\n";
				break;
			}
			case 8:
			{
				msg = MSGTITLE"$ME�Ⲵè�b�a�H��ۼg���O�A�Q��$ME�ۤv�]�ݤ������O�g�F�Ǥ���C\n";
				break;
			}
			case 9:
			{
				bonus = exp/33;
				msg = MSGTITLE"�Ѯv�G�u"+me->query_idname()+"�ڰ�"+pnoun(2, me)+"�A1+1 ����h�֡H�v\n";
				msg += MSGTITLE"$ME��i���y��y�ɻ��P�_�ӡG�u�o�D�ڲש�|�F�A���׬O 11 �C\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 3% "+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				break;
			}
			case 10:
			{
				bonus = exp/20;
				msg = MSGTITLE"$ME���h�a���G�u�q�A�Ѯv�A�o�̱п��F�աA�쩳�������ڤ��n�ñСC�v\n";
				msg += MSGTITLE"�Ѯv�G�u"+me->query_idname()+pnoun(2, me)+"���ηQ���~�F...�v\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 5% "+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				break;
			}
			case 11:
			{
				bonus = exp/10;
				msg = MSGTITLE"$ME�]���a���G�u�Ѯv�p�n���A�n���]�n�nť��C�v\n";
				msg += MSGTITLE"�k�Ѯv�`�ۦa���G�u"+me->query_idname()+"�P�Ǥd�U�O�o�򻡡A���L"+pnoun(2, me)+"�o�즨�Z�N�|�o�캡���C�v\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 10% "+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				break;
			}
			case 12:
			{
				bonus = exp/33;
				msg = MSGTITLE"�Ѯv�G�u��"+me->query_idname()+"�P�ǥ�'�ֳQ'�y�@�ӥy�l�C�v\n";
				msg += MSGTITLE"$ME�G�u�e������k�P�Ǫ��åʹֳQ�ڰ����F�C�v\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 3% ��"+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				break;
			}
			case 13:
			{
				msg = MSGTITLE"$ME��M���ЫǪ��f�ĥh�C\n";
				msg += MSGTITLE"�Ѯv�G�u"+me->query_idname()+pnoun(2, me)+"�W�ҤW��@�b�n�h���I���گ���I�v\n";
				msg += MSGTITLE"$ME�G�u���H���b���ڰө��[�l�W���F��I�I�v\n";
				msg += MSGTITLE"�Ѯv�G�u...�v\n";
				break;
			}
			case 14:
			{
				msg = MSGTITLE"�Ѯv�G�u��"+me->query_idname()+"�P�ǥ�'Mud'�y�@�ӥy�l�C�v\n";
				msg += MSGTITLE"$ME�G�u�������n�n�ں�ı�C�v\n";
				break;
			}
			case 15:
			{
				bonus = exp/50;
				msg = MSGTITLE"�Ѯv���_�Q���ꪺ�I�Wï�|�D�G�u�o�O�ֶðŪ��I�H�v\n";
				msg += MSGTITLE"$ME���۰ŤM���۹j�����}���P�ǡG�u�L�C�v\n";
				msg += MSGTITLE"�j�����}���P���٥��ӱo�Τ����w�g�Q�Ѯv������šC\n";
				msg += MSGTITLE HIC"$ME��֯ӶO 2% ��"+exp_type_name+"�g���("+bonus+")�I�I�I\n"NOR;
				break;
			}
		
		}

		if( exp_type == "combat" )
		{
			if( !me->cost_combat_exp(exp-bonus) )
			{
				tell(me, pnoun(2, me)+"�S��������"+exp_type_name+"�g��Ȩӧ@�o���ǲߡC\n");
				return me->finish_input();
			}
		}
		else if( exp_type == "social" )
		{
			if( !me->cost_social_exp(exp-bonus) )
			{
				tell(me, pnoun(2, me)+"�S��������"+exp_type_name+"�g��Ȩӧ@�o���ǲߡC\n");
				return me->finish_input();
			}
		}
		else
		{
			tell(me, "�п�J���T�����O�榡�C\n");	
			return me->finish_input();
		}

		me->add_skill_exp(skill, exp);

		msg(msg+"$ME�ӶO "HIG+(exp-bonus)+NOR" �I"+exp_type_name+"�g��ȨӾǲߡu"HIY+(SKILL(skill))->query_idname()+NOR"�v�C\n", me, 0, 1);
		me->finish_input();
	}
	else
	{
		tell(me, pnoun(2, me)+"�M�w���ǲ߳o���ޯ�C\n");
		me->finish_input();
	}
}

void do_learn(object me, string arg)
{
	mixed exp;
	string exp_type;
	string exp_type_name;
	string skill;
	object skillob;
	object env = environment(me);

	skill = query("course", env);

	if( !skill )
		return tell(me, "�o���ЫǨèS���}�]����ҵ{�C\n");
	
	if( !SKILL_D->skill_exists(skill) || (SKILL(skill))->self_learning() )
		return tell(me, "�èS�� "+skill+" �o�ؽҵ{�C\n");

	skillob = load_object(SKILL(skill));

	if( !arg || sscanf(arg, "%s by %s", exp, exp_type) != 2  )
		return tell(me, "�п�J���T�����O�榡(help learn)�C\n");

	if( exp_type != "combat" && exp_type != "social" )
		return tell(me, "�g��Ⱥ����п�ܪ��|�g���(social)�ξ԰��g���(combat)�C\n");

	if( exp_type == "combat" )
	{
		if( !(query("type", skillob) & COMBAT_SKILL) )
			return tell(me, "�o���ޯ�L�k�ϥξ԰��g��ȾǲߡC\n");
		
		exp_type_name = "�԰�";
	}
	else if( exp_type == "social" )
	{
		if( !(query("type", skillob) & SOCIAL_SKILL) )
			return tell(me, "�o���ޯ�L�k�ϥΪ��|�g��ȾǲߡC\n");

		exp_type_name = "���|";
	}

	if( exp == "full" )
	{
		exp = to_int((skillob->level_exp(skillob->max_level(me)) - me->query_skill_exp(skill))/skillob->exp_multiple(me));

		if( exp < 1 )
			return tell(me, pnoun(2, me)+"���u"HIY+skillob->query_idname()+NOR"�v�ޯ൥�Ťw�g��F�̤j�ȡC\n");
	}
	
	exp = to_int(exp);
	
	if( exp < 1 || exp > 100000000 )
		return tell(me, "�ӶO��"+exp_type_name+"�g��ȥ������� 1 �� 100,000,000 ����("+exp+")�C\n");

	if( !skillob->allowable_learn(me) )
		return tell(me, skillob->query_idname()+"��"+pnoun(2, me)+"�Ө��ӹL�}�`�C\n");

	if( me->query_skill_level(skill) >= skillob->max_level(me) )
		return tell(me, pnoun(2, me)+"��"+skillob->query_idname()+"�w�g�F��̰����ŤF�C\n");
	
	tell(me, pnoun(2, me)+"�ǳƯӶO "HIG+NUMBER_D->number_symbol(exp)+NOR" �I"+exp_type_name+"�g��ȨӾǲߡu"HIY+skillob->query_idname()+NOR"�v�C\n�O�_�T�w�n�ǲ�(Yes/No)�H\n");	

	input_to( (: confirm_learn, me, skill, exp, exp_type, exp_type_name :) );
}



void leave(object room, object me)
{
	me->stop_condition(KNOWLEDGESANCTUARY);
}

void init(object room, object me)
{
	if( query("function", room) == "classroom" )
	{
		if( !me->in_condition(KNOWLEDGESANCTUARY) )
			me->start_condition(KNOWLEDGESANCTUARY);
	}
}

string look_room(object room)
{
	string course = query("course", room);
	
	if( query("function", room) == "classroom" )
	{
		if( !course )
			return "�ثe�S���}�]����ҵ{�C\n"NOR;
		else if( !SKILL_D->skill_exists(course) )
		{
			delete("course", room);
			return "�ثe�S���}�]����ҵ{�C\n"NOR;
		}
		else
			return "�Ѯv���b���x�W���ѵ�"HIY+(SKILL(course))->query_idname()+NOR"���ҵ{�C\n"NOR;
	}
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"classroom"	:
	([
		"short"	: NOR HIW"�Ы�"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    �o�̬O�j�Ǥ����@���ЫǡC
HELP,

"list":
@HELP
�d�߽ҵ{���O
  list			- �C�X�Ҧ����ҵ{��T
HELP,

"setup":
@HELP
�Ыǽҵ{�����]�w���O
  setup course water	- �]�w���ЫǱM���б²M���Ķ�(Water)���ҵ{
  setup course -d	- �����ҵ{�]�w
HELP,

"learn":
@HELP
�ǲ߫��O
  learn full by '�g��Ⱥ���'	- �۰ʭp��ǲߨ�̰����ũһݪ��g���
  learn 2000 by '�g��Ⱥ���'	- �ӶO 2,000 �I���g��ȨӾǲ߳o���ЫǩҶ}�]���ҵ{
  
  �g��Ⱥ�������ءG
  social			- ���|�g���
  combat			- �԰��g���
 
HELP,
			]),

		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
			
				"list" 		: (: do_list :),
				"learn"		: (: do_learn :),
				"setup"		: (: do_setup :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIM"�j��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,12

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,5

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,ENTERPRISE

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION

	// �}�i�����O��
	,20000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,6
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
