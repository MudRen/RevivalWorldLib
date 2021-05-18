/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : battle.c
 * Author : Clode@RevivalWorld
 * Date   : 2009-08-10
 * Note   : 戰場指令
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <location.h>

inherit COMMAND;

string help = @HELP
   戰場指令，可以報名與離開戰場

   battle info				- 顯示目前戰爭資訊
   battle join				- 報名參加下一場次的戰場
   battle join 驗證碼			- 填入驗證碼以正式加入戰場
   battle cancel			- 取消報名
HELP;

private void do_command(object me, string arg)
{
	//if( !wizardp(me) )
	//	return tell(me, "系統測試中，暫時無法加入戰爭。\n");

	switch(arg)
	{
		case "info":
		{
			string msg;
			string *players = BATTLEFIELD_D->get_sorted_players();
			string score = "";
			int count = 0;
			
			msg = "目前共 "+sizeof(players)+" 位玩家參與戰爭，"+(member_array(me->query_id(1), players)==-1?pnoun(2, me)+"未參與戰爭":pnoun(2, me)+"是其中之一")+"。\n";
						
			if( sizeof(players) > 0 )
			{
				int attack=0, defend=0, medic=0, speed=0, none=0;
				object player;
		
				foreach(string id in players)
				{
					if( !objectp(player = find_player(id)) )
						continue;
			
					if( ++count <= 3 )
						score += player->query_idname()+"、";
					
					switch(query("combat/stance/type", player))
					{
						case "attack":
						case "attack-adv":
							attack++;
							break;
						case "defend":
						case "defend-adv":
							defend++;
							break;
						case "speed":
						case "speed-adv":
							speed++;
							break;
						case "medic":
						case "medic-adv":
							medic++;
							break;
						default:
							none++;
							break;
					}
				}
				
				msg += "參與者採用的戰鬥姿態統計："
				+STANCE_D->query_stance_name("defend")+"["+defend+"]、"
				+STANCE_D->query_stance_name("attack")+"["+attack+"]、"
				+STANCE_D->query_stance_name("speed")+"["+speed+"]、"
				+STANCE_D->query_stance_name("medic")+"["+medic+"]、"
				+WHT"無姿態"NOR"["+none+"]。\n";
				
				msg += "目前戰爭績分排名前三名為："+score[0..<3]+"。\n";
			}
			
			return tell(me, msg);
			break;
		}
		case "join":
		{
			//string id, code;
			
			if( !BATTLEFIELD_D->is_battle_open() )
				return tell(me, "目前無法報名戰場。\n");

			if( BATTLEFIELD_D->in_battle(me) )
				return tell(me, pnoun(2, me)+"已經在戰場中了。\n");
				
			BATTLEFIELD_D->join_battle(me);
			/*	
			id = me->query_id(1);
			code = sprintf("%c%c%c%c%c", range_random(97,122), range_random(97,122), range_random(97,122), range_random(97,122), range_random(97,122));
			
			MYSQL_SCHEDULE_D->set_captcha(id, code);
			set_temp("battlefield_captcha_code", code, me);
			return tell(me, HIY"\n建立新的戰場驗證碼...請至以下網頁取得"+pnoun(2, me)+"的驗證碼：\n\n"NOR"http://www.revivalworld.org/rw/captcha?id="+id+HIG"\n\n取得驗證碼後輸入「battle join 驗證碼」以加入戰場。\n"NOR);
			*/
			break;
		}
		case "cancel":
			BATTLEFIELD_D->cancel_battle(me);
			break;
		default:
		{
			/*
			if( arg && sscanf(arg, "join %s", arg) == 1 && sizeof(arg) > 0 )
			{
				if( arg == query_temp("battlefield_captcha_code", me) )
				{
					MYSQL_SCHEDULE_D->delete_captcha(me->query_id(1));
					delete_temp("battlefield_captcha_code", me);
					BATTLEFIELD_D->join_battle(me);
					
					tell(me, HIG+"填完驗證碼時順手點一下網站上的廣告吧，RW 的設備、網路、電費是需要持續不斷的金錢支出的(泣)\n"HIY"提醒：多點無益，每日點選上面左右兩廣告各 3 次為佳，點完別急著關掉廣告頁面，停留久一點才會有效果喔(羞)\n"NOR);
				}
				else
					return tell(me, "驗證碼輸入錯誤。\n");
			}
			else*/
				return tell(me, "請輸入正確的指令格式(help battle)。\n");
		}
	}
}
