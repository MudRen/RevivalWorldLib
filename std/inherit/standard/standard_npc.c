/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : standard_npc.c
 * Author : Clode@RevivalWorld
 * Date   : 2001-11-5
 * Note   : �з� NPC �~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <feature.h>

/* ��� */
inherit DATABASE;

/* �M�� */
inherit CLEAN_UP;

/* �ƶq */
inherit AMOUNT;

/* ���� */
inherit FLY;

/* �ͪ� */
inherit LIVING_LIV;

/* ���A */
inherit CONDITION_LIV;

/* ���� */
inherit DELAY_LIV;

/* �x�� */
inherit STUCK_LIV;

/* ���� */
inherit MOVE_LIV;

/* �����B�z */
inherit MONEY_LIV;

/* �ޯ� */
inherit SKILL_LIV;

/* �ݩ� */
inherit ATTRIBUTION_LIV;

/* �˳� */
inherit EQUIPMENT_LIV;

/* �W��ID */
inherit IDNAME_NPC;

/* ���O */
inherit COMMAND_NPC;

/* �ʧ@ */
inherit ACTION_NPC;

/* �߸� */
inherit HEART_BEAT_LIV;

/* �T�� */
inherit MESSAGE_NPC;

/* ���ʦ欰 */
inherit BEHAVIOR_WALKING_NPC;

/* �ն� */
inherit GROUP_LIV;

int is_npc()
{
	return 1;
}
