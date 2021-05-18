/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : baseball.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : 棒球場
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <condition.h>
#include <delay.h>
#include <baseball.h>
#include <time.h>

inherit ROOM_ACTION_MOD;

void do_setup(object me, string arg)
{
	string myid = me->query_id(1);
	int status = BASEBALL_D->query_status(myid);
	int in_season = BASEBALL_D->in_season(myid);
	string owner = query("owner", environment(me));
	
	mapping setup;

	if( owner != myid && arg )
		return tell(me, "這裡不是"+pnoun(2, me)+"的棒球場，無法進行任何設定。\n");
		
	setup = BASEBALL_D->query_setup(owner);

	switch(arg)
	{
		case 0:
		{
			string msg = pnoun(2, me)+"的球隊名稱為「"+(setup["name"]||"未命名")+"」隊，目前安排的球員守備位置與打擊棒次資料如下：\n";
			object labor;
			string position;
			string file;
			
			msg += WHT"─────────────────────────────────────\n"NOR;
			msg += sprintf(HIW" %-22s %-20s %-40s\n"NOR, "", "守備位置", "球員名稱" );
			msg += WHT"─────────────────────────────────────\n"NOR;
			
			labor = 0;
			position = 0;

			if( mapp(setup["roster"][0]) )
			{
				file = setup["roster"][0]["file"];
				if( file_exists(file) )
					labor = load_object(file);

				position = BASEBALL_D->query_positionname(setup["roster"][0]["position"]);
			}
		
			msg += sprintf(" %-22s %-20s %-40s\n", 
				"",
				position || WHT"未設定"NOR, 
				objectp(labor) ? labor->query_idname() : WHT"未設定"NOR
			);
				
			msg += WHT"\n"NOR;
			msg += sprintf(HIW" %-22s %-20s %-40s\n"NOR, "棒次", "守備位置", "球員名稱");
			msg += WHT"─────────────────────────────────────\n"NOR;
			
			for(int i=1;i<10;++i)
			{
				labor = 0;
				position = 0;

				if( mapp(setup["roster"][i]) )
				{
					file = setup["roster"][i]["file"];
					if( file_exists(file) )
						labor = load_object(file);

					position = BASEBALL_D->query_positionname(setup["roster"][i]["position"]);
				}

				msg += sprintf(" %-22s %-20s %-40s\n", 
					NOR GRN"第 "HIG+i+NOR GRN" 棒"NOR, 
					position || WHT"未設定"NOR, 
					objectp(labor) ? labor->query_idname() : WHT"未設定"NOR
				);
			}
		
			msg += WHT"─────────────────────────────────────\n"NOR;
			me->more(msg);
			
			break;	
		}
		case "join":
		{
			object labor;
			
			//if( !wizardp(me) )
			//	return tell(me, "目前系統正在測試中，無法加入季賽。\n");

			if( in_season )
				return tell(me, "球隊已加入季賽，請等待系統安排賽事。\n");
				
			if( !mapp(setup) )
				return tell(me, "請先進行球隊各項資料設定，請參考 help setup。\n");
				
			if( !setup["name"] )
				return tell(me, "尚未設定球隊名稱。\n");
				
			foreach(mapping data in setup["roster"])
			{
				if( !mapp(data) || !file_exists(data["file"]) || !objectp(labor = load_object(data["file"])) )
					return tell(me, "球員資料不完整，請利用 setup 查詢棒次與守備位置設定。\n");
					
				if( !same_environment(labor, me) || query("job/type", labor) != SPORTER )
					return tell(me, "名單中的球員不在此處或未將工作設定為球員。\n");
			}

			// 球隊設定不合法
			if( !BASEBALL_D->valid_setup(myid) )
				return tell(me, "球隊設定不完整，請詳閱 help setup 並進行完整設定。\n");
		
			setup["master"] = base_name(environment(me)->query_master());

			msg("$ME宣告其球隊「"+setup["name"]+"」從現在開始正式加入季賽。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("sport", me->query_idname()+"宣告其球隊「"+setup["name"]+"」從現在開始正式加入季賽。");

			BASEBALL_D->join_season(myid);
			break;
		}
		case "leave":
		{	
			if( status == STATUS_PLAYING )
				return tell(me, "目前正在進行比賽，必須等待比賽結束。\n");
				
			if( !in_season )
				return tell(me, pnoun(2, me)+"原本就沒有參加季賽。\n");

			msg("$ME宣告其球隊「"+setup["name"]+"」退出季賽。\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("sport", me->query_idname()+"宣告其球隊「"+setup["name"]+"」退出季賽。");

			BASEBALL_D->leave_season(myid);
			
			break;
		}
		default:
		{
			string value, role;
			int number, n1, n2;

			if( sscanf(arg, "name %s", value) == 1 )
			{
				int len = noansi_strlen(value);
				
				if( status == STATUS_PLAYING )
					return tell(me, "目前正在進行比賽，必須等待比賽結束。\n");
				
				if( len < 4 || len > 12 )
					return tell(me, "球隊名稱長度不得低於 4 個字元或高於 12 個字元("+len+")。\n");
					
				value = kill_repeat_ansi(value+NOR);

				setup["name"] = value;
				
				CHANNEL_D->channel_broadcast("sport", me->query_idname()+"將其棒球隊隊名設定為「"+value+"」。");
				
				BASEBALL_D->set_setup(myid, setup);
			}
			else if( sscanf(arg, "change %d %d", n1, n2) == 2 )
			{
				mapping temp;
				
				if( n1 < 1 || n1 > 9 || n2 < 1 || n2 > 9 )
					return tell(me, "請輸入正確的棒次。\n");
				
				if( status == STATUS_PLAYING )
					return tell(me, "目前正在進行比賽，必須等待比賽結束。\n");
					
				temp = setup["roster"][n1];
				setup["roster"][n1] = setup["roster"][n2];
				setup["roster"][n2] = temp;
				
				BASEBALL_D->set_setup(myid, setup);
				
				tell(me, pnoun(2, me)+"將棒次 "+n1+" 與棒次 "+n2+" 的球員對換。\n");
			}			
			else if( sscanf(arg, "%s play %s %d", value, role, number) == 3 || sscanf(arg, "%s play %s", value, role) == 2 )
			{
				object labor = present(value);
				object oldlabor;
				
				if( status == STATUS_PLAYING )
					return tell(me, "目前正在進行比賽，必須等待比賽結束。\n");
				
				if( !objectp(labor) )
					return tell(me, "這裡沒有 "+value+" 這位員工。\n");
				
				if( query("boss", labor) != me->query_id(1) )
					return tell(me, labor->query_idname()+"並不是"+pnoun(2, me)+"的員工。\n");
						
				if( query("job/type", labor) != SPORTER )
					return tell(me, "請先設定員工的工作種類為球員。\n");

				role = upper_case(role);
				
				if( role != "P" )
				{
					if( number < 1 || number > 9  )
						return tell(me, "棒次設定僅能設定在 1 - 9 棒之間。\n");
				}
				else
				{
					if( !undefinedp(number) )
						return tell(me, "投手不會上場打擊，不需設定棒次。\n");
				}

				// 重覆配置的球員或守備位置
				for(int i=0;i<10;++i)
				{
					if( !mapp(setup["roster"][i]) ) continue;
						
					if( setup["roster"][i]["file"] == base_name(labor) || setup["roster"][i]["position"] == role || number == i )
					{
						oldlabor = load_object(setup["roster"][i]["file"]);
						
						if( role == "P" )
							tell(me, "取消原本由"+oldlabor->query_idname()+"擔任先發"+BASEBALL_D->query_positionname(role)+"的設定。\n");
						else
							tell(me, "取消原本由"+oldlabor->query_idname()+"擔任先發"+BASEBALL_D->query_positionname(role)+"與第 "+number+" 打擊棒次的設定。\n");

						setup["roster"][i] = 0;
					}
				}
				
				switch(role)
				{
					case "P":

						if( labor->query_skill_level("twohands") > 0 )
							return tell(me, "投手位置不可選擇左右開弓的球員擔任。\n");

						setup["roster"][0] = allocate_mapping(0);
						setup["roster"][0]["file"] = base_name(labor);
						setup["roster"][0]["object"] = labor;
						setup["roster"][0]["position"] = role;
						msg("$ME指派$YOU作為球隊的先發"+BASEBALL_D->query_positionname(role)+"。\n", me, labor, 1);
						break;
					case "C":
					case "1B":
					case "2B":
					case "3B":
					case "SS":
					case "RF":
					case "CF":
					case "LF":
					case "DH":
						setup["roster"][number] = allocate_mapping(0);
						setup["roster"][number]["file"] = base_name(labor);
						setup["roster"][number]["object"] = labor;
						setup["roster"][number]["position"] = role;
						msg("$ME指派$YOU作為球隊的先發"+BASEBALL_D->query_positionname(role)+"，並指定為第 "+number+" 棒次。\n", me, labor, 1);
						break;
					default:
						return tell(me, "沒有 "+role+" 這種守備位置，請參考 help setup 進行設定。\n");
						break;
				}
				
				BASEBALL_D->set_setup(myid, setup);
			}
			else
				return tell(me, "指令格式錯誤，請參考 help setup。\n");
		
			break;		
		}
	}
}

// 獲利
void do_gain(object me, string arg)
{
	string env_city;
	int money;
	object env;
	
	env = environment(me)->query_master();
	
	if( !me->is_user_ob() || query("owner", environment(me)) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"無法取走任何資金。\n");
	
	env_city = env->query_city();
	
	money = query("money", env);
	
	if( money <= 0 )
		return msg("$ME望著空白的進帳，不由的流下三滴淚水。\n"NOR, me, 0, 1);

	me->earn_money(MONEY_D->query_default_money_unit(), money);
	
	msg("$ME歡天喜地的將獲得的利潤收了起來，共得 "+HIY+money(MONEY_D->query_default_money_unit(), money)+NOR"。\n"NOR, me, 0, 1);
	
	CHANNEL_D->channel_broadcast("sport", HIY"票房"NOR YEL"收入 "NOR+me->query_idname()+"歡天喜地的將獲得的利潤收了起來，共得 "+HIY+money(MONEY_D->query_default_money_unit(), money)+NOR"。");
	
	delete("money", env);
	
	log_file("command/gain_baseball", me->query_idname()+"收取利潤 "+money(MONEY_D->query_default_money_unit(), money));

	env->save();
}


// 設定建築物內房間型態種類
nosave mapping action_info =
([
	"rest"	:
	([
		"short"	: HIG"休息室"NOR,
		"help"	:
			([
"topics":
@HELP
    球場，用來設定球員角色與參賽的地方
HELP,

"setup":
@HELP
設定比賽的指令，用法如下：[管理指令]
  setup 				- 查詢目前球隊設定
  setup join				- 正式參賽，系統將會自動安排賽事
  setup leave				- 退出參賽
  setup name '球隊名稱'			- 設定這個棒球隊的名稱
  setup change '棒次' '棒次'		- 對換兩個棒次(例 : setup change 1 9)
  setup '員工' play '守備位置' '棒次'	- 設定員工的守備位置(如下代碼)與棒次(1~9)
  					  例：setup girl 1B 2

守備位置代碼如下
  
  P	- 投手		需求技能：各式投球、守備技能
  C	- 捕手		需求技能：各式打擊、守備技能
  1B	- 一壘手	需求技能：各式打擊、守備技能
  2B	- 二壘手	需求技能：各式打擊、守備技能
  3B	- 三壘手	需求技能：各式打擊、守備技能
  SS	- 游擊手	需求技能：各式打擊、守備技能
  RF	- 右外野手	需求技能：各式打擊、守備技能
  CF	- 中外野手	需求技能：各式打擊、守備技能
  LF	- 左外野手	需求技能：各式打擊、守備技能
  DH	- 指定打擊	需求技能：各式打擊技能
  
五大屬性影響
  
  力量：影響基本投球、打擊威力
  體格：影響基本耐久力
  敏捷：影響基本守備能力
  智慧：影響基本跑壘能力
  魅力：影響基本球員粉絲人數、票房收入
  
三大能力值影響

  體力：體力不足將影響比賽至後半段時的表現
  生命：生命影響比賽中可能受傷的機率，不幸受傷時將影響表現
  精神：球隊分數落後的狀況下，精神不足將影響表現
  	
HELP,
			]),
		"heartbeat":0,	// 實際時間 1 秒為單位
		"job": SPORTER,
		"action":
			([
				"setup"		: (: do_setup :),
				"gain"		: (: do_gain :),
			]),
	]),
]);

// 設定建築物資料
nosave array building_info = ({
	
	// 建築物之中文名稱
	HIW"棒球"NOR WHT"場"NOR

	// 開張此建築物之最少房間限制
	,4

	// 城市中最大相同建築數量限制(0 代表不限制)
	,0

	// 建築物建築種類, GOVERNMENT(政府), ENTERPRISE(企業集團), INDIVIDUAL(私人)
	,INDIVIDUAL

	// 建築物可建築區域, AGRICULTURE_REGION(農業), INDUSTRY_REGION(工業), COMMERCE_REGION(商業)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// 開張儀式費用
	,120000000
	
	// 建築物關閉測試標記
	,0

	// 繁榮貢獻度
	,120
	
	// 最高可加蓋樓層
	,1
	
	// 最大相同建築物數量(0 代表不限制)
	,0
	
	// 建築物時代
	,3
});
