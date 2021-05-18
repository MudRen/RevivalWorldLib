/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : module.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <inherit.h>
#include <feature.h>
#include <delay.h>
#include <daemon.h>

inherit PRODUCT_ACTION_MOD;
inherit BULLETIN_BOARD;

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�L",
	"action":
		([
		    "post"		:(: do_post :),
		    "read"		:(: do_read :),
		    "discard"		:(: do_discard :),
		    "followup"		:(: do_followup :),
		    "mail"		:(: do_mail :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�d���O",
	
	//�^��W��
	"id":		"board",

	//���ƻݨD
	"material":	([ "normal_timber":30 ]),

	//�ޯ�ݨD
	"skill":	([ "furniture-fac":30, "technology":30 ]),

	//�Ͳ��u�t
	"factory":	"furniture",
]);
