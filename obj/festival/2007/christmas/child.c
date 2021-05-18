/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : child.c
 * Author : Kyo@RevivalWorld
 * Date   : 2007-12-21
 * Note   : �p��
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

/*
	�t�ιw�]�� NPC �����禡
	void reply_emote(object me, string verb, string args);
	void reply_say(object me, string msg);
	void reply_catch_tell(object me, string verb, string args);
	void reply_get_object(object me, object ob, mixed amount);
	void reply_get_money(object me, string unit, mixed money);
*/

#include <ansi.h>
#include <inherit.h>
#include <gender.h>
#include <daemon.h>
#include <npc.h>
#include <condition.h>
#include <feature.h>

inherit STANDARD_PERSON;

void reply_get_object(object me, object ob, mixed amount)
{
	string basename = base_name(ob);
	string checkid = query("santaid", this_object());
	//int quest = query_temp("christmasgift", me);

	if( !objectp(ob) ) return;

	if(basename == "/obj/festival/2007/christmas/gift_box")
	{
		if(checkid == me->query_id(1))
		{
			destruct(ob);
			do_command("jump " +me->query_id(1));
			do_command("say ����"+pnoun(2, me)+"���ڰe�ӳo��§���A�ڭn���^�a�h�������ݡI�I");
			set_temp("christmas", 0, me); // �B�J�k0
			addn_temp("christmasgift", 1, me); // �W�[�@��
			tell( me,HIR"���ȧ������~�������t�ϦѤH�e�U�@����§���C\n"NOR);
			destruct(this_object());
		}
		else
		{
			do_command("say �o��§�����O�H�a�n���աI�I");
			do_command("drop all "+ob->query_id(1)+"");
		}
	} else {
		do_command("say �H�a�n�t�ϦѤ�������§�����O�o�ӰաI�I");
		do_command("drop all "+ob->query_id(1)+"");
	}

}

void destruct_self()
{
        //shout(HIB"����"NOR" - ������§�����p�ĥ��檺�^�a�F�C\n");
	destruct(this_object());
}

void create()
{
	set_idname("child", "�p��");

	set("unit", "��");
	set("age", range_random(1, 5));
	set("gender", MALE_GENDER);

	set("long", "�@���ݸt�ϦѤH�e§�����p�ġC");
	set("no_fight", 1);

	startup_living();

	if( clonep() )
		call_out( (: destruct_self :), 180 ); // �T�������S�e��N����
}

int remove()
{
	destruct(all_inventory(this_object()));
}

