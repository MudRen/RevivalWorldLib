/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : �����F
 * Update :
 *  o 2002-09-26 
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <money.h>

#define DATA		"/data/daemon/auction.o"

#define STORE_CAPACITY		100000
#define WAREHOUSE_CAPACITY	-1

#define AUCTION_STATUS_GOING	0
#define AUCTION_STATUS_FINISHED	1
#define AUCTION_STATUS_FAILED	2
#define AUCTION_STATUS_SOLD	3

#define REQUEST_STATUS_GOING	0
#define REQUEST_STATUS_FINISHED	1

array auction;
array request;

int save()
{
	save_object(DATA);
}

array query_auction()
{
	return auction;	
}

void add_auction(mapping data)
{	
	auction += ({ data });
	
	save();
}

void remove_auction(int i)
{
	auction = auction[0..i-1] + auction[i+1..];
	
	save();
}

void add_request(mapping data)
{	
	request += ({ data });
	
	save();
}

void remove_request(int i)
{
	request = request[0..i-1] + request[i+1..];
	
	save();
}

void reset_request()
{
	request = allocate(0);
}

array query_request()
{
	return request;
}

void refresh_top_buyer(int i)
{
	object old_top_buyer;
	object new_top_buyer;

	if( i < 0 || i >= sizeof(auction) ) return;

	if( stringp(auction[i]["top_buyer"]) )
		old_top_buyer = find_player(auction[i]["top_buyer"]);
		
	if( sizeof(auction[i]["auction_data"]) )
	{
		auction[i]["top_buyer"] = auction[i]["auction_data"][0][0];
		auction[i]["top_price"] = auction[i]["auction_data"][0][1];
	}
	
	if( stringp(auction[i]["top_buyer"]) )
		new_top_buyer = find_player(auction[i]["top_buyer"]);
	
	if( old_top_buyer != new_top_buyer && objectp(old_top_buyer) )
		tell(old_top_buyer, "\a"+pnoun(2, old_top_buyer)+"�����X���w�g�Q��L�H�W�L�C\n");
}

void refresh_all_top_buyer()
{
	for(int i=0;i<sizeof(auction);i++)
		refresh_top_buyer(i);
}

void do_auction_list(object me, string arg)
{
	int i;
	string msg;
	mapping data;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	int top_price;
	string top_buyer;
	string status;
	
	if( !arg )
	{
		msg = HIB"�ثe���@�ɪ����~�p�U�G\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg +=    "�s�� ���~                                  �ƶq        �ثe�̰��X��    �Ѿl�ɶ�\n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
		foreach(data in auction)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) ) continue;
			
			top_price = to_int(data["top_price"]);
			top_buyer = data["top_buyer"];
		
			if( data["status"] == AUCTION_STATUS_FINISHED )
				status = HIR"����"NOR;
			else if( data["status"] == AUCTION_STATUS_FAILED )
				status = HIG"�y��"NOR;
			else if( data["status"] == AUCTION_STATUS_SOLD )
				status = HIY"��X"NOR;
			else
				status = AUCTION_STATUS_GOING;

			msg += sprintf("%-5d%s%20s %11s\n",
				++i,
				ob->query_idname()+repeat_string(" ", 44-noansi_strlen(ob->query_idname()+NUMBER_D->number_symbol(data["amount"])))+HIG+NUMBER_D->number_symbol(data["amount"])+NOR,
				top_price ? HIY+NUMBER_D->number_symbol(top_price)+NOR : NOR YEL+NUMBER_D->number_symbol(data["base_price"])+NOR,
				status || ((data["minute"]>=60)?HIM+(data["minute"]/60)+"h"+NOR:NOR BLINK MAG+data["minute"]+"m"NOR),
			);
		}

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "list '�s��' �[�ݸө��~���Բө����\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(auction) )
		return tell(me, "�п�J���T���s���C\n");
		
	data = auction[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
		error(data["basename"]+" ������J����");
	
	if( data["status"] == AUCTION_STATUS_FINISHED )
		status = HIR"����"NOR;
	else if( data["status"] == AUCTION_STATUS_FAILED )
		status = HIG"�y��"NOR;
	else if( data["status"] == AUCTION_STATUS_SOLD )
		status = HIY"��X"NOR;
	else
		status = AUCTION_STATUS_GOING;
		
	if( !arrayp(data["list_players"]) )
		data["list_players"] = allocate(0);

	data["list_players"] |= ({ me->query_id(1) });
			
	msg =  "�s�� "+i+" �����~��Ʀp�U�G\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += "����    �G"+capitalize(data["seller"])+"\n";
	msg += "���~    �G"+ob->query_idname()+"\n";
	msg += "�ƶq      �G"+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "��橳��  �G$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["base_price"])+"\n";
	msg += "�����ʶR���G$"+MONEY_D->query_default_money_unit()+" "+NUMBER_D->number_symbol(data["direct_price"])+"\n";
	msg += "�Ѿl�ɶ�  �G"+(status || (data["minute"]+" ����"))+"\n";

	if( data["finish_time"] > 0 )
	{
		if( data["status"] == AUCTION_STATUS_FAILED )
			msg += "�t�Φ^��  �G�����~�N�� "+TIME_D->replace_ctime(data["finish_time"]+14*24*60*60)+" �Ѩt�Φ۰ʧR��\n";
		else
			msg += "���̻�^�G�o�Ъ̥��� "+TIME_D->replace_ctime(data["finish_time"]+7*24*60*60)+" ���e������~�̡A�N�}����̻�^�C\n";
	}

	msg += "�v�Ъ��p  �G\n";
	
	if( !sizeof(data["auction_data"]) )
		msg += "    �L�H�X��\n";
	else foreach(array d in data["auction_data"])
	{
		msg += sprintf("    �X���� %-12s  �X�� %s\n", (wizardp(me) || d[0] == me->query_id(1)) ? capitalize(d[0]) : "(Unknown)", money(money_unit, d[1]));
	}
	
	ob = new(data["basename"]);
	msg += "\n"+ob->long();
	destruct(ob);
	
	msg += "\n�@�� "+sizeof(data["list_players"])+" �쪱�a�d�߹L�o�����~�C\n";
	msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
	me->more(msg);
}

