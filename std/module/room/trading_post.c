/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : trading_post.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-4-13
 * Note   : �ۥѶR��Ұʧ@�~�Ӫ���
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

inherit __DIR__"inherit/_object_database_mod.c";
inherit __DIR__"inherit/_action_list";
inherit __DIR__"inherit/_action_buy";
inherit __DIR__"inherit/_action_sell";

#define TRADING_POST_CAPACITY	-1
#define DISASSEMBLE_MONEY		500000

nosave mapping allowable_trade = 
([

	"�A�@":
		({
			"/obj/farming/seed/herbage_seed", -1,
			"/obj/farming/seed/corn_seed", -1,
			"/obj/farming/seed/rice_seedling", -1,
			"/obj/farming/seed/barley_seedling", -1,
			"/obj/farming/seed/wheat_seedling", -1,
			"/obj/farming/seed/herb_seed", -1,
			"/obj/farming/seed/rose_seed", -1,
			"/obj/farming/seed/sunflower_seed", -1,
			"/obj/farming/seed/lily_seed", -1,
			"/obj/farming/seed/cotton_seed", -1,
		}),
	"����":
		({
			"/obj/pasture/breed/chicken_stud", -1,
			"/obj/pasture/breed/pig_stud", -1,
			"/obj/pasture/breed/sheep_stud", -1,
			"/obj/pasture/breed/cattle_stud", -1,
			"/obj/pasture/breed/deer_stud", -1,
		}),
	"�i��":
		({
			"/obj/fishfarm/breed/young_shrimp", -1,
			"/obj/fishfarm/breed/young_crab", -1,
			"/obj/fishfarm/breed/clam", -1,
		}),
	"����":
		({
			"/obj/fishing/rod/coarse_rod", -1,
			"/obj/fishing/bait/bread_bug", -1,
			"/obj/fishing/bait/prawn", -1,
			"/obj/fishing/bait/tsou_chip", -1,
			"/obj/fishing/bait/worm", -1,
		}),
	"�Ķ�":
		({
			"/obj/collection/tool/coarse_pickax", -1,
			"/obj/collection/tool/coarse_drill", -1,
			"/obj/collection/tool/coarse_bucket", -1,
			"/obj/collection/tool/coarse_saw", -1,
			"/obj/collection/tool/coarse_shovel", -1,
			
		}),
	"����":
		({
			"/obj/systemproducts/pure_water", -1,
			"/obj/systemproducts/steamed_roll", -1,
			"/obj/systemproducts/dark_plum", -1,
		}),
	"�ĤY":
		({
			"/obj/systemproducts/pill_of_purification", -1,
			"/obj/systemproducts/pill_of_invincibility", -1,
			"/obj/systemproducts/pill_of_strength", -1,
			"/obj/systemproducts/pill_of_physique", -1,
			"/obj/systemproducts/pill_of_intelligence", -1,
			"/obj/systemproducts/pill_of_agility", -1,
			"/obj/systemproducts/pill_of_charisma", -1,
		}),
	"��L":
		({
            "/obj/etc/newspaper", -1,
            "/obj/etc/firework", -1,
            "/obj/systemproducts/flatbed_tricycle", -1,
            "/obj/systemproducts/fortune_bag", -1,
		}),

]);

mapping exchange_objects =
([
	"/obj/battlefield/battle_insigne" : ({ HIC"��"NOR CYN"��"HIC"��"NOR CYN"��"NOR, "battle_insigne"}),
]);

void do_exchange(object me, string arg)
{
	object ob;
	string basename;

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�V�j���ӧI�����򪫫~�H\n");	
	
	if( !objectp(ob = present(arg, me)) )
		return tell(me, "�o�̨S�� "+arg+" �o�����~�C\n");
		
	basename = base_name(ob);
	
	if( undefinedp(exchange_objects[basename]) )
		return tell(me, ob->query_idname()+"����O�j���A�L�k�I���C\n");
		
	if( arrayp(exchange_objects[basename]) )
	{
		int amount = ob->query_amount();
		
		msg("$ME�N"+ob->query_idname()+"�浹�j���ӡA�I�����u"+exchange_objects[basename][0]+"�v�I�� "+amount+" �I�C\n", me, 0, 1);
	
		log_file("command/trading_post_exchange", me->query_idname()+"�N"+ob->query_idname()+"�浹�j���ӡA�I�����u"+exchange_objects[basename][0]+"�v�I�� "+amount+" �I�C");
		
		addn(exchange_objects[basename][1], amount, me);
		
		destruct(ob);
	}
	else if( stringp(exchange_objects[basename]) )
	{
		object newob = new(exchange_objects[basename]);

		newob->set_amount(ob->query_amount());
		
		msg("$ME�N"+ob->query_idname()+"�浹�j���ӡA�I�����s��"+newob->query_idname()+"�C\n", me, 0, 1);
	
		log_file("command/trading_post_exchange", me->query_idname()+"�N"+ob->query_idname()+"�浹�j���ӡA�I�����s��"+newob->query_idname()+"�C");
		
		destruct(ob);
	
		if( !me->available_get(newob, newob->query_amount()) )
		{
			tell(me, pnoun(2, me)+"�����ʨ���h���F��A�u�n����b�a�W�F�C\n");
			newob->move_to_environment(me);
		}
		else
			newob->move(me);
	}
		
	me->save();
}

