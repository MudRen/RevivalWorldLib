/*
 * Project:     FTP Server
 * Author:      Cwlin
 * Version:     1.2.6
 * Last Update Time:
 *		2005/4/17 10:05 PM
 * Note:
 * 2003/7/22 09:02 PM	- Fixed -	List 或 File Transfer 中可能因資料尚未
 *					完成傳送即先收到傳送完畢之訊息導致中斷
 * 2005/4/17 10:05 PM	- Modify -	正確宣告私有函數
 *			- Add -		新增自動更改已使用 port 選項
 *			- Modify -	修正變數命名	
 * 2006/6/30 03:06 AM	- Modify -	修正臨時下載中斷時的錯誤
 * 2007/9/24 11:09 AM   - Modify -      自動忽略以 '-' 為開頭的參數, 避免誤判為路徑
 * 
 ****************************
 * 流程圖:
 *      1. 建立連結(Socket create) ->		接收回報(Read Callback)
 *						結束回報(Close Callback)
 *
 *      2. 設定連結埠(Socket Bind)
 *
 *      3. 等待連結(Socket Listen) ->		等待回報(Listen Callback)
 *      4. 等待回報(Listen Callback) ->		連結接受(Socket Accept)
 *
 *      5. 連結接受(Socket Accept) ->		接收回報(Read Callback)
 *						已準備寫入回報(Write Callback)
 *
 *      6. 已準備寫入回報(Write Callback) ->	連結寫入(Socket Write)
 *
 *      7. 轉址(Resolve) ->			轉址回報(Resolve Callback)
 */
/* Includes */
#include <socket.h>
#include <socket_err.h>

/* MUD Feature Includes */
#include <daemon.h>
#include <secure.h>
#include <mudlib.h>

/* Defines */

/*
#define MUD_NAME		"Revival World"
*/
#define FTPD_VERSION		"1.2.6"
#define SEC_IN_YEAR		365 * 24 * 60 * 60

#define HOST_IP                 "210.59.236.38"
#define SOCKET_PORT             4121
#define MAX_PORT_INCRE		3	// Auto increase port when specific port are using
#define TRANSFER_MSG_DELAY	3	// The message delay when recevied data 

/* 自動偵測主機 IP */
#define AUTO_DETECT_HOSTIP

#define MAX_BYTE_TRANSFER	102400	// Specific in MudOS options.h
#define ERROR_LOG		"ftp_d"

#define log_error(x)		log_file(ERROR_LOG, sprintf("%s", x))
#define user_exist(x)		(SECURE_D->is_wizard(x))
#define monitor(x)		CHANNEL_D->channel_broadcast("nch", "FTP_D: " + x)


/* Global variables */
private nosave int socket_fd;		// Port Server FD
private nosave int data_callback_time;
private nosave string host_ip;		// Host Ip
private nosave mapping sockets;		// Sockets
private nosave mapping data_sockets;	// Data Sockets
private nosave mapping resolves;	// Resolve() Keys

/* Prototypes */
private void close_connection(int fd);
private void close_data_connection(int fd);
private void socket_send(int fd, string arg);		// Sending normal message
private void socket_send_file(int fd, string path);	// Sending file
private void socket_send_list(int fd, string listing);	// Sending list

private void write_callback(int fd);
private void read_callback(int fd, string str);
private void close_callback(int fd);
private void listen_callback(int fd);
private void resolve_callback(string addr, string ip, int key);

private void ftp_login(int fd);
private void ftp_input(int fd, string str);

private void data_read_callback(int fd, mixed msg);
private void data_write_callback(int fd);
private void data_close_callback(int fd);
private void data_listen_callback(int fd);

string memory_expression(int m);