void do_request_list(object me, string arg)
{
	int i;
	string msg;
	mapping data;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	string status;
	mapping product_info;
	string idname;

	if( !arg || arg == "all" )
	{
		if( !arg )
			msg = HIB"�ثe���@�ɪ��q��(�ȦC�X�����ת��~)�p�U�G\n"NOR;
		else
			msg = HIB"�ثe���@�ɪ��q��p�U�G\n"NOR;
		
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg +=    "�s�� �q��                                    �ƶq     ���($RW) ���X��      ���A   \n"NOR;
		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		
		foreach(data in request)
		{
			ob = load_object(data["basename"]);
			
			if( !objectp(ob) )
			{
				if( objectp(ob = load_object("/std/module/product/"+data["basename"]+"_module")) )
				{
					product_info = ob->query_product_info();
					idname = product_info["name"]+"("+capitalize(product_info["id"])+")";
				}
				else
					error("�o�Ϳ��~�C\n");
			}
			else
				idname = ob->query_idname();
			
			++i;
			
			if( data["status"] == REQUEST_STATUS_GOING )
				status = YEL"������"NOR;
			else if( data["status"] == REQUEST_STATUS_FINISHED )
			{
				if( !arg )
					continue;
					
				status = HIY"�w����"NOR;
			}
			
			msg += sprintf("%-5d%s%14s %-12s%s\n",
				i,
				idname+repeat_string(" ", 44-noansi_strlen(idname+NUMBER_D->number_symbol(data["amount"])))+HIG+NUMBER_D->number_symbol(data["amount"])+NOR,
				HIY+NUMBER_D->number_symbol(data["price"])+NOR,
				HIW+capitalize(data["buyer"])+NOR,
				status
			);
		}

		msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
		msg += "list '�s��' �[�ݸө��~���Բө����\n\n";
		
		return me->more(msg);
	}
	
	i = to_int(arg);
	
	if( i < 1 || i > sizeof(request) )
		return tell(me, "�п�J���T���s���C\n");
		
	data = request[i-1];
	
	ob = load_object(data["basename"]);
	
	if( !objectp(ob) )
	{
		if( objectp(ob = load_object("/std/module/product/"+data["basename"]+"_module")) )
			product_info = ob->query_product_info();
		else
			error("�o�Ϳ��~�C\n");
	}
				
	if( data["status"] == REQUEST_STATUS_FINISHED )
		status = HIY"�w����"NOR;
	else
		status = YEL"������"NOR;
				
	msg =  "�s�� "+i+" ���q���Ʀp�U�G\n";
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += "�R�a      �G"+capitalize(data["buyer"])+"\n";
	msg += "�Ъ���    �G"+(product_info ? product_info["name"]+"("+capitalize(product_info["id"])+") - "HIY"�u�~���~"NOR : ob->query_idname())+NOR"\n";
	msg += "�ƶq      �G"+NUMBER_D->number_symbol(data["amount"])+"\n";
	msg += "���      �G"HIY+money(money_unit, data["price"])+NOR"\n";
	msg += "�`��      �G"HIY+money(money_unit, data["price"] * data["amount"])+NOR"\n";
	msg += "���A      �G"+status+"\n";
	msg += "�ѳf��    �G"+(data["supporter"] ? capitalize(data["supporter"]) : "�L")+"\n";
	
	if( !product_info )
	{
		ob = new(data["basename"]);
		msg += "\n"+ob->long();
		destruct(ob);
	}

	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

	tell(me, msg);
}

