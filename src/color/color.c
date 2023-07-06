#include "color.h"



/**
 * @brief Writes a colored string to a file stream.
 *
 * This function formats a string using the specified arguments and writes it to
 * the specified file stream. The string can be colored by specifying a color
 * using the `color` parameter.
 *
 * @param flux   The file stream to write the string to.
 * @param color  The color to apply to the string (in the form of an ANSI escape sequence).
 * @param format The formatting string specifying how the arguments will be
 *               inserted into the string.
 * @param ...    The arguments to be inserted into the formatting string.
 */
void fprintfc(FILE * flux, char color[], char * format, ...)
{
    char buffer[2048];
    va_list args;
    if (flux == NULL) {flux = stdout;}
    va_start(args, format);
    vsprintf(buffer, format, args);
    fprintf(flux, "%s%s%s", color, buffer, ANSI_RESET);
}
