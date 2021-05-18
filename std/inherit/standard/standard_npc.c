/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : standard_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-11-5
 * Note   : 標準 NPC 繼承檔
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

/* 資料 */
inherit DATABASE;

/* 清除 */
inherit CLEAN_UP;

/* 數量 */
inherit AMOUNT;

/* 飛行 */
inherit FLY;

/* 生物 */
inherit LIVING_LIV;

/* 狀態 */
inherit CONDITION_LIV;

/* 延遲 */
inherit DELAY_LIV;

/* 困住 */
inherit STUCK_LIV;

/* 移動 */
inherit MOVE_LIV;

/* 金錢處理 */
inherit MONEY_LIV;

/* 技能 */
inherit SKILL_LIV;

/* 屬性 */
inherit ATTRIBUTION_LIV;

/* 裝備 */
inherit EQUIPMENT_LIV;

/* 名稱ID */
inherit IDNAME_NPC;

/* 指令 */
inherit COMMAND_NPC;

/* 動作 */
inherit ACTION_NPC;

/* 心跳 */
inherit HEART_BEAT_LIV;

/* 訊息 */
inherit MESSAGE_NPC;

/* 走動行為 */
inherit BEHAVIOR_WALKING_NPC;

/* 組隊 */
inherit GROUP_LIV;

int is_npc()
{
	return 1;
}
