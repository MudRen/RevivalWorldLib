/* This	program	is a part of RW	mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File	  : description_d.c
 * Author : Clode@RevivalWorld
 * Date	  : 2006-02-08
 * Note	  : �ԭz���F
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
	BUFF_STR			, HIY"�O"NOR YEL"�q"NOR,
	BUFF_PHY			, HIY"��"NOR YEL"��"NOR,
	BUFF_INT			, HIY"��"NOR YEL"��"NOR,
	BUFF_AGI			, HIY"��"NOR YEL"��"NOR,
	BUFF_CHA			, HIY"�y"NOR YEL"�O"NOR,
	BUFF_STAMINA_MAX		, HIG"��O"NOR GRN"�̤j��"NOR,
	BUFF_HEALTH_MAX			, HIG"�ͩR"NOR GRN"�̤j��"NOR,
	BUFF_ENERGY_MAX			, HIG"�믫"NOR GRN"�̤j��"NOR,
	BUFF_STAMINA_REGEN		, HIC"��O"NOR CYN"��_�t��/2s"NOR,
	BUFF_HEALTH_REGEN		, HIC"�ͩR"NOR CYN"��_�t��/2s"NOR,
	BUFF_ENERGY_REGEN		, HIC"�믫"NOR CYN"��_�t��/2s"NOR,
	BUFF_LOADING_MAX		, HIM"�t��"NOR MAG"�W��/kg"NOR,
	BUFF_SLOT_MAX			, HIM"�Ŷ�"NOR MAG"�W��"NOR,
	BUFF_FOOD_MAX			, HIM"����"NOR MAG"�W��"NOR,
	BUFF_DRINK_MAX			, HIM"����"NOR MAG"�W��"NOR,
	BUFF_SOCIAL_EXP_BONUS		, HIB"���|�g��"NOR BLU"�[��/%"NOR,
	BUFF_COMBAT_EXP_BONUS		, HIB"�԰��g��"NOR BLU"�[��/%"NOR,
	BUFF_MOVING_STAMINA		, HIR"������O"NOR RED"�ӶO"NOR,
	BUFF_MOVING_HEALTH		, HIR"���ʥͩR"NOR RED"�ӶO"NOR,
	BUFF_MOVING_ENERGY		, HIR"���ʺ믫"NOR RED"�ӶO"NOR,
	ATTACK				, HIR"��"NOR RED"��"NOR,
	DEFEND				, HIC"��"NOR CYN"�m"NOR,
	SPEED				, HIY"�t"NOR YEL"��"NOR,
	BUFF_FATAL_CHANCE		, HIY"����"NOR YEL"���v/%"NOR,
	BUFF_FATAL_POWER		, HIY"����"NOR YEL"�ˮ`/%"NOR,
	BUFF_ANTIFATAL_CHANCE		, HIY"������"NOR YEL"�v/%"NOR,
	BUFF_SLIP_CHANCE		, HIG"�ƭ�"NOR GRN"���v/%"NOR,
	BUFF_COUNTERATTACK_CHANCE	, HIM"����"NOR MAG"���v/%"NOR,
	BUFF_THORN_CHANCE		, HIB"���"NOR BLU"�ˮ`/%"NOR,
	BUFF_COMBO_CHANCE		, HIC"�s��"NOR CYN"����/%"NOR,
	BUFF_TRADE_ABILITY		, HIC"�T��"NOR CYN"��O/%"NOR,
	BUFF_COLLECT_ABILITY		, HIC"�Ķ�"NOR CYN"��O/%"NOR,
	BUFF_GROW_ABILITY		, HIC"�Ͳ�"NOR CYN"��O/%"NOR,
	BUFF_LEAD_ABILITY		, HIC"���"NOR CYN"��O/%"NOR,
	BUFF_SALE_ABILITY		, HIC"���P"NOR CYN"��O/%"NOR,
	BUFF_BATTER_POWER		, HIY"�βy"NOR YEL"������O"NOR,
	BUFF_PITCHER_POWER		, HIR"�βy"NOR RED"��y��O"NOR,
	BUFF_FIELDER_POWER		, HIC"�βy"NOR CYN"�u�Ư�O"NOR,
});

int buff_list_size;

string buff_description(object ob, string key, string chinese, int indent, int fullsuit)
{
	int buff = query(key, ob) + query_temp(key, ob);
	int buff_fullsuit = fullsuit ? query(BUFF_FULLSUIT+key, ob) + query_temp(BUFF_FULLSUIT+key, ob) : 0;

	if( !buff && !buff_fullsuit ) return 0;

	if( buff > 0 )
		return sprintf(repeat_string(" ", indent)+NOR YEL"�D%-15s "NOR CYN"+"HIC"%d"+(fullsuit && buff_fullsuit?NOR YEL" <"HIY"���M "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese, buff);
	else if( buff == 0 )
		return sprintf(repeat_string(" ", indent)+NOR YEL"�D%-15s "NOR+(fullsuit && buff_fullsuit?NOR YEL"<"HIY"���M "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese);
	else
		return sprintf(repeat_string(" ", indent)+NOR YEL"�D%-15s "NOR RED"-"HIR"%d"+(fullsuit && buff_fullsuit?NOR YEL" <"HIY"���M "+(buff_fullsuit>0?"+":"-")+buff_fullsuit+NOR YEL">"NOR:"")+"\n"NOR, chinese, -buff);
}

string query_object_buff_description(object ob, int indent, int fullsuit)
{
	string description = "";
	
	for(int i=0;i<buff_list_size;i+=2)
		description += buff_description(ob, buff_list[i], buff_list[i+1], indent, fullsuit) || "";
	
	return description;
}

// ���~�ԭz
varargs string query_object_description(object ob, int need_quantity)
{
	int mass = query("mass", ob);
	string mylong = query("long", ob);
	string description = ob->short(need_quantity)+" �w\n";

	description += "������������������������������������������������������������������������������\n\n";	
	
	if( ob->is_module_product() )
	{
		string owner = query("design/owner", ob);
		mapping product_info = (PRODUCT_MODULE_PATH+query("design/type", ob)+"_module.c")->query_product_info();
		mapping inlay = query("design/inlay", ob);

		description += HIW"  ��o"NOR WHT"�@�̡G"NOR+(find_player(owner)?find_player(owner)->query_idname():capitalize(owner))+"\n";
		description += HIW"  ��o"NOR WHT"���a�G"NOR+query("design/room", ob)+"\n";
		description += HIW"  ��o"NOR WHT"�ɶ��G"NOR+TIME_D->replace_ctime(query("design/time", ob))+NOR"\n";
		description += HIW"  ���~"NOR WHT"�����G"NOR+product_info["name"]+"("+capitalize(product_info["id"])+")"+NOR"\n";
		description += HIW"  �^�O"NOR WHT"���~�G"NOR+(mapp(inlay) ? implode(values(inlay), (: $1+$2 :)) +" ��" : "�L")+NOR"\n";	
	}
	if( mylong )
		description += "  "+mylong+"\n";
		
	description += "  ���q "+(mass==-1?"�L��":mass+"g")+"�B���� "+query("value", ob)+(ob->query_description()||"");
	
	if( query("flag/no_amount", ob) )
		description += HIY"  ���i���|"NOR;
	if( query("flag/no_drop", ob) )
		description += HIG"  ���i���"NOR;
	if( query("flag/no_give", ob) )
		description += HIC"  ���i���"NOR;
	if( query("flag/no_import", ob) )
		description += HIM"  ���i��J"NOR;
	
	description += "\n\n"+query_object_buff_description(ob, 2, 1);

	description += "\n������������������������������������������������������������������������������\n";

	return description;
}

// �ͪ��ԭz
varargs string query_living_description(object ob, int need_quantity)
{
	object me = this_player();
	string long = query("long", ob);
	string description = ob->short(need_quantity)+" �w\n";
	int attack_buff = ob->query_condition_buff(ATTACK);
	int defend_buff = ob->query_condition_buff(DEFEND);
	int counterattackbuff = ob->query_all_buff(BUFF_COUNTERATTACK_CHANCE);
	int thorndamagebuff = ob->query_all_buff(BUFF_THORN_CHANCE);
	int fatalchancebuff = ob->query_skill_level("fatalblow")/2 + ob->query_all_buff(BUFF_FATAL_CHANCE);
	int fatalpowerbuff = ob->query_all_buff(BUFF_FATAL_POWER);
	int antifatalbuff = ob->query_all_buff(BUFF_ANTIFATAL_CHANCE);
	int combochancebuff = ob->query_all_buff(BUFF_COMBO_CHANCE);
	
	description += "������������������������������������������������������������������������������\n\n";
	
	description += "  "+pnoun(3, ob)+"���~�֬� "+query("age", ob)+" ���C\n"+(long?"\n"+long+"\n":"");

	if( ob->is_module_npc() )
	{
		string boss = query("boss", ob);
		object bossob;
		
		if( stringp(boss) )
			bossob = find_player(boss);
		
		if( objectp(bossob) )
			description += "  �䨭�����������u�A���D�O "+bossob->query_idname()+"\n";
		else if( !boss )
			description += "  �䨭�����������u�A���L���D���\n";
		else
			description += "  �䨭�����������u�A���D�O "+capitalize(boss)+"\n";
	}

	description += "\n";
	
	description += sprintf(HIR"  ���� "NOR RED"%d"+(attack_buff?"(+"+attack_buff+")":"")+NOR HIC"   ���m "NOR CYN"%d"+(defend_buff?"(+"+defend_buff+")":"")+NOR HIY"   �t�� "NOR YEL"%d    - %s\n"NOR, COMBAT_D->attack_value(ob), COMBAT_D->defend_value(ob), COMBAT_D->speed_value(ob), (STANCE_D->query_stance_name(query("combat/stance/type", ob))||WHT"���ĥΥ���")+"�԰����A"NOR );

	if( objectp(me) && userp(me) )
	{
		int my_totalvalue = COMBAT_D->total_value(me);
		int ob_totalvalue = COMBAT_D->total_value(ob);
		
		if( my_totalvalue*3 < ob_totalvalue )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIR+pnoun(3, ob)+"���԰���O���W�L"+pnoun(2, me)+"�C\n"NOR);
		else if( my_totalvalue*2 < ob_totalvalue )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIY+pnoun(3, ob)+"���԰���O�ӹL"+pnoun(2, me)+"�@�j�I�C\n"NOR);
		else if( my_totalvalue*1.3 < ob_totalvalue )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIY+pnoun(3, ob)+"���԰���O���ө�"+pnoun(2, me)+"�C\n"NOR);
		else if( my_totalvalue*1.3 >= ob_totalvalue && my_totalvalue <= ob_totalvalue*1.3 )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIW+pnoun(3, ob)+"�M"+pnoun(2, me)+"���԰���O�����a���C\n"NOR);
		else if( my_totalvalue > ob_totalvalue*4 )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIC+pnoun(2, me)+"���԰���O���W�L"+pnoun(3, ob)+"�A�N������"+pnoun(3, ob)+"�]�L�k���o����g����y�C\n"NOR);
		else if( my_totalvalue > ob_totalvalue*3 )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIC+pnoun(2, me)+"���԰���O���W�L"+pnoun(3, ob)+"�C\n"NOR);
		else if( my_totalvalue > ob_totalvalue*2 )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIG+pnoun(2, me)+"���԰���O�ӹL"+pnoun(3, ob)+"�@�j�I�C\n"NOR);
		else if( my_totalvalue > ob_totalvalue*1.3 )
			description += sprintf("\n  "HIM"��"NOR MAG"�� "HIG+pnoun(2, me)+"���԰���O���ө�"+pnoun(3, ob)+"�C\n"NOR);
	}
		
	description += "\n";
	
	foreach(object eq in sort_objects(ob->query_equipment_objects()))
		description += "  "+HIW"["NOR+eq->query_part_name()+HIW"] "NOR"�˳� " + eq->short()+"\n";
	
	description += sprintf("\n  ���� %s  ���� %s  ���� %s  ���� %s  �s�� %s  ��� %s\n",HIW+fatalchancebuff+NOR WHT"%"NOR, HIW+antifatalbuff+NOR WHT"%"NOR, HIW+counterattackbuff+NOR WHT"%"NOR, HIW+fatalpowerbuff+NOR WHT"%"NOR, HIW+combochancebuff+NOR WHT"%"NOR, HIW+thorndamagebuff+NOR WHT"%"NOR);

	description += "\n������������������������������������������������������������������������������\n";
	
	return description;
}

void create()
{
	buff_list_size = sizeof(buff_list);
}

string query_name()
{
	return "�ԭz�t��(DESCRIPTION_D)";
}
