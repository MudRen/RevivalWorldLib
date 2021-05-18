/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : futures.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-11-20
 * Note   : ���f�����
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
#include <futures.h>
#include <money.h>
#include <time.h>

inherit ROOM_ACTION_MOD;

#define MAX_LOT_PRICE			100000000000		// �̤j����������B����

float fund_multiplier(int total_gain)
{
	float multiplier = 1. + ((total_gain / 100000000000)/10.);
	
	if( multiplier < 0. )
		multiplier = 0.;
		
	return multiplier;
}

// �C��
void do_list(object me, string arg)
{
	int count = 0;
	int trading = 0;
	int valid_cover_time;
	int personal_total_gain = query("total_futures_gain", me);
	mapping positions = FUTURES_D->query_positions(me->query_id(1));
	
	string *msg = ({"�s��  ���f�W��         �R��       ���       ����       ���^     ���ܤ��ݬ��    ���A\n"});
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});
	
	foreach(array futures in FUTURES_D->query_futures())
	{
		trading = FUTURES_D->is_trading_time(count);
		
		count++;
	
		if( mapp(positions[count-1]) )
			valid_cover_time = positions[count-1]["time"] + futures[FUTURES_VALIDTRADETIME] - time();
		else
			valid_cover_time = 0;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-10s "HIY"%10.2f "HIY"%10.2f "HIY"%10.2f"NOR" %10s "HIM"%16d    %s\n"NOR,
			count,
			futures[FUTURES_NAME], 
			futures[FUTURES_BUYPRICE], 
			futures[FUTURES_SELLPRICE], 
			futures[FUTURES_PRICE], 
			(futures[FUTURES_VARIATION]>0 ? HIR"+" : futures[FUTURES_VARIATION]==0. ? HIW : HIG) + sprintf("%.2f", futures[FUTURES_VARIATION]),
			valid_cover_time >= 0 ? valid_cover_time : 0,
			trading ? HIC"�����"NOR : NOR CYN"���}�L"NOR
		)});
	}

	msg += ({ sprintf("\n"NOR YEL"%s"HIY" %d%%"NOR"\n%s\n%s", "�ӤH����O����", to_int(fund_multiplier(personal_total_gain)*100), NOR YEL"�ӤH�����f�l�q "HIY+money(DEFAULT_MONEY_UNIT, personal_total_gain)+NOR, NOR YEL"�@�ɷ����f�l�q�`�X "HIY+money(DEFAULT_MONEY_UNIT, FUTURES_D->get_world_total_gain())+NOR+"\n") });
	
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});
	msg += ({NOR "�ӤH�����f�l�q�C�W�[ "HIG"1000 ��"NOR"��Q�A�Y���� "HIG"10%"NOR" ����O�C\n"NOR });
	msg += ({NOR "�ӤH�����f�l�q�C��N�k�s���s�p��C\n"NOR });
	msg += ({NOR "�x�W���f����ɶ����}�L�� "HIM"08:45AM"NOR" �� "HIM"01:45PM"NOR"�C\n" });
	msg += ({NOR "�ثe�ӧO���f�̤j��������O�Ҫ��`�B��� "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"�C\n"NOR });
	msg += ({NOR "�Ъ`�N�A��l�q�F"HIR" -100% "NOR"�ɡA�t�αN�j��"HIR"�_�Y���"NOR"�A�Ҧ��O�Ҫ��N�����l���C\n"NOR});
	
	return me->more(implode(msg, "")+"\n");
}

