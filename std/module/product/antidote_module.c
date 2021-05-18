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

#include <feature.h>
#include <condition.h>

inherit PRODUCT_ACTION_MOD;

string* available_poisons = ({ POISON_GIANT_SPIDER, MAGNETIC_STONE });

// �`�g�Ѭr��
void do_inject(object me, string arg, object ob)
{
	object target;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n���֪`�g"+ob->query_idname()+"�H\n");
		
	if( arg == "me" )
		target = me;
	else if( !objectp(target = present(arg)) )
		return tell(me, "�o�̨S�� "+arg+" �o�ӤH�C\n");
		
	if( !target->is_living() )
		return tell(me, pnoun(2, me)+"�L�k��"+target->query_idname()+"�`�g"+ob->query_idname()+"�C\n");
		
	msg("$ME���_"+ob->query_idname()+"��"+(me==target?"$ME":"$YOU")+"���u�W�`�g�i�h�C\n", me, target==me?0:target, 1);
	
	foreach(string poison in available_poisons)
	{
		if( target->in_condition(poison) )
			target->stop_condition(poison);
	}

	destruct(ob, 1);
}

// �]�w����ʧ@
nosave mapping action_info =
([
	"help"	: "�|���غc�����C",
	"action":
		([
			"inject"	: (: do_inject :),
		]),
]);

// �]�w������
nosave mapping product_info = 
([	
	//����W��
	"name":		"�Ѭr��",
	
	//�^��W��
	"id":		"antidote",

	//���ƻݨD
	"material":	([ "distilled_water":10, "/obj/farming/product/herb":20, "glass":1 ]),

	//�ޯ�ݨD
	"skill":	([ "adventure-fac":70, "technology":70 ]),

	//�Ͳ��u�t
	"factory":	"adventure",
]);
