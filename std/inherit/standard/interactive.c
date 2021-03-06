/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : interactive.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-2-1
 * Note   : 基本互動繼承物件
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

/* 資料 */
inherit DATABASE;

/* 儲存 */
inherit SAVE;

/* 數量 */
inherit AMOUNT;

/* 飛行 */
inherit FLY;

/* 生物 */
inherit LIVING_LIV;

/* 任務 */
inherit QUEST_LIV;

/* 狀態 */
inherit CONDITION_LIV;

/* 金錢 */
inherit MONEY_LIV;

/* 屬性 */
inherit ATTRIBUTION_LIV;

/* 延遲 */
inherit DELAY_LIV;

/* 困住 */
inherit STUCK_LIV;

/* 移動 */
inherit MOVE_LIV;

/* 技能 */
inherit SKILL_LIV;

/* 裝備 */
inherit EQUIPMENT_LIV;

/* 登入物件載入 */
inherit INVENTORY_LIV;

/* 名稱ID */
inherit IDNAME_USR; 

/* 訊息 */
inherit MESSAGE_USR;

/* 輸入 */
inherit INPUT_USR;

/* 閱讀 */
inherit MORE_USR;

/* 編輯 */
inherit EDIT_USR;

/* 心跳 */
inherit HEART_BEAT_LIV;

/* 動作 */
inherit ACTION_USR;

/* 載入 */
inherit RESTORE_USR;

/* 指令 */
inherit COMMAND_USR;

/* 組隊 */
inherit GROUP_LIV;

int is_person()
{
	return 1;
}
