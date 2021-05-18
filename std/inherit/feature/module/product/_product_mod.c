/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _object_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-05-14
 * Note   : object module
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

#define PRODUCT_MODULE_PATH		"/std/module/product/"

// ������O��
nosave mapping actions;

int is_module_product()
{
	return 1;
}

string query_module()
{
	string module = query_temp("module");
	
	if( stringp(module) )
	{
		return module;
	}
	else
	{
		sscanf(base_name(this_object()), PRODUCT_MODULE_PATH"%s_object", module);
		
		return module;
	}
}

// �d�� Module Filename
string query_module_file()
{
	string module_file = query_temp("module_file");
	
	if( stringp(module_file) ) 
	{
		return module_file;
	}
	else
	{
		return PRODUCT_MODULE_PATH+query_module()+"_module.c";
	}
}

// �� Action File �d�ߥ\����
varargs mapping query_action_info()
{
	return fetch_variable("action_info", load_object(query_module_file()));
}

// �Ұ� Action
int enable_action()
{
	mapping action_info = query_action_info();
	
	if( !mapp(action_info) ) return 0;
	
	// ���J Actions
	actions = action_info["action"];

	return 1;
}

void init(object ob)
{
	object module = load_object(query_module_file());
	
	if( objectp(module) )
		module->init(ob, this_object());
}

void leave(object ob)
{
	object module = load_object(query_module_file());
	
	if( objectp(module) )
		module->leave(ob, this_object());
}

mapping query_product_info()
{
	return fetch_variable("product_info", load_object(query_module_file()));
}

int query_value()
{
	object ob;
	mapping product_info;
	mapping inlay = query("design/inlay");
	int value;
	
	product_info = fetch_variable("product_info", load_object(query_module_file()));
	
	foreach(string material, int num in product_info["material"])
		if( objectp(ob = load_object(material)) || objectp(ob = load_object(PRODUCT_MODULE_PATH+material+"_object.c")) )
			value += to_int(query("value", ob)*num*1.5);

	if( mapp(inlay) && sizeof(inlay) )
	foreach(string material, int num in inlay)
	{
		if( objectp(ob = load_object(material)) && !query("badsell", ob) )
			value += to_int(query("value", ob)*num*0.8);
	}

	return value;
}

void setup_inlay()
{
	object module = load_object(query_module_file());
	
	if( objectp(module) )
		module->setup_inlay(this_object());
}

// �� Virtual_D �I�s�ðe�J��������W�� file
varargs void create(string file, string type)
{
	string module_file;

	if( !stringp(file) || !stringp(type) ) return;
		
	module_file = PRODUCT_MODULE_PATH+type+"_module.c";

	// �b Virtual_D �̫K�ˬd�O�_�ʤ� Action File
	if( !file_exist(module_file) )
		error("�ݭn Module File �� "+module_file+"�C\n");
	
	// ���� Action File
	set_temp("module_file", module_file);
	set_temp("module", type);

	// �Y�O�Ĥ@���Хߪ���, �h�]�w�򥻸��
	if( !this_object()->restore(file) ) 
	{
		mapping info = query_product_info();

		this_object()->set_idname(info["id"], info["name"]);
	}
	else
		enable_action();
}
