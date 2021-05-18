/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : bank.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-03
 * Note   : �Ȧ�ʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <bank.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <roommodule.h>
#include <citydata.h>

inherit ROOM_ACTION_MOD;

#define INTEREST_RATE	1
#define MAX_INTEREST	100000

// �p��Q��
void count_rate(object env, string id)
{
	object player;
	mapping savings;
	int newmoney, interest;
	
	if( !user_exists(id) )
	{
		delete("savings/"+id, env);
		return;
	}

	savings = query("savings/"+id+"/money", env);

	if( !savings || !sizeof(savings) ) return;

	player = find_player(id);

	foreach( string unit, mixed money in savings )
	{
		money = to_int(money);
		
		// �Q��
		interest = money * INTEREST_RATE / 1000000;

		if( interest > MAX_INTEREST )
			interest = MAX_INTEREST;

		newmoney = money + interest;

		if( objectp(player) )
			tell(player, env->query_room_name()+"��鵲��A�Q���@ "+HIY+money(unit, interest)+NOR"�C\n");
		
		set("savings/"+id+"/money/"+unit, newmoney, env);
	}
}

// �m�T
void do_rob(object me, string arg)
{
	msg("$ME�Ķi�d�x�j�ۡG"HIR"�o�O�m�T�I�I�I�I�I\n"NOR, me, 0, 1);
	msg("�u�����e�b�w�w���d�x�p�j���v��M�ҽk�_�ӡA�b$ME�٨S�ӱo�Τ������e�A�o�w�g�Ө�$ME������ý�F$ME�@�۰j�۽�I�I\n", me, 0, 1);
	me->faint();
}


// �s��
void do_deposit(object me, string arg)
{
	int type;
	string unit;
	mixed money, originmoney;
	string id = me->query_id(1);
	object env = environment(me);
	object master = env->query_master();
	mapping moneydata = MONEY_D->query_moneydata(id);

	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "$%s %s", unit, money) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n�s�i�h�ֿ��H(�ҡG$"+env->query_money_unit()+" 100)\n");

	unit = upper_case(unit);
	money = to_int(big_number_check(money));

	if( !MONEY_D->money_unit_exist(unit) )
	{
		tell(me, "�èS�� "+unit+" �o�سf���C\n");
		return;
	}

	if( !money )
		return tell(me, "�п�J���T���Ʀr�C\n");

	if( money <= 0 )
		return tell(me, "���B�����n�j��s�C\n");

	if( to_int(moneydata["cash"][unit]) < money )
		return tell(me, pnoun(2, me)+"���W�S������h�� $"+unit+" ���I\n");

	originmoney = to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master));
	set("savings/"+me->query_id(1)+"/money/"+unit, money + originmoney, master);

	me->spend_money(unit, money);

	msg("$ME�N "HIY+money(unit, money)+NOR" �s�i�F"+query("short", master)+"�C\n", me, 0 ,1);

	type = moneydata["bank"][base_name(master)];
	
	if( !(type & GENERAL_DEPOSIT) )
		MONEY_D->set_bank(id, "set", base_name(master), type | GENERAL_DEPOSIT);

	master->save();
}

// ����
void do_withdraw(object me, string arg)
{
	string unit;
	mixed money, originmoney;
	string id = me->query_id(1);
	object env = environment(me);
	object master = env->query_master();

	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "$%s %s", unit, money) != 2 )
		return tell(me, pnoun(2, me)+"�Q�n����h�ֿ��H\n");

	unit = upper_case(unit);
	money = to_int(big_number_check(money));

	if( !money )
		return tell(me, "�п�J���T���Ʀr�C\n");

	if( money <= 0 )
		return tell(me, "���B�����n�j��s�C\n");

	if( to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master)) < money )
		return tell(me, pnoun(2, me)+"�b�o���Ȧ檺 $"+unit+" �s�ڨèS������h�C\n");

	originmoney = to_int(query("savings/"+me->query_id(1)+"/money/"+unit, master));

	if( originmoney > money )
		set("savings/"+me->query_id(1)+"/money/"+unit, originmoney - money, master);
	else
	{
		delete("savings/"+me->query_id(1)+"/money/"+unit, master);

		if( !sizeof(query("savings/"+me->query_id(1)+"/money", master)) )
		{
			delete("savings/"+me->query_id(1), master);
			MONEY_D->set_bank(id, "delete", base_name(master));
		}
	}

	me->earn_money(unit, money);

	msg("$ME�V"+query("short", master)+"����F "HIY+money(unit, money)+NOR"�C\n", me, 0 ,1);
	
	master->save();
}