void confirm_bid(object me, int i, object ob, int price, string money_unit, mapping data, string arg)
{
	int num;
	int top_price;
	string top_buyer;
	int next_price;
	object seller;

	if( arg != "yes" && arg != "y" )
	{
		tell(me, pnoun(2, me)+"�M�w���v�гo�ө��~�C\n");
		return me->finish_input();
	}

	num = i+1;

	// ��Ƥ��P
	if( !mapp(auction[i]) || 
		auction[i]["status"] != data["status"] ||
		auction[i]["basename"] != data["basename"] ||
		auction[i]["amount"] != data["amount"] ||
		auction[i]["seller"] != data["seller"] ||
		auction[i]["base_price"] != data["base_price"]
	)
	{
		tell(me, "���~�s�����Ǥw�g���ܡA�Э��s�v�СC\n");
		return me->finish_input();
	}

	if( auction[i]["status"] == AUCTION_STATUS_FINISHED )
	{
		tell(me, "�o�����~�w�g���СC\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == AUCTION_STATUS_FAILED )
	{
		tell(me, "�o�����~�w�g�y�СC\n");
		return me->finish_input();
	}
	else if( auction[i]["status"] == AUCTION_STATUS_SOLD )
	{
		tell(me, "�o�����~�w�g�Q�����ʶR�C\n");
		return me->finish_input();
	}

	if( !sizeof(auction[i]["auction_data"]) )
	{
		if( price < auction[i]["base_price"] )
		{
			tell(me, "�v���������������橳��("+money(money_unit, auction[i]["base_price"])+")�C\n");
			return me->finish_input();
		}

		if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
		{
			tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
			return me->finish_input();
		}

		auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) });
		auction[i]["list_players"] = allocate(0);

		refresh_top_buyer(i);

		if( price >= auction[i]["direct_price"] )
		{
			auction[i]["status"] = AUCTION_STATUS_SOLD;
			auction[i]["minute"] = 0;
			auction[i]["finish_time"] = time();

			if( auction[i]["seller"] != "" )
				MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
			CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"����"NOR"�G�� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	

			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");
			
			me->finish_input();
			return;	
		}

		CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�X��"NOR"�G�� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" ���v�л��C\n");
		
		save();

		return me->finish_input();
	}
	else
	{
		top_buyer = auction[i]["top_buyer"];
		top_price = auction[i]["top_price"];

		// �����ʶR
		if( price >= auction[i]["direct_price"] )
		{
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{
				tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
				return me->finish_input();
			}

			// �N�W�@���v�Ъ̪����k��
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			auction[i]["status"] = AUCTION_STATUS_SOLD;
			auction[i]["minute"] = 0;
			auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
			auction[i]["finish_time"] = time();

			refresh_top_buyer(i);
			
			if( auction[i]["seller"] != "" )
				MONEY_D->earn_money(auction[i]["seller"], money_unit, price);

			CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"����"NOR"�G�� "+num+" �s�������~�u"+ob->query_idname()+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");

			save();

			return me->finish_input();
		}
		else
		{
			if( top_buyer == me->query_id(1) )
			{
				tell(me, pnoun(2, me)+"�w�g�O�ثe�̰��X���̡C\n");
				return me->finish_input();
			}
			
			next_price = top_price + (top_price / 20);
	
			if( next_price == top_price )
				next_price = next_price + 1;
	
			if( price < next_price )
			{
				tell(me, "�C���v�Ъ��t�Z�����O�ثe�̰��v�л� + 5% �������H�W(�t�έp�⵲�G���G"+money(money_unit, next_price)+")\n");
				return me->finish_input();
			}
			
			if( price > top_price * 10 )
			{
				tell(me, "�v�л����o����{���̰��X���� 10 ���C\n");
				return me->finish_input();
			}
	
			if( !me->spend_money(money_unit, price, BANK_PAYMENT) )
			{	
				tell(me, pnoun(2, me)+"�������� "+money(money_unit, price)+"�C\n");
				return me->finish_input();
			}

			// �N�W�@���v�Ъ̪����k��
			if( top_buyer )
				MONEY_D->earn_money(top_buyer, money_unit, top_price);

			if( price >= auction[i]["direct_price"] )
			{
				auction[i]["status"] = AUCTION_STATUS_SOLD;
				auction[i]["minute"] = 0;
				auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
				auction[i]["finish_time"] = time();
	
				refresh_top_buyer(i);
				
				if( auction[i]["seller"] != "" )
					MONEY_D->earn_money(auction[i]["seller"], money_unit, price);
				
				CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"����"NOR"�G�� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" �������ʶR���A��浲�СC\n");	
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, me->query_idname()+"�N "HIY+money(money_unit, price)+NOR" ú�I��"+pnoun(2, seller)+"�C\n");

				return me->finish_input();
			}

			auction[i]["auction_data"] = ({ ({ me->query_id(1), price }) }) + auction[i]["auction_data"];
			
			refresh_top_buyer(i);
			
			if( auction[i]["minute"] < 5 )
			{
				auction[i]["minute"] = 5;
				CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�X��"NOR"�G�� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" ���v�л��A�P�ɩ������ɶ��� 5 �����C\n");	
			}
			else
				CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�X��"NOR"�G�� "+num+" �s�������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ۥX "HIY+money(money_unit, price)+NOR" ���v�л��C\n");	
			
			save();

			return me->finish_input();
		}
	}
	
	return me->finish_input();
}

