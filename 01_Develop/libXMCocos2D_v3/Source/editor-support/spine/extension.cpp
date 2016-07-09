/* -----------------------------------------------------------------------------------
 *
 *      File            extension.cpp
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2013      Esoteric Software. All rights reserved.
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 * 
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions are met:
 * 
 *     1. Redistributions of source code must retain the above copyright notice, this
 *        list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice,
 *        this list of conditions and the following disclaimer in the documentation
 *        and/or other materials provided with the distribution.
 * 
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *     ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ----------------------------------------------------------------------------------- */ 

#include "editor-support/spine/extension.h"

namespace spine {

static KDvoid* (*mallocFunc) (KDsize size) = kdMalloc;
static KDvoid  (*freeFunc) (KDvoid* ptr) = kdFree;

KDvoid _setMalloc ( KDvoid* (*malloc)  (KDsize size ) ) 
{
	mallocFunc = malloc;
}

KDvoid _setFree ( KDvoid (*free) (KDvoid* ptr) )
{
	freeFunc = free;
}

char* _readFile (const char* path, int* length) {
	char *data;
	KDFile*	file = kdFopen(path, "rb");
    int readBytes = 0;
	if (!file) return 0;

	kdFseek(file, 0, KD_SEEK_END);
	*length = kdFtell(file);
	kdFseek(file, 0, KD_SEEK_SET);

	data = MALLOC(char, *length);
	readBytes = kdFread(data, 1, *length, file);
	kdFclose(file);
    if (readBytes != *length)
    {
        FREE(data);
        data = NULL;
    }
	return data;
}

} // namespace spine {
