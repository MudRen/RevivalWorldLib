/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : cotton_seed.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 棉花種子
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
	set_idname("cotton seed","棉花種子");
	
	if( this_object()->set_shadow_ob() ) return;
	
	set("long","可用來生產棉花。\n");
	set("unit", "袋");
	set("mass", 10);
	set("value", 180);
	set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		FARM,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//插秧播種時的訊息
		"start_msg":"棉花種子整齊地佈滿整片田地，一片欣欣向榮的感覺。\n" ,
			
	       	//每單位種子或秧苗需要的原料數量
	       	"material":
	       	([
	       		"/obj/materials/water":14,
	       	]),
			
	       	//收割產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/farming/product/cotton":12,
	       	]),

		//成熟收割時的訊息
		"harvest_msg":"$ME歡喜地收割這片成熟的棉花田。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"farming":90,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"棉花田上小小的幼苗已經漸漸長大了。\n",
			50:	"只見棉花田一片綠意盎然，充滿了生氣。\n",
			500:	"棉花田上的棉花已經漸漸成熟了。\n",
			700:	"只見一整片金黃色的棉花，已經不用再灌溉了，耐心等一陣子便可以收割。\n",
			800:	"這片棉花田上的棉花已經全部成熟了，終於可以收割了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得棉花陸續地枯萎了。\n",
			SHOWER:	"連日來的雨量使得一些棉花泡水腐爛了。\n",
			TYPHOON:"颱風來襲使得部份棉花被強風攔腰折斷。\n",
			FOEHN:	"焚風的吹襲讓部份的棉花快速的枯黃。\n",
		]),
	]));
}

