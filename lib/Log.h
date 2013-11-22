//
//  Log.h
//
//
//  Created by Yermek Garifullanov on 10/28/13.
//  Copyright (c) 2013 mekchi. All rights reserved.
//

#ifndef _MEKCHI_LOG_
#define _MEKCHI_LOG_

#define __LOG(x)            LogWrite(x)
#define __LOGARG(x, ...)    LogWrite(x, __VA_ARGS__)

int LogCreate(const char *dir);
void LogDestroy();
void LogWrite(const char *fmt, ...);

#endif
