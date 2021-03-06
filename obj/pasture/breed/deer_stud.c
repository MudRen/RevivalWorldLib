/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : deer_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 種鹿
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
        set_idname("deer stud","種鹿");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "用來交配生產的種畜。\n");
        set("unit", "隻");
        set("mass", 1000);
        set("value", 8000);
        set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		PASTURE,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//開始生產時的訊息
		"start_msg":"健壯的種鹿群四處散步和食用牧草，充滿活力與朝氣。\n" ,
			
	       	//每單位需要的原料種類與數量
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/herbage":25,
	       	]),
			
	       	//產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/pasture/product/leather":3,
	       		"/obj/pasture/product/antler":3,
	       	]),

		//生成時的訊息
		"harvest_msg":"$ME歡喜地屠宰並取出新鮮的鹿茸與皮革。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"pasture":80,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"進入了濃情蜜意的交配期，種鹿之間開始互相交配。\n",
			50:	"種鹿們恢復到過去的生活，但可以注意到部份的母鹿行為有些不同。\n",
			200:	"部份的母鹿肚子漸漸大了起來，似乎是懷孕了。\n",
			500:	"母鹿們分娩產下了許多健康的小鹿，只見母鹿們細心的圍繞在小鹿身旁並舔拭牠們的身體。\n",
			800:	"小鹿逐漸的成長，愈長愈壯。\n",
			900:	"小鹿們全部健康的長大成為成鹿，可以進行收成了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得鹿群們陸續的凍死。\n",
			SHOWER:	"連日來的雨量使得部份的鹿群們受涼病死。\n",
			TYPHOON:"颱風來襲使得部份的鹿群們被強風吹來的木頭砸死。\n",
			FOEHN:	"焚風的吹襲讓部份的鹿群們過熱中暑而死。\n",
		]),
	]));
}