/* MUD Feature Functions */
/* 取得帳號密嗎 */
private string get_passwd(string user)
{
	return PASSWORD_D->query_password(user);
}
/* 取得目前工作目錄 */
private string get_pwd(string user)
{
	return wiz_home(user);
}
/* 取得路徑 */
private string get_path(string pwd, string file)
{
	if( pwd[<1] != '/' ) pwd += "/";
	return resolve_path(pwd, file);
}
/* 指令 - 目錄清單 */
private string cmd_list(string pwd, string arg, string id)
{
	string path, last_mdfy, auth, res = "";
	int file_size;
	array file, files;

	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path[<1] != '/' ) path += "/";
	files = get_dir(path, -1);
	if( !sizeof(files) )
	{
		if( file_size(path) == -2 ) return "";
		return 0;
	}
	foreach(file in files)
	{
		/*
		File name = file[0];
		File size = file[1];
		Last modify time = file[2];
		*/
		file_size = file[1];
		if( file[0][<1] == '.' ) continue;
		if( file_size == -2 )
		{
			auth = "d";
			file_size = 4096;
		}
		else	auth = "-";
		if( !SECURE_D->permission(path + file[0], id, READ, 0) )
			auth += "-";
		else	auth += "r";
		if( !SECURE_D->permission(path + file[0], id, WRITE, 0) )
			auth += "-";
		else	auth += "w";
		if( !SECURE_D->permission(path + file[0], id, COMPILE, 0) )
			auth += "-";
		else if( file[0][<2..] == ".c" )
			auth += "x";
		else	auth += "-";
		auth += "------";

		if( file[2] + SEC_IN_YEAR < time() )
			last_mdfy = sprintf("%6s %5s", ctime(file[2])[4..9], ctime(file[2])[<4..<0]);
		else	last_mdfy = sprintf("%6s %5s", ctime(file[2])[4..9], ctime(file[2])[11..<9]);

		res += sprintf("%10s %4d %-8d %-8d %8d %12s %s\r\n",
		    auth, 1, 0, 0, file_size, last_mdfy, file[0]);
	}
	return res;
}
/* 指令 - 目錄清單(簡短) */
private string cmd_nlist(string pwd, string arg)
{
	string path, res = "";
	array files;

	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path[<1] != '/' ) path += "/";
	files = get_dir(path);
	if( !sizeof(files) )
	{
		if( file_size(path) == -2 ) return "";
		return 0;
	}
	foreach(string file in files)
	{
		if( file[<1] == '.' ) continue;
		res += sprintf("%s\r\n", file);
	}
	return res;
}
/* 指令 - 更改工作目錄 */
private string cmd_cwd(string pwd, string arg)
{
	string path;
	if( pwd[<1] != '/' ) pwd += "/";
	path = resolve_path(pwd, arg);
	if( path != "/" && path[<1] == '/' ) path = path[0..<2];
	if( file_size(path) != -2 ) return 0;
	return path;
}


/* Functions */
private void setup()
{
	int res, port;

	/* 建立 socket */
	socket_fd = socket_create(STREAM, (: read_callback :), (: close_callback :));
	if( socket_fd < 0 )
	{
		monitor("初始錯誤: socket_create: " + socket_error(socket_fd)); 
		log_error("setup: socket_create: " + socket_error(socket_fd) + ".\n");
		return;
	}

	/* 設定 socket 連結埠 */
	port = SOCKET_PORT;
	do
	{
		res = socket_bind(socket_fd, SOCKET_PORT);
		if( res < 0 )
		{
			monitor("初始錯誤: socket_bind: " + socket_error(res)); 
			log_error("setup: socket_bind: " + socket_error(res) + ".\n");
			if( port - SOCKET_PORT > MAX_PORT_INCRE )
			{
				socket_close(socket_fd);
				return;
			}

		}
	}
	while(res < 0 && port++);

	/* 開始等待 socket 連結 */
	res = socket_listen(socket_fd, (: listen_callback :));
	if( res < 0 )
	{
		socket_close(socket_fd);
		monitor("初始錯誤: socket_listen: " + socket_error(res)); 
		log_error("setup: socket_listen: " + socket_error(res) + ".\n");
	}
	monitor("啟動於 " + host_ip + ":" + port); 
}


/* socket 傳送資料完成通知 */
private void write_callback(int fd) 
{
	monitor("write_callback: " + fd + ".");
}

/* socket 接收資料通知 */
private void read_callback(int fd, string str)
{
	foreach(string cmd in explode(str, "\n"))
	ftp_input(fd, cmd);
}

/* socket 關閉通知 */
private void close_callback(int fd)
{
	if( undefinedp(sockets[fd]) ) return;
	if( !undefinedp(sockets[fd]["login"]) )
		monitor(sprintf("%s(%s) 斷線。", sockets[fd]["user"], sockets[fd]["address"]));
	map_delete(sockets, fd);
}

