/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : description_d.c
 * Author : Clode@RevivalWorld
 * Date	  : 2006-02-08
 * Note	  : 敘述精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <buff.h>
#include <daemon.h>

#define PRODUCT_MODULE_PATH		"/std/module/product/"

string *buff_list = ({
	BUFF_STR			, HIY"力"NOR YEL"量"NOR,
	BUFF_PHY			, HIY"體"NOR YEL"格"NOR,
	BUFF_INT			, HIY"智"NOR YEL"商"NOR,
	BUFF_AGI			, HIY"敏"NOR YEL"捷"NOR,
	BUFF_CHA			, HIY"魅"NOR YEL"力"NOR,
	BUFF_STAMINA_MAX		, HIG"體力"NOR GRN"最大值"NOR,
	BUFF_HEALTH_MAX			, HIG"生命"NOR GRN"最大值"NOR,
	BUFF_ENERGY_MAX			, HIG"精神"NOR GRN"最大值"NOR,
	BUFF_STAMINA_REGEN		, HIC"體力"NOR CYN"恢復速度/2s"NOR,
	BUFF_HEALTH_REGEN		, HIC"生命"NOR CYN"恢復速度/2s"NOR,
	BUFF_ENERGY_REGEN		, HIC"精神"NOR CYN"恢復速度/2s"NOR,
	BUFF_LOADING_MAX		, HIM"負重"NOR MAG"上限/kg"NOR,
	BUFF_SLOT_MAX			, HIM"空間"NOR MAG"上限"NOR,
	BUFF_FOOD_MAX			, HIM"食物"NOR MAG"上限"NOR,
	BUFF_DRINK_MAX			, HIM"飲水"NOR MAG"上限"NOR,
	BUFF_SOCIAL_EXP_BONUS		, HIB"社會經驗"NOR BLU"加成/%"NOR,
	BUFF_COMBAT_EXP_BONUS		, HIB"戰鬥經驗"NOR BLU"加成/%"NOR,
	BUFF_MOVING_STAMINA		, HIR"移動體力"NOR RED"耗費"NOR,
	BUFF_MOVING_HEALTH		, HIR"移動生命"NOR RED"耗費"NOR,
	BUFF_MOVING_ENERGY		, HIR"移動精神"NOR RED"耗費"NOR,
	ATTACK				, HIR"攻"NOR RED"擊"NOR,
	DEFEND				, HIC"防"NOR CYN"禦"NOR,
	SPEED				, HIY"速"NOR YEL"度"NOR,
	BUFF_FATAL_CHANCE		, HIY"暴擊"NOR YEL"機率/%"NOR,
	BUFF_FATAL_POWER		, HIY"瞬間"NOR YEL"傷害/%"NOR,
	BUFF_ANTIFATAL_CHANCE		, HIY"防暴擊"NOR YEL"率/%"NOR,
	BUFF_SLIP_CHANCE		, HIG"滑倒"NOR GRN"機率/%"NOR,
	BUFF_COUNTERATTACK_CHANCE	, HIM"反擊"NOR MAG"機率/%"NOR,
	BUFF_THORN_CHANCE		, HIB"刺棘"NOR BLU"傷害/%"NOR,
	BUFF_COMBO_CHANCE		, HIC"連續"NOR CYN"攻擊/%"NOR,
	BUFF_TRADE_ABILITY		, HIC"貿易"NOR CYN"能力/%"NOR,
	BUFF_COLLECT_ABILITY		, HIC"採集"NOR CYN"能力/%"NOR,
	BUFF_GROW_ABILITY		, HIC"生產"NOR CYN"能力/%"NOR,
	BUFF_LEAD_ABILITY		, HIC"領導"NOR CYN"能力/%"NOR,
	BUFF_SALE_ABILITY		, HIC"推銷"NOR CYN"能力/%"NOR,
	BUFF_BATTER_POWER		, HIY"棒球"NOR YEL"打擊能力"NOR,
	BUFF_PITCHER_POWER		, HIR"棒球"NOR RED"投球能力"NOR,
	BUFF_FIELDER_POWER		, HIC"棒球"NOR CYN"守備能力"NOR,
});

int buff_list_size;

