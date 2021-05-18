/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : research.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-01-07
 * Note   : ��o����
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
#include <skill.h>

inherit ROOM_ACTION_MOD;
#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define RANDOM_THING			({ "�馡����", "�̥x��", "�ަ�|", "������", "���s�Ϧ��l", "�����n", "�J����", "�H����", "�䨧�G", "���Ʀ׶�", "�j�z�]�p�z", "�s�˰^�Y", "�x�n�J�_", "�n��N��", "�H���̯�", "�̯��e", "������", "�H�J�ѽu", "�H�J��", "�ĿL�ư�", "�Ȯa�p��", "��J��", "�����", "����B", "�Y�Ѫ��w��", "�����Y�l", "��ͳ��", "�x���Ӷ���", "����V", "�Q�K�����׺�", "�d�~�F��", "�Ѥs����", "�j�٤�", "�ڦ�iۣ", "�����f�c��", "�ï]����", "�Ȯa�ݯ�", "����", "�ǻ������n�H�d", "���H�h���h���Υä[�|��", "�R�𫽫�", "�б�", "�Ш|����", "�`��"})

#define COST_SOCIAL_EXP	1000000

// ���o�Ҧ��i�H�Ͳ������~ module ����}�C
private object *query_available_products(object me)
{
	string product;
	object *modules, module;
	
	modules = allocate(0);
	
	foreach(product in get_dir(PRODUCT_MODULE_PATH))
	{
		if( product[<9..<1] != "_module.c" ) continue;

		catch( module = load_object(PRODUCT_MODULE_PATH + product) );
		
		if( !objectp(module) || module->no_available(me) ) continue;
				
		modules += ({ module });
	}
	
	return modules;
}

void do_list_action(object me, string arg)
{
	int i;
	string file, msg;
	object product;
	string *productslist;
	object env = environment(me);
	string owner = query("owner", env);
	
	if( owner != me->query_id(1) && wizardp(me) )
	{
		object owner_ob = find_player(owner) || load_user(owner);
		
		productslist = query("products", find_player(owner));
			
		if( !sizeof(productslist) )
			return tell(me, owner+" �|���]�p�X����i�H�Ͳ������~�C\n");
		
		if( !userp(owner_ob) )
			destruct(owner_ob);
	}
	else
	{
		productslist = query("products", me);
	
		if( !sizeof(productslist) )
			return tell(me, pnoun(2, me)+"�|���]�p�X����i�H�Ͳ������~�C\n");
	}
	
	msg = (owner == me->query_id(1) ? pnoun(2, me) : owner+" ")+"�ثe�w�g��o���������~�p�U�G\n"WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		msg += sprintf("%-4s %s %s\n", (++i)+".", product->query_idname(), wizardp(me) ? file : "");
	}
	
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	me->more(msg);
}