/* socket 連結通知 */
private void listen_callback(int fd) 
{
	int res_fd, port, key;
	string addr;

	/* 接受連結 */
	res_fd = socket_accept(fd, (: read_callback :), (: write_callback :));
	if( res_fd < 0 )
	{
		log_error("listen_callback: " + socket_error(res_fd) + ".\n");
		return;
	}

	sscanf(socket_address(res_fd), "%s %d", addr, port);

	sockets[res_fd] = ([
	    "address" : addr,		// IP 位置
	    "name" : addr,		// 網域名稱
	    "port" : port,		// 遠端埠號
	    "time" : time(),		// 連結時間
	    "type" : "ascii",		// 資料傳送類型
	    "rest" : 0,			// 資料傳送起始位元
	    "write_status" : EESUCCESS 	// 連結狀態
	]);

	/* 轉址取得網域名稱 */
	key = resolve(addr, (: resolve_callback :));
	resolves[key] = res_fd;

	/* 登入程序 */
	ftp_login(res_fd);
}

/* 轉址通知 */
private void resolve_callback(string addr, string ip, int key)
{
	if( addr ) {
		int fd;
		fd = resolves[key];
		if( fd && sockets[fd] ) sockets[fd]["name"] = addr;
	}
	map_delete(resolves, key);
}

/* 傳送資料至 socket */
private void socket_send(int fd, string arg)
{
	int res;

	if( !undefinedp(sockets[fd]["pending"]) )
	{
		sockets[fd]["pending"] += arg;
		res = socket_write(fd, sockets[fd]["pending"]);
	}
	else	res = socket_write(fd, arg);

	sockets[fd]["write_status"] = res;
	switch(res) {
	case EESUCCESS:		/* 成功 */
	case EECALLBACK:	/* 等待完成 */
		if( !undefinedp(sockets[fd]["pending"]) )
			map_delete(sockets[fd], "pending");
		break;
	case EEALREADY:		/* 程序已進行 */
	case EEWOULDBLOCK:	/* 程序停滯 */
	case EESEND:		/* 傳送資料錯誤 */
		if( undefinedp(sockets[fd]["pending"]) )
			sockets[fd]["pending"] = arg;
		call_out((: socket_send :), 1, fd, "");
		break;
	default:		/* 其他錯誤 */
		log_error("socket_send: " + socket_error(res) + ".\n");
		socket_close(fd);
		break;
	}
}

private void socket_send_file(int fd, string arg)
{
	int i, len, pos;
	string path;

	if( undefinedp(sockets[fd]) ) return;
	pos = sockets[fd]["rest"];
	path = get_path(sockets[fd]["pwd"], arg);
	if( undefinedp(sockets[fd]["data_fd"]) ) return;
	if( (len = file_size(path)) < 0 )
	{
		socket_send(fd, "550 Failed to open file.\n");
		return;
	}
	if( undefinedp(sockets[fd]["retr"]) )
	{
		sockets[fd]["retr"] = path;
		sockets[fd]["retr_len"] = len;
		data_sockets[sockets[fd]["data_fd"]] = fd;
		socket_send(fd, "150 Opening " + upper_case(sockets[fd]["type"]) + " mode data connection for " + arg + " (" + (len - sockets[fd]["rest"]) + " bytes).\n");
		monitor(sprintf("%s 開始下載檔案 %s。", sockets[fd]["user"], path));
	}
	while(pos <= len)
	{
		if( undefinedp(sockets[fd]["data_fd"]) ) break;
		i = socket_write(sockets[fd]["data_fd"], read_buffer(path, pos, MAX_BYTE_TRANSFER));
		if( i < 0 && i != EECALLBACK ) break;
		pos += ((len - pos) > MAX_BYTE_TRANSFER ? MAX_BYTE_TRANSFER: (len - pos));
	}
	if( i == EEWOULDBLOCK || i == EEALREADY  )
	{
		sockets[fd]["rest"] = pos;
		sockets[fd]["send_handle"] = call_out((: socket_send_file :), 1, fd, path);
		return;
	}
	if( pos == len )
	{
		monitor(sprintf("%s 下載檔案 %s 完成，共 %s。",
			sockets[fd]["user"],
			path,
			memory_expression(sockets[fd]["retr_len"])
		    ));
		socket_send(fd, "226 File send OK.\n");
		sockets[fd]["rest"] = 0;
		map_delete(data_sockets, fd);
		map_delete(sockets[fd], "retr");
		map_delete(sockets[fd], "retr_len");
		close_data_connection(fd);
		return;
	}
	return;
}

