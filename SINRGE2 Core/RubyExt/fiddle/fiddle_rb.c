#include <ruby.h>
#include <windows.h>

void
Init_fiddle_rb()
{
	int status = -1;
	int i = rb_eval_string_protect("File.open('CONOUT$', 'wb'){|io|io.puts 123};class SHIT; 3; end", &status);
	char d[100];
	sprintf(d, "%d", i);
	MessageBoxA(0, d, "TEST 2", 0);
}
