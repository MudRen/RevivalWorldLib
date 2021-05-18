/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : login.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define PPL_PORT		({ 4000, 5000, 6000 })
#define WIZ_PORT		({ 4001 })

#define MOTD			"/system/daemons/etc/logind_motd"
#define WAITING_ENTER_TIME	3

#define MAX_USERS		1000

#define DEFAULT_WINDOW_SIZE	([ "width":80, "height":24 ])

#define STARTROOM		"/wiz/wizhall/room_wizhall_1"
#define WIZROOM			"/wiz/wizhall/room_wizhall_7"
#define WELCOME_MSG		"/system/daemons/etc/logind_welcome_msg"

#define DATA			"/data/daemon/login.o"
#define LOG			"daemon/connect"

