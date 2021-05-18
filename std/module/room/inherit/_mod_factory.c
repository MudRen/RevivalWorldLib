/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _factory_action_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-18
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

inherit __DIR__"_object_database_mod.c";

#include <npc.h>
#include <ansi.h>
#include <message.h>
#include <daemon.h>
#include <material.h>
#include <location.h>
#include <skill.h>
#include <citydata.h>

#define PRODUCT_MODULE_PATH		"/std/module/product/"

#define MAINTAIN_STR_COST	300

void input_object(object env, string database, string basename, int amount);
void output_object(object env, string database, string basename, int amount);
varargs mapping query_objects(object env, string database, string module, string shelflimit);
varargs int query_object_amount(object env, string database, string basename, string shelflimit);

// ���o���u�t�i�H�Ͳ������~ module ����}�C
string *query_available_products(object env)
{
	string file;
	string *types;
	object module;
	mapping product_info;
	string building_type;
	
	types = allocate(0);
	
	building_type = env->query_building_type();
	
	foreach(file in get_dir(PRODUCT_MODULE_PATH))
	{
		if( file[<9..<1] != "_module.c" ) continue;

		catch( module = load_object(PRODUCT_MODULE_PATH+file) );
		
		if( !objectp(module) ) continue;
		
		product_info = module->query_product_info();	
		
		if( product_info["factory"] == building_type )
		{
			sscanf(file, "%s_module.c", file);
			types += ({ file });
		}
	}
	
	return types;
}

void do_line_action(object me, string arg)
{
	int productnum;
	object product;
	object env = environment(me);
	object master = env->query_master();
	object warehouse = load_module(query("warehouse", master));
	string *productslist;

	if( !wizardp(me) && query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o���u�t���֦��̡C\n");
	
	if( !objectp(warehouse) )
		return tell(me, "�|���]�w������x���ߡC\n");
	
	if( !arg || !arg[0] )
	{
		string file;
		mapping product_info;
		mapping inlay;
		mapping material_data;
		string msg;
		object material_ob;
		mapping total_material = allocate_mapping(0);

		if( !(file = query("product", master)) )
			return tell(me, "�|���]�w�s�y�����~�C\n");
			
		catch( product = load_object(file) );
		
		if( !objectp(product) )
			return tell(me, "�L�k���J "+file+" �o�Ӳ��~�I�гq���Ův�B�z�C\n");
	
		product_info = product->query_product_info();
		inlay = query("design/inlay", product) || allocate_mapping(0);
	
		msg = "�ثe���b�Ͳ������~�O�G"+product->query_idname()+"\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += BRED"��ƦW��                       ���ݨD / �w�s�ƶq          ���p\n"NOR;
	
		foreach( string basename_or_module, int need in product_info["material"] )
			total_material[basename_or_module] += need;
		foreach( string basename_or_module, int need in inlay )
			total_material[basename_or_module] += need;
			
		// �ˬd��ƬO�_����
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "�u�t���");
	
			if( !sizeof(material_data) )
			{
				if( !objectp(material_ob = load_object(basename_or_module)) )
				{
					material_ob = load_object("/std/module/product/"+basename_or_module+"_module.c");
					msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_product_info()["name"], need, 0, HIR"��Ƥ���"NOR);
				}
				else	
					msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_idname(), need, 0, HIR"��Ƥ���"NOR);
			}
				
			foreach(string material_file, int amount in material_data)
			{
				material_ob = load_object(material_file);
				
				msg += sprintf("%-31s%8d"WHT" / "NOR"%-14d%s\n", material_ob->query_idname(), need, amount, amount < need ? HIR"��Ƥ���"NOR : HIG"��ƥR��"NOR);
			}
		}
	
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		return tell(me, msg);
	}

	arg = lower_case(arg);
	
	if( arg && sscanf(arg, "setup %d", productnum) == 1 )
	{
		if( query("line/working", master) )
			return tell(me, "�Ͳ��u���b�B�@���A�����������~����Ͳ����~�����C\n");
		
		productslist = query("products", me);
		
		if( productnum <= 0 || productnum > sizeof(productslist) )
			return tell(me, "�S���s�� "+productnum+" �����~�C\n");
		
		catch( product = load_object(productslist[productnum-1]) );
		
		if( !objectp(product) )
			return tell(me, "�L�k���J "+productslist[productnum-1]+" �o�Ӳ��~�I�гq���Ův�B�z�C\n");
	
		if( member_array(product->query_module(), query_available_products(master)) == -1 )
			return tell(me, "�o���u�t�L�k�Ͳ��o�ز��~�C\n");
		
		set("product", base_name(product), master);
		master->save();	
		msg("$ME�N���Ͳ��u���s�y���~�]�w���u"+product->query_idname()+"�v�C\n", me, 0, 1);
	}
	else if( arg == "start" || sscanf(arg, "start %d", productnum) == 1 )
	{
		if( !query("product", master) )
			return tell(me, "�|���]�w���s�y�����~�A�L�k�}�l�Ͳ��C\n");

		catch( product = load_object(query("product", master)) );
		
		if( !objectp(product) )
			return tell(me, "�L�k���J "+query("product", master)+" �o�Ӳ��~�I�гq���Ův�B�z�C\n");

		if( productnum <= 0 )
			productnum = 0;
			
		set("line/working", 1, master);
		set("line/amount", productnum, master);
		master->save();
		
		if( !productnum )
			msg("$ME�ҰʤF�Ͳ��u�W�Ҧ����]�ơA�}�l�L���q�Ͳ�"+product->query_idname()+"�C\n", me, 0, 1);
		else
			msg("$ME�ҰʤF�Ͳ��u�W�Ҧ����]�ơA�}�l�Ͳ� "+productnum+" ��"+product->query_idname()+"�C\n", me, 0, 1);
		
	}
	else if( arg == "stop" )
	{
		delete("line", master);
		master->save();
		msg("$ME�N�Ͳ��u�W�Ҧ����]�������A����Ͳ����~�C\n", me, 0, 1);
	}

	else tell(me, "�п�J���T�����O�榡�C(help line)\n", me, 0, 1);
}

