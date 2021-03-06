/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : score.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-29
 * Note   : score 指令
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <buff.h>
#include <futures.h>
#include <money.h>

inherit COMMAND;

string help = @HELP
查詢角色狀態
    
score		- 查詢角色基本狀態
score s		- 快速查詢能力值(即 hp 指令)
score m 	- 查詢角色現金與存款(即 money 指令)
score 'ID'	- 查詢其他線上玩家資料

score 可使用系統縮寫 sc

HELP;

private string stat_symbol(int cur, int max)
{
	if( cur >= max )
		return NOR RED+cur+"/"HIR+max+NOR;
	else if( cur >= max*3/4 )
		return NOR MAG+cur+"/"HIM+max+NOR;
	else if( cur >= max*1/2 )
		return NOR YEL+cur+"/"HIY+max+NOR;
	else if( cur >= max*1/4 )
		return NOR CYN+cur+"/"HIC+max+NOR;
	else
		return NOR GRN+cur+"/"HIG+max+NOR;
}

private varargs string bonus(int bonus, string unit)
{
	if( bonus > 0 )
		return NOR CYN" +"HIC+bonus+(unit||"")+NOR;
	else if( bonus == 0 )
		return "";
	else
		return NOR RED" -"HIR+(-bonus)+(unit||"")+NOR;
}

private varargs string buffbonus(int bonus, string unit)
{
	if( bonus > 0 )
		return NOR MAG" +"HIM+bonus+(unit||"")+NOR;
	else if( bonus == 0 )
		return HIM" 0"+(unit||"")+NOR;
	else
		return NOR MAG" -"HIM+(-bonus)+(unit||"")+NOR;
}

private varargs string regen(int r)
{
	if( r > 0 )
		return NOR CYN" +"HIC+r+NOR;
	else if( r == 0 )
		return "";
	else
		return NOR RED" -"HIR+(-r)+NOR;
}

private string query_buff_time(int time)
{
	return 
		((time/3600 > 0) ? HIW+(time/3600)+NOR WHT":"NOR : "") + 
		((time/3600 > 0) || ((time%3600)/60 > 0) ? HIW+(((time%3600)/60)<10?"0":"")+((time%3600)/60)+WHT":"NOR : "" ) +
		((time/3600 > 0) || ((time%3600)/60 > 0) || (time%60 > 0) ? HIW+((time%60)<10?"0":"")+(time%60) : "")
	;
}


float fund_multiplier(int total_gain)
{
	return 1. + ((total_gain / 100000000000)/10.);
}