void do_bid(object me, string arg)
{
	int i, num;
	mixed price;
	string money_unit = MONEY_D->query_default_money_unit();
	object ob;

	if( wizardp(me) )
		return tell(me, "�Ův���⤣�i�ѻP�v�СC\n");

	if( !arg || sscanf(arg, "%d %s", num, price) != 2 )
		return tell(me, "�п�J���T���v�и��(help bid)�C\n");
		
	if( num < 1 || num > sizeof(auction) )
		return tell(me, "�п�J���T���s���C\n");

	i = num - 1;
	
	if( auction[i]["status"] == AUCTION_STATUS_FINISHED )
		return tell(me, "�o�����~�w�g���СC\n");
	else if( auction[i]["status"] == AUCTION_STATUS_FAILED )
		return tell(me, "�o�����~�w�g�y�СC\n");
	else if( auction[i]["status"] == AUCTION_STATUS_SOLD )
		return tell(me, "�o�����~�w�g�Q�����ʶR�C\n");

	ob = load_object(auction[i]["basename"]);
	
	if( !objectp(ob) )
		error(auction[i]["basename"]+" ������J���~�C\n");

	if( price == "cancel" )
	{
		if( auction[i]["seller"] != me->query_id(1) )
			return tell(me, "�u������̤~��������C\n");

		if( auction[i]["status"] )
			return tell(me, "�����~�w�g���Щάy�ЩγQ�����ʶR�F�A�L�k�A�������C\n");

		if( sizeof(auction[i]["auction_data"]) )
			return tell(me, "�w�g���H�ѻP�v�СA�L�k�A�������F�C\n");

		auction[i]["status"] = AUCTION_STATUS_FAILED;
		auction[i]["minute"] = 0;
		auction[i]["finish_time"] = time();
		auction[i]["auction_data"] = allocate(0);
		auction[i]["top_buyer"] = 0;
		auction[i]["top_price"] = 0;

		CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"����"NOR"�G"+me->query_idname()+"������ "+num+" �s�����~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�����C\n");
		
		return;
	}

	if( me->query_id(1) == auction[i]["seller"] )
		return tell(me, "���̤���ѻP�v�СC\n");

	if( price == "auto" )
	{
		if( !sizeof(auction[i]["auction_data"]) )
			price = auction[i]["base_price"];
		else if( auction[i]["top_price"] / 20 > 0 )
			price = auction[i]["top_price"] + (auction[i]["top_price"] / 20);
		else
			price = auction[i]["top_price"] + 1;
		
		if( price > auction[i]["direct_price"] )
			price = auction[i]["direct_price"];
	}
	else
		price = to_int(big_number_check(price));
 	
	if( price < 1 || price > 1000000000000 )
		return tell(me, "�п�J���T���v�л�(1-1000000000000)�C\n");	

	tell(me, "�O�_�T�w�H "HIY+money(money_unit, price)+NOR" �������v�Сu"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v(y/n)�H");
	
	input_to( (: confirm_bid, me, i, ob, price, money_unit, copy(auction[i]) :) );
}

