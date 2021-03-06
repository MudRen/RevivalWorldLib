/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : npc_suphia.c
 * Author : tzj@RevivalWorld
 * Date   : 2006-09-0
 * Note   : 
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	系統預設之 NPC 反應函式
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <quest.h>

inherit STANDARD_PERSON;

void do_command(string cmd);
void make_money();

void set_behavior()
{
	set("behavior/random", 
	([
		(: do_command("stare 遙遠的東方") :)      	: 10,
		(: do_command("ponder 班傑明... 哥哥") :) 	:  5,
	]));
}
void make_money()
{

}
void delete_behavior()
{

}

void reply_say(object me, string msg)
{
	
	if( objectp(me) && msg && strsrch(msg, "飯") != -1 )
		do_command("say 嗯... 廚房正在煮菜，"+ pnoun(2, me) +"要一起吃嗎？");
}

void reply_emote(object me, string verb, string args)
{
	
	//quest相關問答
 	if( verb == "ask" )
 	{
/*
 		if(args == "")
 			do_command("nod" + " " + me->query_id(1) );
 			
 		if( strsrch(args, "東方") >= 0 )
 		{
	 		if( strsrch(args, "有什麼") >= 0 )
	 		{
	 			do_command("say 呃... 嗯... 其實沒有什麼重要的∼請別在意我...");
	 			msg("$ME似乎不願意將話題繼續下去... \n",this_object(), 0, 1);
	 		}

	 		else if ( strsrch(args, "班傑明") >= 0 )
	 		{
	 			do_command("say 是啊... 班傑明哥哥大概在前往東方的路途上吧... ");
	 		}
	 		else if ( strsrch(args, "溫泉") >= 0 )
	 		{
	 			do_command("say 有聽說過在很遠的東方，似乎有個著名溫泉的樣子？可是我沒去過... ");
	 			msg("$ME低聲喃喃自語道: " + HIG + "要是能去就好了... \n" + NOR ,this_object(), 0, 1);
	 		}
	 		else	
	 		{
	 			switch(random(5))
	 			{
	 				case 0:
	 					do_command("?" + " " + me->query_id(1) );
	 					do_command("say 嗯... ？"+ pnoun(2, me) +"是想問我東方有什麼嗎？");
	 					break;
	 				case 1:
	 				case 2:
	 					do_command("say 東方怎樣了？");
	 					break;
	 				case 3:
	 					do_command("? 東方");
	 					break;
	 				case 4:
	 					do_command("tense" + " " + me->query_id(1) + " " + "東方傳來什麼事嗎！？");
	 					break;
	 			}
	 		}
	 	}
	 	else if ( strsrch(args, "班傑明" ) >= 0 )
	 	{
	 		if( (strsrch(args, "是") >= 0 ) && (strsrch(args,"誰") >= 0 ) )
 			{
				do_command("say 班傑明哥哥是村子裡的名人哦！聽說他在幫人蓋溫泉旅館... ");
 				msg("$ME很高興地說完之後，表情卻看起來得有些落寞。\n",this_object(), 0, 1);
 			}
 			else if( strsrch(args, "班傑明溫泉") >= 0 )
 			{
 				do_command("say" + " " + "那是什麼怪溫泉名啊！");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			else if( strsrch(args, "班傑明旅館") >= 0 )
 			{
 				do_command("say" + " " + "那是什麼怪旅館名啊！");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			
 			else if( (strsrch(args, "溫泉") >= 0 ) || (strsrch(args,"旅館") >= 0 ) )
 			{
				do_command("say 是有聽說班傑明哥哥在幫人蓋溫泉旅館... ");
 			}
 			else
 			{
 				switch(random(10))
 				{
 					case 0:
 					case 1:
 					case 2:
 						do_command("hand" + " " +  me->query_id(1) + " " + pnoun(2, me) + "認識班傑明哥哥嗎？");
 						break;
 					case 3:
 						do_command("peer" + " " + me->query_id(1));
 						do_command("say" + " " + pnoun(2, me) + "知道班傑明哥哥是誰嗎？");
 						break;
 					case 4:
 					case 5:
 						do_command("boggle" + " " + me->query_id(1));
 						do_command("say" + " "+ "啊！我的自言自由被"+ pnoun(2, me) +"聽到啦！？");
 						break;
 					case 6:
 					case 7:
 						msg("$ME突然臉紅了起來，但什麼也沒說。\n",this_object(), 0, 1);
 						break;
 					case 8:
 					case 9:
 						msg("$ME似乎陷入沉思之中，不發一語。\n",this_object(), 0, 1);
 						break;
 				}

  			}

	 	}
	 	else if ( strsrch(args, "溫泉" ) >= 0 )
	 	{
	 		if ( strsrch(args, "旅館" ) >= 0 )
	 		{
	 			do_command("pout");
	 			do_command("say" + " " + "什麼溫泉旅館嘛，我最討厭溫泉旅館了！... 啊，抱歉，失禮了。");	 		
	 		}
	 		else if ( (strsrch(args, "哪" ) >= 0) || (strsrch(args, "何處" ) >= 0) )
	 		{
	 			do_command("hmm" + " " +me->query_id(1));
	 			do_command("say" + " " + "我不太清楚溫泉的事，但有聽說過似乎東方有個著名溫泉的樣子？");	 		
	 		}
	 		else
	 		{
		 		do_command("?" + " " + me->query_id(1));
		 		do_command("say" + " "+ "嗯... ？" + pnoun(2, me) + "是想問我哪裡有溫泉嗎？？");
	 		}
	 	}
*/
 	}
 	else
 	{
 		switch(verb)
 		{
 			case "dunno"	:
 			case "sob"	:
 			case "cry"	:
 			case "inn"	:
 				switch(random(2))
 				{
 					case 0:
		 				do_command("pat" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("comfort" + " " + me->query_id(1));
		 				break;
 				}
 				break;
 				
 			case "gan"	:
 			case "fuck"	:
 				switch(random(3))
 				{
 					case 0:
		 				do_command("?" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("inn" + " " + me->query_id(1) + " " + "為什麼要兇我...");
		 				break;
		 			case 2:
		 				do_command("twiddle");
		 				do_command("say" + " " + "我讓你不高興了？");
		 				break;
 				}
  				break;
 
  			case "bonk"	:
  			case "kick"	:
 			case "flog"	:
 			case "slap"	:
				switch(random(3))
 				{
 					case 0:
		 				do_command("sob" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("inn" + " " + me->query_id(1) + " " + "好痛哦...");
		 				break;
		 			case 2:
		 				do_command("draw");
		 				do_command("say" + " " + "我做錯了什麼嗎？");
		 		}		break;
  				break;

  			case "cat"	:
  			case "smile"	:
  			case "hi"	:
  				switch(random(3))
  				{
  					case 0:
  						do_command("hug" + " " + me->query_id(1));
  						break;
  					case 1:
  						do_command("smile" + " " + me->query_id(1));
  						break;
  					case 2:
  						do_command("cuddle" + " " + me->query_id(1));
  						break;
  				}
  				break;
  			
  			case "good"	:
  			case "clap"	:
  			case "cheer"	:
				switch(random(4))
  				{
  					case 0:
  						do_command("blush");
  						break;
  					case 1:
  						do_command("ya" + " " + me->query_id(1));
  						break;
  					case 2:
  						do_command("thank" + " " + me->query_id(1));
  						break;
  					case 3:
  						do_command("bow" + " " + me->query_id(1));
  						break;
  				}
  				break;

  			case "hi"	:
  				do_command("hi" + " " + me->query_id(1));
  				break;

 		}
 	}
}

int target_gone(object me)
{

}

void reply_get_object(object me, object ob, mixed amount)
{

}

void reply_get_money(object me, string unit, mixed money)
{

}

void create()
{
	set_idname("suphia","蘇菲亞");
	
	set("nickname", "天真迷糊");

	set("unit", "位");
	
	set("age", 17);

	set("gender", FEMALE_GENDER);

	set("long",@TEXT
　　眼前的少女年紀約在十七歲，黑色眼眸棕色短髮，稚氣未脫的臉上有些許汗
珠。她身穿著簡樸的農家女常穿的衣服，坐在椅子上似乎若有所思的望向遠遠的
東方。
TEXT);

	set("heartbeat", 1); // 永久性心跳
		
	set("no_fight", 1);

	// 啟動主動行為
	set_behavior();

	startup_living();
}
