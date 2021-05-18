/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : plist.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : ���a���O�{��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <skill.h>
#include <feature.h>
#include <daemon.h>

#define PRODUCT_MODULE_PATH "/std/module/product/"

inherit COMMAND;

string help = @HELP
�u�t�i�Ͳ������~�C��

plist			- �C�X�Ҧ��i�Ͳ����~�C��
plist '���~ID'		- �ԲӦC�X�Y�����~���Ͳ����
plist product		- �C�X�z�w��o�]�p�X�Ӫ��Ҧ����~
plist product '�s��'	- �C�X�Y�Ӭ�o�]�p���~���ԲӸ��

�������O: blist
HELP;

string *sorted_factory = ({
	"metal",
	"stone",
	"water",
	"wood",
	"fuel",
	
	"food",
	"drink",
	"clothing",
	"furniture",
	"hardware",
	"chemical",
	"machinery",
	"electrics",
	"transportation",
	"medicine",
	"adventure",
	"shortrange",
	"armor",
	"perfume",
	
	"instrument",
	"entertainment",
	"longrange",
	"heavyarmor",
	"magic",
	"aircraft",
});

private void do_command(object me, string arg)
{
	int number;
	mapping info, subinfo;
	object materialob;
	object target;
	object ob;
	string msg;
	string *products = allocate(0);
	int value;

	foreach( string file in get_dir(PRODUCT_MODULE_PATH) )
	{
		if( sscanf(file, "%s_module.c", file) == 1 )
			products |= ({ file });
	}
	
	products = sort_array(products, (: 
		member_array((PRODUCT_MODULE_PATH+$1+"_module.c")->query_product_info()["factory"], sorted_factory) - 
		member_array((PRODUCT_MODULE_PATH+$2+"_module.c")->query_product_info()["factory"], sorted_factory) ||
		query("value", load_object(PRODUCT_MODULE_PATH+$1+"_object.c")) - query("value", load_object(PRODUCT_MODULE_PATH+$2+"_object.c"))
		 :));

	if( !arg || objectp(target = find_player(arg)) )
	{
		string module;
		mapping inlay_effect;

		msg = "���~�C��A�ثe�@�i�Ͳ� "+sizeof(products)+" �ز��~\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg +=    "�W��                          �Ͳ��u�t     �^�O   ���p�Ӯ�     �̧C����\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		foreach(string product in products)
		{
			module = PRODUCT_MODULE_PATH+product+"_module.c";
			info = module->query_product_info();
			ob = load_object(PRODUCT_MODULE_PATH+product+"_object.c");
			value = query("value", ob);
			inlay_effect = fetch_variable("inlay_effect", load_object(module));
			
			info["timecost"] = to_int(pow(value, 0.32));
	
			msg += sprintf("%-30s%-12s  %-5s "HIG"%-8s     "HIY"%s"NOR"\n",
				info["name"]+"("+capitalize(info["id"])+")",
				BUILDING_D->query_building(info["factory"])[0],
				sizeof(inlay_effect) > 0 ? HIY + sizeof(inlay_effect) + NOR : "",
				NUMBER_D->number_symbol(info["timecost"]),
				NUMBER_D->number_symbol(value),
				);
		}
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "��J plist '���~ID' �i�d�߲Ӹ`���\n\n";
		
		return me->more(msg);
	}
	else if( arg == "product" )
	{
		int i;
		mapping product_info;

		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"�|���]�p�X����i�H�Ͳ������~�C\n");

		msg = pnoun(2, me)+"�ثe�w�g��o���������~�̷ӳ̷s�o���Ǧp�U��G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += HIW"�s�� ���~�W��                          ���~����                      ����          \n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		i = 0;
		foreach(string file in products)
		{
			catch(ob = load_object(file));
		
			if( !objectp(ob) ) continue;
				
			product_info = ob->query_product_info();
			
			msg += sprintf("%-4s %-33s %-30s%s\n", (++i)+".", ob->query_idname(), product_info["name"]+"("+product_info["id"]+")", NUMBER_D->number_symbol(query("value", ob)));
		}
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

		return me->more(msg);
	}
	else if( sscanf(arg, "product %d", number) )
	{
		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"�|���]�p�X����i�H�Ͳ������~�C\n");
		
		if( number <= 0 || number > sizeof(products) )
			return tell(me, "�п�J���T�����~�s���C\n");

		catch(ob = new(products[number-1]));
		
		tell(me, ob->long());
		
		destruct(ob);

		return;
	}
	/* �u�f�ɶ��w�I��
	else if( sscanf(arg, "return %d", number) )
	{
		mapping inlay;
		object inlay_ob;

		products = query("products", me);
		
		if( !arrayp(products) || !sizeof(products) )
			return tell(me, pnoun(2, me)+"�|���]�p�X����i�H�Ͳ������~�C\n");
		
		if( number <= 0 || number > sizeof(products) )
			return tell(me, "�п�J���T�����~�s���C\n");

		catch(ob = load_object(products[number-1]));
		
		if( query("inlay_returned", ob) )
			return tell(me, pnoun(2, me)+"�w�g�^���L�o�Ӳ��~���¦��^�O���~�C\n");
		
		inlay = query("design/inlay", ob);
		
		if( !mapp(inlay) || !sizeof(inlay) )
			return tell(me, "�o�����~�q�S���^�O�L���󪫫~�C\n");
			
		foreach(string basename, int num in inlay)
		{
			inlay_ob = new(basename);
		
			if( !objectp(inlay_ob) ) continue;

			inlay_ob->set_keep();

			msg("$ME���^ "+num+" ��"+inlay_ob->query_idname()+"�C\n", me, 0, 1);

			if( inlay_ob->no_amount() )
				inlay_ob->move(me, num);
			else
				inlay_ob->set_amount(num);
			
			inlay_ob->move(me);
		}

		set("inlay_returned", 1, ob);
	
		ob->save();
		me->save();

		msg("$ME�N�Ҧ��^�O���~���^�C\n", me, 0, 1);

		return;
	}
	*/
	else if( member_array(replace_string(lower_case(arg), " ", "_"), products) != -1 )
	{
		string module;
		mapping inlay_effect;

		arg = replace_string(lower_case(arg), " ", "_");
		
		module = PRODUCT_MODULE_PATH+arg+"_module.c";
		info = module->query_product_info();
		ob = load_object(PRODUCT_MODULE_PATH+arg+"_object.c");
		value = query("value", ob);

		info["timecost"] = to_int(pow(value, 0.32));
		info["cost"] = value*1000;
		info["technology"] = to_int(pow(value, 0.7));

		msg = info["name"]+"("+capitalize(info["id"])+")���ԲӲ��~��Ʀp�U�G\n";
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += sprintf(HIW"���~"NOR WHT"�W��"NOR" %-30s "HIW"�s�y"NOR WHT"�Ӯ�"NOR" %s\n", info["name"]+"("+capitalize(info["id"])+")", HIG+NUMBER_D->number_symbol(info["timecost"])+NOR GRN" ��/���(���^�O)"NOR);
		msg += sprintf(HIW"�Ͳ�"NOR WHT"�u�t"NOR" %-30s "HIW"���"NOR WHT"�ӶO"NOR" %s\n", BUILDING_D->query_building(info["factory"])[0], HIC+NUMBER_D->number_symbol(info["technology"])+NOR CYN" ��ޭ�"NOR);
		msg += sprintf(HIW"�̧C"NOR WHT"����"NOR" %-30s "HIW"��o"NOR WHT"�O��"NOR" %s\n", HIY+NUMBER_D->number_symbol(value)+NOR YEL" ��"NOR, HIY+NUMBER_D->number_symbol(info["cost"])+NOR YEL" ��"NOR);
		msg += sprintf(HIW"���~"NOR WHT"���q"NOR" %-30s \n", HIR+NUMBER_D->number_symbol(query("mass", ob))+NOR RED" ���J"NOR);

		msg += HIW"�s�y"NOR WHT"���\n"NOR;
		
		foreach( string material, int amount in info["material"] )
		{
			if( !objectp(materialob = load_object(material)) )
			{
				subinfo = (PRODUCT_MODULE_PATH+material+"_module.c")->query_product_info();
				msg += sprintf("         %-39s "HIM"%d"NOR MAG" ���\n"NOR, subinfo["name"]+"("+capitalize(subinfo["id"])+")", amount);
			}
			else
				msg += sprintf("         %-39s "HIM"%d"NOR MAG" ���\n"NOR, materialob->query_idname(), amount);
		}

		msg += HIW"�ޯ�"NOR WHT"�ݨD\n"NOR;
		foreach( string skill, int level in info["skill"] )
			msg += sprintf("         %-39s "HIW"%-3d"NOR WHT" ��\n"NOR, (SKILL(skill))->query_idname(), level);
		
		inlay_effect = fetch_variable("inlay_effect", load_object(module));
		
		if( mapp(inlay_effect) )
		{
			msg += HIW"�S��"NOR WHT"�^�O\n"NOR;
			foreach( string file, function fp in inlay_effect )
				msg += sprintf("         %s\n"NOR, file->query_idname());
		}

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		return tell(me, msg);
	}
	return tell(me, "�S�� "+arg+" �o�ز��~�C\n");
}