void do_disassemble(object me, string arg)
{
	object ob;
	object material;
	int too_heavy;
	string msg="";
	mapping materials;
	int random_amount;
	int count = 1;
	string money_unit = environment(me)->query_money_unit();
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n��Ѥ��򪫫~�H\n");	
	
	sscanf(arg, "%d %s", count, arg);
	
	if( count < 1 ) count = 1;
		
	if( !objectp(ob = present(arg, me)) )
		return tell(me, "�o�̨S�� "+arg+" �o�����~�C\n");
	
	if( count > ob->query_amount() )
		return tell(me, pnoun(2, me)+"���W�S���o��h��"+ob->query_idname()+"�C\n");
		
	if( ob->is_keeping() )
		return tell(me, pnoun(2, me)+"�������Ѱ�"+ob->query_idname()+"���O�d���A�C\n");
	
	if( ob->is_module_product() )
		materials = query("design/inlay", ob);
	else
		materials = query("disassemble", ob);

	if( !sizeof(materials) )
		return tell(me, ob->query_idname()+"�L�k��ѡC\n");

	if( !ob->is_module_product() )
	{
		if( !me->spend_money(money_unit, DISASSEMBLE_MONEY * count) )
			return tell(me, pnoun(2, me)+"���W�������� "HIY+money(money_unit, DISASSEMBLE_MONEY * count)+NOR"�C\n");

		msg("$ME��O "HIY+money(money_unit, DISASSEMBLE_MONEY * count)+NOR" �N "+count+" "+(query("unit", ob)||"��")+ob->query_idname()+"�浹�u�H��ѡA���^�U�C���~�G\n", me, 0, 1);
	}

	for(int i=0;i<count;++i)
	{
		foreach(string material_name, int amount in materials)
		{
			material = new(material_name);
			
			if( ob->is_module_product() )
			{
				if( amount > 8000 )
					random_amount = amount * 99 / 100;
				else
					random_amount = amount;
			}
			else if( random(5) <= random(amount) )
				random_amount = random(amount + 1);
			else
				random_amount = 0;
				
			if( random_amount <= 0 || random_amount > amount )
				continue;
			
			material->set_amount(random_amount);
			
			msg += NOR YEL"�D"NOR+trim(QUANTITY_D->obj(material, random_amount))+"\n";
			
			if( query("special", material) )
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"��"+ob->query_idname()+"�W��ѥX�u"+QUANTITY_D->obj(material, random_amount)+"�v");
	
			if( !me->available_get(material, material->query_amount()) )
			{
				too_heavy = 1;
				material->move_to_environment(me);
			}
			else
				material->move(me);
		}
	}
	
	if( msg == "" )
		msg += NOR YEL"�D"HIY"����ѥX���󪫫~\n"NOR;

	else
		log_file("command/disassemble", msg);

	msg(msg, me, 0, 1);

	if( too_heavy )
		tell(me, pnoun(2, me)+"�����ʨ���h���F��A�u�n����b�a�W�F�C\n");

	destruct(ob, count);

	me->save();
}

