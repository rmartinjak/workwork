#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <wordexp.h>

/* the following macros need to be defined, e.g. via the Makefile:
 *
 * program names (string literals):
 *   PINGCMD
 *   STARTCMD
 *   STOPCMD
 * states (character literals):
 *   WORKING
 *   NOTWORKING
 */


#ifndef TIMEOUT
#define TIMEOUT 600
#endif

#ifndef STATEFILE
#define STATEFILE "~/.workwork"
#endif

#define FMT "%ld %ld %ld %c\n"

FILE *open_file(const char *path, const char *mode)
{
    FILE *f;
    wordexp_t exp;
    if (wordexp(path, &exp, 0))
        return NULL;
    f = fopen(exp.we_wordv[0], mode);
    wordfree(&exp);
    return f;
}

int load(const char *path, long *total, long *start, long *last, char *state)
{
    int res = 0;
    FILE *stream = open_file(path, "r");

    if (!stream)
    {
        *total = *start = *last = 0;
        *state = '-';
        return 0;
    }

    if (fscanf(stream, FMT, total, start, last, state) != 4)
        res = -1;
    fclose(stream);
    return res;
}

int save(const char *path, long total, long start, long last, char state)
{
    FILE *stream = open_file(path, "w");

    if (!stream)
        return -1;

    fprintf(stream, FMT, total, start, last, state);
    return 0;
}

int progname_equals(const char *progname, const char *s)
{
    const char *p = strrchr(progname, '/');

    if (p)
        p += 1;
    else
        p = progname;

    return !strcmp(p, s);
}

int main(int argc, char **argv)
{
    long total, start, last, now;
    char state;
    char *statefile = STATEFILE;

    if (argc >= 2)
        statefile = argv[1];

    if (load(statefile, &total, &start, &last, &state))
    {
        fprintf(stderr, "error loading %s\n", statefile);
        return EXIT_FAILURE;
    }
    now = time(NULL);

    if (state == WORKING)
    {
        if (progname_equals(argv[0], STOPCMD) || (progname_equals(argv[0], PINGCMD) && now > last + TIMEOUT))
        {
            state = NOTWORKING;
            total += last - start;
        }
        if (progname_equals(argv[0], PINGCMD))
        {
            last = now;
        }
    }
    else if (state == NOTWORKING)
    {
        if (progname_equals(argv[0], STARTCMD))
        {
            state = WORKING;
            start = last = now;
        }
    }

    if (save(statefile, total, start, last, state))
    {
        fprintf(stderr, "error saving %s\n", statefile);
        return EXIT_FAILURE;
    }

    if (progname_equals(argv[0], PINGCMD) && argc <= 2)
        putchar(state);
    else
        printf("%.2f\n", total / 3600.0);
    return EXIT_SUCCESS;
}
