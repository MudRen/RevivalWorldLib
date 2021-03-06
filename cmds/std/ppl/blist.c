/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : blist.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-09-14
 * Note   : 玩家指令程式
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <map.h>
#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <roommodule.h>

inherit COMMAND;

string generate_building_list();

string help = @HELP
 建築物列表                                                           [blist]
�歈���������������������������������������������������������������������������
      這個指令可以讓你看到目前RW裡開放的建築物，以及要開張這建築所需的最低條
  件。但你須要先建造([1;33mbuild[m) 密合的牆壁，而且必須要有一扇以上的門。

      當你打下[1;33mblist[m 時，會出現如下畫面：
HELP + generate_building_list()+ @HELP

  ．種類、名稱：
          就是這建築的主要功用，若是看不懂英文... 看他的中文名稱也大概可以了
      解他主要的能力吧？

  ．最少房間限制：
          不包括牆壁在內的最少格數限制，例如[1;36m小型[m商店好了... 它的最少房間限制
      是一，說白一點就是你得買九格地。在八塊地上蓋上[1m牆壁[m，剩下中間那白色的一
      格，才可以開張([1;33mopen[m)你的[1;36m小型[m商店。

          以圖示舉例的話：
        
        ■←「最少房間限制」所算的數量
        □←必須要拿來蓋牆壁用的地。

          最少房間限制 = 3

      　□□□□    ■ = 4    大於最少房間限制。
        □■■□
        □■■□    □ = 12   單純拿來蓋密合牆壁的地。
      　□□□□

             當然你高興的話也可以蓋成：
                                                        □□□
        □□□□    □□□□    □□□        □□□    □■□
        □■■□    □■■□    □■□□    □□■□    □■□    □□□□□
        □□■□    □■□□    □■■□    □■■□    □■□    □■■■□
          □□□    □□□      □□□□    □□□□    □□□    □□□□□

          這樣雖然需要買的地可以少一格，但事實上牆壁佔的格數還是跟正方形一樣。

  ．屬性：
          簡單的來講就是你所屬的身份，一開始都是由[36m私人[m開始。而後可能金錢或擁
      有的房地產有一定程度之後可以開公司（[1;30m企業[m），或者是經由投票選舉而成為首
      長（[35m政府[m）。

          但目前還只有[36m私人[m屬性，[1;30m企業[m和[35m政府[m屬性正在開發中，所以無法詳述。

  ．區域：
          在一開始的時候，荒地已經被政府劃分好了各種用地。所有的建築物都必須
      建築政府規劃好的用地之上，有些只能被建築在某些用地上。如：[1;33m銀行[m，只能開
      張於[1;36;46m商業區[m之上；[1;36m小型[m商店則是三種用地都可以開張。每種用地的地價都不同，
      在建造時須要注意。

  ．開張花費：
          當你已經將以上條件一一達成之時，在開張之前的最後一個限制，就是開張
      花費。假設你要蓋的是一間[1;36m小型[m商店，已在用地上買了足夠的格數，並一一建造
      出密合的牆壁（包括門）。你身上必須要有足夠的金錢，才能[1;33mopen store[m，開張
      你的[1;36m小型[m商店。

[33m    相關指令: build, open
[m�裺���������������������������������������������������������������������������
                                                            文件持續更新維護中
HELP;



string generate_building_list()
{
        array data;
        string msg, buildingtype, buildingregion, *sortid;
        mapping building_table = BUILDING_D->query_building_table();
	string *manual_sort = ({
		"cityhall",
		"recycle",
		"labor",
		"trading_post",
		"bank",
		"postoffice",
		"hospital",
		"jail",
		"auction",
		"futures",
		"church",
		"trainstation",
		"airport",
		"space",
		"nuclear",
		"prodigy",

		0,
		"enterprise",
		"stock",
		"university",
		0,
		"store",
		"warehouse",
		"house",
		"fence",
		"cabin",
		"expedition",
		"research",
		"resource",
		"baseball",
		0,
		
		"metal",
		"stone",
		"water",
		"wood",
		"fuel",
		
		0,
		"food",
		"drink",
		"clothing",
		"furniture",
		"hardware",
		"chemical",
		"machinery",
		"electrics",
		"transportation",		
		"medicine",
		"adventure",
		"shortrange",
		"armor",
		"perfume",

		0,
		"defendtower",
		"instrument",
		"entertainment",
		"longrange",
		"heavyarmor",
		"magic",
		"aircraft",
	});

        sortid = manual_sort | keys(building_table);
       
       	msg = "建築列表，目前共可建造 "+sizeof(manual_sort-({0}))+" 種建築：\n";
        msg += NOR WHT"───────────────────────────────────────────\n"NOR;
        msg +=        "建築種類       名稱   最少房間 屬 規劃區 繁榮 樓層        花費/每房 個限 城限 時代 開\n";
        msg += NOR WHT"───────────────────────────────────────────\n"NOR;
        
        
        foreach( string id in sortid )
        {
        	if( !stringp(id) )
        	{
        		msg += "\n";
        		continue;
        	}

                data = building_table[id];
                
                if( !data || data[ROOMMODULE_OPENCOST] == -1 ) continue;
                
                if( data[ROOMMODULE_BUILDINGTYPE] & GOVERNMENT )
                        buildingtype = MAG"政"NOR;
                else if( data[ROOMMODULE_BUILDINGTYPE] & ENTERPRISE )
                        buildingtype = WHT"企"NOR;
                else if( data[ROOMMODULE_BUILDINGTYPE] & INDIVIDUAL )
                        buildingtype = CYN"個"NOR;
		
		if( data[ROOMMODULE_REGION] & AREA_REGION )
			buildingregion = HIW BRED" 郊區 "NOR;
		else
                	buildingregion = ( data[ROOMMODULE_REGION] & AGRICULTURE_REGION ? HIG BGRN"農"NOR : "  ")+
					 ( data[ROOMMODULE_REGION] & INDUSTRY_REGION ? HIY BYEL"工"NOR : "  ")+
					 ( data[ROOMMODULE_REGION] & COMMERCE_REGION ? HIC BCYN"商"NOR : "  ");

                msg += sprintf("%-15s%-13s%2d %-3s%-7s%-5d%-4d%17s %|4s %|4s %4s %s\n", 
                        HIY+capitalize(id)+NOR, 
                        data[ROOMMODULE_SHORT],
                        data[ROOMMODULE_ROOMLIMIT],
                        buildingtype,
                        buildingregion,
                        data[ROOMMODULE_FLOURISH],
                        data[ROOMMODULE_MAXFLOOR],
                        HIW+NUMBER_D->number_symbol(data[ROOMMODULE_OPENCOST])+NOR,
                        NOR WHT+(data[ROOMMODULE_MAXLIMIT]||""),
                        NOR WHT+(data[ROOMMODULE_MAXBUILDINGLIMIT]||""),
                       	CITY_D->query_age_name(data[ROOMMODULE_AGE], 1),
                        data[ROOMMODULE_TESTING] ? HIR"╳"NOR : HIG"○"NOR,
                        );
        }

        msg += NOR WHT"───────────────────────────────────────────\n"NOR;
        
        
        return msg;
}


private void do_command(object me, string arg)
{
	me->more(generate_building_list());
}
