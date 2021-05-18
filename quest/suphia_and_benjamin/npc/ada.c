/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : ada.c
 * Author : tzj@RevivalWorld
 * Date   : 2007-08-17
 * Note   : 
 * Update :
 *  o 2000-00-00
 *  
 -----------------------------------------
 */

/*
	¨t²Î¹w³]¤§ NPC ¤ÏÀ³¨ç¦¡
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
		(: do_command("ponder ¦n¹³ÁÙ®t¤@ÂI") 						  :) 						: 3,
		(: msg("¿LÁçµo¥X©BÂP©BÂPªºªmÄËÁnÅT... °}°}­»¨ý¼³»ó... \n",this_object(), 0, 1)    :)	: 3,
		(: msg("»·³B¶Ç¨ÓÂû¸sªº»ï¥sÁn¡A$MEÅ¥¨ì«á¦ü¥G¦³ÂI§¤¥ßÃø¦w¡C\n",this_object(), 0, 1) :)	: 3,
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
	
	if( objectp(me) && msg && strsrch(msg, "¶º") != -1 )
		do_command("say ¶º¡H"+ pnoun(2, me) +"¬O¾j¤F¶Ü¡H§Úªº¿L¦×¥i¬O§ø¸Ì¦³¦Wªº®@¡I¡I");
}

void reply_emote(object me, string verb, string args)
{
	
	//quest¬ÛÃö°Ýµª
 	if( verb == "ask" )
 	{
/*
 		if(args == "")
 			do_command("nod" + " " + me->query_id(1) );
 			
 		if( strsrch(args, "ªF¤è") >= 0 )
 		{
	 		if( strsrch(args, "¦³¤°»ò") >= 0 )
	 		{
	 			do_command("say §c... ¶â... ¨ä¹ê¨S¦³¤°»ò­«­nªº¡ã½Ð§O¦b·N§Ú...");
	 			msg("$ME¦ü¥G¤£Ä@·N±N¸ÜÃDÄ~Äò¤U¥h... \n",this_object(), 0, 1);
	 		}

	 		else if ( strsrch(args, "¯Z³Ç©ú") >= 0 )
	 		{
	 			do_command("say ¬O°Ú... ¯Z³Ç©ú­ô­ô¤j·§¦b«e©¹ªF¤èªº¸ô³~¤W§a... ");
	 		}
	 		else if ( strsrch(args, "·Å¬u") >= 0 )
	 		{
	 			do_command("say ¦³Å¥»¡¹L¦b«Ü»·ªºªF¤è¡A¦ü¥G¦³­ÓµÛ¦W·Å¬uªº¼Ë¤l¡H¥i¬O§Ú¨S¥h¹L... ");
	 			msg("$ME§CÁn³ä³ä¦Û»y¹D: " + HIG + "­n¬O¯à¥h´N¦n¤F... \n" + NOR ,this_object(), 0, 1);
	 		}
	 		else	
	 		{
	 			switch(random(5))
	 			{
	 				case 0:
	 					do_command("?" + " " + me->query_id(1) );
	 					do_command("say ¶â... ¡H"+ pnoun(2, me) +"¬O·Q°Ý§ÚªF¤è¦³¤°»ò¶Ü¡H");
	 					break;
	 				case 1:
	 				case 2:
	 					do_command("say ªF¤è«ç¼Ë¤F¡H");
	 					break;
	 				case 3:
	 					do_command("? ªF¤è");
	 					break;
	 				case 4:
	 					do_command("tense" + " " + me->query_id(1) + " " + "ªF¤è¶Ç¨Ó¤°»ò¨Æ¶Ü¡I¡H");
	 					break;
	 			}
	 		}
	 	}
	 	else if ( strsrch(args, "¯Z³Ç©ú" ) >= 0 )
	 	{
	 		if( (strsrch(args, "¬O") >= 0 ) && (strsrch(args,"½Ö") >= 0 ) )
 			{
				do_command("say ¯Z³Ç©ú­ô­ô¬O§ø¤l¸Ìªº¦W¤H®@¡IÅ¥»¡¥L¦bÀ°¤H»\·Å¬u®ÈÀ]... ");
 				msg("$ME«Ü°ª¿³¦a»¡§¹¤§«á¡Aªí±¡«o¬Ý°_¨Ó±o¦³¨Ç¸¨¹æ¡C\n",this_object(), 0, 1);
 			}
 			else if( strsrch(args, "¯Z³Ç©ú·Å¬u") >= 0 )
 			{
 				do_command("say" + " " + "¨º¬O¤°»ò©Ç·Å¬u¦W°Ú¡I");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			else if( strsrch(args, "¯Z³Ç©ú®ÈÀ]") >= 0 )
 			{
 				do_command("say" + " " + "¨º¬O¤°»ò©Ç®ÈÀ]¦W°Ú¡I");
 				do_command("laugh" + " " +  me->query_id(1));
 			}
 			
 			else if( (strsrch(args, "·Å¬u") >= 0 ) || (strsrch(args,"®ÈÀ]") >= 0 ) )
 			{
				do_command("say ¬O¦³Å¥»¡¯Z³Ç©ú­ô­ô¦bÀ°¤H»\·Å¬u®ÈÀ]... ");
 			}
 			else
 			{
 				switch(random(10))
 				{
 					case 0:
 					case 1:
 					case 2:
 						do_command("hand" + " " +  me->query_id(1) + " " + pnoun(2, me) + "»{ÃÑ¯Z³Ç©ú­ô­ô¶Ü¡H");
 						break;
 					case 3:
 						do_command("peer" + " " + me->query_id(1));
 						do_command("say" + " " + pnoun(2, me) + "ª¾¹D¯Z³Ç©ú­ô­ô¬O½Ö¶Ü¡H");
 						break;
 					case 4:
 					case 5:
 						do_command("boggle" + " " + me->query_id(1));
 						do_command("say" + " "+ "°Ú¡I§Úªº¦Û¨¥¦Û¥Ñ³Q"+ pnoun(2, me) +"Å¥¨ì°Õ¡I¡H");
 						break;
 					case 6:
 					case 7:
 						msg("$ME¬ðµMÁy¬õ¤F°_¨Ó¡A¦ý¤°»ò¤]¨S»¡¡C\n",this_object(), 0, 1);
 						break;
 					case 8:
 					case 9:
 						msg("$ME¦ü¥G³´¤J¨I«ä¤§¤¤¡A¤£µo¤@»y¡C\n",this_object(), 0, 1);
 						break;
 				}

  			}

	 	}
	 	else if ( strsrch(args, "·Å¬u" ) >= 0 )
	 	{
	 		if ( strsrch(args, "®ÈÀ]" ) >= 0 )
	 		{
	 			do_command("pout");
	 			do_command("say" + " " + "¤°»ò·Å¬u®ÈÀ]¹À¡A§Ú³Ì°Q¹½·Å¬u®ÈÀ]¤F¡I... °Ú¡A©êºp¡A¥¢Â§¤F¡C");	 		
	 		}
	 		else if ( (strsrch(args, "­þ" ) >= 0) || (strsrch(args, "¦ó³B" ) >= 0) )
	 		{
	 			do_command("hmm" + " " +me->query_id(1));
	 			do_command("say" + " " + "§Ú¤£¤Ó²M·¡·Å¬uªº¨Æ¡A¦ý¦³Å¥»¡¹L¦ü¥GªF¤è¦³­ÓµÛ¦W·Å¬uªº¼Ë¤l¡H");	 		
	 		}
	 		else
	 		{
		 		do_command("?" + " " + me->query_id(1));
		 		do_command("say" + " "+ "¶â... ¡H" + pnoun(2, me) + "¬O·Q°Ý§Ú­þ¸Ì¦³·Å¬u¶Ü¡H¡H");
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
 				switch(random(2))
 				{
 					case 0:
		 				do_command("?" + " " +  me->query_id(1));
		 				break;
		 			case 1:
		 				do_command("arc" + " " + me->query_id(1) + " " + "¦³¤°»ò¤ò¯f¡I¡H");
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
		 				do_command("arc" + " " +  me->query_id(1));
		 				break;
		 		}
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
  						do_command("hmm" + " " + me->query_id(1));
  						break;
  				}
  				break;
  			
  			case "good"	:
  			case "clap"	:
  			case "cheer"	:
				switch(random(4))
  				{
  					case 0:
  						do_command("leer");
  						break;
  					case 1:
  						do_command("hmm" + " " + me->query_id(1));
  						break;
  					case 2:
  						do_command("ponder" + " " + me->query_id(1));
  						break;
  					case 3:
  						do_command("peer" + " " + me->query_id(1));
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
	set_idname("ada","¦ã¹F");
	
	set("nickname", "±Mª`²i¶¹");

	set("unit", "¦ì");
	
	set("age", 38);

	set("gender", FEMALE_GENDER);

	set("long",@TEXT
¡@¡@¤@ÀY¬õ´Ä¦â¤jªi®ö±²¾v§ô¦¨¤è«K¦æ°Êªº°¨§À¡AÅéºAÁöºâ¤£¤WÅÖ½G¡A¤]¤£¦Ü©ó
·|¨ì³QºÙ¬°¾Ö¸~¡C³o¦W¬Ý°_¨Ó¦³¨Ç¦~¬öªº¤k¤h¦b²´¯«¥æ·|¤¤Â§»ª©ÊÂIÀY·L¯º«á¡A
’ú§YÄ~Äò¯d·NÄl¤õ¡A±Mª`©ó²i¶¹¤§¤¤¡C
TEXT);

	set("heartbeat", 1); // ¥Ã¤[©Ê¤ß¸õ
		
	set("no_fight", 1);

	// ±Ò°Ê¥D°Ê¦æ¬°
	set_behavior();

	startup_living();
}
