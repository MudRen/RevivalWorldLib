string replace_usa_ansi( string arg )
{
    string msg = terminal_colour(arg, ([
    "RED" : "\e[31m",
    "BLUE" : "\e[34m",
    "ORANGE" : "\e[33m",
    "YELLOW" : "\e[1m \e[33m",
    "GREEN" : "\e[32m",
    "BLACK" : "\e[30m",
    "WHITE" : "\e[37m",
    "CYAN"  : "\e[36m",
    "MAGENTA" : "\e[35m",
    "B_RED" : "\e[41m",
    "B_BLUE" : "\e[44m",
    "B_YELLOW" : "\e[1m \e[43m",
    "B_MAGENTA" : "\e[45m",
    "B_BLACK" : "\e[40m",
    "B_WHITE" : "\e[47m",
    "B_CYAN" : "\e[46m",
    "B_GREEN" : "\e42m",
    "B_ORANGE" : "\e43m",
    "BOLD" : "\e[1m",
    "INITTERM" : "\e[H\e[2J",
    "RESET" : "\e[0m",
    "FLASH" : "\e[5m",
    "CLEARLINE" : "\e[L\e[G",
    "HOME" : "\e[H",
    "WINDOW" : "",
    "ENDTERM" : "",
    "STATUS" : "",
    "REVERSE" : "",
    "UNDERLINE" : "",   
    ]));
    
    return msg;
}