void do_auction_receive(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	object cloneob;

	if( !arg )
		return tell(me, "�п�J���T�����O(help receive)�C\n");

	// �B���Y�Ӧa�I
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "�п�J���T���s���C\n");

		i--;

		if( !auction[i]["status"] )
			return tell(me, "�������~�٦b�v�е{�Ǥ��C\n");

		if( auction[i]["status"] == AUCTION_STATUS_FAILED && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "�y�Ъ��~�����ѩ��̻�^�C\n");

		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" ��Ƹ��J���~�C\n");

		if( auction[i]["status"] != AUCTION_STATUS_FAILED && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"�èS���o�гo�ө��~�C\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "�y��"+loc_short(loc)+"�B�èS������ؿv���C\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"�ä��O"+pnoun(2, me)+"���ؿv���C\n");
		
		delivery_master = delivery->query_master();

		switch( delivery->query_building_type() )
		{
			case "store":
				capacity = STORE_CAPACITY;
				break;
			case "warehouse":
				capacity = WAREHOUSE_CAPACITY;
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", auction[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");

		if( auction[i]["status"] != AUCTION_STATUS_FAILED )
		{
			log_file("daemon/auction", me->query_idname()+"�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v");
			msg("$ME�N�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�B���"+query("short", delivery)+loc_short(loc)+"�C\n", me, 0, 1);	
		}
		else
			msg("$ME�N�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v��^�ùB���"+query("short", delivery)+loc_short(loc)+"�C\n", me, 0, 1);
			
		delivery->query_module_file()->input_object(delivery_master, "products", auction[i]["basename"], auction[i]["amount"]);
		remove_auction(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(auction) )
			return tell(me, "�п�J���T���s���C\n");
		
		i--;

		if( !auction[i]["status"] )
			return tell(me, "�������~�٦b�v�е{�Ǥ��C\n");

		if( auction[i]["status"] == AUCTION_STATUS_FAILED && me->query_id(1) != auction[i]["seller"] )
			return tell(me, "�y�Ъ��~�����ѩ��̻�^�C\n");
			
		
		ob = load_object(auction[i]["basename"]);
		
		if( !objectp(ob) )
			error(auction[i]["basename"]+" ��Ƹ��J���~�C\n");
	
		if( auction[i]["status"] != AUCTION_STATUS_FAILED && auction[i]["top_buyer"] != me->query_id(1) )
			return tell(me, pnoun(2, me)+"�èS���o�гo�ө��~�C\n");

		cloneob = new(auction[i]["basename"]);
		
		if( !me->available_get(cloneob, auction[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"�����ʳo��h�F��C\n");
		}
			
		if( auction[i]["status"] != AUCTION_STATUS_FAILED )
		{
			log_file("daemon/auction", me->query_idname()+"�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v");
			msg("$ME�N�o�Ъ��~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v����쨭�W�C\n", me, 0, 1);
		}
		else
			msg("$ME�N�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v��^�쨭�W�C\n", me, 0, 1);
				
		if( !cloneob->no_amount() )
		{
			cloneob->set_amount(auction[i]["amount"]);
			cloneob->move(me);
		}
		else
			cloneob->move(me, auction[i]["amount"]);
		

		remove_auction(i);
	}
	else
		return tell(me, "�п�J���T�����f���O(help receive)�C\n");
}

void do_request_receive(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	object cloneob;

	if( !arg )
		return tell(me, "�п�J���T�����O(help receive)�C\n");

	// �B���Y�Ӧa�I
	if( sscanf(arg, "%d to %d %d,%d", i, num, x, y) == 4 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "�п�J���T���s���C\n");

		i--;

		if( request[i]["status"] != REQUEST_STATUS_FINISHED )
			return tell(me, "�����q��|�L�H�ѳf�C\n");

		if( request[i]["buyer"] != me->query_id(1) )
			return tell(me, "�o�i�q�椣�O"+pnoun(2, me)+"�U���C\n");

		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
			error(request[i]["basename"]+" ��Ƹ��J���~�C\n");

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "�y��"+loc_short(loc)+"�B�èS������ؿv���C\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"�ä��O"+pnoun(2, me)+"���ؿv���C\n");
		
		delivery_master = delivery->query_master();

		switch( delivery->query_building_type() )
		{
			case "store":
				capacity = STORE_CAPACITY;
				break;
			case "warehouse":
				capacity = WAREHOUSE_CAPACITY;
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k��������ӫ~�C\n");
		}
		
		if( delivery->query_module_file()->over_capacity(delivery_master, "products", request[i]["amount"], capacity) )
			return tell(me, delivery->query_room_name()+"�L�k�A�e�ǳo��h�����~�F�C\n");

		msg("$ME�N�f���u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�B���"+query("short", delivery)+loc_short(loc)+"�C\n", me, 0, 1);	
			
		delivery->query_module_file()->input_object(delivery_master, "products", request[i]["basename"], request[i]["amount"]);
		remove_request(i);
		
		delivery_master->save();
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "�п�J���T���s���C\n");
		
		i--;

		if( request[i]["status"] != REQUEST_STATUS_FINISHED )
			return tell(me, "�����q��|�L�H�ѳf�C\n");

		if( request[i]["buyer"] != me->query_id(1) )
			return tell(me, "�o�i�q�椣�O"+pnoun(2, me)+"�U���C\n");
		
		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
			error(request[i]["basename"]+" ��Ƹ��J���~�C\n");

		cloneob = new(request[i]["basename"]);
		
		if( !me->available_get(cloneob, request[i]["amount"]) )
		{
			destruct(cloneob);
			return tell(me, pnoun(2, me)+"�����ʳo��h�F��C\n");
		}
			
		msg("$ME�N�f���u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v����쨭�W�C\n", me, 0, 1);
				
		if( !cloneob->no_amount() )
		{
			cloneob->set_amount(request[i]["amount"]);
			cloneob->move(me);
		}
		else
			cloneob->move(me, request[i]["amount"]);
		
		remove_request(i);
	}
	else
		return tell(me, "�п�J���T�����f���O(help receive)�C\n");
}

