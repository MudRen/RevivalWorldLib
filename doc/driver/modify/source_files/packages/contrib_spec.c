#include "spec.h"

#ifndef NO_SHADOWS
int remove_shadow(object);
#endif
#ifndef NO_ADD_ACTION
mixed query_notify_fail();
object *named_livings();
#endif
#if 0
void set_prompt(string, void|object);
#endif
mixed copy(mixed);
string *functions(object, int default: 0);
string *variables(object, int default: 0);
object *heart_beats();
#ifdef COMPAT_32
object *heart_beat_info heart_beats();
#endif
string terminal_colour(string, mapping, void | int, void | int);
string pluralize(string);
int file_length(string);
string upper_case(string);
int replaceable(object, void | string *);
mapping program_info(void | object);
void remove_input_to(void | object);
void store_variable(string, mixed , void | object);
mixed fetch_variable(string,void | object);
object find_player(string);
string remove_fringe_blanks(string);
string base_name(object);
int remove_interactive(object);
int query_ip_port(void | object);
#if 0
string zonetime(string, int);
int is_daylight_savings_time(string, int);
#endif
void debug_message(string);
object function_owner(function);
string repeat_string(string, int);
mapping memory_summary();
string query_replaced_program(void|object);
mapping network_stats();