void do_design_action(object me, string arg)
{
	string value;
	mapping product_info;
	object *products, product;
	object env = environment(me);
	object master = env->query_master();

	if( !arg )
	{
		string msg;
		mapping design = query("design", master);
		
		if( !mapp(design) )
			return tell(me, "�ثe�S�����󲣫~��o��ơA�O�_�n��o���~�H(type, id, name, inlay, finish)\n");
			
		msg = "�ثe�̷s��o�������~���G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += HIW"���~����"NOR WHT"(type)"NOR" �G"NOR+(design["type"]||"���]�p")+"\n";
		msg += HIR"���~�^��"NOR RED"(id)"NOR"   �G"NOR+(design["id"]||"���]�p")+"\n";
		msg += HIC"���~����"NOR CYN"(name)"NOR" �G"NOR+(design["name"]||"���]�p")+"\n";
		msg += HIY"�^�O���~"NOR YEL"(inlay)"NOR"�G\n"NOR;
			
		if( mapp(design["inlay"]) )
		foreach(string file, int num in copy(design["inlay"]))
		{
			if( !file_exists(file) ) continue;

			product = load_object(file);
			
			if( !objectp(product) ) continue;
			
			msg += "                 "+QUANTITY_D->obj(product, num, 1)+"\n";
		}
		else
			msg += "                 ���^�O���󪫫~\n";
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

		
		tell(me, msg);
		return;
	}

	if( query("owner", env) != me->query_id(1) && !wizardp(me) )
		return tell(me, pnoun(2, me)+"���O�o����o���ߪ��֦��̡C\n");

	sscanf(arg, "%s %s", arg, value);
	
	switch(arg)
	{
		case "type":
		{
			if( query("design/type", master) )
				return tell(me, pnoun(2, me)+"�����������Ψ����ثe���b��o�����~�C\n");
				
			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n��o���@�ز��~(plist)�H\n");

			value = lower_case(value);

			products = query_available_products(me);
		
			if( !sizeof(products) )
				return tell(me, "�ثe�L�k��o���󲣫~�C\n");
			
			foreach( product in products )
			{
				product_info = product->query_product_info();
	
				if( product_info["id"] == value )
				{
					foreach(string skill, int level in product_info["skill"])
						if( me->query_skill_level(skill) < level )
							return tell(me, pnoun(2, me)+"��"+(SKILL(skill))->query_name()+"���Ť��� "+level+" �L�k��o�������~�C\n");
	
					set("design/type", value, master);

					msg("$ME�N�}�o�����~�����]�w���u"+product_info["name"]+"("+product_info["id"]+")�v�C\n", me, 0, 1);
					master->save();
					return;
				}
			}
			
			return tell(me, "�L�k��o "+value+" �o�ز��~�A"+pnoun(2, me)+"�Q�n��o���@�ز��~(plist)�H\n");
			break;
		}
		case "name":
		{
			string name;
			if( !query("design/type", master) )
				return tell(me, "�Х��M�w�n��o�����~����(type)�C\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�����~������˪��W�١H\n");

			products = query_available_products(me);
			foreach( product in products )
			{
				product_info = product->query_product_info();
				
				if( product_info["id"] == query("design/type", master) )
				{
					name = product_info["name"];
					break;
				}
			}

			value = kill_repeat_ansi(trim(value)+NOR);
	
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 2 )
				return tell(me, "���~�W�٪��ץ����b 2 �� 16 �Ӧr�������C\n");

			msg("$ME�N���~���W�ٳ]�p���u"+value+"�v�C\n", me, 0, 1);
			
			set("design/name", value, master);
			
			master->save();
			break;
		}
		case "id":
		{
			if( !query("design/type", master) )
				return tell(me, "�Х��M�w�n��o�����~����(type)�C\n");

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�����~������˪��^��W�١H\n");

			value = kill_repeat_ansi(lower_case(trim(value))+NOR);

			if( !is_english(value) )
				return tell(me, "�ХΡu�^��v�����~���^��W�١C\n");
			
			if( noansi_strlen(value) > 16 || noansi_strlen(value) < 3 )
				return tell(me, "�^��W�٥����b 3 �� 16 �ӭ^��r�Ƥ����C\n");

			msg("$ME�N���~���^��W�ٳ]�p���u"+value+"�v�C\n", me, 0, 1);
			
			set("design/id", value, master);
			
			master->save();
			break;
		}
		case "delete":
		{
			string *productslist = query("products", me)||allocate(0);
			int num;
			
			if( !value || !value[0] || !to_int(value) )
				return tell(me, "�п�J���T���s���C\n");
			
			num = to_int(value) - 1;
				
			if( num < 0 || num > sizeof(productslist)-1 )
				return tell(me, "�S���o�ӽs�������~�C\n");
			
			product = load_object(productslist[num]);
			
			productslist = productslist[0..num-1] + productslist[num+1..];
			
			set("products", copy(productslist), me);
			
			msg("$ME�N�s���� "+value+" �����~�u$YOU�v�q��o���~�C���R���C\n", me, product, 1);
			
			me->save();
			break;
		}
		case "inlay":
		{
			object ob;
			mapping inlay;
			int amount;

			if( !value || !value[0] )
				return tell(me, pnoun(2, me)+"�Q�n�N���򪫫~�^�O�b���~���H\n");
			
			if( value == "cancel" )
			{
				inlay = query("design/inlay", master);
				
				if( !mapp(inlay) || !sizeof(inlay) )
					return tell(me, "�ثe�S���^�O���󪫫~�C\n");
					
				foreach(string basename, int num in inlay)
				{
					ob = new(basename);
				
					if( query("flag/no_amount", ob) )
						ob->move(me, num);
					else
						ob->set_amount(num);
						
					ob->move(me);
				}	
				
				delete("design/inlay", master);
				master->save();
				msg("$ME�N�Ҧ��^�O���~���^�C\n", me, 0, 1);
				return;
			}
			
			sscanf(value, "%d %s", amount, value);

			if( !objectp(ob = present(value, me)) )
				return tell(me, pnoun(2, me)+"�����W�S�� "+value+" �o�ت��~�C\n");
			
			if(me->is_equipping_object(ob) )
				return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"���˳ƪ��A�C\n");

			if(ob->is_keeping())
				return tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"���O�d���A�C\n");

			if( amount <= 0 )
				amount = 1;
				
			if( amount > ob->query_amount() )
				return tell(me, pnoun(2, me)+"���W�S���o��h��"+ob->query_idname()+"�C\n");

			inlay = query("design/inlay", master) || allocate_mapping(0);
			
			inlay[base_name(ob)] += amount;

			set("design/inlay", inlay, master);

			msg("$ME�N "+amount+" ��$YOU�^�O�첣�~�]�p�ҫ��W�i����աA���~���������o�X"HIW"�_�S"NOR"��"WHT"���~"NOR"�C\n", me, ob, 1);	
			
			destruct(ob, amount);
			master->delay_save(60);
			me->delay_save(60);
			break;
		}
		case "cancel":
		{
			object ob;
			mapping inlay = query("design/inlay", master);
			
			if( mapp(inlay) )		
			foreach(string basename, int num in inlay)
			{
				ob = new(basename);
			
				if( query("flag/no_amount", ob) )
					ob->move(me, num);
				else
				{
					ob->set_amount(num);
					ob->move(me);
				}
			}	

			msg("$ME�N�Ҧ��^�O���~���^�C\n", me, 0, 1);
			msg("$ME�����F�ثe���b��o�����~�C\n", me, 0, 1);
			delete("design", master);

			master->save();
			me->save();
			break;
		}	
		case "finish":
		{
			object book;
			int technology;
			string *productslist = query("products", me)||allocate(0);
			string type = query("design/type", master);
			string id = query("design/id", master);
			string name = query("design/name", master);
			mapping inlay = query("design/inlay", master);
			string money_unit = master->query_money_unit();
			int inlay_num;
			int cost_money;
				
			if( !type )
				return tell(me, "���]�w�Ͳ����~�����C\n");

			if( !id )
				return tell(me, "���]�w���~�^��W�١C\n");

			if( !name )
				return tell(me, "���]�w���~����W�١C\n");

			type = replace_string(type, " ", "_");

			product = load_object(PRODUCT_MODULE_PATH+type+"_object.c");
			product_info = load_object(PRODUCT_MODULE_PATH+type+"_module.c")->query_product_info();

			technology = to_int(pow(to_float(query("value", product)), 0.7));	

			if( query("technology", master) < technology )
				return tell(me, "��o���ߥثe�ֿn����ޭȤ����H�}�o���ز��~�C\n");

			if( count(me->query_social_exp_cur(), "<", COST_SOCIAL_EXP) )
				return tell(me, pnoun(2, me)+"�����|�g��Ȥ��� "+COST_SOCIAL_EXP+"�A�L�k�i���o�C\n");

			foreach(object inv in all_inventory(me))
			if( base_name(inv) == "/obj/book/product/"+type )
			{
				book = inv;
				break;
			}
			
			if( !objectp(book) )
				return tell(me, pnoun(2, me)+"���W�����n���u"+product_info["name"]+"("+product_info["id"]+")�v���t��Ѥ~�i�H�i�沣�~�}�o�C\n");

			cost_money = query("value", product) * 1000;

			if( !me->spend_money(money_unit, cost_money) )
				return tell(me, pnoun(2, me)+"���{������ "HIY+money(money_unit, cost_money)+NOR" �����H������o�g�O�C\n");

			if( !me->cost_social_exp(COST_SOCIAL_EXP) )
				return tell(me, pnoun(2, me)+"�����|�g��Ȥ��� "+COST_SOCIAL_EXP+"�A�L�k�i���o�C\n");

			product = load_object("/product/"+type+"/"+SERIAL_NUMBER_D->query_serial_number("product/"+type));
			
			product->set_idname(id, name);		

			msg("$ME�ӶO�F "HIC+technology+NOR" �I��ޭȡB"HIG+COST_SOCIAL_EXP+NOR" ���|�g��ȻP "HIY+money(money_unit, cost_money)+NOR" �i��u"+product->query_idname()+"�v���~���}�o�C\n", me, 0, 1);
			
			addn("technology", -technology, master);

			set("design/owner", me->query_id(1), product);
			set("design/city", CITY_D->query_city_idname(master->query_city()), product);
			set("design/room", env->query_room_name(), product);
			set("design/time", time(), product);
			set("design/type", type, product);

			// �@�^�O�����]�w
			if( mapp(inlay) )
			{
				object inlay_ob;

				inlay_num = implode(values(inlay), (: $1+$2 :));
				set("design/inlay", inlay, product);		
				
				foreach(string basename, int num in inlay)
				{
					inlay_ob = new(basename);
				
					msg("$ME���^ "+num+" ��"+inlay_ob->query_idname()+"�C\n", me, 0, 1);
				
					if( query("flag/no_amount", inlay_ob) )
						inlay_ob->move(me, num);
					else
					{
						inlay_ob->set_amount(num);
						inlay_ob->move(me);
					}
				}
			}

			set("value", product->query_value(), product);
			product->save();			
			productslist = ({ base_name(product) }) | productslist;
			
			delete("design", master);

			destruct(book, 1);

			set("products", copy(productslist), me);
			me->save();
			master->save();

			if( !wizardp(me) )
				CHANNEL_D->channel_broadcast("news", me->query_idname()+"��"+query("short", env)+"�����̷s�u"+product_info["name"]+"("+product_info["id"]+")�v���~�]�p�éR�W���u"+product->query_idname()+"�v�A�^�O���~�Ƭ� "+inlay_num+"�A���Ȭ� "HIY+NUMBER_D->number_symbol(query("value", product))+NOR"�C");

			break;
		}
		default:
			return tell(me, "�L�k�}�o "+arg+" �o�ز��~�C\n");
			break;
	}
}

