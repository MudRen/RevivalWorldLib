/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/buff.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define BUFF_STR			"buff/attr/str"			// 力量
#define BUFF_PHY			"buff/attr/phy"			// 體格
#define BUFF_INT			"buff/attr/int"			// 智商
#define BUFF_AGI			"buff/attr/agi"			// 敏捷
#define BUFF_CHA			"buff/attr/cha"			// 魅力

#define BUFF_STAMINA_MAX		"buff/abi/stamina/max"		// 體力最大值
#define BUFF_HEALTH_MAX			"buff/abi/health/max"		// 生命最大值
#define BUFF_ENERGY_MAX			"buff/abi/energy/max"		// 精神最大值

#define BUFF_STAMINA_REGEN		"buff/abi/stamina/regen"	// 體力恢復速度
#define BUFF_HEALTH_REGEN		"buff/abi/health/regen"		// 生命恢復速度
#define BUFF_ENERGY_REGEN		"buff/abi/energy/regen"		// 精神恢復速度

#define BUFF_LOADING_MAX		"buff/loading/max"		// 負重最大值
#define BUFF_SLOT_MAX			"buff/slot/max"			// 空間最大值
#define BUFF_FOOD_MAX			"buff/stat/food/max"		// 食物最大值
#define BUFF_DRINK_MAX			"buff/stat/drink/max"		// 飲水最大值

#define BUFF_SOCIAL_EXP_BONUS		"buff/exp/social/bonus"		// 社會經驗值加成
#define BUFF_COMBAT_EXP_BONUS		"buff/exp/combat/bonus"		// 戰鬥經驗值加成

#define BUFF_MOVING_STAMINA		"buff/moving/stamina"		// 移動時的體力耗費
#define BUFF_MOVING_HEALTH		"buff/moving/health"		// 移動時的生命耗費
#define BUFF_MOVING_ENERGY		"buff/moving/energy"		// 移動時的精神耗費

#define BUFF_FULLSUIT			"buff/fullsuit/"		// 全套裝備加成

#define ATTACK				"attack"			// 攻擊
#define DEFEND				"defend"			// 防禦
#define SPEED				"speed"				// 攻速

#define BUFF_TRADE_ABILITY		"buff/trade/ability"		// 貿易能力
#define BUFF_COLLECT_ABILITY		"buff/collect/ability"		// 採集能力
#define BUFF_GROW_ABILITY		"buff/grow/ability"		// 生產能力
#define BUFF_LEAD_ABILITY		"buff/lead/ability"		// 領導能力
#define BUFF_SALE_ABILITY		"buff/sale/ability"		// 推銷能力

#define BUFF_FATAL_CHANCE		"buff/fatal/chance"		// 爆擊率
#define BUFF_FATAL_POWER		"buff/fatal/power"		// 爆擊力
#define BUFF_ANTIFATAL_CHANCE		"buff/antifatal/chance"		// 防爆擊率
#define BUFF_COUNTERATTACK_CHANCE	"buff/counterattack/chance"	// 反擊率
#define BUFF_SLIP_CHANCE		"buff/slip/chance"		// 滑倒率
#define BUFF_THORN_CHANCE		"buff/thorn/chance"		// 刺棘傷害
#define BUFF_COMBO_CHANCE		"buff/combo/chance"		// 連續攻擊

#define BUFF_BATTER_POWER		"buff/batter/power"		// 打擊威力
#define BUFF_PITCHER_POWER		"buff/pitcher/power"		// 投手威力
#define BUFF_FIELDER_POWER		"buff/fielder/power"		// 守備力

#define CONDITION_TYPE_POSITIVE		"[1;36m加[0;36m持[m"
#define CONDITION_TYPE_NEGATIVE		"[1;31m詛[0;31m咒[m"
#define CONDITION_TYPE_COUNTDOWN	"[1;32m計[0;32m時[m"
#define CONDITION_TYPE_SPECIAL		"[1;33m特[0;33m殊[m"