void do_account(object me, string arg)
{
	string msg = "";
	object env = environment(me);
	object master = env->query_master();

	mapping savings = query("savings/"+me->query_id(1)+"/money", master);

	if( !savings || !sizeof(savings) )
		return tell(me, pnoun(2, me)+"�b�o���Ȧ�èS�����󪺦s�ڡC\n");

	msg += pnoun(2, me)+"�b"+query("short", master)+"�@���H�U�s�ڡG\n";
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	foreach( string unit, string money in savings )
	msg += " "+HIC+money(unit, money)+NOR"\n";

	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	me->more(msg);
}

void do_information(object me, string arg)
{	
	string msg="";	
	object env = environment(me);
	object master = env->query_master();

	msg += "\n���s�ڽƧQ�Q�v���G"+INTEREST_RATE+" / 100000\n";

	msg += "�s�ڤH��ơG\n";

	if( !mapp(query("savings", master)) )
		msg += " �L����s�ڸ��\n";
	else
		foreach( string id, mapping savings in query("savings", master) )
	{
		msg += " "+capitalize(id)+" :\n";

		foreach( string moneyunit, string money in savings["money"] )
			msg += "      "+money(moneyunit, money)+"\n";
	}

	me->more(msg);
}

void do_autotransfer(object me, string arg)
{
	int type;
	string id = me->query_id(1);
	object master = environment(me)->query_master();
	string bankfile = base_name(master);
	mapping moneydata = MONEY_D->query_moneydata(id);
	
	if( !userp(me) ) return;

	type = moneydata["bank"][bankfile];
	
	if( !type )
		return tell(me, pnoun(2, me)+"�b�o���Ȧ�èS������s�ڡA�L�k�]�w�۰���b�C\n");
		
	if( type & AUTOTRANSFER )
	{
		MONEY_D->set_bank(id, "set", bankfile, type ^ AUTOTRANSFER);
		return msg("$ME�����F�b�o���Ȧ�ҳ]�w���۰���b�\��C\n", me, 0, 1);
	}
	else
	{
		MONEY_D->set_bank(id, "set", bankfile, type | AUTOTRANSFER);
		msg("$ME�b���ӽФF�Ȧ檺�۰���b�\��A�C����b�N�u�� "+query("transfercost", master)+" ��������O�ΡC\n", me, 0, 1);
	}
	
	master->save();
}

