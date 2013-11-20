/*
 * Copyright (C) 2006 Christian Stigen Larsen, http://csl.sublevel3.org
 * Distributed under the GNU General Public License (GPL) v2.
 *
 * Project homepage on http://jp2a.sf.net
 *
 * $Id: jp2a.c 465 2006-10-02 11:34:48Z csl $
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#undef HAVE_STDLIB_H
#endif

#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "jp2a.h"
#include "options.h"
#include "image.h"

#ifdef WIN32
#ifdef FEAT_CURL
#include <io.h>
#define close _close
#endif
#include <fcntl.h>
#endif

#include "handle_input.h"
#include <curses.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    setupCurses();
    //drawLine("\t");

    int store_width, store_height, store_autow, store_autoh;
    FILE *fout = stdout;
#ifdef FEAT_CURL
    FILE *fr;
    int fd;
#endif
    FILE *fp;
    int n;

    parse_options(argc, argv);

    store_width = width;
    store_height = height;
    store_autow = auto_width;
    store_autoh = auto_height;

    if ( strcmp(fileout, "-") ) {
        if ( (fout = fopen(fileout, "wb")) == NULL ) {
            fprintf(stderr, "Could not open '%s' for writing.\n", fileout);
            return 1;
        }
    }

    for ( n=1; n<argc; ++n ) {

        width = store_width;
        height = store_height;
        auto_width = store_autow;
        auto_height = store_autoh;

        // skip options
        if ( argv[n][0]=='-' && argv[n][1] )
            continue;

        // read from stdin
        if ( argv[n][0]=='-' && !argv[n][1] ) {

            #ifdef _WIN32
            _setmode(_fileno(stdin), _O_BINARY);
            #endif

            image_t *i = image_read(stdin);
            image_t *s = image_new(width, height);
            image_clear(s);
            image_resize(i, s);
            image_print(s, fout);

            image_destroy(i);
            image_destroy(s);
            continue;
        }

        #ifdef FEAT_CURL
        if ( is_url(argv[n]) ) {

            if ( verbose )
                fprintf(stderr, "URL: %s\n", argv[n]);

            fd = curl_download(argv[n], debug);

            if ( (fr = fdopen(fd, "rb")) == NULL ) {
                fputs("Could not fdopen read pipe\n", stderr);
                return 1;
            }

            image_t *i = image_read(fr);
            fclose(fr);
            close(fd);
            image_t *s = image_new(width, height);
            image_clear(s);
            image_resize(i, s);
            image_print(s, fout);

            image_destroy(i);
            image_destroy(s);
            continue;
        }
        #endif

        // read files
        if ( (fp = fopen(argv[n], "rb")) != NULL ) {
            if ( verbose )
                fprintf(stderr, "File: %s\n", argv[n]);

            image_t *i = image_read(fp);
            fclose(fp);
            image_t *s = image_new(width, height);
            image_clear(s);
            image_resize(i, s);
            image_print(s, fout);

            image_destroy(i);
            image_destroy(s);
            continue;

        } else {
            fprintf(stderr, "Can't open %s\n", argv[n]);
            return 1;
        }
    }

    if ( fout != stdout )
        fclose(fout);

    for (;;)
        getch();
    killCurses();
    return 0;
}