void do_futures(object me, string arg)
{
	int count;
	int valid_cover_time;
	int personal_total_gain = query("total_futures_gain", me);
	mapping positions = FUTURES_D->query_positions(me->query_id(1));
	
	string *msg = ({"�s��  ���f�W��         �O�Ҫ�/�C�f     ���B/����      ����O/�C�f     ���ܮɭ�\n"});
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});
	
	foreach(array futures in FUTURES_D->query_futures())
	{
		count++;
	
		if( mapp(positions[count-1]) )
			valid_cover_time = positions[count-1]["time"] + futures[FUTURES_VALIDTRADETIME] - time();
		else
			valid_cover_time = 0;
		
		msg += ({sprintf(HIM"%-4d"HIW"  %-12s "HIY"%14s"NOR YEL" %14s %12s%-5s "HIM" %10d\n"NOR,
			count,
			futures[FUTURES_NAME], 
			NUMBER_D->number_symbol(futures[FUTURES_MARGIN]), 
			NUMBER_D->number_symbol(futures[FUTURES_VALUE]),
			NUMBER_D->number_symbol(to_int(fund_multiplier(personal_total_gain) * futures[FUTURES_PROCEDUREFUND])),
			"("+(to_int(fund_multiplier(personal_total_gain) * futures[FUTURES_PROCEDUREFUND])*100/futures[FUTURES_MARGIN])+"%)",
			futures[FUTURES_VALIDTRADETIME],
		)});
	}

	msg += ({ sprintf("\n"NOR YEL"%s"HIY" %d%%"NOR"\n%s\n%s", "�ӤH����O����", to_int(fund_multiplier(personal_total_gain)*100), NOR YEL"�ӤH�����f�l�q "HIY+money(DEFAULT_MONEY_UNIT, personal_total_gain)+NOR, NOR YEL"�@�ɷ����f�l�q�`�X "HIY+money(DEFAULT_MONEY_UNIT, FUTURES_D->get_world_total_gain())+NOR+"\n") });
	
	msg += ({"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"});
	msg += ({NOR "�ӤH�����f�l�q�C�W�[ "HIG"1000 ��"NOR"��Q�A�Y���� "HIG"10%"NOR" ����O�C\n"NOR });
	msg += ({NOR "�ӤH�����f�l�q�C��N�k�s���s�p��C\n"NOR });
	msg += ({NOR "�x�W���f����ɶ����}�L�� "HIM"08:45AM"NOR" �� "HIM"01:45PM"NOR"�C\n" });
	msg += ({NOR "�ثe�ӧO���f�̤j��������O�Ҫ��`�B��� "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"�C\n"NOR });
	msg += ({NOR "�Ъ`�N�A��l�q�F"HIR" -100% "NOR"�ɡA�t�αN�j��"HIR"�_�Y���"NOR"�A�Ҧ��O�Ҫ��N�����l���C\n"NOR});
	
	return me->more(implode(msg, "")+"\n");
}

void do_listen(object me, string arg)
{
	array futures = FUTURES_D->query_futures();
	int num, sec;
	
	if( !arg ) return tell(me, "���O�榡���~�C\n");

	if( arg[<4..<1] == "stop" && sscanf(arg, "%d stop", num) == 1 )
	{
		FUTURES_D->stop_listen(me->query_id(1), num-1);
		tell(me, pnoun(2, me)+"���ť�u"+futures[num-1][FUTURES_NAME]+"�v���Y�ɳ����C\n");	
	}
	else if( sscanf(arg, "%d %ds", num, sec) == 2 )
	{
		set("futures_listen_tick/"+(num-1), sec, me);
		
		if( num <= 0 || num > sizeof(futures) )
			return tell(me, "�S���o�Ӵ��f�ӫ~�s���C\n");
	
		FUTURES_D->start_listen(me->query_id(1), num-1);
		tell(me, pnoun(2, me)+"�}�l��ť�u"+futures[num-1][FUTURES_NAME]+"�v���Y�ɳ����C\n");
	}
	else
		return tell(me, "���O�榡���~�C\n");
}

// �Q��
string benefit_ratio(float gain, float margin)
{
	if( gain > 0. )
		return sprintf(NOR RED"+"HIR"%.2f%%"NOR, gain*100./margin);
	else if( gain == 0. )
		return HIW"0.00%"NOR;
	else
		return sprintf(NOR GRN"-"HIG"%.2f%%"NOR, -gain*100./margin);
}

