/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : score.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-6-29
 * Note   : score «ü¥O
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
¬d¸ß¨¤¦âª¬ºA
    
score		- ¬d¸ß¨¤¦â°ò¥»ª¬ºA
score s		- §Ö³t¬d¸ß¯à¤O­È(§Y hp «ü¥O)
score m 	- ¬d¸ß¨¤¦â²{ª÷»P¦s´Ú(§Y money «ü¥O)
score 'ID'	- ¬d¸ß¨ä¥L½u¤Wª±®a¸ê®Æ

score ¥i¨Ï¥Î¨t²ÎÁY¼g sc

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

	string mycity = CITY_D->query_city_name(dbase["city"])||"µL°êÂÇ";

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
			mycity = HIY"¬y"NOR YEL"®ö"NOR;
	}

	msgs += ({ HIW"¢~¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢¡" });
	msgs += ({ sprintf(WHT"¡U"NOR"%-70s"WHT"¡U", "¡i"+mycity+"¡j"+(ENTERPRISE_D->query_enterprise_color_id(dbase["enterprise"])||"")+(dbase["title"]?" "+dbase["title"]:"")+(dbase["nickname"]?" "+dbase["nickname"]:"")+" "+ob->query_idname()) });
	msgs += ({ "¢u¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w "HIG"¨¤"NOR GRN"¦â "WHT"¢t" });
	msgs += ({ sprintf("¡U"NOR" ¤O¶q "WHT"Str %-10s"WHT"¡U"NOR"¦û»â %-9d§ðÀ» %-10s­¹ª« %-14s"WHT"¡U", ob->query_str()+bonus(ob->query_all_buff(BUFF_STR)), dbase["area_influence"], COMBAT_D->attack_value(ob)+(attackbuff!=0?NOR RED" "+(attackbuff>0?"+":"")+HIR+attackbuff+NOR:""), stat_symbol(ob->query_food_cur(), ob->query_food_max())+bonus(ob->query_all_buff(BUFF_FOOD_MAX))) });
	msgs += ({ sprintf("¡U"NOR" Åé®æ "WHT"Phy %-10s"WHT"¡U"NOR"Án±æ %-9d¨¾¿m %-10s¶¼¤ô %-14s"WHT"¡U", ob->query_phy()+bonus(ob->query_all_buff(BUFF_PHY)), dbase["prestige"], COMBAT_D->defend_value(ob)+(defendbuff!=0?NOR CYN" "+(defendbuff>0?"+":"")+HIC+defendbuff+NOR:""), stat_symbol(ob->query_drink_cur(), ob->query_drink_max())+bonus(ob->query_all_buff(BUFF_DRINK_MAX))) });
	msgs += ({ sprintf("¡U"NOR" ´¼°Ó "WHT"Int %-10s"WHT"¡U"NOR"À»±þ %-9d³t«× %-10s¯h³Ò %-14s"WHT"¡U", myint+bonus(ob->query_all_buff(BUFF_INT)), query("combat/kill/win", ob), COMBAT_D->speed_value(ob)+(speedbuff!=0?NOR YEL" "+(speedbuff>0?"+":"")+HIY+speedbuff+NOR:""), stat_symbol(dbase["fatigue"], 1000)) });
	msgs += ({ sprintf("¡U"NOR" ±Ó±¶ "WHT"Agi %-10s"WHT"¡U"NOR"ªÀ·|¸gÅç%-8s%-32s"WHT"¡U", ob->query_agi()+bonus(ob->query_all_buff(BUFF_AGI)), bonus(mysocialexpbuff, "%"), ob->query_social_exp_cur()+" / "+ob->query_social_exp_tot()) });
	msgs += ({ sprintf("¡U"NOR" ¾y¤O "WHT"Cha %-10s"WHT"¡U"NOR"¾Ô°«¸gÅç%-8s%-32s"WHT"¡U", ob->query_cha()+bonus(ob->query_all_buff(BUFF_CHA)), bonus(mycombatexpbuff, "%"), ob->query_combat_exp_cur()+" / "+ob->query_combat_exp_tot()) });
	msgs += ({ "¡U                                                                      ¡U" });
	msgs += ({ sprintf("¡U"NOR" Åé¤O %5d / %-11s%-40s%-5s"WHT"¡U", mystaminacur, mystaminamax+bonus(mystaminamaxbuff), GRAPH_D->graph(mystaminacur,mystaminamax,2,0,20), regen(ob->query_all_buff(BUFF_STAMINA_REGEN))  ) });
	msgs += ({ sprintf("¡U"NOR" ¥Í©R %5d / %-11s%-40s%-5s"WHT"¡U", myhealthcur , myhealthmax+bonus(myhealthmaxbuff), GRAPH_D->graph(myhealthcur,myhealthmax,1,0,20), regen(ob->query_all_buff(BUFF_HEALTH_REGEN))) });
	msgs += ({ sprintf("¡U"NOR" ºë¯« %5d / %-11s%-40s%-5s"WHT"¡U", myenergycur , myenergymax+bonus(myenergymaxbuff), GRAPH_D->graph(myenergycur,myenergymax,4,0,20), regen(ob->query_all_buff(BUFF_ENERGY_REGEN))) });
	msgs += ({ "¡U                                                                      ¡U" });
	msgs += ({ "¡U"NOR" ¶T©ö  ±Ä¶°  ¥Í²£  »â¾É  ±À¾P "WHT" ¡U  "NOR"¼ÉÀ»  ¨¾¼É  ¤ÏÀ»  Àþ¶Ë  ³s§ð  ¨ë´Æ "WHT"¡U" });	
	msgs += ({ sprintf("¡U "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  ¡U  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%  "HIW"%3d"WHT"%%"WHT" ¡U", ob->query_all_buff(BUFF_TRADE_ABILITY),ob->query_all_buff(BUFF_COLLECT_ABILITY),ob->query_all_buff(BUFF_GROW_ABILITY),ob->query_all_buff(BUFF_LEAD_ABILITY),ob->query_all_buff(BUFF_SALE_ABILITY),(ob->query_skill_level("fatalblow")/2 + ob->query_all_buff(BUFF_FATAL_CHANCE)), ob->query_all_buff(BUFF_ANTIFATAL_CHANCE), ob->query_all_buff(BUFF_COUNTERATTACK_CHANCE), ob->query_all_buff(BUFF_FATAL_POWER), ob->query_all_buff(BUFF_COMBO_CHANCE), ob->query_all_buff(BUFF_THORN_CHANCE)) });
	if( sizeof(dbase["condition"]) )
	{
		object condition_ob;	
		msgs += ({ "¡U                                                                      ¡U" });
		i = 0;
		foreach( string condition, mapping data in dbase["condition"] )
		{
			condition_ob = load_object(replace_string(condition,"#", "/"));	
			i++;
			
			if( i%2 )
			{
				msg = sprintf("%-18s%-4s%11s", condition_ob->query_condition_name()+(data["stack"]>1?WHT" x "HIW+data["stack"]+NOR:""), condition_ob->query_condition_type(), query_buff_time(data["time"]) );
				if( i == sizeof(dbase["condition"]) )
					msgs += ({ "¡U "+msg+WHT"¡U                                  ¡U" });
			}
			else
			{
				msg += sprintf(WHT"¡U%-18s%-4s%11s", condition_ob->query_condition_name()+(data["stack"]>1?WHT" x "HIW+data["stack"]+NOR:""), condition_ob->query_condition_type(), query_buff_time(data["time"]) );
				msgs += ({ "¡U "+msg+WHT" ¡U" });
			}
		}
	}
	msgs += ({ "¢u¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w "HIY"ª÷"NOR YEL"¿Ä "WHT"¢t" });
	msgs += ({ "¡U "HIY"²{"NOR YEL"ª÷                                                                 "WHT"¡U" });
	
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
			msgs += ({ sprintf("¡U   "HIY"$%-2s %-63s"WHT"¡U", unit, NUMBER_D->number_symbol(cash_info[unit])) });
		}
	}
	else
		msgs += ({ "¡U   "NOR"¨­¤W¨S¦³¥ô¦ó²{ª÷                                                   "WHT"¡U" });

	if( mapp(bank_info) && sizeof(bank_info) && ob->is_user_ob() )
	{
		foreach( string bankfile, mixed data in bank_info )
		{
			if( !file_exist(bankfile+".o") || !objectp(room = load_object(bankfile)) )
			{
				//delete("banks/"+bankfile, ob);
				continue;
			}
	
			msgs += ({ sprintf("¡U "NOR"%-69s"WHT"¡U", room->query_room_name() + (MONEY_D->is_bank_autotransfer(id, bankfile)?NOR GRN" ("HIG"¦Û°ÊÂà±b"NOR GRN")"NOR:"") ) });
	
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
					msgs += ({ sprintf("¡U   "HIY"$%s %-62s "WHT"¡U", unit, NUMBER_D->number_symbol(money)) });
				}
			}
		}
	}
	else
		msgs += ({ "¡U   "NOR"»È¦æ¨S¦³¥ô¦ó¦s´Ú                                                   "WHT"¡U" });

	msgs += ({ "¡U "HIY"ªÑ"NOR YEL"²¼                                                                 "WHT"¡U" });
	
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
			curstockvalue = stocks[num]["¦¬½L"];

			gain = (curstockvalue - ownstockvalue)*100. / ownstockvalue;

			if( stocks[num]["º¦¶^"] < 0 )
			{
				stockcolor = HIG;

				if( stocks[num]["ª¬ºA"] == "t3g2" )
					stockcolor += BGRN;

				stockdesc = " ¡¿"+sprintf("%.2f", -stocks[num]["º¦¶^"]);
			}
			else if( stocks[num]["º¦¶^"] > 0 )
			{
				stockcolor = HIR;

				if( stocks[num]["ª¬ºA"] == "t3r2" )
					stockcolor += BRED;

				stockdesc = " ¡¶"+sprintf("%.2f", stocks[num]["º¦¶^"]);
			}
			else
			{
				stockcolor = HIW;
				stockdesc = " ¢w";
			}

			msgs += ({ sprintf("¡U "NOR"%-12s%12s %-12s%-19s%12s"WHT" ¡U", HIM+num + NOR" "HIW +stocks[num]["ªÑ²¼¦WºÙ"]+NOR, WHT +NUMBER_D->number_symbol(data["amount"]) +NOR" ±i", NOR YEL"«ùªÑ "HIW+sprintf("%.2f", ownstockvalue||0.0)+NOR, NOR YEL"¦¬½L "+stockcolor+sprintf("%6.2f", curstockvalue||0.0)+stockdesc+NOR, NOR YEL"§Q¼í "+(curstockvalue>ownstockvalue?HIR:curstockvalue<ownstockvalue?HIG:HIW)+(gain>0?"+":gain==0.?" ":"")+sprintf("%.2f", gain||0.0)+"%"+NOR ) });
		}

	}
	else
		msgs += ({ "¡U   "NOR"¤â¤W¨S¦³«ù¦³¥ô¦óªÑ²¼                                               "WHT"¡U" });

	msgs += ({ sprintf("¡U "NOR"%-69s"WHT"¡U", HIY"´Á"NOR YEL"³f"NOR) });

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
				
			msgs += ({ sprintf("¡U "HIM"%-3d"HIW"%-10s"NOR" %2s%6s ¤f "NOR YEL"«ù¦³ "HIY"%8.2f"NOR YEL" ²{»ù "HIY"%8.2f "NOR YEL"·l¯q%8s%%"WHT" ¡U", 
				futures[FUTURES_NUM], 
				futures[FUTURES_NAME], 
				data["amount"] > 0 ? RED"¶R " : GRN"½æ ",
				data["amount"] > 0 ? HIR+data["amount"]+NOR : HIG+abs(data["amount"])+NOR,
				data["price"] || 0., 
				futures[FUTURES_PRICE] || 0.,
				futures[FUTURES_PRICE] <= 0 ? "¥¼§ó·s" : (gain > 0 ? HIR"+" : gain == 0. ? HIW : HIG) + sprintf("%.2f", gain),
			) });
		}
	}
	else
		msgs += ({ "¡U   [m¤â¤W¨S¦³«ù¦³¥ô¦ó´Á³f³¡¦ì                                           "WHT"¡U" });
		
	msgs += ({ sprintf("¡U %68s "WHT"¡U", (dbase["total_futures_gain"] != 0 ? NOR YEL"·í¤ë·l¯q "HIY+money(DEFAULT_MONEY_UNIT, dbase["total_futures_gain"]) : "") + " / " + (dbase["total_futures_gain_history"] != 0 ? NOR YEL"¾ú¥v·l¯q "HIY+money(DEFAULT_MONEY_UNIT, dbase["total_futures_gain_history"]) : "")) });

	msgs += ({ sprintf("¡U "HIC"¾Ô"NOR CYN"ª§"HIC"¾±"NOR CYN"³¹ "HIY"%-10s                                                  "WHT"¡U", NUMBER_D->number_symbol(dbase["battle_insigne"])) });
	msgs += ({ HIW"¢¢¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w¢w"+(STANCE_D->query_stance_name(query("combat/stance/type", ob))||"¢w¢w")+"¢w"+CITY_D->query_age_name(dbase["city"])+"¢w¢£"NOR });

	return implode(msgs, "\n")+"\n";
}

