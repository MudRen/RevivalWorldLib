/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : recycle.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �U���^�����ʧ@����
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

#define RECYCLE_MONEY	0.7

void do_recycle(object me, string arg)
{
	int value;
	int money = 0;
	string moneyunit = MONEY_D->query_default_money_unit();
	int amount;
	string option;
	object ob, env = environment(me);

	if( !arg )
		return tell(me, "�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n");

	if( sscanf(arg, "%s %s", option, arg) == 2 )
	{
		if( option == "all" ) ;
		// �p�G option �ä��O�Ʀr���
		else if( !big_number_check(option) )
		{
			arg = option + " " + arg;
			amount = 1;	
		}	
		else if( to_int(option) < 1 ) 
			amount = 1;
		else
			amount = to_int(option);
	}
	else amount = 1;
	
	if( objectp(ob = present(arg, me)) )
	{
		if( ob->no_amount() )
		{
			string idname = ob->query_idname();
			string basename = base_name(ob);
			object *obs = filter_array(all_inventory(me), (: base_name($1) == $(basename) :));
			int size = sizeof(obs);

			if( option == "all" ) 
				amount = size;
			else if( amount > size )
				return tell(me, pnoun(2, me)+"���W�u�� "+size+" "+(query("unit",ob)||"��")+ob->query_idname()+"�C\n");

			size = amount;
			
			value = query("value", ob);

			while(amount--)
			{
				if( !objectp(ob = present(arg, me)) )
				{
					size--;
					continue;
				}
			
				if( ob->is_keeping() )
				{
					tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
					size--;
					continue;
				}
				
				destruct(ob);
			}
			
			money = size * RECYCLE_MONEY * value;
			set("numbers", count(query("numbers", env), "+", size), env);
			msg("$ME�� "+size+" ��"+idname+"��i�귽�^����A��o�^���� "HIY+money(moneyunit, money)+NOR"�A�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n", me, 0, 1);	
		} 
		else
		{
			if( option == "all" )
				amount = ob->query_amount();
			else if( amount > ob->query_amount() ) 
				return tell(me, pnoun(2, me)+"���W�u�� "+ob->query_amount()+" "+(query("unit",ob)||"��")+ob->query_idname()+"�C\n");
				
			if( ob->is_keeping() )
				return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d(keep)���A�C\n");
			
			// �]�׮t�L�j�|�y���R����Q, �Ȯ� mark
			value = query("value", ob);
			money = amount * RECYCLE_MONEY * value;
	
			if( money < 0 )
				money = 0;
			
			me->earn_money(moneyunit, money);
	
			set("numbers", count(query("numbers", env), "+", amount), env);
			msg("$ME��"+QUANTITY_D->obj(ob, amount)+"��i�귽�^����A��o�^���� "HIY+money(moneyunit, money)+NOR"�A�ثe�^���`�ơG"+NUMBER_D->number_symbol(query("numbers", env))+" �ӡC\n", me, 0, 1);
			
			destruct(ob, amount);
		}
		return;
	}

	return tell(me, pnoun(2, me)+"���W�S�� "+arg+" �o�󪫫~�C\n");
}

string look_room(object room)
{
	if( query("function", room) == "recycle" )
		return NOR GRN"�^���`�� "HIG+NUMBER_D->number_symbol(query("numbers", room))+NOR GRN" ��"NOR"\n";
}	

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"recycle"	:
	([
		"short"	: NOR GRN"�^����"NOR,
		"help"	:
			([
"topics":
@HELP
    �^�����ߥi�H�^���Ҧ����󤣨���Ȫ����~�A�P�ɬ��F���y���O�A�C�^���@�󪫫~�i�o 10 �������C
HELP,

"recycle":
@HELP
�^���귽�����O(�i��o��� $RW 70% ���^����)�A�Ϊk�p�U�G
  recycle		- ��ܥثe�w�^���`��
  recycle '���~ID'	- �^���������~
  recycle 3 '���~ID'	- �^�� 3 ��������~
  recycle all '���~ID'	- �^���Ҧ��������~
HELP,

			]),
		"action":
			([
				"recycle"	: (: do_recycle :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIG"�U���^����"NOR

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
	,30
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