string score_standard(object ob)
{
	string *msgs = allocate(0);
	string msg;
	string id = ob->query_id(1);
	int i;
	mapping dbase = ob->query_database();	

	string mycity = CITY_D->query_city_name(dbase["city"])||"無國藉";

	int myint = ob->query_int();

	int mysocialexpbuff = myint*2-20+ob->query_all_buff(BUFF_SOCIAL_EXP_BONUS);
	int mycombatexpbuff = myint*2-20+ob->query_all_buff(BUFF_COMBAT_EXP_BONUS);

	int mystaminacur = ob->query_stamina_cur();
	int mystaminamax = ob->query_stamina_max();
	int mystaminamaxbuff = ob->query_all_buff(BUFF_STAMINA_MAX);
	int myhealthcur = ob->query_health_cur();
	int myhealthmax = ob->query_health_max();
	int myhealthmaxbuff = ob->query_all_buff(BUFF_HEALTH_MAX);
	int myenergycur = ob->query_energy_cur();
	int myenergymax = ob->query_energy_max();
	int myenergymaxbuff = ob->query_all_buff(BUFF_ENERGY_MAX);

	int attackbuff = ob->query_all_buff(ATTACK);
	int defendbuff = ob->query_all_buff(DEFEND);
	int speedbuff = ob->query_all_buff(SPEED);

	mapping moneydata = MONEY_D->query_moneydata(id);
	mapping cash_info;
	mapping bank_info;
	mapping futures_positions;
	object room;
	
	if( ob->is_npc() )
	{
		string myboss = dbase["boss"];

		if( stringp(myboss) )
		{
			if( find_player(myboss) )
				mycity = find_player(myboss)->query_name();
			else
				mycity = capitalize(myboss);
		}
		else
			mycity = HIY"流"NOR YEL"浪"NOR;
	}

	msgs += ({ HIW"╭───────────────────────────────────╮" });
	msgs += ({ sprintf(WHT"｜"NOR"%-70s"WHT"｜", "【"+mycity+"】"+(ENTERPRISE_D->query_enterprise_color_id(dbase["enterprise"])||"")+(dbase["title"]?" "+dbase["title"]:"")+(dbase["nickname"]?" "+dbase["nickname"]:"")+" "+ob->query_idname()) });
	msgs += ({ "├──────────────────────────────── "HIG"角"NOR GRN"色 "WHT"┤" });
	msgs += ({ sprintf("｜"NOR" 力量 "WHT"Str %-10s"WHT"｜"NOR"佔領 %-9d攻擊 %-10s食物 %-14s"WHT"｜", ob->query_str()+bonus(ob->query_all_buff(BUFF_STR)), dbase["area_influence"], COMBAT_D->attack_value(ob)+(attackbuff!=0?NOR RED" "+(attackbuff>0?"+":"")+HIR+attackbuff+NOR:""), stat_symbol(ob->query_food_cur(), ob->query_food_max())+bonus(ob->query_all_buff(BUFF_FOOD_MAX))) });
	msgs += ({ sprintf("｜"NOR" 體格 "WHT"Phy %-10s"WHT"｜"NOR"聲望 %-9d防禦 %-10s飲水 %-14s"WHT"｜", ob->query_phy()+bonus(ob->query_all_buff(BUFF_PHY)), dbase["prestige"], COMBAT_D->defend_value(ob)+(defendbuff!=0?NOR CYN" "+(defendbuff>0?"+":"")+HIC+defendbuff+NOR:""), stat_symbol(ob->query_drink_cur(), ob->query_drink_max())+bonus(ob->query_all_buff(BUFF_DRINK_MAX))) });
	msgs += ({ sprintf("｜"NOR" 智商 "WHT"Int %-10s"WHT"｜"NOR"擊殺 %-9d速度 %-10s疲勞 %-14s"WHT"｜", myint+bonus(ob->query_all_buff(BUFF_INT)), query("combat/kill/win", ob), COMBAT_D->speed_value(ob)+(speedbuff!=0?NOR YEL" "+(speedbuff>0?"+":"")+HIY+speedbuff+NOR:""), stat_symbol(dbase["fatigue"], 1000)) });
	msgs += ({ sprintf("｜"NOR" 敏捷 "WHT"Agi %-10s"WHT"｜"NOR"社會經驗%-8s%-32s"WHT"｜", ob->query_agi()+bonus(ob->query_all_buff(BUFF_AGI)), bonus(mysocialexpbuff, "%"), ob->query_social_exp_cur()+" / "+ob->query_social_exp_tot()) });
	msgs += ({ sprintf("｜"NOR" 魅力 "WHT"Cha %-10s"WHT"｜"NOR"戰鬥經驗%-8s%-32s"WHT"｜", ob->query_cha()+bonus(ob->query_all_buff(BUFF_CHA)), bonus(mycombatexpbuff, "%"), ob->query_combat_exp_cur()+" / "+ob->query_combat_exp_tot()) });
	msgs += ({ "｜                                                                      ｜" });
	msgs += ({ sprintf("｜"NOR" 體力 %5d / %-11s%-40s%-5s"WHT"｜", mystaminacur, mystaminamax+bonus(mystaminamaxbuff), GRAPH_D->graph(mystaminacur,mystaminamax,2,0,20), regen(ob->query_all_buff(BUFF_STAMINA_REGEN))  ) });
	msgs += ({ sprintf("｜"NOR" 生命 %5d / %-11s%-40s%-5s"WHT"｜", myhealthcur , myhealthmax+bonus(myhealthmaxbuff), GRAPH_D->graph(myhealthcur,myhealthmax,1,0,20), regen(ob->query_all_buff(BUFF_HEALTH_REGEN))) });
	msgs += ({ sprintf("｜"NOR" 精神 %5d / %-11s%-40s%-5s"WHT"｜", myenergycur , myenergymax+bonus(myenergymaxbuff), GRAPH_D->graph(myenergycur,myenergymax,4,0,20), regen(ob->query_all_buff(BUFF_ENERGY_REGEN))) });
	msgs += ({ "｜                                                                      ｜" });
	msgs += ({ "｜"NOR" 貿易  採集  生產  領導  推銷 "WHT" ｜  "NOR"暴擊  防暴  反擊  瞬傷  連攻  刺棘 "WHT"｜" });	
	msgs += ({ sprintf("｜ "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  ｜  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%"WHT" ｜", ob->query_all_buff(BUFF_TRADE_ABILITY),ob->query_all_buff(BUFF_COLLECT_ABILITY),ob->query_all_buff(BUFF_GROW_ABILITY),ob->query_all_buff(BUFF_LEAD_ABILITY),ob->query_all_buff(BUFF_SALE_ABILITY),(ob->query_skill_level("fatalblow")/2 + ob->query_all_buff(BUFF_FATAL_CHANCE)), ob->query_all_buff(BUFF_ANTIFATAL_CHANCE), ob->query_all_buff(BUFF_COUNTERATTACK_CHANCE), ob->query_all_buff(BUFF_FATAL_POWER), ob->query_all_buff(BUFF_COMBO_CHANCE), ob->query_all_buff(BUFF_THORN_CHANCE)) });
	if( sizeof(dbase["condition"]) )
	{
		object condition_ob;	
		msgs += ({ "｜                                                                      ｜" });
		i = 0;
		foreach( string condition, mapping data in dbase["condition"] )
		{
			condition_ob = load_object(replace_string(condition,"#", "/"));	
			i++;
			
			if( i%2 )
			{
				msg = sprintf("%-18s%-4s%11s", condition_ob->query_condition_name()+(data["stack"]>1?WHT" x "HIW+data["stack"]+NOR:""), condition_ob->query_condition_type(), query_buff_time(data["time"]) );
				if( i == sizeof(dbase["condition"]) )
					msgs += ({ "｜ "+msg+WHT"｜                                  ｜" });
			}
			else
			{
				msg += sprintf(WHT"｜%-18s%-4s%11s", condition_ob->query_condition_name()+(data["stack"]>1?WHT" x "HIW+data["stack"]+NOR:""), condition_ob->query_condition_type(), query_buff_time(data["time"]) );
				msgs += ({ "｜ "+msg+WHT" ｜" });
			}
		}
	}
	msgs += ({ "├──────────────────────────────── "HIY"金"NOR YEL"融 "WHT"┤" });
	msgs += ({ "｜ "HIY"現"NOR YEL"金                                                                 "WHT"｜" });
	
	if( mapp(moneydata) )
	{
		cash_info = moneydata["cash"];
		bank_info = moneydata["bank"];
	}

	if( mapp(cash_info) && sizeof(cash_info) && ob->is_user_ob() )
	{
		foreach( string unit in sort_array(keys(cash_info), 1) )
		{
			if( !MONEY_D->money_unit_exist(unit) )
			{
				MONEY_D->clear_money(id, unit);
				continue;
			}
			msgs += ({ sprintf("｜   "HIY"$%-2s %-63s"WHT"｜", unit, NUMBER_D->number_symbol(cash_info[unit])) });
		}
	}
	else
		msgs += ({ "｜   "NOR"身上沒有任何現金                                                   "WHT"｜" });

	if( mapp(bank_info) && sizeof(bank_info) && ob->is_user_ob() )
	{
		foreach( string bankfile, mixed data in bank_info )
		{
			if( !file_exist(bankfile+".o") || !objectp(room = load_object(bankfile)) )
			{
				//delete("banks/"+bankfile, ob);
				continue;
			}
	
			msgs += ({ sprintf("｜ "NOR"%-69s"WHT"｜", room->query_room_name() + (MONEY_D->is_bank_autotransfer(id, bankfile)?NOR GRN" ("HIG"自動轉帳"NOR GRN")"NOR:"") ) });
	
			data = query("savings/"+id, room);
	
			if( mapp(data) && mapp(data = data["money"]) )
			{
				foreach( string unit, string money in data )
				{
					if( !MONEY_D->money_unit_exist(unit) )
					{
						delete("savings/"+id+"/money/"+unit, room);
						continue;
					}
					msgs += ({ sprintf("｜   "HIY"$%s %-62s "WHT"｜", unit, NUMBER_D->number_symbol(money)) });
				}
			}
		}
	}
	else
		msgs += ({ "｜   "NOR"銀行沒有任何存款                                                   "WHT"｜" });

	msgs += ({ "｜ "HIY"股"NOR YEL"票                                                                 "WHT"｜" });
	
	if( sizeof(dbase["stock"]) )
	{
		int num;
		mapping stocks = STOCK_D->query_stocks();
		float ownstockvalue, curstockvalue, gain;
		string stockcolor, stockdesc;

		foreach(string stock, mapping data in dbase["stock"])
		{
			num = to_int(stock);
			ownstockvalue = data["value"];
			if( undefinedp(stocks[num]) ) continue;
			curstockvalue = stocks[num]["收盤"];

			gain = (curstockvalue - ownstockvalue)*100. / ownstockvalue;

			if( stocks[num]["漲跌"] < 0 )
			{
				stockcolor = HIG;

				if( stocks[num]["狀態"] == "t3g2" )
					stockcolor += BGRN;

				stockdesc = " ▼"+sprintf("%.2f", -stocks[num]["漲跌"]);
			}
			else if( stocks[num]["漲跌"] > 0 )
			{
				stockcolor = HIR;

				if( stocks[num]["狀態"] == "t3r2" )
					stockcolor += BRED;

				stockdesc = " ▲"+sprintf("%.2f", stocks[num]["漲跌"]);
			}
			else
			{
				stockcolor = HIW;
				stockdesc = " ─";
			}

			msgs += ({ sprintf("｜ "NOR"%-12s%12s %-12s%-19s%12s"WHT" ｜", HIM+num + NOR" "HIW +stocks[num]["股票名稱"]+NOR, WHT +NUMBER_D->number_symbol(data["amount"]) +NOR" 張", NOR YEL"持股 "HIW+sprintf("%.2f", ownstockvalue||0.0)+NOR, NOR YEL"收盤 "+stockcolor+sprintf("%6.2f", curstockvalue||0.0)+stockdesc+NOR, NOR YEL"利潤 "+(curstockvalue>ownstockvalue?HIR:curstockvalue<ownstockvalue?HIG:HIW)+(gain>0?"+":gain==0.?" ":"")+sprintf("%.2f", gain||0.0)+"%"+NOR ) });
		}

	}
	else
		msgs += ({ "｜   "NOR"手上沒有持有任何股票                                               "WHT"｜" });

	msgs += ({ sprintf("｜ "NOR"%-69s"WHT"｜", HIY"期"NOR YEL"貨"NOR) });

	futures_positions = FUTURES_D->query_positions(id);
	
	if( sizeof(futures_positions) )
	{
		array futures_list = FUTURES_D->query_futures();
		array futures;
		float gain;

		foreach(int num, mapping data in futures_positions)
		{
			futures = futures_list[num];
			
			if( data["amount"] > 0 )
			{
				gain = ((futures[FUTURES_PRICE] - data["price"]) * to_int(futures[FUTURES_VALUE]*100) - (fund_multiplier(dbase["total_futures_gain"])*futures[FUTURES_PROCEDUREFUND]*100)) / futures[FUTURES_MARGIN];
			}
			else
			{
				gain = ((data["price"] - futures[FUTURES_PRICE]) * to_int(futures[FUTURES_VALUE]*100) - (fund_multiplier(dbase["total_futures_gain"])*futures[FUTURES_PROCEDUREFUND]*100)) / futures[FUTURES_MARGIN];
			}
				
			msgs += ({ sprintf("｜ "HIM"%-3d"HIW"%-10s"NOR" %2s%6s 口 "NOR YEL"持有 "HIY"%8.2f"NOR YEL" 現價 "HIY"%8.2f "NOR YEL"損益%8s%%"WHT" ｜", 
				futures[FUTURES_NUM], 
				futures[FUTURES_NAME], 
				data["amount"] > 0 ? RED"買 " : GRN"賣 ",
				data["amount"] > 0 ? HIR+data["amount"]+NOR : HIG+abs(data["amount"])+NOR,
				data["price"] || 0., 
				futures[FUTURES_PRICE] || 0.,
				futures[FUTURES_PRICE] <= 0 ? "未更新" : (gain > 0 ? HIR"+" : gain == 0. ? HIW : HIG) + sprintf("%.2f", gain),
			) });
		}
	}
	else
		msgs += ({ "｜   [m手上沒有持有任何期貨部位                                           "WHT"｜" });
		
	msgs += ({ sprintf("｜ %68s "WHT"｜", (dbase["total_futures_gain"] != 0 ? NOR YEL"當月損益 "HIY+money(DEFAULT_MONEY_UNIT, dbase["total_futures_gain"]) : "") + " / " + (dbase["total_futures_gain_history"] != 0 ? NOR YEL"歷史損益 "HIY+money(DEFAULT_MONEY_UNIT, dbase["total_futures_gain_history"]) : "")) });

	msgs += ({ sprintf("｜ "HIC"戰"NOR CYN"爭"HIC"勳"NOR CYN"章 "HIY"%-10s                                                  "WHT"｜", NUMBER_D->number_symbol(dbase["battle_insigne"])) });
	msgs += ({ HIW"╰───────────────────────────"+(STANCE_D->query_stance_name(query("combat/stance/type", ob))||"──")+"─"+CITY_D->query_age_name(dbase["city"])+"─╯"NOR });

	return implode(msgs, "\n")+"\n";
}

string score_simple(object ob)
{
	return sprintf("身體狀態："HIG"體力[%d/%d]"NOR"、"HIR"生命[%d/%d]"NOR"、"HIB"精神[%d/%d]"NOR"。\n"
	    ,ob->query_stamina_cur(), ob->query_stamina_max()
	    ,ob->query_health_cur(), ob->query_health_max()
	    ,ob->query_energy_cur(), ob->query_energy_max()); 
}

private void do_command(object me, string arg)
{
	object ob;
	string msg, arg2;

	if( arg && arg != "s" && arg != "m" )
	{
		if( objectp(ob = present(arg)) && ob->is_module_npc() )
		{
			if( query("boss", ob) != me->query_id(1) && !wizardp(me) )
				return tell(me, ob->query_idname()+"並不是"+pnoun(2, me)+"雇用的勞工。\n");
		}
		else if( !objectp(ob = find_player(arg))  )
		{
			if( sscanf(arg, "%s %s", arg, arg2) == 2 )
			{
				if( objectp(ob = find_player(arg)) )
					arg = arg2;
			}
		}
	}

	if( !objectp(ob) ) 
		ob = me;	

	if( ob != me && userp(ob) && !wizardp(me) )
	{
		if( query_temp("score/"+ob->query_id(1), me) + 300 > time() )
			return tell(me, pnoun(2, me)+"不久前才查詢過"+ob->query_idname()+"的角色資料，必須再等一陣子才能再進行查詢。\n");

		if( wizardp(ob) )
			tell(ob, me->query_idname()+"正在查詢"+pnoun(2, ob)+"的角色狀態。\n");
		else if( CITY_D->city_exist(query("city", me)) )
			tell(ob, "某位"+CITY_D->query_city_idname(query("city", me))+"的玩家正在查詢"+pnoun(2, ob)+"的角色狀態。\n");
		else
			tell(ob, "某位無市籍的玩家正在查詢"+pnoun(2, ob)+"的角色狀態。\n");

		set_temp("score/"+ob->query_id(1), time(), me);
	}

	switch(arg)
	{
		case "s":
		case "simple":
			msg = score_simple(ob);
			break;
		default:
			msg = score_standard(ob);
			break;
	}

	tell(me, msg);
}