string score_simple(object ob)
{
	return sprintf("¨­Åéª¬ºA¡G"HIG"Åé¤O[%d/%d]"NOR"¡B"HIR"¥Í©R[%d/%d]"NOR"¡B"HIB"ºë¯«[%d/%d]"NOR"¡C\n"
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
				return tell(me, ob->query_idname()+"¨Ã¤£¬O"+pnoun(2, me)+"¶±¥Îªº³Ò¤u¡C\n");
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
			return tell(me, pnoun(2, me)+"¤£¤[«e¤~¬d¸ß¹L"+ob->query_idname()+"ªº¨¤¦â¸ê®Æ¡A¥²¶·¦Aµ¥¤@°}¤l¤~¯à¦A¶i¦æ¬d¸ß¡C\n");

		if( wizardp(ob) )
			tell(ob, me->query_idname()+"¥¿¦b¬d¸ß"+pnoun(2, ob)+"ªº¨¤¦âª¬ºA¡C\n");
		else if( CITY_D->city_exist(query("city", me)) )
			tell(ob, "¬Y¦ì"+CITY_D->query_city_idname(query("city", me))+"ªºª±®a¥¿¦b¬d¸ß"+pnoun(2, ob)+"ªº¨¤¦âª¬ºA¡C\n");
		else
			tell(ob, "¬Y¦ìµL¥«Äyªºª±®a¥¿¦b¬d¸ß"+pnoun(2, ob)+"ªº¨¤¦âª¬ºA¡C\n");

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
