//
//  Log.cpp
//  dream
//
//  Created by Yermek Garifullanov on 10/28/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#include "Log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

static int isStreamFile = 0;

int LogCreate(const char *dir)
{
	char temp[1024];
    
	memset((void*)temp, 0, 1024);
    sprintf(temp, "%slog.txt", dir);
    if (freopen(temp, "a", stderr) == NULL)
    {
        perror("Faild to open the log file");
        
        return 0;
    }
    
    isStreamFile = 1;
    
    return 1;
}

void LogDestroy()
{
    if (isStreamFile)
        fclose(stderr);
}

void LogWrite(const char *fmt, ...)
{
	time_t t;
	struct tm* st;
	va_list list;
    
	time(&t);
	st = localtime(&t);
    
	fprintf(stderr, "%02d:%02d:%02d: ", st->tm_hour, st->tm_min, st->tm_sec);
	va_start(list, fmt);
	vfprintf(stderr, fmt, list);
	va_end(list);
    
	fprintf(stderr, "\n");
}
