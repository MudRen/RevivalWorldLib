/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : clean_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2003-06-05
 * Note   : 清除精靈
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <daemon.h>

#define ADAY	(60*60*24)
#define LOG	"daemon/clean"

#define CLEAN_NEWBIE_ESTATE_DAYS	7	// 7 天未上線且總上線時間未滿 2 小時之新手角色砍除其房地產
#define CLEAN_NEWBIE_CHAR_DAYS		180	// 180 天未上線且總上線時間未滿 2 小時之新手角色砍除其角色
#define CLEAN_ESTATE_DAYS		365	// 365 天未上線之老玩家刪除其房地產
#define CLEAN_CHAR_DAYS			3650	// 3650 天未上線之老玩家刪除其角色


void clean_user()
{
	object user;
	int total, time = time(), overtime;
	string msg, dir, id;
        string *root_dir = filter_array(get_dir("/data/user/"), (: strlen($1) == 1 :));

	CHANNEL_D->channel_broadcast("news", "系統執行帳號清理作業，請稍候。");

        foreach( dir in root_dir )
        {
        	reset_eval_cost();

        	foreach( id in get_dir("/data/user/"+dir+"/") )
        	{
        		total++;
        		
        		if( SECURE_D->is_wizard(id) ) continue;

			if( find_player(id) && environment(find_player(id)) ) continue;
			
        		user = load_user(id);

			if( !objectp(user) )
			{
				CHANNEL_D->channel_broadcast("sys", "無法載入使用者物件 "+id);
				log_file(LOG, "無法載入使用者物件 "+id);
				continue;
			}
			
			overtime = (time - query("last_on/time", user))/ADAY;

			// 總上線時間未滿 2 小時之新手角色
			if( query("total_online_time", user) < 7200 )
			{
				if( overtime > CLEAN_NEWBIE_ESTATE_DAYS && sizeof(ESTATE_D->query_player_estate(id)) )
				{
					ESTATE_D->remove_estate(id);
					msg = capitalize(id)+" 總上線時間未滿 2 小時且超過 "+CLEAN_NEWBIE_ESTATE_DAYS+" 天未上線("+overtime+" 天)，回收其房地產";
					
					LOGIN_D->add_login_message(id, msg);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
				else if( overtime > CLEAN_NEWBIE_CHAR_DAYS )
				{
					set("password", PASSWORD_D->query_password(id), user);
					user->save();
					
					msg = capitalize(id)+" 總上線時間未滿 2 小時且超過 "+CLEAN_NEWBIE_CHAR_DAYS+" 天未上線("+overtime+" 天)，刪除其角色";
					
					CHAR_D->destruct_char(id);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
			}
			else
			{
				if( overtime > CLEAN_ESTATE_DAYS && sizeof(ESTATE_D->query_player_estate(id)) )
				{
					ESTATE_D->remove_estate(id);
					msg = capitalize(id)+" 超過 "+CLEAN_ESTATE_DAYS+" 天未上線("+overtime+" 天)，回收其房地產";
					
					LOGIN_D->add_login_message(id, msg);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
				else if( overtime > CLEAN_CHAR_DAYS )
				{
					set("password", PASSWORD_D->query_password(id), user);
					user->save();
					
					msg = capitalize(id)+" 超過 "+CLEAN_CHAR_DAYS+" 天未上線("+overtime+" 天)，刪除其角色";
					CHAR_D->destruct_char(id);
					CHANNEL_D->channel_broadcast("news", msg);
					log_file(LOG, msg);
				}
			}

			if( objectp(user) && !userp(user) )
				destruct(user);
        	}
        }
        
        CHANNEL_D->channel_broadcast("news", "帳號總數："+total+"，清理作業完成。");
}

void create()
{
	

}

string query_name()
{
	return "檔案清除系統(CLEAN_D)";
}
