/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : top.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-15
 * Note   : top 指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <skill.h>
#include <map.h>
#include <money.h>

inherit COMMAND;

string help = @HELP
列出各種排行榜：

top 選項		- 預設列出世界前 30 名列表
top 選項 100 		- 列出世界前 100 名列表
top 選項 all		- 列出全部排名(最多 1000 名)

項目如下

經濟
top rich		- 世界富豪排行榜(現金＋房地產總值)
top cash		- 世界現金排行榜
top estate		- 世界房地產排行榜
top product		- 世界產品產值排行榜
top store		- 世界最佳商店排行榜
top building		- 世界高樓建築排行榜
top enterprise		- 世界企業排行榜
top stock		- 世界股王排行榜
top maintain		- 世界土地等級排行榜
top city		- 世界城市繁榮排行榜
top futures		- 世界期貨損益排行榜

戰鬥
top killnpc		- 世界擊殺數量排行榜
top killplayer		- 世界擊殺玩家排行榜
top battlescore		- 世界戰場功績排行榜
top attack		- 世界攻擊力排行榜
top defend		- 世界防禦力排行榜
top speed		- 世界攻擊速度排行榜
top damage		- 世界一般戰鬥最高傷害值排行榜
top battledamage	- 世界戰場戰鬥最高傷害值排行榜

角色
top skill		- 世界技能宗師排行榜
top skill 技能名	- 世界單獨技能等級排行榜
top attr 屬性名		- 世界屬性數值排行榜
top socialexp		- 世界社會經驗排行榜
top combatexp		- 世界戰鬥經驗排行榜
top quest		- 世界任務完成數量排行榜
top age			- 世界玩家登入時數排行榜
top title		- 世界玩家稱號數量排行榜
HELP;

