/*
  [A      Move cursor up one row, stop if a top of screen
  [B      Move cursor down one row, stop if at bottom of screen
  [C      Move cursor forward one column, stop if at right edge of screen
  [D      Move cursor backward one column, stop if at left edge of screen
  [H      Home to row 1 column 1 (also [1;1H)
  [J      Clear from current position to bottom of screen
  [K      Clear from current position to end of line
  [24;80H Position to line 24 column 80 (any line 1 to 24, any column 1 to 132)
  [0m     Clear attributes to normal characters
  [7m     Add the inverse video attribute to succeeding characters
  [0;7m   Set character attributes to inverse video only
*/

#include <daemon.h>
#include <feature.h>
#include <ansi.h>

//#define ESC	"\e"

#define CURSOR_UP		ESC"[A"
#define CURSOR_DOWN		ESC"[B"
#define CURSOR_RIGHT	ESC"[C"
#define CURSOR_LEFT		ESC"[D"
#define CHR_DELETE		ESC"[3~"

#define BACKSPACE		ESC"U"
#define ERASE_LINE		ESC"[1K" 
#define CHA				ESC"[0G"
//#define CLR		ESC"[2J"	// [J      Clear from current position to bottom of screen

inherit COMMAND;

string help = @HELP
        標準 realgo 指令。
HELP;
void command_shell(string arg, object me);

nosave string msg_buffer = "";
nosave string command = "";
nosave string *commands = ({
    "apple",
    "banana",
    "cake",
    "disk",
    "elephant",
    "fish",
    "gun",
    "hello",
});
nosave int index = 0, h_index = 0;

string *get_commands()
{
        string *res = allocate(0);
        mixed cmds = COMMAND_D->query_commands();
        foreach(string type, string *type_cmds in cmds)
        {
                if( arrayp(type_cmds) ) res = res | type_cmds;
        }
        return res;
}
void command(object me, string arg)
{
        //int h = query_temp("windowsize/height", me);
        //int w = query_temp("windowsize/width", me);
        msg_buffer = "";
        command = "";
        commands = get_commands();
        index=0; h_index=0;
        get_commands();
        tell(me, "進入 Command Shell (離開 CTRL+C, 送出 CTRL+D, TAB 自動完成指令)\n");
        tell(me, ERASE_LINE + CHA);

        //print_command(me, "", h, w);
        get_char((: command_shell :), 1, me, 0);
}
void command_shell(string arg, object me)
{
        /*
        tell(me, "Arg Code: ");
        for(int i=0;i<strlen(arg); i++)
                tell(me, sprintf("%d, ", arg[i]));
        tell(me, "\n");
        */
        if( strlen(msg_buffer) == 0 )
        {
                if( arg[0] == 27 )
                {
                        msg_buffer += " ";
                        msg_buffer[<1] = arg[0];
                }
        }
        else
        {
                msg_buffer += " ";
                msg_buffer[<1] = arg[0];
        }

        // Special chars from ascii 1 ~ 31
        if( arg[0] == 3 )// CTRL-C
        {
                tell(me, "\n離開 Command Shell。\n");
                return; 
        }
        if( arg[0] == 4 )// CTRL-D
        {
                tell(me, "\n\n離開 Command Shell。\n");
                tell(me, sprintf("送出指令: %s\n", command));

                if( !me->force_me(command) )
                        tell(me, "執行失敗\n");
                return; 
        }
        if( arg[0] == 9 ) // TAB
        {
                // Auto-complete command

                string *res;
                res = regexp(commands, "^" + command);
                if( sizeof(res) )
                {
                        command = res[0];
                        h_index = strlen(command);
                        tell(me, ERASE_LINE + CHA);
                        tell(me, command);
                }
        }
        else
                // Control chars start with ESC + [
        if( strlen(msg_buffer) > 2 && strlen(msg_buffer) < 5 && msg_buffer[1] == '[' )
        {
                if( msg_buffer == CURSOR_UP )
                {
                        index--;
                        if( index < 0 ) index = sizeof(commands) + index;
                        command = commands[index];
                        h_index = strlen(command);
                        tell(me, ERASE_LINE + CHA);
                        tell(me, command);
                        msg_buffer = "";
                }
                else if( msg_buffer == CURSOR_DOWN )
                {
                        index++;
                        if( index >= sizeof(commands) ) index = 0;
                        command = commands[index];
                        h_index = strlen(command);
                        tell(me, ERASE_LINE + CHA);
                        tell(me, command);
                        msg_buffer = "";
                }
                else if( msg_buffer == CURSOR_LEFT )
                {
                        h_index--;
                        if( h_index < 0 ) h_index = 0;
                        else tell(me, CURSOR_LEFT);
                        msg_buffer = "";
                }
                else if( msg_buffer == CURSOR_RIGHT )
                {
                        h_index++;
                        if( h_index > strlen(command) ) h_index = strlen(command);
                        else tell(me, CURSOR_RIGHT);
                        msg_buffer = "";
                }
                else if( msg_buffer == CHR_DELETE )
                {
                        command = command[0..h_index-1] + command[h_index+1..];
                        tell(me, ESC"[1P");
                        msg_buffer = "";
                }
                else
                {
                        // do nothing...
                        /*
                        tell(me, "Unknown msg buffer code: ");
                        for(int i=0;i<strlen(msg_buffer); i++)
                                tell(me, sprintf("%d, ", msg_buffer[i]));
                        tell(me, "\n");
                        */
                }
        }
        // Normal input chars
        else if( strlen(msg_buffer) == 0 )
        {
                if( arg[0] == 8 ) // BACKSPACE
                {
                        h_index--;
                        if( h_index < 0 ) h_index = 0;
                        command = command[0..h_index-1] + command[h_index..];
                        tell(me, BACKSPACE);
                }
                else
                {
                        // Do insert commands
                        tell(me, sprintf(ESC"[@%c", arg[0])); 
                        command = command[0..h_index-1] + sprintf("%c", arg[0]) + command[h_index..];
                        h_index++;
                }
        }
        // redraw
        tell(me, ERASE_LINE + CHA);
        tell(me, command);
        tell(me, ESC"["+(h_index+1)+"G");
        get_char((: command_shell :), 1, me);
}