void do_support(object me, string arg)
{
	int i;
	int num;
	int x;
	int y;
	object env = environment(me);
	string city = env->query_city();
	array loc;
	object delivery;
	object delivery_master;
	int capacity;
	object ob;
	string money_unit = MONEY_D->query_default_money_unit();
	mapping product_info;
	mapping products;

	if( !arg )
		return tell(me, "�п�J���T�����O(help support)�C\n");

	// �B���Y�Ӧa�I
	if( sscanf(arg, "%d from %d %d,%d", i, num, x, y) == 4 )
	{
		int myamount;
		
		if( i < 1 || i > sizeof(request) )
			return tell(me, "�п�J���T���s���C\n");

		i--;

		if( request[i]["status"] == REQUEST_STATUS_FINISHED )
			return tell(me, "�����q��w�g���סC\n");

		if( request[i]["buyer"] == me->query_id(1) )
			return tell(me, "�o�O"+pnoun(2, me)+"�Ҵ��X���q��A�Y�n���������q��аѦ� help request�C\n");
			
		ob = load_object(request[i]["basename"]);
		
		if( !objectp(ob) )
		{
			if( objectp(ob = load_object("/std/module/product/"+request[i]["basename"]+"_module")) )
				product_info = ob->query_product_info();
			else
				error("�o�Ϳ��~�C\n");
		}

		if( !CITY_D->city_exist(city, num-1) )
			return tell(me, "�o�y�����èS���� "+num+" �ìP�����C\n");
	
		loc = arrange_city_location(x-1, y-1, city, num-1);

		delivery = load_module(loc);
		
		if( !objectp(delivery) )
			return tell(me, "�y��"+loc_short(loc)+"�B�èS������ؿv���C\n");
			
		if( query("owner", delivery) != me->query_id(1) )
			return tell(me, delivery->query_room_name()+"�ä��O"+pnoun(2, me)+"���ؿv���C\n");
		
		delivery_master = delivery->query_master();

		switch( delivery->query_building_type() )
		{
			case "store":
				capacity = STORE_CAPACITY;
				break;
			case "warehouse":
				capacity = WAREHOUSE_CAPACITY;
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k���ѥ���f�~�C\n");
		}
		
		switch( query("function", delivery) )
		{
			case "front":
			case "warehouse":
				break;
			default:
				return tell(me, delivery->query_room_name()+"�S����k���ѥ���f�~�C\n");
		}

		myamount = delivery->query_module_file()->query_object_amount(delivery_master, "products", request[i]["basename"], "�q��ѳf");
		
		if( myamount < request[i]["amount"] )
			return tell(me, delivery->query_room_name()+"���u�q��ѳf�v�����̨S���o��h�������f�~�C\n");

		if( product_info )
		{
			products = delivery->query_module_file()->query_objects(delivery_master, "products", request[i]["basename"], "�q��ѳf");
			
			foreach(string basename, int count in products)
			{
				if( count < request[i]["amount"] ) continue;

				me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
		
				ob = load_object(basename);

				msg("$ME�q"+query("short", delivery)+loc_short(loc)+"���ѡu"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�A���� "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"�C\n", me, 0, 1);
				CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"����"NOR"�G"+me->query_idname()+"�ѵ� "+capitalize(request[i]["buyer"])+" �� "+(i+1)+" �s�����q��A�f���W�G�u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�C\n");

				delivery->query_module_file()->output_object(delivery_master, "products", basename, request[i]["amount"]);
				
				request[i]["status"] = REQUEST_STATUS_FINISHED;
				request[i]["supporter"] = me->query_idname();
				request[i]["basename"] = basename;

				save();
		
				delivery_master->save();	
				
				return;	
			}

			return tell(me, delivery->query_room_name()+"���u�q��ѳf�v�����̨S���o��h�������f�~�C\n");
		}
		else
		{ 
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
	
			msg("$ME�q"+query("short", delivery)+loc_short(loc)+"���ѡu"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�A���� "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"�C\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"����"NOR"�G"+me->query_idname()+"�ѵ� "+capitalize(request[i]["buyer"])+" �� "+(i+1)+" �s�����q��A�f���W�G�u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�C\n");

			delivery->query_module_file()->output_object(delivery_master, "products", request[i]["basename"], request[i]["amount"]);
			
			request[i]["status"] = REQUEST_STATUS_FINISHED;
			request[i]["supporter"] = me->query_idname();
			
			save();
	
			delivery_master->save();
		}
	}
	else if( big_number_check(arg) && sscanf(arg, "%d", i) == 1 )
	{
		if( i < 1 || i > sizeof(request) )
			return tell(me, "�п�J���T���s���C\n");
		
		i--;

		if( request[i]["status"] == REQUEST_STATUS_FINISHED )
			return tell(me, "�����q��w�g���סC\n");
	
		foreach(ob in filter_array(all_inventory(me), (: base_name($1) == $(request)[$(i)]["basename"] || $1->query_module() == $(request)[$(i)]["basename"] :)))
		{
			if( ob->is_keeping() )
			{
				tell(me, pnoun(2, me)+"����������"+ob->query_idname()+"���O�d���A�C\n");
				continue;
			}
	
			if( ob->query_amount() < request[i]["amount"] )
			{
				tell(me, ob->query_idname()+"���ƶq�����C\n");
				continue;
			}
				
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
	
			msg("$ME���ѡu"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�A���� "HIY+money(money_unit, request[i]["amount"] * request[i]["price"])+NOR"�C\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"����"NOR"�G"+me->query_idname()+"�ѵ� "+capitalize(request[i]["buyer"])+" �� "+(i+1)+" �s�����q��A�f���W�G�u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�C\n");

			request[i]["status"] = REQUEST_STATUS_FINISHED;
			request[i]["supporter"] = me->query_idname();
			request[i]["basename"] = base_name(ob);	

			destruct(ob, request[i]["amount"]);
	
			save();
			
			return;
		}
			
		return tell(me, pnoun(2, me)+"�����W�S���i�ѳf�����~�άO�ƶq�����C\n");
	}
	else
		return tell(me, "�п�J���T���ѳf���O(help support)�C\n");
}