void do_list_action(object me, string arg)
{	
	int i;
	string file, msg;
	object product;
	object env = environment(me);
	object master = env->query_master();
	string *productslist;
	string *available_products = query_available_products(master);
	string description;
	string current_product = query("product", master);
	string owner = query("owner", master);
	object ownerob;
	
	if( owner == me->query_id(1) )
		ownerob = me;
	else if( user_exists(owner) )
		ownerob = load_user(owner);
	else
		return tell(me, "�L�k�d�߸�ơC\n");
			
	productslist = query("products", ownerob);
	
	if( !sizeof(productslist) )
		return tell(me, (ownerob == me ? pnoun(2, me) : ownerob->query_idname())+"�|���]�p�X����i�H�Ͳ������~�C\n");
	
	msg = (ownerob == me ? pnoun(2, me) : ownerob->query_idname())+"�ثe�w�g��o���������~�p�U�G\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += "�s�� ���~�W��                       ���u�t�i�Ͳ�   �򥻻���\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	i = 0;
	foreach(file in productslist)
	{
		catch(product = load_object(file));
	
		if( !objectp(product) ) continue;
			
		if( current_product == file )
			description = HIM"�Ͳ���"NOR;
		else if( member_array(product->query_module(), available_products) != -1 )
			description = HIC"�i�Ͳ�"NOR;
		else
			description = HIR"���i�Ͳ�"NOR;

		msg += sprintf("%-4d %-31s%-15s%s\n", (++i), product->query_idname(), description, NUMBER_D->number_symbol(product->query_value()));
	}
	
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	me->more(msg);
	
	if( !userp(ownerob) )
		destruct(ownerob);
}

