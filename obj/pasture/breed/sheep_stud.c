/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : sheep_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 種羊
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
        set_idname("sheep stud","種羊");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "用來交配生產的種畜。\n");
        set("unit", "隻");
        set("mass", 1000);
        set("value", 2000);
        set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		PASTURE,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//開始生產時的訊息
		"start_msg":"健壯的種羊群四處散步和食用牧草，充滿活力與朝氣。\n" ,
			
	       	//每單位需要的原料種類與數量
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/herbage":15,
	       	]),
			
	       	//產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/pasture/product/mutton":8,
	       		"/obj/pasture/product/wool":8,
	       	]),

		//生成時的訊息
		"harvest_msg":"$ME歡喜地屠宰並取出新鮮的羊肉與羊毛。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"pasture":40,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"進入了濃情蜜意的交配期，種羊之間開始互相交配。\n",
			50:	"種羊們恢復到過去的生活，但可以注意到部份的母羊行為有些不同。\n",
			150:	"部份的母羊肚子漸漸大了起來，似乎是懷孕了。\n",
			400:	"母羊們分娩產下了許多健康的小羊，只見種羊們細心的圍繞在小羊身旁並舔拭牠們的身體。\n",
			600:	"小羊逐漸的成長，愈長愈壯。\n",
			700:	"小羊們全部健康的長大成為成羊，可以進行收成了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得羊群們陸續的凍死。\n",
			SHOWER:	"連日來的雨量使得部份的羊群們受涼病死。\n",
			TYPHOON:"颱風來襲使得部份的羊群們被強風吹來的木頭砸死。\n",
			FOEHN:	"焚風的吹襲讓部份的羊群們過熱中暑而死。\n",
		]),
	]));
}
