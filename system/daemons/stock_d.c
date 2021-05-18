/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : stock_d.c
 * Author : Clode@Revival World
 * Date   : 2007-09-06
 * Note   : �Ѳ����F
 * Update : ��ƨӷ� : http://just.honsec.com.tw/z/ze/zeg/zeg_EB011000E_I.djhtm
 *
 -----------------------------------------
 */
#include <daemon.h>
#include <socket.h>
#include <socket_err.h>

#define DATA_PATH			"/data/daemon/stock.o"

mapping stocks;
int last_update_time;

nosave mapping connection;
nosave string *stock_pages =
({
	// �W�����q
	"EB011000E",
	"EB012000E",
	"EB013000E",
	"EB014000E",
	"EB015000E",
	"EB016000E",
	"EB030000E",
	"EB031000E",
	"EB018000E",
	"EB019000E",
	"EB020000E",
	"EB021000E",
	"EB022000E",
	"EB032000E",
	"EB033000E",
	"EB034000E",
	"EB035000E",
	"EB036000E",
	"EB037000E",
	"EB038000E",
	"EB039000E",
	"EB025000E",
	"EB026000E",
	"EB027000E",
	"EB028000E",
	"EB029000E",
	"EB040000E",
	"EB091000E",
	"EB099000E",
	
	// �W�d���q
	"EB142000B",
	"EB143000B",
	"EB144000B",
	"EB145000B",
	"EB146000B",
	"EB147000B",
	"EB141000B",
	"EB148000B",
	"EB150000B",
	"EB151000B",
	"EB162000B",
	"EB163000B",
	"EB164000B",
	"EB165000B",
	"EB166000B",
	"EB167000B",
	"EB168000B",
	"EB169000B",
	"EB155000B",
	"EB156000B",
	"EB157000B",
	"EB158000B",
	"EB159000B",
	"EB161000B",
	"EB187000B",
	"EB188000B",
	"EB189000B",
});                     

int save()
{
	return save_object(DATA_PATH);	
}

int remove()
{
	return save();
}

mapping query_stocks()
{
	return stocks;
}
      
void create()
{
	if( !restore_object(DATA_PATH) )
	{
		stocks = allocate_mapping(0);
		save();
	}	
}       

nosave string text = @SYNTAX
genlinkfield('%*d' '%s');
//--></script>
</TD>
<td class=%s>%f</td>
<td class=%*s>%f</td>
<td class=%*s>%f%%</td>
<td class=%*s>%f</td>
<td class=%*s>%f</td>
<td class=%*s>%f</td>
<td class=%*s>%s</td>
<td class=%*s>%*f</td>
<td class=%*s>%*f</td>
SYNTAX;

// �ѪR���
void parse_data(string data)
{
	int pos_s;
	int pos_e;
	int code;
	string temp;

	while(sizeof(data))
	{
		pos_s = strsrch(data, "genlinkfield");
		
		if( pos_s == -1 ) break;
		
		data = data[pos_s..];

		pos_e = strsrch(data, "</tr>")-1;

		temp = data[0..pos_e];

		data = data[pos_e..];
		
		if( sscanf(temp, "%*sgenlinkfield('%d',%*s", code) != 3 )
			continue;

		stocks[code] = allocate_mapping(0);

		temp = replace_string(temp, "\r", "");
		temp = replace_string(temp, "\"", "");
		temp = replace_string(temp, ",", "");

		if( sscanf(temp, text, stocks[code]["�Ѳ��W��"], stocks[code]["���A"], stocks[code]["���L"], stocks[code]["���^"], stocks[code]["���^�T"], stocks[code]["�}�L"], stocks[code]["�̰�"], stocks[code]["�̧C"], stocks[code]["����i��"]) != 20 )
			CHANNEL_D->channel_broadcast("sys", "�L�k�ѪR "+code+" �ӪѸ��("+temp+")");
		else
			CHANNEL_D->channel_broadcast("nch", "���\�ѪR "+code+stocks[code]["�Ѳ��W��"]+" �ӪѸ��(���L�� "+stocks[code]["���L"]+")");
	}

	connection["data"] = "";
	return ;
}

int query_last_update_time()
{
	return last_update_time;
}

void get_stock_data()
{                       
	int err, fd;  

	if( !sizeof(connection["pages"]) )
	{
		last_update_time = time();
		save();
		return;
	}

	/* �}�Һ����s�� */
	fd = socket_create(STREAM, "read_callback", "close_socket");

	if ( fd < 0 )
		return;

	err = socket_connect(fd, "218.32.80.123 80", "read_callback", "write_callback");

	if( err != EESUCCESS )
		CHANNEL_D->channel_broadcast("sys", "�L�k�s�u�Ѳ������C");

	return;
}


void refresh_stock_data()
{
	connection = allocate_mapping(0);
	connection["pages"] = copy(stock_pages);
	connection["data"] = "";

	CHANNEL_D->channel_broadcast("sys", "�}�l��s�ѥ���ơC");
	get_stock_data();
}

int close_socket(int fd)
{
	socket_close(fd);

	parse_data(connection["data"]);

	call_out((: get_stock_data :), 3);
}

void read_callback(int fd, mixed message)
{
	/* ��C�����o����Ʀs�i strs �� */
	connection["data"] += message;
	
	return ;
}

void write_callback(int fd)
{
	CHANNEL_D->channel_broadcast("nch", "���"+connection["pages"][0]+"��ơC");

	socket_write(fd, "GET /z/ze/zeg/zeg_"+connection["pages"][0]+"_I.djhtm\r\n");

	connection["pages"] = connection["pages"][1..];

	return ;
}


string query_name()
{
	return "�Ѳ��t��(STOCK_D)";
}
