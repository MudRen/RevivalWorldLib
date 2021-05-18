/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : hospital.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-10-19
 * Note   : ��|�ʧ@����
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

inherit ROOM_ACTION_MOD;

#define BASE_MONEY	10000
#define EXTRA_MONEY	10

#define CURE_FATIGUE	"fatigue"
#define CURE_FOOD	"food"
#define CURE_DRINK	"drink"

void confirm_cure(object me, object target_ob, string money_unit, int total_money, string type, string arg)
{
	string city;
	
	if( !arg || !arg[0] || lower_case(arg) != "y" )
	{
		tell(me, "�����v���C\n");
		return me->finish_input();
	}

	if( !me->spend_money(money_unit, total_money) )
		return tell(me, pnoun(2, me)+"���W�������� "HIY+money(money_unit, total_money)+NOR"�C\n");	
	
	switch( type )
	{
		case CURE_FATIGUE:
			target_ob->reset_fatigue();
			msg("$ME��O�F "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"��$YOU")+"�v¡�F�������h�һĵh�C\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_FATIGUE, time(), target_ob);
			break;
		case CURE_FOOD:
			target_ob->cost_food(target_ob->query_food_cur());
			msg("$ME��O�F "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"��$YOU")+"�i����z�A�N�@�ﭹ�����ԤF�X�ӡC\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_FOOD, time(), target_ob);
			break;
		case CURE_DRINK:
			target_ob->cost_drink(target_ob->query_drink_cur());
			msg("$ME��O�F "HIY+money(money_unit, total_money)+NOR" "+(target_ob==me?"":"��$YOU")+"�i��ʦR�A�N�@�ﶼ�Ƴ��R�F�X�ӡC\n", me, target_ob==me?0:target_ob, 1);
			set_temp("cure/"CURE_DRINK, time(), target_ob);
			break;	
	}
	
	city = environment(me)->query_city();
	
	if( CITY_D->city_exist(city) )
		CITY_D->change_assets(city, total_money/5);

	me->save();
	me->finish_input();
}

void do_cure(object me, string arg)
{
	string target;
	object target_ob;
	int total_skill_level;	
	mapping skills;
	int total_money;
	string money_unit = environment(me)->query_money_unit();

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�v������H\n");

	target = arg[0..strsrch(arg, " ", -1)-1];
	
	arg = arg[strsrch(arg, " ", -1)+1..];

	if( !stringp(target) || !objectp(target_ob = present(target)) )
		target_ob = me;

	skills = query("skills", target_ob);

	if( mapp(skills) )
	{
		foreach(string skill, mapping data in skills)
			total_skill_level += data["level"];
	}
	else
		total_skill_level = 0;

	switch( arg )
	{
		case CURE_FATIGUE:
		{
			int fatigue = target_ob->query_fatigue();
			
			if( !fatigue )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���ݭn�v���h�ҡC\n");
	
			if( query_temp("cure/"CURE_FATIGUE, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���[���e�~�v���L�A�������� "+(query_temp("cure/"CURE_FATIGUE, me) + 120 - time())+" ��~��A�i��v���C\n");

			total_money = BASE_MONEY + EXTRA_MONEY * fatigue * total_skill_level;
			
			tell(me, pnoun(2, me)+"�O�_�n��O "HIY+money(money_unit, total_money)+NOR" ��"+(target_ob==me?"":"��"+target_ob->query_idname())+"�v���h�ҡH(Y/N)�G");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_FATIGUE :));
				
			break;
		}
		case CURE_FOOD:
		{
			int food = target_ob->query_food_cur();
			
			if( !food )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���ݭn�i����z�C\n");
	
			if( query_temp("cure/"CURE_FOOD, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���[���e�~�v���L�A�������� "+(query_temp("cure/"CURE_FOOD, me) + 120 - time())+" ��~��A�i��v���C\n");

			total_money = BASE_MONEY + EXTRA_MONEY * food * total_skill_level;
			
			tell(me, pnoun(2, me)+"�O�_�n��O "HIY+money(money_unit, total_money)+NOR" ��"+(target_ob==me?"":"��"+target_ob->query_idname())+"�i����z�H(Y/N)�G");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_FOOD :));

			break;
		}
		case CURE_DRINK:
		{
			int drink = target_ob->query_drink_cur();
			
			if( !drink )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���ݭn�i����z�C\n");

			if( query_temp("cure/"CURE_DRINK, target_ob) + 120 > time() )
				return tell(me, (target_ob==me?pnoun(2, me):target_ob->query_idname())+"���[���e�~�v���L�A�������� "+(query_temp("cure/"CURE_DRINK, me) + 120 - time())+" ��~��A�i��v���C\n");

			total_money = BASE_MONEY + EXTRA_MONEY * drink * total_skill_level;
			
			tell(me, pnoun(2, me)+"�O�_�n��O "HIY+money(money_unit, total_money)+NOR" ��"+(target_ob==me?"":"��"+target_ob->query_idname())+"�i��ʦR�H(Y/N)�G");
		
			input_to((: confirm_cure, me, target_ob, money_unit, total_money, CURE_DRINK :));

			break;
		}
		default:
			tell(me, pnoun(2, me)+"�Q�n�v������H\n");
			break;
	}
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"emergency"	:
	([
		"short"	: NOR GRN"��E��"NOR,
		"help"	:
			([
"topics":
@HELP
    ��|�A�i���������a��C
HELP,

"cure":
@HELP
�i��v�������O�A�Ϊk�p�U�G
cure fatigue		- �i�樭�餧�h�һP�e�f�v��
cure food		- �i����z�A�ϭ������C
cure drink		- �i��ʦR�A�϶������C
cure �H�W ����		- ���L�H�i��W�z���v��(�ҡGcure labor fatigue)

�v�������H���⤧�ޯ൥�żƭȥ[�`�өw
HELP,

			]),
		"action":
			([
				"cure"	: (: do_cure :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIW"��"NOR WHT"�|"NOR

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
	,5
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