private void socket_send_list(int fd, string listing)
{
	int i;

	if( undefinedp(sockets[fd]["data_fd"]) ) return;
	if( undefinedp(sockets[fd]["list"]) )
	{
		sockets[fd]["list"] = 1; // Send list flag
		socket_send(fd, "150 Here comes the directory listing.\n");
	}
	i = socket_write(sockets[fd]["data_fd"], listing);
	if( i < 0 )
	{
		// monitor("socket_send_list: " + socket_error(i)); 
		sockets[fd]["send_handle"] = call_out((: socket_send_list :), 1, fd, listing);
		return;
	}
	close_data_connection(fd);
	socket_send(fd, "226 Directory send OK.\n");
	map_delete(sockets[fd], "list");
	return;
}

/* 結束 socket 連線 */
private void close_connection(int fd)
{
	/* 若傳送資料程序送尚未完成, 繼續連線 */
	if( sockets[fd]["write_status"] == EECALLBACK ) return;
	map_delete(sockets, fd);
	socket_close(fd);
}

private void close_data_connection(int fd)
{
	if( undefinedp(sockets[fd]) ) return;
	socket_close(sockets[fd]["data_fd"]);
	map_delete(sockets[fd], "data_conn");
	map_delete(sockets[fd], "data_fd");
}

#ifdef AUTO_DETECT_HOSTIP
void hostip_resolve_callback(string addr, string ip, int key)
{
	if( ip )
	{
		host_ip = ip;
		setup();
		return;
	}
	log_error("create: auto_detect_hostip: cannot detect host ip.\n");
}
#endif

void create()
{
	/* 初始化變數 */
	sockets = allocate_mapping(0);
	data_sockets = allocate_mapping(0);
	resolves = allocate_mapping(0);

#ifdef AUTO_DETECT_HOSTIP
	/* 自動偵測主機名稱 */
	{
		string host = query_host_name();
		resolve(host, (: hostip_resolve_callback :));
	}
#else
	host_ip = HOST_IP;
	/* 啟動 */
	setup();
#endif
}

/* 登入程序 */
private void ftp_login(int fd)
{
	socket_send(fd, "220 "MUD_NAME" FTP server (Version "FTPD_VERSION" (MudOS/LPC)) ready.\n");
}