private void do_command(object me, string arg)
{
	int i, top;
	string msg, arg1, arg2, arg3;

	if( !arg ) 
		return tell(me, help);

	if( sscanf(arg, "%s %s %s", arg1, arg2, arg3) == 3 )
	{
		if( arg3 == "all" ) 
			i = 1000;
		else 
			i = to_int(arg3);
	}
	else if( sscanf(arg, "%s %s", arg1, arg2) == 2 )
	{
		if( arg2 == "all" )
			i = 1000;
		else if( to_int(arg2) )
		{
			i = to_int(arg2);
			arg2= 0;
		}
		else
			i = 30;
	}
	else
	{
		i = 30;
		arg1 = arg;
	}

	if( i < 1 || i > 1000 )
		return tell(me, "顯示排名數量不得小於 1 或大於 1000。\n");

	switch( arg1 )
	{
		case "rich":
			{

				msg = NOR YEL"世界"HIY"前 "+i+" 富豪"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "排名", "玩家", "城市", "總資產($RW)", "紀錄");
				msg += NOR WHT"────────────────────────────────────────\n"NOR;

				foreach(array data in TOP_D->query_top("rich"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "cash":
			{

				msg = NOR YEL"世界"HIY"前 "+i+" 現金"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "排名", "玩家", "城市", "現金($RW)", "紀錄");
				msg += NOR WHT"────────────────────────────────────────\n"NOR;

				foreach(array data in TOP_D->query_top("cash"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "estate":
			{

				msg = NOR YEL"世界"HIY"前 "+i+" 房地產"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "排名", "玩家", "城市", "房地產($RW)", "紀錄");
				msg += NOR WHT"────────────────────────────────────────\n"NOR;

				foreach(array data in TOP_D->query_top("estate"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "product":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 產品產值"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"──────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-34s%-20s%-15s%10s\n"NOR, "排名", "產品", "產品產值", "開發者", "紀錄");
				msg += NOR WHT"──────────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("products"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-34s"HIY"%-20s"HIG"%-15s"NOR YEL"%10s\n"NOR,
						top,
						data[3],
						NUMBER_D->number_symbol(data[1]),
						//sizeof(data)>5 ? data[5] : 0,
						capitalize(data[4]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"──────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "store":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 最佳商店排行榜"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-31s %14s %-15s %12s\n"NOR, "排名", "商店", "銷售總數", "店長", "紀錄");
				msg += NOR WHT"────────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("storesell"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-31s"HIW" %14s"HIG" %-15s"NOR YEL" %12s\n"NOR,
						top,
						data[4],
						NUMBER_D->number_symbol(data[1]),
						//(CITY_D->query_city_id(data[5]) || ""),
						capitalize(data[3]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "skill":
			{
				if( !arg2 )
				{
					msg = NOR YEL"世界"HIY"前 "+i+" 技能宗師"NOR YEL"排行榜：\n"NOR;
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "排名", "玩家", "城市", "神技數", "紀錄");
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill"))
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							(CITY_D->query_city_name(data[4]) || ""),
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
				}
				else
				{
					string owner;
					
					arg2 = lower_case(arg2);

					if( !SKILL_D->skill_exists(arg2) )
						return tell(me, "並沒有這種技能。\n");
					
					msg = NOR YEL"世界"HIY"前 "+i+" "+(SKILL(arg2))->query_idname()+"技能等級"NOR YEL"排行榜：\n"NOR;
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-24s %8s %12s\n"NOR, "排名", "玩家", "所屬城市/老闆", "技能等級", "紀錄");
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill/"+arg2))
					{
						if( ++top > i ) break;
						
						owner = 0;	
						if( sizeof(data) > 4 )
						{
							owner = CITY_D->query_city_name(data[4]);
							
							if( !owner )
							{
								object user = load_user(data[4]);
								
								if( objectp(user) )
								{
									owner = user->query_idname();
								
									if( !userp(user) )
										destruct(user);
								}
							}
						}

						msg += sprintf(HIW"%3d."NOR" %-28s %-24s "HIY"%8s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							owner || "",
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
					
					
				}
				return me->more(msg+"\n");
			}
			break;
		case "attr":
			{
				if( !arg2 )
					return tell(me, pnoun(2, me)+"想要查詢哪一項屬性？(strength、physique、intelligence、agility、charisma、stamina、health、energy)\n");
				else
				{
					array topdata;

					arg2 = lower_case(arg2);

					topdata = TOP_D->query_top("attr/"+arg2);
					
					if( !sizeof(topdata) )
						return tell(me, pnoun(2, me)+"想要查詢哪一項屬性？(strength、physique、intelligence、agility、charisma、stamina、health、energy)\n");
					
					msg = NOR YEL"世界"HIY"前 "+i+" "+capitalize(arg2)+" 屬性等級"NOR YEL"排行榜：\n"NOR;
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "排名", "玩家", "城市", "屬性數值", "紀錄");
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
	
					foreach(array data in topdata)
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							sizeof(data) > 4 ? (CITY_D->query_city_name(data[4]) || "") : "",
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"────────────────────────────────────────\n"NOR;
				}
				return me->more(msg+"\n");
			}
			break;
		case "building":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 高樓建築"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				msg += sprintf(HIW"%4s %-14s %-32s %-10s %4s %10s\n"NOR, "排名", "玩家", "商店", "城市", "樓層", "紀錄");
				msg += NOR WHT"────────────────────────────────────────\n"NOR;

				foreach(array data in TOP_D->query_top("building"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."HIY" %-14s "NOR"%-32s %-10s "HIG"%4s"NOR YEL" %10s\n"NOR,
						top,
						data[3],
						data[4],
						(CITY_D->query_city_name(data[5]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"────────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;

		case "stock":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 股王"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          持有股票現值        紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("stockvalue") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;

		case "quest":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 任務完成"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          任務完成數量        紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("quest") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "enterprise":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 企業"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 企業                     資產總值		               紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("stock") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s"HIY"%-22s"NOR YEL"%s\n"NOR,
						top,
						data[2],
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "socialexp":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 社會經驗總值"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          社會經驗總值        紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("social_exp") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "combatexp":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 戰鬥經驗總值"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          戰鬥經驗總值        紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_exp") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "maintain":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 土地等級"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                      座標                 種類 等級   紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("maintain") )
				{
					if( ++top > i ) break;
						
					switch(data[4])
					{
						case FARM: arg1 = HIG"農"NOR GRN"田"NOR; break;
						case PASTURE: arg1 = HIY"牧"NOR YEL"場"NOR; break;
						case FISHFARM: arg1 = HIB"養"NOR BLU"殖"NOR; break;
					}
					msg += sprintf(HIW"%-3d."NOR" %-25s%-22s%4s"HIR"%5s"NOR YEL" %s\n"NOR,
						top,
						data[3],
						loc_short(restore_variable(data[0])),
						arg1,
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "killnpc":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 擊殺電腦角色數量"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          擊殺數量            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_kill_win") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "killplayer":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 擊殺玩家數量"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          擊殺數量            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_kill_winplayer") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "battlescore":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 戰場功績"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          戰場功績            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_battle_score") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "attack":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 攻擊能力"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          攻擊能力            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_attack") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "defend":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 防禦能力"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          防禦能力            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_defend") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "speed":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 攻擊速度"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          攻擊速度            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_speed") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
		case "city":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 城市"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 城市                     規模     繁榮度     居民數        紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("city") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-9s"HIG"%-11s"HIW"%-12s"NOR YEL"%s\n"NOR,
						top,
						CITY_D->query_city_name(data[3], data[4]),
						CITY_D->query_city_scale(data[3], data[4]),
						NUMBER_D->number_symbol(data[1]),
						NUMBER_D->number_symbol(CITY_D->query_section_info(data[3], data[4], "resident")),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
		case "age":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 登入時數"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     登入時數                          紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("age") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s"HIG"%-32s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						FINGER_D->query_time_long(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "damage":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 一般戰鬥最高傷害"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          最高傷害            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("combat_max_damage") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "battledamage":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 戰場戰鬥最高傷害"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          最高傷害            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("battle_max_damage") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "title":
			{
				msg = NOR YEL"世界"HIY"前 "+i+" 稱號數量"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                     城市          稱號數量            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("title") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "futures":
			{
				int totalgain = 0;
				
				msg = NOR YEL"世界"HIY"前 "+i+" 當月期貨損益"NOR YEL"排行榜：\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg +=     HIW"排名 玩家                    城市       當月期貨損益            紀錄時間\n"NOR;
				msg += NOR WHT"────────────────────────────────────\n"NOR;

				foreach( array data in TOP_D->query_top("futures") )
				{
					totalgain += data[1];
					
					if( ++top <= i )
					{
						msg += sprintf(HIW"%-3d."NOR" %-24s%-11s"HIY"%-22s"NOR YEL"%s\n"NOR,
							top,
							data[3],
							CITY_D->query_city_name(data[4])||"",
							money(DEFAULT_MONEY_UNIT, data[1]),
							TIME_D->replace_ctime(data[2])[0..9]);
					}
				}
				msg += NOR WHT"────────────────────────────────────\n"NOR;
				msg += sprintf("%72s", NOR YEL"世界期貨當月損益總合 "HIY+money(DEFAULT_MONEY_UNIT, totalgain)+NOR);
				return me->more(msg+"\n");
			}
			break;
		default:
			tell(me, help);
			break;
	}
}		
