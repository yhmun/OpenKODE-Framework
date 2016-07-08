/***********************************************************************
 * Software License Agreement (BSD License)
 *
 * Copyright 2008-2009  Marius Muja (mariusm@cs.ubc.ca). All rights reserved.
 * Copyright 2008-2009  David G. Lowe (lowe@cs.ubc.ca). All rights reserved.
 *
 * THE BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *************************************************************************/

#ifndef OPENCV_FLANN_LOGGER_H
#define OPENCV_FLANN_LOGGER_H

#include <kode.h>

#include "defines.h"


namespace cvflann
{

class Logger
{
    Logger ( ) : stream ( KD_NULL ), logLevel ( FLANN_LOG_WARN ) 
	{
		_setDestination ( KD_NULL );
	}

    ~Logger ( )
    {
        if ( stream != KD_NULL )
		{
            kdFclose ( stream );
        }
    }

    static Logger& instance()
    {
        static Logger logger;
        return logger;
    }

    void _setDestination(const char* name)
    {
		if ( stream )
		{
			kdFclose ( stream );
			stream = KD_NULL;
		}
        
		if ( name )
		{
			stream = kdFopen ( name, "w" );
		}

        if ( !stream )
		{
			stream = kdFopen ( "/data/CV_FLANN.log", "w" );
		}
    }

    int _log ( int level, const char* fmt, KDVaListKHR arglist )
    {
        if (level > logLevel ) return -1;
        int ret = kdVfprintfKHR ( stream, fmt, arglist );
        return ret;
    }

public:
    /**
     * Sets the logging level. All messages with lower priority will be ignored.
     * @param level Logging level
     */
    static void setLevel(int level) { instance().logLevel = level; }

    /**
     * Sets the logging destination
     * @param name Filename or NULL for console
     */
    static void setDestination(const char* name) { instance()._setDestination(name); }

    /**
     * Print log message
     * @param level Log level
     * @param fmt Message format
     * @return
     */
    static int log(int level, const char* fmt, ...)
    {
        KDVaListKHR arglist;
        KD_VA_START_KHR ( arglist, fmt );
        int ret = instance()._log(level,fmt,arglist);
        KD_VA_END_KHR ( arglist );
        return ret;
    }

#define LOG_METHOD(NAME,LEVEL) \
    static int NAME(const char* fmt, ...) \
    { \
        KDVaListKHR ap; \
        KD_VA_START_KHR ( ap, fmt ); \
        int ret = instance()._log(LEVEL, fmt, ap); \
        KD_VA_END_KHR ( ap ); \
        return ret; \
    }

    LOG_METHOD(fatal, FLANN_LOG_FATAL)
    LOG_METHOD(error, FLANN_LOG_ERROR)
    LOG_METHOD(warn, FLANN_LOG_WARN)
    LOG_METHOD(info, FLANN_LOG_INFO)

private:
    KDFile* stream;
    int		logLevel;
};

}

#endif //OPENCV_FLANN_LOGGER_H