/* 指令處理程序 */
private void ftp_input(int fd, string str)
{
	string *command, verb, arg;

	/* 過濾指令格式 */
	if( !str ) return;
	if( str[<1] == '\n' ) str = str[0..<2];
	if( str[<1] == '\r' ) str = str[0..<2];
	if( str == "" ) return;

	command = explode(str, " ");
	if( !sizeof(command) ) return;
	verb = command[0];
	verb = lower_case(verb);
	//arg = implode(command[1..], " ");
	arg = "";
	for(int i=1; i<sizeof(command); i++)
	{
		if( command[i][0] == '-' ) continue;
		arg += command[i] + " ";
	}
	if( strlen(arg) && arg[<1] == ' ' ) arg = arg[0..<2];

	/* 未登入前限制使用指令 */
	if( undefinedp(sockets[fd]["login"]) && member_array(verb, ({ "user", "pass", "quit" })) == -1 )
	{
		socket_send(fd, "530 Please login with USER and PASS.\n");
		return;
	}

	/* 記錄指令 */
	sockets[fd]["lastcmd"] = str;

	switch(verb)
	{
	case "user":	/* 使用者帳號 */
		sockets[fd]["user"] = arg;
		socket_send(fd, "331 Please specify the password.\n");
		break;
	case "pass":	/* 使用者密碼 */
		if( undefinedp(sockets[fd]["user"]) )
		{
			socket_send(fd, "503 Login with USER first.\n");
			break;
		}
		{
			string pass = get_passwd(sockets[fd]["user"]);
			if( user_exist(sockets[fd]["user"]) && crypt(arg, pass) == pass )
			{
				string pwd = get_pwd(sockets[fd]["user"]);

				socket_send(fd, "230 Login successful.\n");
				monitor(sprintf("%s(%s) 登入。", sockets[fd]["user"], sockets[fd]["address"]));
				if( file_size(pwd) != -2 ) pwd = "/";
				sockets[fd]["pwd"] = pwd;
				sockets[fd]["login"] = 1;
				break;
			}
			socket_send(fd, "530 Login incorrect.\n");
		}
		break;
	case "type":	/* 資料傳送類型 */
		arg = lower_case(arg);
		if( arg == "i" )
		{
			sockets[fd]["type"] = "binary";
			socket_send(fd, "200 Switching to Binary mode.\n");
			break;
		}
		if( arg == "a" )
		{
			sockets[fd]["type"] = "ascii";
			socket_send(fd, "200 Switching to ASCII mode.\n");
			break;
		}
		socket_send(fd, "500 Unrecognised TYPE command.\n" );
		break;
	case "rest":	/* 資料傳送起始位元 */
		{
			int pos;
			sscanf(arg, "%d", pos);
			socket_send(fd, "350 Restart position accepted (" + pos + ").\n");
			sockets[fd]["rest"] = pos;
		}
		break;
	case "pwd":	/* 目前工作目錄 */
		socket_send(fd, "257 \"" + sockets[fd]["pwd"] + "\"\n");
		break;
	case "abor":	/* 中斷傳輸動作 */
		socket_send(fd, "426 Transfer aborted. Data connection closed.\n");
		monitor(sprintf("%s 中斷檔案傳輸動作。", sockets[fd]["user"]));
		if( !undefinedp(sockets[fd]["send_handle"]) )
		{
			if( find_call_out(sockets[fd]["send_handle"]) )
				remove_call_out(sockets[fd]["send_handle"]);
			monitor(sprintf("%s 已移除檔案傳輸呼叫。", sockets[fd]["user"]));
			map_delete(sockets[fd], "send_handle");
		}
		sockets[fd]["rest"] = 0;

		map_delete(sockets[fd], "list");
		map_delete(sockets[fd], "retr");
		map_delete(sockets[fd], "retr_len");
		map_delete(sockets[fd], "stor");
		map_delete(sockets[fd], "stor_len");
		close_data_connection(fd);
		socket_send(fd, "226 Abort successful.\n");
		break;
	case "port":	/* 客戶端要求伺服器連上客戶端*/
		{
			string *args, data_addr;
			int data_mode, data_port, data_fd, res;

			args = explode(arg, ",");
			if( sizeof(args) < 6 )
			{
				socket_send(fd, "500 Illegal PORT command.\n");
				break;
			}
			data_addr = implode(args[0..3], ".");
			data_port = (to_int(args[4]) << 8) + to_int(args[5]);
			if( sockets[fd]["type"] == "ascii" ) data_mode = STREAM;
			if( sockets[fd]["type"] == "binary" ) data_mode = STREAM_BINARY;
			data_fd = socket_create(data_mode, (: data_read_callback :), (: data_close_callback :));
			if( data_fd < 0 )
			{
				socket_send(fd, "425 Can't create data socket.\n");
				break;
			}
			res = socket_connect(data_fd,
			    data_addr + " " + data_port,
			    (: data_read_callback :), (: data_write_callback :));
			if( res < 0 )
			{
				socket_close(data_fd);
				socket_send(fd, "425 Can't build data connection.\n");
			}
			sockets[fd]["data_conn"] = "port";
			sockets[fd]["data_fd"] = data_fd;
			data_sockets[data_fd] = fd;
			socket_send(fd, "200 PORT command successful.\n");
		}
		break;
	case "pasv":	/* 伺服器端要求客戶端連上伺服器端 */
		{       
			int data_mode, data_fd, port, res;
			string host;
			if( sockets[fd]["type"] == "ascii" ) data_mode = STREAM;
			if( sockets[fd]["type"] == "binary" ) data_mode = STREAM_BINARY;
			data_fd = socket_create(data_mode, (: data_read_callback :), (: data_close_callback :));

			if( data_fd < 0 )
			{
				socket_send(data_fd, "425 Can't create data socket.\n");
				break;
			}

			do
			{
				port = random((1 << 16) - (1 << 10)) + (1 << 10);
				res = socket_bind(data_fd, port);
			}
			while(res == EEADDRINUSE);

			if( res < 0 )
			{
				socket_close(socket_fd);
				socket_send(fd, "425 Can't bind socket port.\n");
				return;
			}
			res = socket_listen(data_fd, (: data_listen_callback :));
			if( res < 0 )
			{
				socket_close(socket_fd);
				socket_send(fd, "425 Can't bind listen socket.\n");
				return;
			}
			host = host_ip;
			host = replace_string(host, ".", ",");
			socket_send(fd, sprintf("227 Entering Passive Mode (%s,%d,%d)\n",
				host, (port >> 8), port - ((port >> 8) << 8)));
			sockets[fd]["data_conn"] = "pasv";
			data_sockets[data_fd] = fd;
		}
		break;
	case "list":	/* 目錄清單 */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string list = cmd_list(sockets[fd]["pwd"], arg, sockets[fd]["user"]);
			if( !list )
			{
				socket_send(fd, "550 No files found.\n");
				close_data_connection(fd);
				break;
			}
			socket_send_list(fd, list);
		}
		break;
	case "nlst":	/* 目錄清單(簡短) */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string list = cmd_nlist(sockets[fd]["pwd"], arg);
			if( !list )
			{
				socket_send(fd, "550 No files found.\n");
				close_data_connection(fd);
				break;
			}
			socket_send_list(fd, list);
		}
		break;
	case "cdup":	/* 更變工作目錄至上一層 */
		arg = "..";
	case "cwd":	/* 更變目前工作目錄 */
		{
			string pwd = cmd_cwd(sockets[fd]["pwd"], arg);
			if( arg != "" && pwd )
			{
				sockets[fd]["pwd"] = pwd;
				socket_send(fd, "250 Directory successfully changed.\n");
				break;
			}
			socket_send(fd, "550 Failed to change directory.\n");
		}
		break;
	case "retr":	/* 接收檔案 */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		socket_send_file(fd, arg);
		break;
	case "stor":	/* 傳送檔案 */
		if( undefinedp(sockets[fd]) ) return;
		if( undefinedp(sockets[fd]["data_fd"]) )
		{
			if( undefinedp(sockets[fd]["data_conn"]) )
			{
				socket_send(fd, "425 Use PORT or PASV first.\n");
				break;
			}
			call_out((: ftp_input :), 1, fd, str);
			break;
		}
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( file_size(path) != -1 && !sockets[fd]["rest"] && !write_file(path, "", 1) )
			{
				socket_send(fd, "553 Could not create file.\n");
				break;
			}
			sockets[fd]["stor"] = path;
			sockets[fd]["stor_len"] = 0;
			socket_send(fd, "150 Ok to send data.\n");
			monitor(sprintf("%s 開始上傳檔案 %s。", sockets[fd]["user"], path));
			data_sockets[sockets[fd]["data_fd"]] = fd;
		}
		break;
	case "dele":	/* 刪除檔案 */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( rm(path) )
			{
				socket_send(fd, "250 Delete operation successful.\n");
				monitor(sprintf("%s 刪除檔案 %s。", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "550 Delete operation failed.\n");
		}
		break;
	case "mkd":	/* 建立目錄 */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( mkdir(path) )
			{
				socket_send(fd, "257 \"" + path + "\" created.\n");
				monitor(sprintf("%s 建立目錄 %s。", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "550 Create directory operation failed. \n");
		}
		break;
	case "rmd":	/* 刪除目錄 */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			if( rmdir(path) )
			{
				socket_send(fd, "250 Remove directory operation successful.\n");
				monitor(sprintf("%s 刪除目錄 %s。", sockets[fd]["user"], path));
				break;
			}
			socket_send(fd, "250 Remove directory operation failed.\n");
		}
		break;
	case "rnfr":	/* 更改名稱從 */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			sockets[fd]["rnfr"] = path;
			socket_send(fd, "350 Ready for RNTO.\n");
		}
		break;
	case "rnto":	/* 更改名稱至 */
		{
			string path = get_path(sockets[fd]["pwd"], arg);
			sockets[fd]["rnto"] = path;
			if( !rename(sockets[fd]["rnfr"], sockets[fd]["rnto"]) )
			{
				socket_send(fd, "250 Rename successful.\n");
				monitor(sprintf("%s 更改 %s 檔案名稱為 %s。", sockets[fd]["user"], sockets[fd]["rnfr"], sockets[fd]["rnto"]));
				break;
			}
			socket_send(fd, "250 Rename failed.\n");
		}
		break;
	case "syst":	/* 顯示目前伺服器的系統類型 */
		socket_send(fd, "215 MUD Type: " + MUDLIB_NAME +"\n");
		break;
	case "quit":	/* 離開 */
		socket_send(fd, "221 Goodbye.\n");
		if( !undefinedp(sockets[fd]["login"]) )
			monitor(sprintf("%s(%s) 離開。", sockets[fd]["user"], sockets[fd]["address"]));

		close_data_connection(fd);
		close_connection(fd);
		break;
	default:	/* 其他指令 */
		socket_send(fd, "500 Unknown command.\n");
		break;
	}
}