// �T�{���f���
void confirm_trade_futures(object me, int num, array futures, int trade_amount, int old_amount, float old_price, string arg)
{
	int cost;
	int gain;
	int total_gain;
	int margin;
	int cover_amount;
	int new_amount;
	int personal_total_gain = query("total_futures_gain", me);
	float new_price;
	mapping position = FUTURES_D->query_positions(me->query_id(1), num);
	int valid_cover_time = position["time"] + futures[FUTURES_VALIDTRADETIME] - time();

	if( !arg )
		return me->finish_input();

	arg = lower_case(arg);
	
	if( arg != "y" && arg != "yes" )
	{
		tell(me, "�������f����C\n");
		return me->finish_input();
	}

	if( futures[FUTURES_UPDATETIME] < time() - 180 )
	{
		tell(me, futures[FUTURES_NAME]+"��Ʃ|����s�A�Ȯɰ���R��C\n");
		return me->finish_input();
	}
		
	if( !FUTURES_D->is_trading_time(num) )
		return tell(me, futures[FUTURES_NAME]+"�ثe���b����ɶ����A�L�k�i�����C\n");

	// �R�i
	if( trade_amount > 0 )
	{
		// ����
		if( old_amount < 0 )
		{
			// ��������
			if( trade_amount >= abs(old_amount) )
			{
				cover_amount = abs(old_amount);
				trade_amount -= cover_amount;
				old_amount = 0;
			}
			// ��������
			else
			{
				cover_amount = trade_amount;
				trade_amount = 0;
				old_amount += cover_amount;
			}
		}
		
		// �p�⥭�ܷl�q
		if( cover_amount > 0 )
		{
			if( valid_cover_time > 0 )
			{
				tell(me, pnoun(2, me)+"�����A���� "+valid_cover_time +" ���~�७�ܡu"HIW+futures[FUTURES_NAME]+NOR"�v���f�C\n");
				return me->finish_input();
			}

			gain = cover_amount * (to_int(old_price*100) - to_int(futures[FUTURES_SELLPRICE]*100)) * futures[FUTURES_VALUE] / 100;
			margin = cover_amount * futures[FUTURES_MARGIN];
		
			if( margin + gain > 0 )
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);
			
			total_gain = gain - to_int(cover_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));

			addn("total_futures_gain", total_gain, me);
			
			TOP_D->update_top_futures(me);
			
			tell(me, pnoun(2, me)+"���� "+cover_amount+" �f�u"+futures[FUTURES_NAME]+"�v���f�A�@���^ "HIY+money(DEFAULT_MONEY_UNIT, margin+gain)+NOR"�A�l�q "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"�C\n");
			CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", futures[FUTURES_SELLPRICE])+NOR" ����("HIY+sprintf("%.2f", old_price)+NOR")"HIR"�R�i "+NUMBER_D->number_symbol(cover_amount)+" �f"NOR"�u"HIW+futures[FUTURES_NAME]+NOR"�v���f"HIY"����"NOR"�A�l�q "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"("+benefit_ratio(total_gain, margin)+")�C");
			
			position["amount"] += cover_amount;
			
			FUTURES_D->set_positions(me->query_id(1), num, position);
		}			
		
		// �R�i���f
		if( trade_amount > 0 )
		{
			margin = trade_amount * futures[FUTURES_MARGIN];
			
			cost = margin + to_int(trade_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));
				
			if( !me->spend_money(DEFAULT_MONEY_UNIT, cost) )
			{
				tell(me, pnoun(2, me)+"���W�������� "+money(DEFAULT_MONEY_UNIT, cost)+"�C\n");
				return me->finish_input();
			}
			else
			{
				if( old_amount != 0 )
				{
					new_amount = old_amount + trade_amount;
					new_price = to_int(((old_amount * to_int(old_price*100)) + (trade_amount * to_int(futures[FUTURES_SELLPRICE]*100))) / new_amount) / 100.;
				}
				else
				{
					new_amount = trade_amount;
					new_price = futures[FUTURES_SELLPRICE];
				}
				
				position["amount"] = new_amount;
				position["price"] = new_price;
				position["time"] = time();
				
				FUTURES_D->set_positions(me->query_id(1), num, position);
				
				tell(me, pnoun(2, me)+"�`�@��X "HIY+money(DEFAULT_MONEY_UNIT, cost)+NOR" �R�i "+trade_amount+" �f�u"HIW+futures[FUTURES_NAME]+NOR"�v���f�C\n");
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", futures[FUTURES_SELLPRICE])+NOR" ����"HIR"�R�i "+NUMBER_D->number_symbol(trade_amount)+" �f"NOR"�u"HIW+futures[FUTURES_NAME]+NOR"�v���f�C");
			}
		}
	}
	// ��X
	else
	{
		// ����
		if( old_amount > 0 )
		{
			// ��������
			if( abs(trade_amount) >= old_amount )
			{
				cover_amount = old_amount;
				trade_amount += cover_amount;
				old_amount = 0;
			}
			// ��������
			else
			{
				cover_amount = abs(trade_amount);
				trade_amount = 0;
				old_amount -= cover_amount;
			}
		}
		
		// �p�⥭�ܷl�q
		if( cover_amount > 0 )
		{
			if( valid_cover_time > 0 )
			{
				tell(me, pnoun(2, me)+"�����A���� "+valid_cover_time +" ���~�७�ܡu"HIW+futures[FUTURES_NAME]+NOR"�v���f�C\n");
				return me->finish_input();
			}
			
			gain = cover_amount * (to_int(futures[FUTURES_BUYPRICE]*100) - to_int(old_price*100)) * futures[FUTURES_VALUE] / 100;
			margin = cover_amount * futures[FUTURES_MARGIN];
			
			if( margin + gain > 0 )
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);

			total_gain = gain - to_int(cover_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));

			addn("total_futures_gain", total_gain, me);
			
			TOP_D->update_top_futures(me);
			
			tell(me, pnoun(2, me)+"���� "+cover_amount+" �f�u"+futures[FUTURES_NAME]+"�v���f�A�@���^ "HIY+money(DEFAULT_MONEY_UNIT, margin+gain)+NOR"�A�l�q "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"�C\n");
			CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", futures[FUTURES_BUYPRICE])+NOR" ����("HIY+sprintf("%.2f", old_price)+NOR")"HIG"��X "+NUMBER_D->number_symbol(cover_amount)+" �f"NOR"�u"HIW+futures[FUTURES_NAME]+NOR"�v���f"HIY"����"NOR"�A�l�q "HIY+money(DEFAULT_MONEY_UNIT, total_gain)+NOR"("+benefit_ratio(total_gain, margin)+")�C");
			
			position["amount"] += -cover_amount;
			
			FUTURES_D->set_positions(me->query_id(1), num, position);
		}			
		
		// ��X���f
		if( trade_amount < 0 )
		{
			margin = -trade_amount * futures[FUTURES_MARGIN];

			cost = margin - to_int(trade_amount * futures[FUTURES_PROCEDUREFUND] * fund_multiplier(personal_total_gain));
				
			if( !me->spend_money(DEFAULT_MONEY_UNIT, cost) )
			{
				tell(me, pnoun(2, me)+"���W�������� "+money(DEFAULT_MONEY_UNIT, cost)+"�C\n");
				return me->finish_input();
			}
			else
			{
				if( old_amount != 0 )
				{
					new_amount = old_amount + trade_amount;
					new_price = to_int(((old_amount * to_int(old_price*100)) + (trade_amount * to_int(futures[FUTURES_BUYPRICE]*100))) / new_amount) / 100.;
				}
				else
				{
					new_amount = trade_amount;
					new_price = futures[FUTURES_BUYPRICE];
				}
				
				position["amount"] = new_amount;
				position["price"] = new_price;
				position["time"] = time();
				
				FUTURES_D->set_positions(me->query_id(1), num, position);
				
				tell(me, pnoun(2, me)+"�`�@��X "HIY+money(DEFAULT_MONEY_UNIT, cost)+NOR" ��X "+(-trade_amount)+" �f�u"HIW+futures[FUTURES_NAME]+NOR"�v�C\n");
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"�H "HIY+sprintf("%.2f", futures[FUTURES_BUYPRICE])+NOR" ����"HIG"��X "+NUMBER_D->number_symbol(-trade_amount)+" �f"NOR"�u"HIW+futures[FUTURES_NAME]+NOR"�v���f�C");
			}
		}
	}

	me->save();
	me->finish_input();
}

