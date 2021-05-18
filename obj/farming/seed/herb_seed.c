/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : herb_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-07-22
 * Note   : 藥草種子
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <nature.h>
#include <map.h>

inherit STANDARD_OBJECT;

void create()
{
	set_idname("herb seed","藥草種子");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","可用來作成各種藥品。\n");
	set("unit", "袋");
	set("mass", 10);
	set("value", 100);
	set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		FARM,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//插秧播種時的訊息
		"start_msg":"藥草秧苗整齊地佈滿整片田地，一片欣欣向榮的感覺。\n" ,
			
	       	//每單位種子或秧苗需要的原料數量
	       	"material":
	       	([
	       		"/obj/materials/water":10,
	       	]),
			
	       	//收割產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/farming/product/herb":18,
	       	]),

		//成熟收割時的訊息
		"harvest_msg":"$ME歡喜地收割這片成熟的藥草田。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"farming":50,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"藥草田上小小的幼苗已經漸漸長大了。\n",
			50:	"只見藥草田一片綠意盎然，充滿了生氣。\n",
			300:	"藥草田上的藥草已經漸漸成熟了。\n",
			450:	"只見一整片深綠色的藥草，已經不用再灌溉了，耐心等一陣子便可以收割。\n",
			550:	"這片藥草田上的藥草已經全部成熟了，終於可以收割了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得藥草陸續地枯萎了。\n",
			SHOWER:	"連日來的雨量使得一些藥草泡水腐爛了。\n",
			TYPHOON:"颱風來襲使得部份藥草被強風攔腰折斷。\n",
			FOEHN:	"焚風的吹襲讓部份的藥草快速的枯黃。\n",
		]),
	]));
}