/* socket 傳送資料完成通知 */
private void data_write_callback(int fd)
{
	int percentage; 

	if( undefinedp(sockets[data_sockets[fd]]["retr"]) ) return;
	//if( !data_callback_time ) data_callback_time = time();
	//if( (time() - data_callback_time) < TRANSFER_MSG_DELAY ) return;
	data_callback_time = time();
	percentage = to_int(to_float(sockets[data_sockets[fd]]["rest"]) / sockets[data_sockets[fd]]["retr_len"] * 100);

	monitor(sprintf("%s 下載檔案 %s 進度 %s/%s (%d%%)",
		sockets[data_sockets[fd]]["user"],
		sockets[data_sockets[fd]]["retr"],
		memory_expression(sockets[data_sockets[fd]]["rest"]),
		memory_expression(sockets[data_sockets[fd]]["retr_len"]),
		percentage,
	    ));
}

/* socket 資料接收通知 */
private void data_read_callback(int fd, mixed msg)
{
	int size = (stringp(msg) ? strlen(msg):sizeof(msg));
	// If data mode are using Binary the msg type is array
	// If data mode are using Ascii the msg type is string

	if( undefinedp(sockets[data_sockets[fd]]["stor"]) ) return;
	if( size > MAX_BYTE_TRANSFER )
	{
		monitor(sprintf("%s 傳輸送度超過寫入限制檔案 %s 可能損毀。",
			sockets[data_sockets[fd]]["user"],
			sockets[data_sockets[fd]]["stor"]));
	}
	write_buffer(sockets[data_sockets[fd]]["stor"], sockets[data_sockets[fd]]["rest"], msg);
	sockets[data_sockets[fd]]["rest"] += size; 	// 檔案大小 (續傳位置 + 上傳大小)
	sockets[data_sockets[fd]]["stor_len"] += size; 	// 本次上傳大小 (上傳大小)
}