void do_convert(object me, string arg)
{
	int ratio;
	string msg;
	string default_money_unit = MONEY_D->query_default_money_unit();
	string src_unit, src_city, dst_unit, dst_city;
	mixed src_money, dst_money;

	if( !userp(me) ) return;

	//return tell(me, "�ר�ק襤�A����I������~���C\n");

	if( !arg || sscanf(arg, "$%s %s %*(to|TO) $%s", src_unit, src_money, dst_unit) != 4 )
		return tell(me, pnoun(2, me)+"�n�N���سf���I�������سf���H(�ҡGconvert $CT 100 to $DL)�C\n");

	src_unit = upper_case(src_unit);
	dst_unit = upper_case(dst_unit);
	
	src_city = MONEY_D->money_unit_to_city(src_unit);
	dst_city = MONEY_D->money_unit_to_city(dst_unit);

	if( src_unit == dst_unit )
		return tell(me, "�L�k�I�����ۦP�f���C\n");

	if( !wizardp(me) && dst_unit == "WZ" )
		return tell(me, pnoun(2, me)+"����I���o�سf���C\n");

	if( src_unit != default_money_unit && !CITY_D->city_exist(src_city) )
		return tell(me, "�ϥ� $"+src_unit+" �f���������w�g�����A���سf���w���A�㦳���ȡC\n");
	
	if( dst_unit != default_money_unit && !CITY_D->city_exist(dst_city) )
		return tell(me, "�ϥ� $"+dst_unit+" �f���������w�g�����A���سf���w���A�㦳���ȡC\n");
	
	if( !(src_money = to_int(big_number_check(src_money))) || src_money < 1 )
		return tell(me, "�п�J���T���f���I���ƶq�C\n");
	
	if( src_money < 10000 )
		return tell(me, "�C���I���ܤ֭n 10,000 �H�W���B�C\n");

	if( !MONEY_D->money_unit_exist(src_unit) )
		return tell(me, "�o�ӥ@�ɤW�èS�� "+src_unit+" �o�سf���C\n");

	if( !MONEY_D->money_unit_exist(dst_unit) )
		return tell(me, "�o�ӥ@�ɤW�èS�� "+dst_unit+" �o�سf���C\n");
	
	dst_money = to_int(EXCHANGE_D->convert(src_money, src_unit, dst_unit));

	if( !me->spend_money(src_unit, src_money) )
		return tell(me, pnoun(2, me)+"���W�èS������h�� "+src_unit+" �{���C\n");

	ratio = 50 - to_int(me->query_skill_level("exchange")/5);
	dst_money -= (dst_money * ratio) / 1000;

	msg = "\n�ثe��ڶײv��� "HIC+src_unit+NOR":"HIC+dst_unit+NOR" �� "HIG+EXCHANGE_D->query_exchange_data(src_unit)+NOR":"HIG+EXCHANGE_D->query_exchange_data(dst_unit)+NOR"�C\n";
	msg += "���� "+ratio+"/1000 ����O��A"+pnoun(2, me)+"�I�� "HIY"$"+src_unit+" "+NUMBER_D->number_symbol(src_money)+NOR" �o�� "HIY"$"+dst_unit+" "+NUMBER_D->number_symbol(dst_money)+NOR"�C\n\n";

	log_file("command/convert", me->query_idname()+"�I�� $"+src_unit+" "+NUMBER_D->number_symbol(src_money)+" �o�� $"+dst_unit+" "+NUMBER_D->number_symbol(dst_money));
	me->earn_money(dst_unit, dst_money);
	tell(me, msg);
}

void do_list(object me, string arg)
{
	string msg, city;
	float ex;
	mapping exchange = EXCHANGE_D->query_exchange_data();
	string *sort_exchange;

	msg = HIC"�U�j����"NOR CYN"�f���ײv��Ȧp�U�G\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += sprintf("%-24s "HIG"%-5s"NOR HIW"%6.10f\n"NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"NOR, HIW"���ͪ��@�ɼзǳf��"NOR, MONEY_D->query_default_money_unit(), EXCHANGE_D->query_exchange_data(MONEY_D->query_default_money_unit()));
	msg += NOR HIY"����                   "NOR GRN"����               "NOR WHT"�ײv      "NOR YEL"�����`�c�a��\n"NOR;
	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	sort_exchange = sort_array(keys(exchange), (: $(exchange)[$1] < $(exchange)[$2] ? 1 : -1 :));
		
	foreach( string unit in sort_exchange )
	{
		ex = exchange[unit];

		city = MONEY_D->money_unit_to_city(unit);
		
		if( city == "wizard" ) continue;

		if( CITY_D->city_exist(city) )
			msg += sprintf("%-24s "HIG"%-5s    "NOR HIW"%10.10f   "HIY"%15s\n"NOR, CITY_D->query_city_idname(city), unit, ex, NUMBER_D->number_symbol(CITY_D->query_city_info(city, "totalflourish")));
	}

	msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	me->more(msg);
}