void do_request(object me, string arg)
{
	int num, i;
	int amount;
	mixed price;
	string name;
	object ob, product_module;
	mapping request_data = allocate_mapping(0);
	string money_unit = MONEY_D->query_default_money_unit();
	mapping product_info;

	if( !arg )
		return tell(me, "�п�J���T�����O(help request)�C\n");
		
	if( sscanf(arg, "%d %s per %s", amount, name, price) != 3 )
	{
		if( sscanf(arg, "%d cancel", num) == 1 )
		{
			i = num - 1;
			
			if( i < 0 || i >= sizeof(request) )
				return tell(me, "�S���s�� "+num+" ���q��C\n");
				
			if( request[i]["buyer"] != me->query_id(1) )
				return tell(me, "�s�� "+num+" ���q��ä��O"+pnoun(2, me)+"�Ҵ��X���A�L�k�����C\n");
			
			if( request[i]["status"] != REQUEST_STATUS_GOING )
				return tell(me, "�s�� "+num+" ���q��w�g���H�ѳf�F�A�L�k�����C\n");
	
			if( objectp(ob = load_object("/std/module/product/"+request[i]["basename"]+"_module")) )
					product_info = ob->query_product_info();
			else if( !objectp(ob = load_object(request[i]["basename"])) )
					error("�o�Ϳ��~�C\n");

			tell(me, pnoun(2, me)+"�����F�s�� "+num+" ���q��C\n");

			if( product_info )
				CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"����"NOR"�G"+me->query_idname()+"�����s�� "+num+" ���u�~���~�u"+product_info["name"]+"("+capitalize(product_info["id"])+")�v�q��C\n");
			else	
				CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"����"NOR"�G"+me->query_idname()+"�����s�� "+num+" ���u"+QUANTITY_D->obj(ob, request[i]["amount"], 1)+"�v�q��C\n");
	
			me->earn_money(money_unit, request[i]["amount"] * request[i]["price"]);
			
			remove_request(i);
	
			return;
		}
	
		return tell(me, "�п�J���T�����O(help request)�C\n");
	}

	price = to_int(big_number_check(price));

	if( price < 1 || price > 9999999999999 )
		return tell(me, "����d�򥲶��b 1 �� "+9999999999999+" �����C\n");
	
	if( amount < 1 || amount > 100000 )
		return tell(me, "�ƶq�d�򥲶��b 1 �� "+100000+" �����C\n");
	
	if( !objectp(product_module = load_object("/std/module/product/"+replace_string(lower_case(name), " ", "_")+"_module.c"))  && !objectp(ob = present(name, me)) )
		return tell(me, pnoun(2, me)+"�����W�èS���u"+name+"�v�o�����~�C\n");

	if( objectp(ob) && query("flag/no_give", ob) )
		return tell(me, ob->query_idname()+"�L�k�]�w���q��C\n");
		
	if( !me->spend_money(money_unit, amount * price + 10000) )
		return tell(me, pnoun(2, me)+"���{������ "HIY+money(money_unit, amount * price + 10000)+NOR"�C\n");

	request_data["buyer"] = me->query_id(1);
	request_data["amount"] = amount;
	request_data["price"] = price;
	request_data["time"] = time();
	
	if( objectp(product_module) )
	{
		product_info = product_module->query_product_info();

		request_data["basename"] = replace_string(lower_case(name), " ", "_");
	
		msg("$ME��O "+HIY+money(money_unit, amount * price)+NOR" ���q����B�P "+HIY+money(money_unit, 10000)+NOR+" ������O���X�q��C\n", me, 0, 1);
		CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"���X"NOR"�G"+me->query_idname()+"���X�u�~���~�u"+product_info["name"]+"("+capitalize(product_info["id"])+")�v�B�ƶq "HIG+amount+NOR"�B��� "HIY+money(money_unit, price)+NOR" ���q��C\n");
	}
	else
	{
		request_data["basename"] = base_name(ob);

		msg("$ME��O "+HIY+money(money_unit, amount * price)+NOR" ���q����B�P "+HIY+money(money_unit, 10000)+NOR+" ������O���X�q��C\n", me, 0, 1);
		CHANNEL_D->channel_broadcast("auc", HIC"�q��"NOR CYN"���X"NOR"�G"+me->query_idname()+"���X�u"+ob->query_idname()+"�v�B�ƶq "HIG+amount+NOR"�B��� "HIY+money(money_unit, price)+NOR" ���q��C\n");
	}
	
	add_request(request_data);
}

