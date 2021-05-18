/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _object_database_mod.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-17
 * Note   : 
 * Update :
 *  o 2002-00-00
 *
 -----------------------------------------
 */

 /*
物品資料儲存格式
([
	"分類1":
	({
		"basename_1", amount_1,
		"basename_2", amount_2,
		"basename_3", amount_3,
	}),
	"分類2":
	({
		"basename_1", amount_1,
		"basename_2", amount_2,
		"basename_3", amount_3,
	}),
])
*/

// 超過容量限制
int over_capacity(object env, string database, int amount, int capacity)
{
	int size;
	mapping objects;
	array objectlist;
	int totalamount;
	
	if( capacity == -1 )
		return 0;

	objects = query(database, env) || allocate_mapping(0);
	objectlist = implode(values(objects), (:$1+$2:)) || allocate(0);
	
	capacity *= sizeof(query("slave", env))+1;
	
	size = sizeof(objectlist);
	
	for(int i=0;i<size;i+=2)
		totalamount += to_int(objectlist[i+1]);

	if( amount + totalamount > capacity )
		return 1;
	
	return 0;
}



// 輸入物品
void input_object(object env, string database, string basename, int amount)
{
	mapping objects;
	array objectlist;
	string classname;
	
	objects = query(database, env) || allocate_mapping(0);
	objectlist = implode(values(objects), (:$1+$2:))||allocate(0);

	classname = query("setup/class/"+replace_string(basename, "/", "#"), env);
	
	// 如果原本沒有這種商品
	if( member_array(basename, objectlist) == -1 )
	{
		if( !arrayp(objects[classname]) )
			objects[classname] = ({ basename, amount });
		else
			objects[classname] += ({ basename, amount });
	}
	else
	{
		int idx;
		string shelf;
		array data;
		foreach(shelf, data in objects)
		{
			idx = member_array(basename, data);
			
			if( idx != -1 )
			{
				objects[shelf][idx+1] = to_int(data[idx+1]) + amount;
				break;
			}
		}
	}
	
	set(database, objects, env);
}

// 輸出物品
void output_object(object env, string database, string basename, int amount)
{
	int idx;
	string shelf;
	array data;
	mapping objects;
	
	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		idx = member_array(basename, data);
		
		if( idx != -1 )
		{
			// 完全刪除
			if( amount == -3 || to_int(data[idx+1]) == amount )
			{
				objects[shelf] = objects[shelf][0..idx-1] + objects[shelf][idx+2..];
				
				if( !sizeof(objects[shelf]) )
					map_delete(objects, shelf);
			}
			else
				objects[shelf][idx+1] = to_int(data[idx+1]) - amount;
			
			break;
		}
	}
	
	if( sizeof(objects) )
		set(database, objects, env);
	else
		delete(database, env);
}

// 搜尋物品
varargs mapping query_objects(object env, string database, string module_or_basename, string shelflimit)
{
	int idx;
	int size;
	array data;
	mapping objects;
	object ob;
	mapping result = allocate_mapping(0);

	objects = query(database, env) || allocate_mapping(0);
	
	if( undefinedp(shelflimit) )
		data = implode(values(objects), (: $1 + $2 :));
	else
		data = objects[shelflimit];
		
	if( (size = sizeof(data)) > 0 )
	for(idx=0;idx<size;idx+=2)
	{
		if( module_or_basename == data[idx] )
			result[module_or_basename] = to_int(data[idx+1]);
		else
		{
			catch( ob = load_object(data[idx]) );
		
			if( objectp(ob) && ob->query_module() == module_or_basename )
				result[data[idx]] = to_int(data[idx+1]);
		}
	}		
	
	return result;
}


// 回傳物品數量
varargs int query_object_amount(object env, string database, string module_or_basename, string shelflimit)
{
	mapping objects;

	if( undefinedp(shelflimit) )
		objects = query_objects(env, database, module_or_basename);
	else
		objects = query_objects(env, database, module_or_basename, shelflimit);
	
	return implode(values(objects), (: $1 + $2 :));
}


// 轉移物品
void transfer_object(object from, object to, string fromdb, string todb, string basename, int amount)
{
	output_object(from, fromdb, basename, amount);
	input_object(to, todb, basename, amount);
}

// 重整分類項目
void refresh_class(object env, string database)
{
	int i, size;
	string newshelf, shelf, basename;
	array data;
	mapping objects;
	mapping new_objects = allocate_mapping(0);

	objects = query(database, env) || allocate_mapping(0);
	
	foreach(shelf, data in objects)
	{
		size = sizeof(data);

		for(i=0;i<size;i+=2)
		{
			basename = data[i];
			
			newshelf = query("setup/class/"+replace_string(basename, "/", "#"), env);
			
			if( undefinedp(new_objects[newshelf]) )
				new_objects[newshelf] = allocate(0);
				
			new_objects[newshelf] += data[i..i+1];				
		}
	}
	
	set(database, new_objects, env);
}