void do_transferdata(object me, string arg)
{
	int num, x, y;
	object env = environment(me);
	string city = env->query_city();
	string file;
	string old_bank_file = base_name(env);
	object target;
	mapping savings = query("savings", env);
	mapping moneydata;

	if( !CITY_D->is_mayor(city, me) )
		return tell(me, pnoun(2, me)+"���O�����������A�L�k�ಾ�s�ɸ�ơC\n");

	if( !sizeof(savings) )
		return tell(me, "�o�̨S���s�ڸ�ơC\n");
		
	if( !arg || sscanf(arg, "%d %d,%d", num, x, y) != 3 )
		return tell(me, "�п�J���T���Ȧ�y�Ю榡(�ҡG2 24,59)�C\n");
	
	file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "bank");
	
	if( !objectp(target = load_module(file)) || query("function", target) != "lobby"  )
		return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�ä��O�Ȧ�j�U�C\n");
		
	if( target == env )
		return tell(me, "�L�k�ಾ�s�ڸ�ƨ�@��m�C\n");

	foreach(string id, mapping data in savings)
	{
		moneydata = MONEY_D->query_moneydata(id);

		if( mapp(data["money"]) )
		foreach(string unit, string money in data["money"])
		{
			set("savings/"+id+"/money/"+unit, count(money, "+", query("savings/"+id+"/money/"+unit, target)), target);
		}
		
		MONEY_D->set_bank(id, "set", file, moneydata["bank"][old_bank_file]);
		MONEY_D->set_bank(id, "delete", old_bank_file);
	}
	
	delete("savings", env);
	env->save();
	
	target->save();
	
	msg("$ME�N�Ҧ��s�ڸ���ಾ��"+target->query_room_name()+"�C\n", me, 0, 1);
	
	MONEY_D->save();
}


void heart_beat(object env)
{
	int timetick = addn("timetick", 1, env);

	// �]�� heart_beat ����, �� time_precision �Ӯե�
	if( !(timetick % 3600) )
	{
		mapping savings = query("savings", env);
		
		// �u���s��D���i�H�o��Q��
		if( env->query_master() != env ) return;

		if( sizeof(savings) )
		foreach( string id, mapping data in savings )
			count_rate(env, id);

		env->save();
	}
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
    	"lobby"	:
    	([
		"short"	: HIW"�Ȧ�j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ȧ�j�U�O���a�̦s��������a��C
HELP,

"deposit":
@HELP
�N�{���s�J�Ȧ檺���O�A�Ϊk�p�U�G
  deposit $CT 100	- �N�{�� $CT 100 �s�J�Ȧ�b����
HELP,

"withdraw":
@HELP
�N�s�ڴ��⬰�{�������O�A�Ϊk�p�U�G
  withdraw $CT 100	- ����X $CT 100 ���s��
HELP,

"account":
@HELP
�d�ߥثe���s�ڪ��p�A�Ϊk�p�U�G
  account		- ��ܥثe���s�ڪ��p
HELP,

"info":
@HELP
�d�ߥثe�Ȧ�Ҧ��s�ڸ�ơA�Ϊk�p�U�G[�޲z���O]
  info			- ��ܩҦ��s�ڸ��
HELP,

"transferdata":
@HELP
�ಾ�ثe�Ȧ�Ҧ��s�ڸ�ơA�Ϊk�p�U�G[�޲z���O]
  transferdata 2 50,50	- �N�o�̩Ҧ����s�ڸ���ಾ�ܲĤG������ 50,50 �Ȧ�j�U��(�|�۰ʲ֥[����)
HELP,

			]),
		"heartbeat":1,	// ��ڮɶ� 1 �����
		"master":1,
		"action":
		([
	    		"deposit"	: (: do_deposit :),
	    		"withdraw"	: (: do_withdraw :),
	    		"account"	: (: do_account :),
	    		"information"	: (: do_information :),
	    		"autotransfer"	: (: do_autotransfer :),
	    		"rob"		: (: do_rob :),
	    		"transferdata"	: (: do_transferdata :),
		]),
    	]),
    	"exchange":
    	([
		"short"	: HIW"�קI��"NOR,
		"help"	:
			([
"topics":
@HELP
    �o�O�@���קI�ǡC
HELP,

"convert":
@HELP
�I���f�������O�A�Ϊk�p�U�G
  convert $CT 100 to $DL	- �N $CT 100 �I���� $DL
HELP,

"list":
@HELP
�C�X�ثe�Ҧ������f���ײv����Ӫ�A�Ϊk�p�U�G
  list			- �C�X�ײv��Ӫ�
HELP,
			]),
		"action":
		([
	    		"convert"	: (: do_convert :),
	    		"list"		: (: do_list :),
		]),
    	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIY"�Ȧ�"NOR

	// �}�i���ؿv�����̤֩ж�����
	,2

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,5000000
	
	// �ؿv���������ռаO
	,0
	
	// �c�a�^�m��
	,40
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
