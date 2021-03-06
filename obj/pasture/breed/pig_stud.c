/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : pig_stud.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-09-09
 * Note   : 種豬
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
        set_idname("pig stud","種豬");
        
        if( this_object()->set_shadow_ob() ) return;
        
        set("long", "用來交配生產的種畜。\n");
        set("unit", "隻");
        set("mass", 1000);
        set("value", 1000);
        set("badsell", 1);

	// 種子資訊
	set("growth",
	([
		//種類
		"type":		PASTURE,

		//名稱
		"idname":	this_object()->query_idname(),
		
		//開始生產時的訊息
		"start_msg":"胖嘟嘟的種豬看到飼料就衝過去狂吃，吃得滿嘴都是飼料。\n" ,
			
	       	//每單位需要的原料種類與數量
	       	"material":
	       	([
	       		"/obj/materials/water":5,
	       		"/obj/farming/product/corn":10,
	       	]),
			
	       	//產品的數量設定與產品路徑
	       	"harvest":
	       	([
	       		"/obj/pasture/product/pork":11,
	       	]),

		//生成時的訊息
		"harvest_msg":"$ME歡喜地屠宰並取出新鮮的豬肉。\n",

		//種植此類產品所需要的技能與其等級
		"skill":
		([
			"pasture":20,
		]),

		//成長時的訊息
		"growing":
		([
			5:	"進入了濃情蜜意的交配期，種豬之間開始互相交配。\n",
			50:	"種豬們恢復到過去的生活，但可以注意到部份的母豬行為有些不同。\n",
			150:	"部份的母豬肚子漸漸大了起來，似乎是懷孕了。\n",
			300:	"母豬們分娩產下了許多健康的小豬，只見母豬們細心的圍繞在小豬身旁並舔拭牠們的身體。\n",
			500:	"小豬逐漸的成長，愈長愈壯。\n",
			600:	"小豬們全部健康的長大成為成豬，可以進行收成了。\n",
		]),
	
		// 天氣影響敘述
		"weather":
		([
			SNOWY:	"大雪連連嚴重使得豬群們陸續的凍死。\n",
			SHOWER:	"連日來的雨量使得部份的豬群們受涼病死。\n",
			TYPHOON:"颱風來襲使得部份的豬群們被強風吹來的木頭砸死。\n",
			FOEHN:	"焚風的吹襲讓部份的豬群們過熱中暑而死。\n",
		]),
	]));
}
