/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : _bulletin_board.c
 * Author : Clode@RevivalWorld
 * Date   : 2007-08-16
 * Note   : �d���O�\���~����
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>

#define BOARD_DATA_PATH		"/data/board/"

array summary;

string query_board_id(object ob)
{
	object env = environment(ob);

	string board_id;
	
	if( !query_temp("decorated", ob) )
		return 0;
	
	board_id = query("board_id", ob);

	if( !stringp(board_id) )
	{
		// ��v�����ӤH�d���O
		if( env->query_room_function() == "livingroom" && user_exists(query("owner", env)) )
			board_id = "player_"+query("owner", env);
		// �����d���O
		else if( stringp(env->query_city()) )
			board_id = "city_"+env->query_city();
	}

	return board_id;	
}

// ���o�s�ɸ��|
string query_path(object ob)
{
	string path = BOARD_DATA_PATH + query_board_id(ob) + "/";
	
	// �Y�S���d���O�ؿ��h�إ�
	if( file_size(path) != -2 ) mkdir(path);
		
	return path;	
}

// ���s��z�K�n
varargs array check_summary(object ob)
{
	string author_name;
	string author_id;
	string title;
	string file;
	string *files;
	string path;

	if( !query_board_id(ob) )
		return 0;

	path = query_path(ob);
	files = get_dir(path);

	if( arrayp(summary) && sizeof(summary) && sizeof(summary)/3 == sizeof(files) && summary[<3] == to_int(files[<1]) )
		return summary;

	summary = allocate(0);

	// Ū���s�ƩҦ��d�����
	foreach( file in files )
	{
		sscanf( read_file(path+file,1,1), "%s(%s)|%s\n", author_name, author_id, title );

		summary += ({ to_int(file), author_name+"("+author_id+")", title });
	}
	
	return summary;
}

void done_post(object me, int time, string title, object ob, string arg)
{
	// ñ�W��
	string sig = query("signature", me);
	string content;

	content = me->query_idname()+"|"+title+"\n"+arg+(sig ? "\n\n"NOR WHT"�w�w\n"NOR+sig+NOR : NOR);

	if( write_file( query_path(ob)+time, content ) )
	{
		tell(me, "�d�������C\n");
		
		if( !wizardp(me) )
			set_temp("lastpost/time", time(), me);

		me->finish_input();
	}
	else
		return tell(me, "�d�����ѡA�гq���Ův�B�z�C\n");
}

void do_post(object me, string arg, object ob)
{
	int time = time();
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "�{�b�L�k�o��峹�C\n");

	if(!arg)
		return tell(me, "�Ь��z���峹���w�@�Ӽ��D�C\n");

	if( noansi_strlen(arg) > 26 )
		return tell(me, "�A�����D�Ӫ��F�A���Y�u�A�����D�C\n");

	if( time() - query_temp("lastpost/time", me) < 20 )
		return tell(me, "��p�ثe�o��ɶ��������j 20 ��C\n");

	while( member_array(time, summary) != -1 ) time++;

	summary += ({ time, me->query_idname(), arg });

	me->edit( (: done_post, me, time, arg, ob :) );
}

void do_discard(object me, string arg, object ob)
{
	int num, file;
	string board_id = query_board_id(ob);

	check_summary(ob);

	if( !board_id )
		return tell(me, "�{�b�L�k�R���峹�C\n");

	if( !arg || !sscanf(arg, "%d", num) )
		return tell(me, "�п�J�w�R�����峹�s���C\n");

	num = (num-1)*3;

	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "�S���o�i�d���C\n");

	file = summary[num];

	if( me->query_idname() != summary[num+1] )
	{
		string author_id;
		string my_id = me->query_id(1);
		sscanf( remove_ansi(summary[num+1]), "%*s(%s)", author_id );
		author_id = lower_case(author_id);

		if( 
			my_id == author_id || 
			(wizardp(me) && SECURE_D->level_num(my_id) >= SECURE_D->level_num(author_id)) ||
			(board_id[0..5] == "player" && my_id == board_id[7..]) ||
			(board_id[0..3] == "city" && CITY_D->is_mayor(board_id[5..], me))
		) ;
		else
			return tell(me, "�o�i�d�����O"+ pnoun(2, me) +"�g���C\n");
	}

	if( rm(query_path(ob)+(file<1000000000?"0":"")+file) )
	{
		summary = summary[0..num-1] + summary[num+3..];

		tell(me, "�R�������C\n");
	}
	else tell(me, "�R�����ѡA�гq���Ův�B�z�C\n");
}

