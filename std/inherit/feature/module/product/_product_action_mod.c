/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _product_action_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2005-04-06
 * Note   : product action module
 * Update :
 *
 *
 -----------------------------------------
 */

void create()
{
	object ob, env;
	string this_basename = base_name(this_object())+".c";
		
	// 暴力更新所有使用此模組之物件 action...
	foreach( ob in objects( (: $1->query_module_file() == $(this_basename) :) ) )
	{
		env = environment(ob);
		
		if( objectp(env) )
			env->remove_action(ob);

		ob->enable_action();
		
		if( objectp(env) )
			env->add_action(ob);
	}
}

varargs int standard_calculate(int num, int shift)
{
	int i;

	if( num <= 0 )
		return 0;

	for(i=1;i<100;i++)
	{
		if( i<5 )
		{
			if( num >= (i-1)*i*5 && num < i*(i+1)*5 )
				break;
		}
		else
		{
			if( num >= 100+(i-5)*(i-4)*20 && num < 100+(i-4)*(i-3)*20 )
				break;
		}
	}

	i -= shift;
	
	if( i <= 0 )
		return 0;

	return i;
}

void setup_inlay(object ob)
{
	mapping inlay = query("design/inlay", ob);
	mapping inlay_effect = fetch_variable("inlay_effect");
	
	if( mapp(inlay_effect) && mapp(inlay) )
	{
		foreach(string file, int num in inlay)
		{
			if( functionp(inlay_effect[file]) )
				evaluate(inlay_effect[file], num, ob);
		}
	}
}

mapping query_product_info()
{
	return fetch_variable("product_info");
}