/* socket 關閉通知 */
private void data_close_callback(int fd)
{
	if( undefinedp(data_sockets[fd]) ) return;
	if( undefinedp(sockets[data_sockets[fd]]["stor"]) ) return;

	socket_send(data_sockets[fd], "226 File receive OK.\n");
	monitor(sprintf("%s 上傳檔案 %s 完成，共 %s。",
		sockets[data_sockets[fd]]["user"],
		sockets[data_sockets[fd]]["stor"],
		memory_expression(sockets[data_sockets[fd]]["stor_len"]),
	    ));

	close_data_connection(data_sockets[fd]);

	sockets[data_sockets[fd]]["rest"] = 0;

	map_delete(sockets[data_sockets[fd]], "stor");
	map_delete(sockets[data_sockets[fd]], "stor_len");
	map_delete(data_sockets, fd);
}

/* socket 連結通知 */
private void data_listen_callback(int fd)
{
	int res_fd;

	res_fd = socket_accept(fd, (: data_read_callback :), (: data_write_callback :));
	if( res_fd < 0 )
	{
		log_error("data_listen_callback: " + socket_error(res_fd) + ".\n");
		return;
	}
	sockets[data_sockets[fd]]["data_fd"] = res_fd;
	map_delete(data_sockets, fd);
	socket_close(fd);
}

mapping query_socket(int fd)
{
	return sockets[fd];
}

mapping query_sockets() 
{
	return sockets;
}

mapping query_resolves()
{
	return resolves;
}

string memory_expression(int m)
{ 
	float mem = to_float(m);
	if( mem<1024 ) return sprintf("%.1f Bytes", mem);
	if( mem<1024*1024 ) return sprintf("%.1f KB", (float)mem / 1024);
	return sprintf("%.2f MB", (float)mem / (1024*1024));
}
string query_name()
{
	return "檔案傳輸系統(FTP_D)";
}