string buff_description(object ob, string key, string chinese, int indent, int fullsuit)
{
	int buff = query(key, ob) + query_temp(key, ob);
	int buff_fullsuit = fullsuit ? query(BUFF_FULLSUIT+key, ob) + query_temp(BUFF_FULLSUIT+key, ob) : 0;

	if( !buff && !buff_fullsuit ) return 0;

	if( buff > 0 )
		return sprintf(repeat_string(" ", indent)+NOR YEL"．%-15s "NOR CYN"+"HIC"%d"+(fullsuit && buff_fullsuit?NOR YEL" <"HIY"全套 "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese, buff);
	else if( buff == 0 )
		return sprintf(repeat_string(" ", indent)+NOR YEL"．%-15s "NOR+(fullsuit && buff_fullsuit?NOR YEL"<"HIY"全套 "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese);
	else
		return sprintf(repeat_string(" ", indent)+NOR YEL"．%-15s "NOR RED"-"HIR"%d"+(fullsuit && buff_fullsuit?NOR YEL" <"HIY"全套 "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese, -buff);
}

string query_object_buff_description(object ob, int indent, int fullsuit)
{
	string description = "";
	
	for(int i=0;i<buff_list_size;i+=2)
		description += buff_description(ob, buff_list[i], buff_list[i+1], indent, fullsuit) || "";
	
	return description;
}

// 物品敘述
varargs string query_object_description(object ob, int need_quantity)
{
	int mass = query("mass", ob);
	string mylong = query("long", ob);
	string description = ob->short(need_quantity)+" ─\n";

	description += "╔═════════════════════════════════════╗\n\n";	
	
	if( ob->is_module_product() )
	{
		string owner = query("design/owner", ob);
		mapping product_info = (PRODUCT_MODULE_PATH+query("design/type", ob)+"_module.c")->query_product_info();
		mapping inlay = query("design/inlay", ob);

		description += HIW"  研發"NOR WHT"作者："NOR+(find_player(owner)?find_player(owner)->query_idname():capitalize(owner))+"\n";
		description += HIW"  研發"NOR WHT"產地："NOR+query("design/room", ob)+"\n";
		description += HIW"  研發"NOR WHT"時間："NOR+TIME_D->replace_ctime(query("design/time", ob))+NOR"\n";
		description += HIW"  產品"NOR WHT"種類："NOR+product_info["name"]+"("+capitalize(product_info["id"])+")"+NOR"\n";
		description += HIW"  鑲嵌"NOR WHT"物品："NOR+(mapp(inlay) ? implode(values(inlay), (: $1+$2 :)) +" 個" : "無")+NOR"\n";	
	}
	if( mylong )
		description += "  "+mylong+"\n";
		
	description += "  重量 "+(mass==-1?"無限":mass+"g")+"、價值 "+query("value", ob)+(ob->query_description()||"");
	
	if( query("flag/no_amount", ob) )
		description += HIY"  不可堆疊"NOR;
	if( query("flag/no_drop", ob) )
		description += HIG"  不可丟棄"NOR;
	if( query("flag/no_give", ob) )
		description += HIC"  不可交易"NOR;
	if( query("flag/no_import", ob) )
		description += HIM"  不可輸入"NOR;
	
	description += "\n\n"+query_object_buff_description(ob, 2, 1);

	description += "\n╚═════════════════════════════════════╝\n";

	return description;
}

// 生物敘述
varargs string query_living_description(object ob, int need_quantity)
{
	object me = this_player();
	string long = query("long", ob);
	string description = ob->short(need_quantity)+" ─\n";
	int attack_buff = ob->query_condition_buff(ATTACK);
	int defend_buff = ob->query_condition_buff(DEFEND);
	int counterattackbuff = ob->query_all_buff(BUFF_COUNTERATTACK_CHANCE);
	int thorndamagebuff = ob->query_all_buff(BUFF_THORN_CHANCE);
	int fatalchancebuff = ob->query_skill_level("fatalblow")/2 + ob->query_all_buff(BUFF_FATAL_CHANCE);
	int fatalpowerbuff = ob->query_all_buff(BUFF_FATAL_POWER);
	int antifatalbuff = ob->query_all_buff(BUFF_ANTIFATAL_CHANCE);
	int combochancebuff = ob->query_all_buff(BUFF_COMBO_CHANCE);
	
	description += "╔═════════════════════════════════════╗\n\n";
	
	description += "  "+pnoun(3, ob)+"的年齡為 "+query("age", ob)+" 歲。\n"+(long?"\n"+long+"\n":"");

	if( ob->is_module_npc() )
	{
		string boss = query("boss", ob);
		object bossob;
		
		if( stringp(boss) )
			bossob = find_player(boss);
		
		if( objectp(bossob) )
			description += "  其身份為受雇員工，雇主是 "+bossob->query_idname()+"\n";
		else if( !boss )
			description += "  其身份為受雇員工，但無雇主資料\n";
		else
			description += "  其身份為受雇員工，雇主是 "+capitalize(boss)+"\n";
	}

	description += "\n";
	
	description += sprintf(HIR"  攻擊 "NOR RED"%d"+(attack_buff?"(+"+attack_buff+")":"")+NOR HIC"   防禦 "NOR CYN"%d"+(defend_buff?"(+"+defend_buff+")":"")+NOR HIY"   速度 "NOR YEL"%d    - %s\n"NOR, COMBAT_D->attack_value(ob), COMBAT_D->defend_value(ob), COMBAT_D->speed_value(ob), (STANCE_D->query_stance_name(query("combat/stance/type", ob))||WHT"未採用任何")+"戰鬥姿態"NOR );

	if( objectp(me) && userp(me) )
	{
		int my_totalvalue = COMBAT_D->total_value(me);
		int ob_totalvalue = COMBAT_D->total_value(ob);
		
		if( my_totalvalue*3 < ob_totalvalue )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIR+pnoun(3, ob)+"的戰鬥能力遠超過"+pnoun(2, me)+"。\n"NOR);
		else if( my_totalvalue*2 < ob_totalvalue )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIY+pnoun(3, ob)+"的戰鬥能力勝過"+pnoun(2, me)+"一大截。\n"NOR);
		else if( my_totalvalue*1.3 < ob_totalvalue )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIY+pnoun(3, ob)+"的戰鬥能力略勝於"+pnoun(2, me)+"。\n"NOR);
		else if( my_totalvalue*1.3 >= ob_totalvalue && my_totalvalue <= ob_totalvalue*1.3 )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIW+pnoun(3, ob)+"和"+pnoun(2, me)+"的戰鬥能力不分軒輊。\n"NOR);
		else if( my_totalvalue > ob_totalvalue*4 )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIC+pnoun(2, me)+"的戰鬥能力遠超過"+pnoun(3, ob)+"，就算擊敗"+pnoun(3, ob)+"也無法取得任何經驗獎勵。\n"NOR);
		else if( my_totalvalue > ob_totalvalue*3 )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIC+pnoun(2, me)+"的戰鬥能力遠超過"+pnoun(3, ob)+"。\n"NOR);
		else if( my_totalvalue > ob_totalvalue*2 )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIG+pnoun(2, me)+"的戰鬥能力勝過"+pnoun(3, ob)+"一大截。\n"NOR);
		else if( my_totalvalue > ob_totalvalue*1.3 )
			description += sprintf("\n  "HIM"評"NOR MAG"比 "HIG+pnoun(2, me)+"的戰鬥能力略勝於"+pnoun(3, ob)+"。\n"NOR);
	}
		
	description += "\n";
	
	foreach(object eq in sort_objects(ob->query_equipment_objects()))
		description += "  "+HIW"["NOR+eq->query_part_name()+HIW"] "NOR"裝備 " + eq->short()+"\n";
	
	description += sprintf("\n  暴擊 %s  防暴 %s  反擊 %s  瞬傷 %s  連攻 %s  刺棘 %s\n",HIW+fatalchancebuff+NOR WHT"%"NOR, HIW+antifatalbuff+NOR WHT"%"NOR, HIW+counterattackbuff+NOR WHT"%"NOR, HIW+fatalpowerbuff+NOR WHT"%"NOR, HIW+combochancebuff+NOR WHT"%"NOR, HIW+thorndamagebuff+NOR WHT"%"NOR);

	description += "\n╚═════════════════════════════════════╝\n";
	
	return description;
}

void create()
{
	buff_list_size = sizeof(buff_list);
}

string query_name()
{
	return "敘述系統(DESCRIPTION_D)";
}
