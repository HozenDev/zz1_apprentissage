#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "log.h"

int no_log = 1;
int no_debug_log = 0;

/**
 * @brief Enable logging based on command line arguments.
 *
 * This function enables logging based on the command line arguments passed to the program. If the argument count
 * `argc` is less than 2, a message is printed to the standard output indicating how to enable logs. If the argument
 * `argv[1]` starts with '-l', all logs are enabled. If the argument `argv[1]` starts with '-d', all logs except debug
 * logs are enabled. The function sets the `no_log` and `no_debug_log` variables accordingly and prints a log message
 * indicating the enabled logging level.
 *
 * @param argc  The number of command line arguments.
 * @param argv  An array of command line argument strings.
 */
void enable_log(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stdout,
                "To enable all logs use : -l argument\n"
                "To enable all logs without debug use : -d argument");
    }
    else {
	if (argv[1][0] == '-' && argv[1][1] == 'l') {
	    no_log = 0;
	    zlog(stdout, INFO, "All logs enable", NULL);
	}
        if (argv[1][0] == '-' && argv[1][1] == 'd') {
            no_log = 0;
	    no_debug_log = 1;
	    zlog(stdout, INFO, "All logs without debug enable", NULL);
	}
    }
}


/**
 * @brief Perform a test of logging functionality.
 *
 * This function performs a test of the logging functionality by generating random log messages of different levels.
 * It uses the `zlog` function to print log messages to the standard output. The function generates a random number
 * between 1 and 4 to determine the log level. Based on the log level, different log messages are printed. The
 * messages include informative, warning, error, and debug logs. The function uses the `sleep` function to introduce
 * a delay of 1 second between log messages.
 */
void test_log(void)
{
    int random_log;
    int n = 15;

    srand(time(NULL));
    while (n--) {
	random_log = rand()%4+1;
	switch(random_log) {
	case 1:
	    zlog(stdout, INFO, "salut", "oui");
	    zlog(stdout, INFO, "%s", "Informations utiles");
	    break;
	case 2:
	    zlog(stdout, WARNING,
		 "Attention : '%s'", "Faites attention");
	    break;
	case 3:
	    zlog(stdout, ERROR, "Fatal Error : '%s'", "Test");
	    break;
	case 4:
	    zlog(stdout, DEBUG, "n=%d", n);
	    break;
	}
	sleep(1);
    }
}


/**
 * @brief Generate the start message for a log.
 *
 * This function generates the start message for a log based on the provided log level, file name, and line number.
 * It allocates memory for the message buffer and returns a pointer to the generated message. The log level determines
 * the title and color of the message. The file name and line number are formatted along with the title and color
 * information. The generated message includes the file name, line number, color, and title, all formatted for display.
 *
 * @param log_level     The log level (INFO, ERROR, DEBUG, or WARNING).
 * @param file_name     The name of the source file where the log is generated.
 * @param line_number   The line number in the source file where the log is generated.
 * @return              A pointer to the generated start message.
 */
char* get_start_msg_log(int log_level,
			char* file_name,
			int line_number)
{
    /* char buffer[2048]; */
    char* buffer = (char*) malloc(sizeof(char)*200);
    char title[8] = "UNKNOWN";
    char color[11] = ANSI_WHITE;
    
    switch(log_level) {
    case INFO:
	strncpy(title, " INFO  ", 8);
	strncpy(color, ANSI_WHITE, 11);
	break;
    case ERROR:
	strncpy(title, " ERROR ", 8);
	strncpy(color, ANSI_RED, 11);
	break;
    case DEBUG:
	strncpy(title, " DEBUG ", 8);
	strncpy(color, ANSI_WHITE, 11);
	break;
    case WARNING:
	strncpy(title, "WARNING", 8);
	strncpy(color, ANSI_YELLOW, 11);
	break;
    }
    sprintf(buffer, "%-20s:%5d   %s[%s]   \033[0m",
	    file_name,
	    line_number,
	    color,
	    title);
    return buffer;
}


/**
 * @brief Print a log message to the specified file stream.
 *
 * This function prints a log message to the specified file stream based on the provided log level, file name, format,
 * and additional variable arguments. If the file stream `flux` is NULL, the standard output stream is used. The function
 * checks the logging configuration variables `no_log` and `no_debug_log` to determine whether the log message should be
 * printed. If `no_log` is false and either `no_debug_log` is true or the log level is not DEBUG, the log message is printed.
 * The log message is formatted using the provided `format` string and additional variable arguments using `vsprintf`. The
 * start message for the log is obtained using the `get_start_msg_log` function, and the log message is printed with the
 * start message to the file stream. The file stream is then flushed, and the start message memory is freed.
 *
 * @param flux          The file stream to print the log message to (or NULL for standard output).
 * @param log_level     The log level (INFO, ERROR, DEBUG, or WARNING).
 * @param file_name     The name of the source file where the log message is generated.
 * @param line_number   The line number in the source file where the log message is generated.
 * @param format        The format string for the log message.
 * @param ...           Additional variable arguments for formatting the log message.
 */
void print_log(FILE* flux,
	       unsigned int log_level,
	       char* file_name,
	       int line_number,
	       char* format,
	        ...)
{
    char buffer[2048];
    va_list args;
    char* buffer_start;

    if (flux == NULL) {flux = stdout;}
    if (!no_log && (!no_debug_log || log_level != DEBUG)) {
	if (log_level == INFO
	    || log_level == ERROR
	    || log_level == DEBUG
	    || log_level == WARNING) {

	    va_start(args, format);
	    vsprintf(buffer, format, args);
            buffer_start = get_start_msg_log(log_level,
                                             file_name,
                                             line_number);
            fprintf(flux, "%s%s\n", buffer_start, buffer);
            fflush(flux);
            free(buffer_start);
	}
    }
}