void do_setup_action(object me, string arg)
{
	string option;
	int x, y, num;
	object warehouse;
	string file;
	string city;
	object env = environment(me);
	object master = env->query_master();
	array loc = env->query_location();

	city = loc[CITY];

	if( !arg )
		return tell(me, pnoun(2, me)+"�Q�n�]�w���򶵥ءH(help setup)\n");
	
	if( query("owner", master) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"���O�o�Ӥu�t���֦��̡C\n");

	sscanf(arg, "%s %s", arg, option);

	switch(arg)
	{	
		case "warehouse":
		{
			if( option == "-d" )
			{
				if( !query("warehouse", master) )
					return tell(me, "�o�̭쥻�K�S���]�w���x���ߡC\n");
					
				delete("delivery", master);
				return tell(me, "�����s�f�ؼЧ����C\n");
			}

			if( !option || sscanf(option, "%d %d,%d", num, x, y) != 3 )
				return tell(me, "�п�J���T���ܮw�y��(�� 1 21,33)�C\n");
			
			// ��f�ܭܮw
			file = CITY_ROOM_MODULE(city, (num-1), (x-1), (y-1), "warehouse");

			if( !objectp(warehouse = load_module(file)) || query("function", warehouse) != "warehouse" )
				return tell(me, "�y��"+loc_short(city, num-1, x-1, y-1)+"�èS���ܮw�C\n");
			
			if( warehouse != warehouse->query_master() )
			{
				file = base_name(warehouse->query_master());
				warehouse = warehouse->query_master();
			}

			if( query("owner", warehouse) != me->query_id(1) )
				return tell(me, warehouse->query_room_name()+"�ä��ݩ�"+pnoun(2, me)+"�C\n");
			
			set("warehouse", file, master);
			
			master->save();
			
			msg("$ME�]�w"+warehouse->query_room_name()+"�����Ͳ��u�����x���ߡC\n", me, 0, 1);
			
			break;
		}

		default:
			return tell(me, "�п�J���T���ﶵ(help setup)�C\n");
	}
}

void broadcast_msg(object *rooms, string msg)
{
	foreach(object room in rooms)
		broadcast(room, msg);
}

