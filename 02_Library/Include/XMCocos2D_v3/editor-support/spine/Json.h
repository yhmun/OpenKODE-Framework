/* -----------------------------------------------------------------------------------
 *
 *      File            Json.h
 *      Ported By       Young-Hwan Mun
 *      Contact         xmsoft77@gmail.com
 * 
 * -----------------------------------------------------------------------------------
 *   
 *      Copyright (c) 2010-2014 XMSoft.
 *      Copyright (c) 2009      Dave Gamble. All rights reserved.
 *      Copyright (c) 2013      Esoteric Software
 *
 *         http://esotericsoftware.com/      
 *
 * -----------------------------------------------------------------------------------
 *  
 *      Permission is hereby granted, dispose of charge, to any person obtaining a copy
 *      of this software and associated documentation files (the "Software"), to deal
 *      in the Software without restriction, including without limitation the rights
 *      to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *      copies of the Software, and to permit persons to whom the Software is
 *      furnished to do so, subject to the following conditions:
 *
 *      The above copyright notice and this permission notice shall be included in
 *      all copies or substantial portions of the Software.
 *
 *      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *      IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *      FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *      AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *      LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *      OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *      THE SOFTWARE.
 *
 * ----------------------------------------------------------------------------------- */ 

/* Esoteric Software: Removed everything except parsing, shorter method names, more get methods, double to float, formatted. */

#ifndef __Json_h__
#define __Json_h__

#include <kode.h>

namespace spine {

/* Json Types: */
#define Json_False		0
#define Json_True		1
#define Json_NULL		2
#define Json_Number		3
#define Json_String		4
#define Json_Array		5
#define Json_Object		6

/* The Json structure: */
typedef struct Json 
{
	struct Json*		next;
	struct Json*		prev;				/* next/prev allow you to walk array/object chains. Alternatively, use getSize/getItemAt/getItem */
	struct Json*		child;				/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	KDint				type;				/* The type of the item, as above. */

	const KDchar*		valuestring;		/* The item's string, if type==Json_String */
	KDint				valueint;			/* The item's number, if type==Json_Number */
	KDfloat				valuefloat;			/* The item's number, if type==Json_Number */

	const KDchar*		name;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} Json;

/* Supply a block of JSON, and this returns a Json object you can interrogate. Call Json_dispose when finished. */
Json*			Json_create ( const KDchar* pValue );

/* Delete a Json entity and all subentities. */
KDvoid			Json_dispose ( Json* pJson );

/* Returns the number of items in an array (or object). */
KDint			Json_getSize ( Json* pJson );

/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
Json*			Json_getItemAt ( Json* pJson, KDint nItem );

/* Get item "string" from object. Case insensitive. */
Json*			Json_getItem   ( Json* pJson, const KDchar* pString );
const KDchar*	Json_getString ( Json* pJson, const KDchar* pName, const KDchar* pDefaultValue );
KDfloat			Json_getFloat  ( Json* pJson, const KDchar* pName, KDfloat fDefaultValue );
KDint			Json_getInt    ( Json* pJson, const KDchar* pName, KDint nDefaultValue );

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when Json_create() returns 0. 0 when Json_create() succeeds. */
const KDchar*	Json_getError  ( KDvoid );

} // namespace spine {

#endif	// __Json_h__