void heart_beat()
{
	int size = sizeof(auction);
	object ob;
	object seller;
	string money_unit = MONEY_D->query_default_money_unit();

	for(int i=0;i<size;i++)
	{
		// �W�L 14 �ѥ���������~�Ѩt�ΧR��
		if( auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 14*24*60*60 )
		{
			ob = load_object(auction[i]["basename"]);

			if( !objectp(ob) ) continue;

			CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�R��"NOR"�G"+"�s�� "+(i+1)+" �����y�Ы~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�W�L 14 �ѵL�H��^�A���~�R���C\n");

			auction[i] = 0;
		}
		// �W�L 7 �ѥ���������~�ŧi�y��
		else if( (auction[i]["status"] == 1 || auction[i]["status"] == 3) && auction[i]["finish_time"] > 0 && time() > auction[i]["finish_time"] + 7*24*60*60 )
		{
			ob = load_object(auction[i]["basename"]);

			if( !objectp(ob) ) continue;

			auction[i]["status"] = 2;
			auction[i]["finish_time"] = time();
			
			if( objectp(seller = find_player(auction[i]["seller"])) )
				tell(seller, pnoun(2, seller)+"�����~�W�L 7 �ѵL�H����A�}����̻�^�C\n");
				
			CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�y��"NOR"�G"+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�W�L 7 �ѵL�H����A�ŧi�y�СC\n");
		}
		else if( auction[i]["minute"] && !(--auction[i]["minute"]) )
		{
			ob = load_object(auction[i]["basename"]);
			
			if( !objectp(ob) ) continue;

			if( !sizeof(auction[i]["auction_data"]) )
			{
				auction[i]["status"] = 2;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�y��"NOR"�G"+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ŧi�y�СC\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"�����~�w�g�y�СA�Цۦ��^�C\n");
					
				if( auction[i]["seller"] == "" )
					auction[i] = 0;
			}
			else
			{
				refresh_top_buyer(i);
				
				if( auction[i]["seller"] != "" )
					MONEY_D->earn_money(auction[i]["seller"], money_unit, auction[i]["top_price"]);

				auction[i]["status"] = 1;
				auction[i]["finish_time"] = time();

				CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"����"NOR"�G"+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�H "HIY+money(money_unit, auction[i]["top_price"])+NOR" ���浲�СC\n");
				
				if( objectp(seller = find_player(auction[i]["seller"])) )
					tell(seller, pnoun(2, seller)+"�����~���\���СA���� "HIY+money(money_unit, auction[i]["top_price"])+NOR"�C\n");
			}
		}
		else if( auction[i]["minute"] == 30 || auction[i]["minute"] == 15 || auction[i]["minute"] == 10 || auction[i]["minute"] == 5 || auction[i]["minute"] == 1 )
		{
			ob = load_object(auction[i]["basename"]);
			
			CHANNEL_D->channel_broadcast("auc", HIB"���"NOR BLU"�T��"NOR"�G"+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�Y�N�b "+auction[i]["minute"]+" �����ᵲ�СA�ثe�X�� "HIY+money(money_unit, auction[i]["top_price"])+NOR"�C\n");
		}
	}
	
	auction -= ({ 0 });
}

int remove()
{
	return save();
}

void fix_auction()
{

}

// �j���y��
void force_give_up_auction(int i)
{
	object ob = load_object(auction[i]["basename"]);
	CHANNEL_D->channel_broadcast("auc", HIG"���"NOR GRN"�y��"NOR"�G"+"�s�� "+(i+1)+" �������~�u"+QUANTITY_D->obj(ob, auction[i]["amount"], 1)+"�v�ŧi�y�СC\n");	
	
	auction[i]["status"] = 2;
	auction[i]["finish_time"] = time();
	auction[i]["auction_data"] = allocate(0);
}

void fix_request()
{

}

void create()
{
	if( !restore_object(DATA) )
	{
		auction = allocate(0);

		save_object(DATA);
	}
	
	if( !arrayp(request) )
		request = allocate(0);

	set_heart_beat(60);
}

string query_name()
{
	return "���t��(AUCTION_D)";
}
