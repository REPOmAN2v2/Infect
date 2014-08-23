#include "log.h"
#include "display.h"
#include <stdarg.h>

#define LOG "../infect.log"
#define MAX 500

/* logMsg() logs a message to a log file.
 *
 * It takes in a variable length argument list to generate the proper string.
 * This lets you easily log strings taking arguments, the same way you would
 * use printf().
 *
 * Usage: logMsg("%d infected left", units->infected);
 */

void logMsg(const char* format, ...)
{
	FILE *file = NULL;
	file = fopen(LOG, "a");
	
	if (file == NULL) {
		printError("Could not open the log file\n");
	} else {
		char buffer[MAX];

		va_list args;
		va_start(args, format);

		vsprintf(buffer, format, args);

		fputs(buffer, file);
		fclose(file);

		va_end(args);
	}

	fclose(file);
	return;
}

/*void closeLog()
{
	fclose(file);
}*/