void do_element(object me, string arg)
{
	object ob;
	object newob;
	int amount;

	string to;
	string from;
	
	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�V�����j�L�ӧI�����򪫫~�H\n");	
	
	if( sscanf(arg, "%d %s to %s", amount, from, to) != 3 )
		return tell(me, "�п�J���T�����O�榡�C\n");

	if( !objectp(ob = present(from, me)) )
		return tell(me, "�o�̨S�� "+from+" �o�����~�C\n");
		
	if( !ob->is_gem_object() )
		return tell(me, "�u�������~��i��I���C\n");
			
	if( amount <= 0 )
		amount = 1;

	if( amount > ob->query_amount() )
		return tell(me, pnoun(2, me)+"���W�S���o��h��"+ob->query_idname()+"�C\n");
		
	switch(lower_case(to))
	{
		case "red": 
			newob = new("/obj/gem/gem0");
			break;
		case "orange":
			newob = new("/obj/gem/gem1");
			break;
		case "yellow":
			newob = new("/obj/gem/gem2");
			break;	
		case "green":
			newob = new("/obj/gem/gem3");
			break;	
		case "blue":
			newob = new("/obj/gem/gem4");
			break;	
		case "indigo":
			newob = new("/obj/gem/gem5");
			break;	
		case "purple":
			newob = new("/obj/gem/gem6");
			break;	
		default:
			return tell(me, "�S�� "+to+" �o�غ����������C\n");
	}
	
	log_file("command/element", me->query_idname()+"�N"+amount+" "+ob->query_idname()+"�浹�j�L�ӡA�I����"+newob->query_idname()+"�C");
	
	newob->set_amount(amount);
		
	msg("$ME�N "+amount+" ��"+ob->query_idname()+"�浹�j�L�ӡA�����I����"+newob->query_idname()+"�C\n", me, 0, 1);
	
	destruct(ob, amount);
	
	if( !me->available_get(newob, newob->query_amount()) )
	{
		tell(me, pnoun(2, me)+"�����ʨ���h���F��A�u�n����b�a�W�F�C\n");
		newob->move_to_environment(me);
	}
	else
		newob->move(me);
		
	me->save();
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"lobby"	:
	([
		"short"	: HIW"�R��j�U"NOR,
		"help"	:
			([
"topics":
@HELP
    �R��j�U�O���a�ʶR�P�c��ӫ~���a��A�ѩ�O�ѬF���}�]�A�ȷ|�H��������V���a���ʰӫ~�C
HELP,

"list":
@HELP
�C�X�Ҧ��c�椤���ӫ~�����O�A�Ϊk�p�U�G
  list			- �C�X�Ҧ��c�椤���ӫ~
  list 2		- �C�X�ĤG���ӫ~���ԲӸ��
HELP,

"buy":
@HELP
�ʶR�ӫ~�����O�A�Ϊk�p�U�G
  buy all rice			- �R�U�Ҧ��� rice �ӫ~
  buy 3 rice			- �R�U 3 �� rice �ӫ~
  buy rice			- �R�U 1 �� rice �ӫ~
  buy 3 2			- �R�U 3 ��s���� 2 ���ӫ~
  buy 2				- �R�U 1 ��s���� 2 ���ӫ~
  buy rice to center 1 51,22	- �R�U 1 �� rice �ӫ~�ðe�F center ������ 1 �ìP�����y��(51,22)�B���ؿv��
HELP,
			]),
		"action":
			([
				"list"		: (: do_list($1, $2, "products", TRADING_POST_CAPACITY) :),
				"buy"		: (: do_buy($1, $2, "products", "GOVERNMENT") :),
			]),
	]),
	"workroom"	:
	([
		"short"	: HIW"�u�{"NOR,
		"help"	:
			([
"topics":
@HELP
    �u�{�i�H�Ψӥ洫�j�����~�A�H�αN�@�ǵ}�����~��Ѧ������C
HELP,

"exchange":
@HELP
�V�j���ӧI�����~�����O�A�Ϊk�p�U�G
  exchange '���~'		- �V�j���ӧI�����W���Y�����~
HELP,

"element":
@HELP
�V�j�L�ӧI�����������O�A�Ϊk�p�U�G
  element '�ƶq' '�C��' to '�C��' - �V�j�L�ӧI�����������O(ex: element 5 blue to red)
  				    (�C������]�t red/orange/yellow/green/blue/indigo/purple)
HELP,

"disassemble":
@HELP
�Фu�H���z�N���~��Ѧ������A�Ϊk�p�U�G
  disassemble '���~'		- ��ѬY�����~
HELP,
			]),
		"action":
			([
				"exchange"	: (: do_exchange :),
				"disassemble"	: (: do_disassemble :),
				"element"	: (: do_element :),
			]),
	]),
]);


// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	NOR YEL"���y����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,5000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,10
	
	// �̰��i�[�\�Ӽh
	,2
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});