// �ʤJ���f
void do_buy_futures(object me, string arg)
{
	int num, amount;
	array futures_list = FUTURES_D->query_futures();
	int old_amount;
	float old_price;
	int personal_total_gain = query("total_futures_gain", me);
	mapping position;
	
	if( !userp(me) ) return;

	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�A�ҡGbuy 1 100�C\n");

	if( amount < 1 || amount > 1000000 )
		return tell(me, "�C�������������f�f�Ƭ� 1 �� 1000000�C\n");
		
	if( num < 1 || num > sizeof(futures_list) )
		return tell(me, "�S�� "+num+" �o�@�s�����f�C\n");
	
	num--;

	if( futures_list[num][FUTURES_UPDATETIME] < time() - 180 )
		return tell(me, futures_list[num][FUTURES_NAME]+"��Ʃ|����s�A�Ȯɰ���R��C\n");

	position = FUTURES_D->query_positions(me->query_id(1), num);
	
	old_amount = position["amount"];
	old_price = position["price"] || 0.;

	if( amount < old_amount/5 )
		return tell(me, "�C������f�Ƥ��o�C��ثe�����f�ƪ� 20%�C\n");
		
	if( (amount + old_amount) * futures_list[num][FUTURES_MARGIN] > MAX_LOT_PRICE )
		return tell(me, "�P�@���f�̤j��������O�Ҫ����B���i�W�L "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"�A�u"HIW+futures_list[num][FUTURES_NAME]+NOR"�v�ثe�̤j�u����� "+(MAX_LOT_PRICE/futures_list[num][FUTURES_MARGIN])+" �f�C\n");
		
	tell(me,
		WHT"���f�W��      "NOR HIW+futures_list[num][FUTURES_NAME]+NOR"\n"
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		YEL"�ثe���f���  "HIY+sprintf("%.2f", futures_list[num][FUTURES_SELLPRICE])+"\n"NOR
		GRN"�ثe���ܧ���  "HIG+sprintf("%.2f", old_price)+"\n"NOR
		GRN"�ثe��������  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"�R�i���f�f��  "HIC+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"�R�i���f�`��  "HIC+money(DEFAULT_MONEY_UNIT, amount * futures_list[num][FUTURES_MARGIN])+"\n"NOR
		YEL"�������O��  "HIY+money(DEFAULT_MONEY_UNIT, to_int(amount * futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)))+NOR YEL" (���ܮɤ��ݤ�I)\n"NOR
		YEL"����R�i�l�q  "HIY+sprintf("%.2f%%", (-futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)*100./futures_list[num][FUTURES_MARGIN]))+NOR YEL" (�ݦܤ֤W�� "HIY+sprintf("%.2f", futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)/futures_list[num][FUTURES_VALUE])+NOR YEL" �I�~�i�l�q�⥭)\n"NOR
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		HIY"�O�_�T�w�R�i���f?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_trade_futures, me, num, futures_list[num], amount, old_amount, old_price :));
}

// ��X���f
void do_sell_futures(object me, string arg)
{
	int num, amount;
	array futures_list = FUTURES_D->query_futures();
	int old_amount;
	float old_price;
	int personal_total_gain = query("total_futures_gain", me);
	mapping position;

	if( !userp(me) ) return;
		
	if( !arg || sscanf(arg, "%d %d", num, amount) != 2 )
		return tell(me, "�п�J���T���榡�A�ҡGbuy 1 100�C\n");

	if( amount < 1 || amount > 1000000 )
		return tell(me, "�C�������������f�f�Ƭ� 1 �� 1000000�C\n");
		
	if( num < 1 || num > sizeof(futures_list) )
		return tell(me, "�S�� "+num+" �o�@�s�����f�C\n");
		
	num--;

	if( futures_list[num][FUTURES_UPDATETIME] < time() - 180 )
		return tell(me, futures_list[num][FUTURES_NAME]+"��Ʃ|����s�A�Ȯɰ���R��C\n");

	position = FUTURES_D->query_positions(me->query_id(1), num);
	
	old_amount = position["amount"];
	old_price = position["price"] || 0.;

	if( amount < old_amount/5 )
		return tell(me, "�C������f�Ƥ��o�C��ثe�����f�ƪ� 20%�C\n");
			
	if( (amount - old_amount) * futures_list[num][FUTURES_MARGIN] > MAX_LOT_PRICE )
		return tell(me, "�P�@���f�̤j��������O�Ҫ����B���i�W�L "HIY+money(DEFAULT_MONEY_UNIT, MAX_LOT_PRICE)+NOR"�A�u"HIW+futures_list[num][FUTURES_NAME]+NOR"�v�ثe�̤j�u����� "+(MAX_LOT_PRICE/futures_list[num][FUTURES_MARGIN])+" �f�C\n");
			
	tell(me,
		WHT"���f�W��      "NOR HIW+futures_list[num][FUTURES_NAME]+NOR"\n"
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		YEL"�ثe���f�R��  "HIY+sprintf("%.2f", futures_list[num][FUTURES_BUYPRICE])+"\n"NOR
		GRN"�ثe���ܧ���  "HIG+sprintf("%.2f", old_price)+"\n"NOR
		GRN"�ثe��������  "HIG+NUMBER_D->number_symbol(old_amount)+"\n"NOR
		CYN"��X���f�f��  "HIC+NUMBER_D->number_symbol(amount)+"\n"NOR
		CYN"��X���f�`��  "HIC+money(DEFAULT_MONEY_UNIT, amount * futures_list[num][FUTURES_MARGIN])+"\n"NOR
		YEL"�������O��  "HIY+money(DEFAULT_MONEY_UNIT, to_int(amount * futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)))+NOR YEL" (���ܤ��ݤ�I)\n"NOR
		YEL"�����X�l�q  "HIY+sprintf("%.2f%%", (-futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)*100./futures_list[num][FUTURES_MARGIN]))+NOR YEL" (�ݦܤ֤U�^ "HIY+sprintf("%.2f", futures_list[num][FUTURES_PROCEDUREFUND]*fund_multiplier(personal_total_gain)/futures_list[num][FUTURES_VALUE])+NOR YEL" �I�~�i�l�q�⥭)\n"NOR
		"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"
		HIY"�O�_�T�w��X���f?"NOR YEL"(Yes/No)"NOR
	);

	input_to((: confirm_trade_futures, me, num, futures_list[num], -amount, old_amount, old_price :));
}

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
��ܴ��f�Y�ɸ�T
list		�C�X�Ҧ��ثe�i������f�Y�ɸ��
HELP,

"futures":
@HELP
��ܴ��f��T
futures		�C�X�Ҧ��ثe�i������f�]�w
HELP,

"listen":
@HELP
��ť�Y�ɴ��f��T
listen 1 5s		�]�w�C 5 ������ť�@���s�� 1 ���f���Y�ɳ���
listen 1 stop	������ť�s�� 1 ���f���Y�ɳ���
HELP,

"buy":
@HELP
�R�J���f�����O
buy 1 300	�R�J�s�� 1 �����f 300 �f
HELP,

"sell":
@HELP
��X���f�����O
sell 1 100	��X�s�� 1 �����f 100 �f
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": 0,
		"action":
			([
				"list" : (: do_list :),
				"futures": (: do_futures :),
				"listen" : (: do_listen :),
				"buy" : (: do_buy_futures :),
				"sell" : (: do_sell_futures :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"���f"NOR CYN"�����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

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
	,2
});
