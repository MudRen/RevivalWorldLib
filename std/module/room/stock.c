/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stock.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-05-31
 * Note   : �Ҩ�����
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

#define PROCEDURE_FUND	0.02		// 2/100 ������O
#define STOCK_VALUE	10000		// �@�i�Ѳ�����

varargs string stockvalue_change_description(float change, string type)
{
	string bgcolor;
	
	switch(type)
	{
		case "t3g2":
			bgcolor = BGRN;
			break;
		case "t3r2":
			bgcolor = BRED;
			break;
		default:
			bgcolor = "";
			break;
	}

	change = to_float(change);

	if( change > 0 )
		return bgcolor+sprintf(HIR"��%5.2f", change);
	else if( change < 0 )
		return bgcolor+sprintf(HIG"��%5.2f", -change);
	else
		return bgcolor+sprintf(HIW"  %5.2f", change);
}


void do_list(object me, string arg)
{
	int count;
	mapping stocks = STOCK_D->query_stocks();
	int *stock_sort = sort_array(keys(stocks), 1);
	mapping data;
	string *msg = allocate(0);
	int min, max;
	float percent = PROCEDURE_FUND - me->query_skill_level("stock")/10000.;

	if( arg )
	{
		if( sscanf(arg, "%d %d", min, max) != 2 )
			sscanf(arg, "%d", min);
	}
	
	msg = ({"�N��  �Ѳ��W�� ���馬�L ���麦�^ ���^�T�� �}�L�� �̰��� �̧C�� ����i�� \n"});
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});

	foreach(int stock in stock_sort)
	{
		count++;

		if( max )
		{
			if( stock < min || stock > max ) continue;
		}
		else
		{
			if( min && stock != min ) continue;
		}

		data = stocks[stock];
		
		if( !data["�Ѳ��W��"] ) continue;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-8s "HIY"%8.2f"NOR" %-8s %7.2f%% "NOR WHT"%6.2f "HIB"%6.2f %6.2f "NOR BYEL HIY"%s\n"NOR,
			stock,
			data["�Ѳ��W��"], 
			data["���L"], 
			stockvalue_change_description(data["���^"], data["���A"]), 
			data["���^�T"],
			data["�}�L"],
			data["�̰�"],
			data["�̧C"],
			data["����i��"]
		)});
	}
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});
	msg += ({MAG"�@ "HIM+count+NOR MAG" �ɤW���W�d�Ѳ��A"YEL"�Ѳ���쬰 "HIY"10,000"NOR YEL" ��/�i�A"NOR RED"�������O�� "HIR+sprintf("%.2f%%", percent*100.)+NOR RED"�A"HIC"���L��Ƥ@�ѧ�s�@��\n"NOR});
	msg += ({WHT"�]�ƾڨëD�Y�ɧ�s�A�]���g�@�ܶg�������W�E�I�ܤU�ȥ|�I�������L���P��Ƨ�s�����T����\n\n"NOR});

	return me->more(implode(msg, ""));
}

void confirm_buystock(object me, int num, int totalcost, int new_amount, float new_value, float cur_value, int amount, string stock_name, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�����ʤJ�Ѳ��C\n");
		return me->finish_input();
	}

	if( !me->spend_money(MONEY_D->query_default_money_unit(), totalcost) )
	{
		tell(me, pnoun(2, me)+"���W�������� "+money(MONEY_D->query_default_money_unit(), totalcost)+"�C\n");
		return me->finish_input();
	}
	
	set("stock/"+num+"/amount", new_amount, me);
	set("stock/"+num+"/value", new_value, me);
		
	CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", cur_value)+NOR" �ѻ��ʤJ�u"HIW+stock_name+NOR"�v�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�C");
	me->finish_input();
	
	TOP_D->update_top_rich(me);
	me->save();
}