void heart_beat(object room)
{
	int technologylevel;
	int researchmanage;
	int technology;
	int userinside;
	int npcinside;
	string owner = query("owner", room);

	object npc, ob;
	object *npcs;
	string *slaves;

	if( !find_player(owner) ) return;

	npcs = allocate(0);
	slaves = room->query_slave();

	if( !query_temp("research_heart_beat", room) )
		set_temp("research_heart_beat", random(10240), room);
		
	if( !(addn_temp("research_heart_beat", 1, room)%240) )
		room->save();

	foreach(ob in all_inventory(room))
	{
		if( ob->is_module_npc() && query("job/type", ob) == RD && query("boss",ob) == owner )
		{
			npcs += ({ ob });
			++npcinside;
		}
		
		if( userp(ob) )
			userinside = 1;
	}
	
	if( npcinside )
	{
		foreach(npc in npcs)
		{
			technologylevel += npc->query_skill_level("technology");
			researchmanage += npc->query_skill_level("researchmanage");
		}
		
		technologylevel /= npcinside;
		researchmanage /= 10;

		npc = npcs[random(sizeof(npcs))];
	}

	set("technologylevel", technologylevel, room);
	set("researchmanage", researchmanage, room);

	if( researchmanage < sizeof(slaves) + 1 )
	{
		if( userinside )
			broadcast(room, "��o�޲z����O�L�k�伵�L�q����o���߳s��C\n");
		return;	
	}

	if( npcinside )
	{
		if( random(technologylevel) < 20 )
		{
			technology = to_int((-random(2)-1) * pow(sizeof(slaves)+1, 0.3));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME���F�Ӥj����A�u����I�I�v�f���w��q���O�W��]�Ƶ��N�a�F�I(��ޭ� "HIR+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME��o�_�Q��u"HIY+RANDOM_THING[random(sizeof(RANDOM_THING))]+NOR"�v�q�q���ƾǭ�Ƹ̭ˡA�u���@�@"WHT"�·�"NOR"�G����ӹ���ǡI(��ޭ� "HIR+technology+NOR")\n", npc, 0, 1);
			}
		}
		else
		{
			technology = random(to_int(pow(technologylevel / 6 * (sizeof(slaves)+1), 0.5)));
			
			if( userinside )
			{
				if( !random(2) )
					msg("$ME�̴`�ۼзǪ�����{�ǡA�N�s�� "+sprintf(HIG"%c%c"NOR"-"GRN"%d%d%d%d%d"NOR, range_random(65,90),range_random(65,90),random(10),random(10),random(10),random(10),random(10))+" �����綵�ئp�������C(��ޭ� "HIG"+"+technology+NOR")\n", npc, 0, 1);
				else if( !random(3) )
					msg("$ME�@�ɫ鯫�A�H��N�u"HIY+RANDOM_THING[random(sizeof(RANDOM_THING))]+NOR"�v���ƾǭ�Ƹ̥��A�S�Q��o�ϱo���J���������禨�\�F�I�H(��ޭ� "HIG"+"+technology+NOR")\n", npc, 0, 1);
				else
					msg("$ME�q���Ѥ�����g��A�N�쥻���~����s��ƭ��s��z��g�C(��ޭ� "HIG"+"+technology+NOR")\n", npc, 0, 1);
			}
		}
	}
	
	addn("technology", technology, room);
	addn("total_technology", technology, room);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"laboratory"	:
	([
		"short"	: HIC"��o"NOR CYN"�����"NOR,
		"heartbeat":60,	// ��ڮɶ� 1 �����
		"job"	: RD,
		"master": 1,
		"help"	:
			([
"topics":
@HELP
    ���Ѳ��~���s�y�P��ެ�o�C
HELP,


"design":
@HELP
�]�p���s���~�����O�A�Ϊk�p�U�G[�޲z���O]
  design type car		- �Ͳ��T��(car)�������~�A�Ҧ����~�����i�� plist �d��
  design id porsche		- �N���~�� ID �]�w�� porsche (�i�ϥΦ�X)
  design name �O�ɱ�		- �N���~���W�ٳ]�w�� �O�ɱ� (�i�ϥΦ�X)
  design inlay '���~'		- �N�Y�ǯS���~�յ��^�O�첣�~�ҫ��W�A�����~���ͯS��\��
  design inlay 100 '���~'	- �N 100 �Ӫ��~�յ��^�O�첣�~�ҫ��W�A�����~���ͯS��\��
  design inlay cancel		- ���^�Ҧ��^�O���~
  design delete 3		- �N��o���~�C���s���� 3 �����~��T�R��
  design cancel			- �����ثe���b��o�����~
  design finish			- �������~�]�p�A�o��s���~