void do_read(object me, string arg, object ob)
{
	string msg;
	int num, last_read, file;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "�{�b�L�k�\Ū�峹�C\n");

	last_read = query("board_last_read/"+query_board_id(ob), me);

	if( !arg || arg == "new" || arg == "next" )
	{
		if( last_read )
		{
			int* unreads = filter_array(summary, (: intp($1) && $(last_read) < $1 :));
			
			if( sizeof(unreads) )
				num = member_array(unreads[0], summary);
			else
				num = sizeof(summary);
		}
		else
			num = 0;
	}
	else if( sscanf(arg, "%d", num) )
		num = (num-1) * 3;
	else
		return tell(me, "�A�nŪ�ĴX�i�d���S\n");

	if( num < 0 || num > sizeof(summary) )
		return tell(me, "�S���o�i�d���C\n");

	else if( num == sizeof(summary) )
		return tell(me, "�S���s���d���C\n");

	file = summary[num];

	msg = sprintf("[1;46m�g ��[m %-27s[1;46m�@ ��[m %s\n[1;46m�D �D[m %-27s[1;46m�� ��[m %s[m\n�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w[1;30m[[1;37mContent[m[1;30m][m�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n%s\n%s",
	    "��"+CHINESE_D->chinese_number(num/3+1)+"�g",
	    summary[num+1],
	    summary[num+2],
	    TIME_D->replace_ctime(file),
	    read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2),
	    "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w[1;30m[[1;37mEnd[m[1;30m][m�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"
	);

	me->more( msg );

	if( file >= last_read ) 
		set("board_last_read/"+query_board_id(ob), file, me);
}

void do_followup(object me, string arg, object ob)
{
	int num, file, time = time();
	string msg="";
	string title;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "�{�b�L�k�^�Ф峹�C\n");

	if( !arg )
		return tell(me, "�п�J���^�Ъ��峹�s���� last �^�г̫�@�g�峹�C\n");

	if( arg == "last" ) 
		num = sizeof(summary)-3;
	else if( sscanf(arg, "%d", num) )
		num = (num-1)*3;	
	else
		return tell(me, "�п�J���^�Ъ��峹�s���C\n");
		
	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "�S���o�i�d���C\n");

	file = summary[num];

	foreach( string word in explode( read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2), "\n" ) )
	{
		if( word == "" || word[0..7]=="[1;30m>" ) continue;
		else if( word == NOR WHT"�w�w" ) break;
		else if( word[0..5] == "[33m>" ) msg += "[1;30m> "+word[5..]+"\n";
		else if( word[0..5] == "[36m>" ) msg += "[33m> "+word[5..]+"\n";
		else msg += "[36m> "+word+"\n";
	}

	if( msg[0..5] == "[36m>" ) 
		msg = "[36m> ��"+summary[num+1]+" �b "+TIME_D->replace_ctime(file)+" �d�U�o�g�d���R\n" + msg;

	msg += "\n��"+me->query_idname()+" �b "+TIME_D->replace_ctime(time)+" �d�U�o�g�d���R\n";

	while( member_array(time, summary) != -1 ) time++;

	title = (summary[num+2][0..5] != "�^�СG" ? "�^�СG" : "") + summary[num+2];

	summary += ({ time, me->query_idname(), title });

	me->edit( (: done_post, me, time, title, ob :), msg );
}

void do_mail(object me, string arg, object ob)
{
	int file, num;
	string msg, email;
	
	check_summary(ob);

	if( !query_board_id(ob) )
		return tell(me, "�{�b�L�k�l�H�峹�C\n");

	if( !arg || !sscanf(arg, "%d", num) )
		return tell(me, "���O�榡: mail <�峹�s��>\n");

	num = (num-1)*3;

	if( num < 0 || num >= sizeof(summary) )
		return tell(me, "�S���o�i�d���C\n");

	if( !stringp(email = query("email", me)) )
		return tell(me, pnoun(2, me)+"�S���]�w�q�l�l��C\n");

	if( sscanf(email, "%*s@%*s.%*s") != 3 )
		return tell(me, pnoun(2, me)+"�ҳ]�w���q�l�l��H�c�榡�O�����T���C\n");

	file = summary[num];

	msg = "�H�U�T������� ���ͪ��@��(Revival World)\n\n";
	msg += sprintf("[1;46m�g ��[m %-27s[1;46m�@ ��[m %s\n[1;46m�D �D[m %-27s[1;46m�� ��[m %s[m\n�w�w�w�w�w�w�w�w�w�w�w�w�w[1;30m[[1;37mContent[m[1;30m][m�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n%s\n%s",
	    "��"+CHINESE_D->chinese_number(num/3+1)+"�g",
	    summary[num+1],
	    summary[num+2],
	    TIME_D->replace_ctime(file),
	    read_file(query_path(ob)+(file<1000000000?"0":"")+file, 2),
	    "�w�w�w�w�w�w�w�w�w�w�w�w�w�w[1;30m[[1;37mEnd[m[1;30m][m�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n\n"
	);
	msg = remove_ansi(msg);
	SMTP_D->mail(email, msg, "���ͪ��@��(Revival World) - �峹���");
	tell(me, "�峹�w�e��"+pnoun(2, me)+"�� E-mail �H�c("+email+")�C\n");
}