void do_buystock(object me, string arg)
{
	mapping stocks = STOCK_D->query_stocks();
	int num, amount;
	float old_value;
	float new_value;
	float cur_value;
	int old_amount;
	int new_amount;
	int cost;
	int cost_extra;
	int *nowtime = TIME_D->query_realtime_array();
	float percent;

	return tell(me, "�Ѳ��t�Υثe�L�k�B�z���s���P�B���v�B���������D�A�u�����N�L�k�ʶR�Ѳ��C\n");

	if( !wizardp(me) && nowtime[2] >= 1 && nowtime[2] <= 5 && nowtime[1] >= 9 && nowtime[1] <= 16 )
		return tell(me, "�g�@�ܶg�������W�E�I�ܤU�ȥ|�I�T�����C\n");

	if( STOCK_D->query_last_update_time() < time() - 24*60*60 )
		return tell(me, "�Ѳ���Ʃ|����s�A�L�k�i�����C\n");

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�A�ҡGbuy 1101 1�C\n");

	if( amount < 1 )
		return tell(me, pnoun(2, me)+"�ܤ֥����ʶR�@�i�Ѳ��C\n");

	if( !mapp(stocks[num]) || !stocks[num]["�Ѳ��W��"] )
		return tell(me, "�S�� "+num+" �o�@�ɪѲ��C\n");

	if( stocks[num]["���A"] == "t3r2" )
		return tell(me, "�o�ɪѲ��w�g�����A�L�k�A�R�F�C\n");

	old_amount = query("stock/"+num+"/amount", me);
	new_amount = amount + old_amount;

	if( new_amount > 1000000 )
		return tell(me, "�C�ɪѲ��̦h�u���ʶR 1,000,000 �i�C\n");
		
	old_value = to_float(query("stock/"+num+"/value", me));
	cur_value = stocks[num]["���L"];
	new_value = (old_value * old_amount + cur_value * amount) / (amount + old_amount);

	percent = PROCEDURE_FUND - me->query_skill_level("stock")/10000.;

	cost = to_int(to_int(cur_value*100) * STOCK_VALUE * amount / 100);
	cost_extra = to_int(cost * percent);

	if( cost + cost_extra < 1 )
		return tell(me, "��ƭp����~�A�гq���Ův�B�z�C\n");

	tell(me,
		WHT"�Ѳ��W��      "NOR HIW+num+" "+stocks[num]["�Ѳ��W��"]+NOR"\n"
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		YEL"���馬�L����  "HIY+sprintf("%.2f", cur_value)+"\n"NOR
		GRN"���ʪѲ��i��  "HIG+NUMBER_D->number_symbol(amount)+"\n"NOR
		GRN"�ثe���ѻ���  "HIG+sprintf("%.2f", old_value)+"\n"NOR
		GRN"�ثe���ѱi��  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"�����������  "HIC+sprintf("%.2f", new_value)+"\n"NOR
		CYN"���ʪѲ��`��  "HIC+money(MONEY_D->query_default_money_unit(), cost)+"\n"NOR
		CYN"�������O��  "HIC+money(MONEY_D->query_default_money_unit(), cost_extra)+HIR"("+sprintf("%.2f", percent*100.)+"%)\n"NOR
		CYN"�ʤJ�`�@��O  "HIC+money(MONEY_D->query_default_money_unit(), cost + cost_extra)+"\n"NOR
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		HIY"�O�_�T�w�ʤJ�Ѳ�?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_buystock, me, num, cost + cost_extra, new_amount, new_value, cur_value, amount, num+" "+stocks[num]["�Ѳ��W��"] :));
}

void confirm_sellstock(object me, int num, int totalearn, int new_amount, float old_value, float cur_value, int amount, string stock_name, string arg)
{
	if( !arg )
		return me->finish_input();
		
	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�����c��Ѳ��C\n");
		return me->finish_input();
	}
	
	me->earn_money(MONEY_D->query_default_money_unit(), totalearn);
	
	if( new_amount <= 0 )
		delete("stock/"+num, me);
	else
		set("stock/"+num+"/amount", new_amount, me);
	
	if( cur_value == old_value )
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", cur_value)+NOR" �ѻ���X�u"HIW+stock_name+NOR"�v�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�C");
	else if( cur_value > old_value )
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", cur_value)+NOR" �ѻ���X�u"HIW+stock_name+NOR"�v�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�A��Q "HIY+money(MONEY_D->query_default_money_unit(), to_int(to_int((cur_value-old_value)*100)*STOCK_VALUE/100*amount))+NOR"�C");
	else
		CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", cur_value)+NOR" �ѻ���X�u"HIW+stock_name+NOR"�v�Ѳ� "+NUMBER_D->number_symbol(amount)+" �i�A�{�� "HIY+money(MONEY_D->query_default_money_unit(), to_int(to_int((old_value-cur_value)*100)*STOCK_VALUE/100*amount))+NOR"�C");
	
	me->finish_input();
	TOP_D->update_top_rich(me);
	me->save();
}

void do_sellstock(object me, string arg)
{
	mapping stocks = STOCK_D->query_stocks();
	int num, amount;
	int old_amount;
	int new_amount;
	float cur_value;
	float old_value;
	int earn;
	int *nowtime = TIME_D->query_realtime_array();

	if( !wizardp(me) && nowtime[2] >= 1 && nowtime[2] <= 5 && nowtime[1] >= 9 && nowtime[1] <= 16 )
		return tell(me, "�g�@�ܶg�������W�E�I�ܤU�ȥ|�I�T�����C\n");

	if( STOCK_D->query_last_update_time() < time() - 24*60*60 )
		return tell(me, "�Ѳ���Ʃ|����s�A�L�k�i�����C\n");

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�A�ҡGsell 1101 1�C\n");

	if( amount < 1 )
		return tell(me, pnoun(2, me)+"�ܤ֥����X��@�i�Ѳ��C\n");
	
	if( !mapp(stocks[num]) || !stocks[num]["�Ѳ��W��"] )
		return tell(me, "�S�� "+num+" �o�@�ɪѲ��C\n");

	if( stocks[num]["���A"] == "t3g2" )
		return tell(me, "�o�ɪѲ��w�g�^���A�L�k�A��F�C\n");

	old_amount = query("stock/"+num+"/amount", me);
	new_amount = old_amount - amount;
	
	old_value = to_float(query("stock/"+num+"/value", me));
	cur_value = stocks[num]["���L"];

	if( old_amount < 1 )
		return tell(me, pnoun(2, me)+"��W�èS�� "+num+" �o�ɪѲ��C\n");

	if( new_amount < 0 )
		return tell(me, "�o�ɪѲ�"+pnoun(2, me)+"��W�u�� "+old_amount+" �i�C\n");

	earn = to_int(to_int(cur_value*100) * amount * STOCK_VALUE / 100);
	
	if( earn < 1 )
		return tell(me, "��ƭp����~�A�гq���Ův�B�z�C\n");

	tell(me,
		WHT"�Ѳ��W��      "NOR HIW+num+" "+stocks[num]["�Ѳ��W��"]+NOR"\n"
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		YEL"���馬�L����  "HIY+sprintf("%.2f", cur_value)+"\n"NOR
		GRN"�ثe���ѻ���  "HIG+sprintf("%.2f", old_value)+"\n"NOR
		GRN"�ثe���ѧQ��  "HIG+sprintf("%.2f%%", (cur_value - old_value) * 100. / old_value)+"\n"NOR
		GRN"����Ѳ��i��  "HIG+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"��X�`�@��o  "HIC+money(MONEY_D->query_default_money_unit(), earn)+"\n"NOR
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		HIY"�O�_�T�w��X�Ѳ�?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_sellstock, me, num, earn, new_amount, old_value, cur_value, amount, num+" "+stocks[num]["�Ѳ��W��"] :));
}

/*
void do_clearstock(object me, string arg)
{
	mapping stockdata = query("stock", me);
	
	if( !sizeof(stockdata) )
		return tell(me, pnoun(2, me)+"���⤤����������Ѳ��C\n");

	foreach( string enterprise, mapping data in stockdata )	
	{
		if( !ENTERPRISE_D->enterprise_exists(enterprise) )
		{
			tell(me, "�M�� "+enterprise+" ���Ѳ���ơC\n");
			delete("stock/"+enterprise, me);
			me->save();
		}
	}
	
	tell(me, "�����U���Ѳ��M�z�C\n");
}
*/

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: NOR HIW"����Ҥj�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �Ҩ������a��C
HELP,

"list":
@HELP
��ܪѥ���T
list		�C�X�Ҧ��O�W�W���W�d�Ѳ��������
list 1101	�C�X�N�� 1101 �Ѳ������
list 1100 1300  �C�X�N�� 1100 �� 1300 �����Ҧ����Ѳ����
HELP,

"buy":
@HELP
�R�J�Ѳ������O
buy 1101 300	�R�J 1101(�x�d) �Ѳ� 300 �i
HELP,

"sell":
@HELP
��X�Ѳ������O
sell 1101 100	��X 1101(�x�d) �Ѳ� 100 �i
HELP,

"clear":
@HELP
�а��w�U���Ѳ�
clear		�M���w�U���Ѳ����(�o��)
HELP,

			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"buy" : (: do_buystock :),
				"sell" : (: do_sellstock :),
				//"clear" : (: do_clearstock :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIG"�Ҩ�"NOR GRN"�����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,2

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,5

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,ENTERPRISE

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,75000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,0
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