HELP,

"list":
@HELP
�C�X�w��o�]�p�����~��T�A�Ϊk�p�U�G
  list			 	- �C�X�H������o�]�p�����~��T
HELP,

			]),
		"action":
			([
				"design"	: (: do_design_action($1, $2) :),
				"list"		: (: do_list_action($1, $2) :),
			]),
	]),
]);

string look_room(object env)
{
	string msg = "";
	object master = env->query_master();
	mapping design = query("design", master);

	if( mapp(design) )
	{
		object inlay;

		msg = "�ثe�̷s��o�������~���G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += HIW"���~����"NOR WHT"(type)"NOR" �G"NOR+(design["type"]||"���]�p")+"\n";
		msg += HIR"���~�^��"NOR RED"(id)"NOR"   �G"NOR+(design["id"]||"���]�p")+"\n";
		msg += HIC"���~����"NOR CYN"(name)"NOR" �G"NOR+(design["name"]||"���]�p")+"\n";
		msg += HIY"�^�O���~"NOR YEL"(inlay)"NOR"�G\n"NOR;
		
		if( mapp(design["inlay"]) )
		foreach(string file, int num in copy(design["inlay"]))
		{
			if( !file_exists(file) ) continue;

			inlay = load_object(file);
			
			if( !objectp(inlay) ) continue;
			
			msg += "                 "+QUANTITY_D->obj(inlay, num, 1)+"\n";
		}
		else
			msg += "                 ���^�O���󪫫~\n";

		
		if( design["type"] )
		{
			object product = load_object(PRODUCT_MODULE_PATH+replace_string(design["type"], " ", "_")+"_object.c");	
			
			if( mapp(design["inlay"]) && sizeof(design["inlay"]) )
			{
				set("design/inlay", copy(design["inlay"]), product);
				product->setup_inlay();
			}
			
			msg += HIM"\n���~���"NOR MAG"(data) "NOR"�G\n                 ���q "+query("mass", product)+"g�B���� "+query("value", product)+(product->query_description()||"")+"\n";
			msg += DESCRIPTION_D->query_object_buff_description(product, 15, 0);
			delete("design/inlay", product);
			destruct(product);
		}
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	}

	msg += "��ޭ� "HIC+query("technology", master)+NOR CYN"/"+query("total_technology", master)+NOR" ";
	msg += "������o�O "HIY+query("technologylevel", master)+NOR+" ";
	msg += "��o�޲z "HIY+(sizeof(master->query_slave())+1)+"/"+query("researchmanage", master)+NOR+"\n";
	
	return msg;
}

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIC"��o"NOR CYN"����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,2

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,30000000

	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,5
	
	// �̰��i�[�\�Ӽh
	,20
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,2
});