// 1 ��I�s�@��
void heart_beat(object room)
{
	int timecost;
	int factory_heart_beat;
	object master = room->query_master();
	object product;
	string product_file;
	string ownerid = query("owner", master);
	object owner = find_player(ownerid);

	if( !objectp(owner) || room != master || query("function", master) != "line" || !query("line/working", master) ) return;

	if( !(factory_heart_beat = query_temp("factory_heart_beat", master)) )
		factory_heart_beat = set_temp("factory_heart_beat", random(1200)+1, master);
	else
		factory_heart_beat = addn_temp("factory_heart_beat", 1, master);

	product_file = query("product", master);
	
	if( catch( product = load_object(product_file) ) || !objectp(product) )
		return;
	
	timecost = to_int(pow(to_float(query("value", product)), 0.32));

	if( timecost < 1 )
		timecost = 1;

	set_temp("timecost", timecost, master);

	// �i��Ͳ�
	if( !(factory_heart_beat % timecost) )
	{
		object *userinside_room = allocate(0);
		int management;
		int not_enough_material;
		string *slaves = master->query_slave() || allocate(0);
		int amount = sizeof(slaves) + 1;
		string msg = "";
		int material_count;
		int total_need;
		mapping material_data;
		string material_file;
		mapping product_info = product->query_product_info();
		mapping product_info_temp;
		object warehouse = load_module(query("warehouse", master));
		object material_ob;
		mapping inlay = query("design/inlay", product);
		int total_amount = query("line/amount", master);
		mapping total_material = allocate_mapping(0);

		if( !objectp(warehouse) || query("function", warehouse) != "warehouse" )
		{
			broadcast(master, "�u�t�ʤ֭��x���ߪ��䴩�A�ɭP�Ͳ�����C\n");
			delete("line", master);
			master->save();
			return;
		}

		if( total_amount > 0 && total_amount < amount )
			amount = total_amount;

		if( !mapp(inlay) )
			inlay = allocate_mapping(0);

		foreach(object worker in filter(all_inventory(master), (: $1->is_module_npc() && query("boss", $1) == $(ownerid) && query("job/type", $1) == WORKER :)))
			management += worker->query_skill_level("factorymanage")/5;
	
		set("management", management, master);
		
		if( management < sizeof(slaves)+1 )
		{
			broadcast(master, "���u�ʥF�u�t�޲z��O�A�Ͳ�����C\n");
			delete("line", master);
			master->save();
			return;
		}
	
		// �p��O�_�����a�b�t��
		foreach(string file in ({ base_name(master) }) + slaves)
			if( sizeof(filter_array(all_inventory(load_object(file)), (: userp($1) :))) )
				userinside_room += ({ load_object(file) });

		foreach( string basename_or_module, int need in product_info["material"] )
			total_material[basename_or_module] += need;
		foreach( string basename_or_module, int need in inlay )
			total_material[basename_or_module] += need;
			
		// �ˬd��ƬO�_����
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "�u�t���");

			if( implode(values(material_data), (: $1 + $2 :)) < need * amount )
			{
				not_enough_material = 1;

				if( sizeof(userinside_room) )
				{
					if( objectp(material_ob = load_object(basename_or_module)) )
						msg += "�ܮw�u�u�t��ơv�������ʥF�������u"+material_ob->query_idname()+"�v���(�ܤֻݭn "+(need*amount)+" ��)�C\n";
					else if( objectp(material_ob = load_object("/std/module/product/"+basename_or_module+"_module.c")) )
					{
						product_info_temp = material_ob->query_product_info();
						msg += "�ܮw�u�u�t��ơv�������ʥF�������u"+product_info_temp["name"]+"("+capitalize(product_info_temp["id"])+")"+"�v���(�ܤֻݭn "+(need*amount)+" ��)�C\n";
					}
					else
						msg += "�ܮw�u�u�t��ơv�������ʥF�������u"+basename_or_module+"�v���(�ܤֻݭn "+(need*amount)+" ��)�C\n";
				}
			}
		}
		
		if( not_enough_material )
		{
			broadcast_msg(userinside_room, msg);
			return;
		}
		
		foreach( string basename_or_module, int need in total_material )
		{
			material_data = query_objects(warehouse, "products", basename_or_module, "�u�t���");
			
			total_need = need * amount;

			foreach(material_file, material_count in material_data)
			{
				if( material_count >= total_need )
				{
					output_object(warehouse, "products", material_file, total_need);
					
					if( sizeof(userinside_room) )
						msg += "�Ҧ��s��u�t���u�@���ӤF "+total_need+" ��"+load_object(material_file)->query_idname()+"�C\n";

					break;
				}
				else
				{
					output_object(warehouse, "products", material_file, material_count);
				
					if( sizeof(userinside_room) )
						msg += "�Ҧ��s��u�t���u�@���ӤF "+material_count+" ��"+load_object(material_file)->query_idname()+"�C\n";
						
					total_need -= material_count;

					continue;
				}
			}
		}
		
		if( sizeof(userinside_room) )
			msg += "�Ҧ��s��u�t���u�@�s�y�Ͳ��X "+amount+" ��"+product->query_idname()+"�C\n";
			
		input_object(warehouse, "products", product_file, amount);
		
		if( sizeof(userinside_room) )
			broadcast_msg(userinside_room, msg);

		PRODUCT_D->count_product_manufacture(product_file, amount);

		if( !(factory_heart_beat % (timecost*100)) )
			warehouse->delay_save(600);
			
		if( total_amount > 0 && addn("line/amount", -amount, master) <= 0 )
		{
			broadcast(master, "�w�F�ؼв��q�A�Ͳ�����C\n");
			delete("line", master);
			master->save();
		}
	}
}

string look_room(object room)
{
	if( query("function", room) == "line" )
	{
		string msg = "";
		object master = room->query_master();
		int timecost = query_temp("timecost", master);
		int factory_heart_beat = query_temp("factory_heart_beat", master);
		object warehouse = load_module(query("warehouse", master));
		object product = load_module(query("product", master));
		int amount = query("line/amount", master);
		int working = query("line/working", master);
		
		msg += "���u�B�@�G"+(working ? HIG"�Ͳ���"NOR GRN"(�U�����X�ɶ��G"+(timecost ? (timecost - factory_heart_beat%timecost)+" ���":"����")+")"NOR : HIR"���"NOR)+"\n";

		if( working )
			msg += "�Ѿl���q�G"+(!amount ? "�L���q" : amount+" ��")+"\n";

		msg += "�s�y���~�G"+(objectp(product) ? product->query_idname() : "�L")+"\n";
		msg += "���x���ߡG"+(objectp(warehouse) ? warehouse->query_room_name() : "�L")+"\n";
		msg += "�u�t�޲z�G"HIY+(sizeof(master->query_slave())+1)+"/"+query("management", master)+NOR"\n\n";
		return msg;
	}
}
