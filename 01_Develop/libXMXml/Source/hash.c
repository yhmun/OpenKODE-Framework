/*
 * hash.c: chained hash tables
 *
 * Reference: Your favorite introductory book on algorithms
 *
 * Copyright (C) 2000 Bjorn Reese and Daniel Veillard.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 * Author: breese@users.sourceforge.net
 */

#define IN_LIBXML
#include "libxml.h"

#include <XMXML/parser.h>
#include <XMXML/hash.h>
#include <XMXML/xmlmemory.h>
#include <XMXML/xmlerror.h>
#include <XMXML/globals.h>

#define MAX_HASH_LEN 8

/* #define DEBUG_GROW */

/*
 * A single entry in the hash table
 */
typedef struct _xmlHashEntry xmlHashEntry;
typedef xmlHashEntry *xmlHashEntryPtr;
struct _xmlHashEntry {
    struct _xmlHashEntry *next;
    xmlChar *name;
    xmlChar *name2;
    xmlChar *name3;
    void *payload;
    int valid;
};

/*
 * The entire hash table
 */
struct _xmlHashTable {
    struct _xmlHashEntry *table;
    int size;
    int nbElems;
    xmlDictPtr dict;
};

/*
 * xmlHashComputeKey:
 * Calculate the hash key
 */
static unsigned long
xmlHashComputeKey(xmlHashTablePtr table, const xmlChar *name,
	          const xmlChar *name2, const xmlChar *name3) {
    unsigned long value = 0L;
    char ch;
    
    if (name != KD_NULL) {
	value += 30 * (*name);
	while ((ch = *name++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (name2 != KD_NULL) {
	while ((ch = *name2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (name3 != KD_NULL) {
	while ((ch = *name3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    return (value % table->size);
}

static unsigned long
xmlHashComputeQKey(xmlHashTablePtr table,
		   const xmlChar *prefix, const xmlChar *name,
		   const xmlChar *prefix2, const xmlChar *name2,
		   const xmlChar *prefix3, const xmlChar *name3) {
    unsigned long value = 0L;
    char ch;
    
    if (prefix != KD_NULL)
	value += 30 * (*prefix);
    else
	value += 30 * (*name);

    if (prefix != KD_NULL) {
	while ((ch = *prefix++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name != KD_NULL) {
	while ((ch = *name++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (prefix2 != KD_NULL) {
	while ((ch = *prefix2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name2 != KD_NULL) {
	while ((ch = *name2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (prefix3 != KD_NULL) {
	while ((ch = *prefix3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name3 != KD_NULL) {
	while ((ch = *name3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    return (value % table->size);
}

/**
 * xmlHashCreate:
 * @size: the size of the hash table
 *
 * Create a new xmlHashTablePtr.
 *
 * Returns the newly created object, or KD_NULL if an error occured.
 */
xmlHashTablePtr
xmlHashCreate(int size) {
    xmlHashTablePtr table;
  
    if (size <= 0)
        size = 256;
  
    table = (xmlHashTable*) xmlMalloc(sizeof(xmlHashTable));
    if (table) {
        table->dict = KD_NULL;
        table->size = size;
	table->nbElems = 0;
        table->table = (xmlHashEntryPtr) xmlMalloc(size * sizeof(xmlHashEntry));
        if (table->table) {
  	    kdMemset(table->table, 0, size * sizeof(xmlHashEntry));
  	    return(table);
        }
        xmlFree(table);
    }
    return(KD_NULL);
}

/**
 * xmlHashCreateDict:
 * @size: the size of the hash table
 * @dict: a dictionary to use for the hash
 *
 * Create a new xmlHashTablePtr which will use @dict as the internal dictionary
 *
 * Returns the newly created object, or KD_NULL if an error occured.
 */
xmlHashTablePtr
xmlHashCreateDict(int size, xmlDictPtr dict) {
    xmlHashTablePtr table;

    table = xmlHashCreate(size);
    if (table != KD_NULL) {
        table->dict = dict;
	xmlDictReference(dict);
    }
    return(table);
}

/**
 * xmlHashGrow:
 * @table: the hash table
 * @size: the new size of the hash table
 *
 * resize the hash table
 *
 * Returns 0 in case of success, -1 in case of failure
 */
static int
xmlHashGrow(xmlHashTablePtr table, int size) {
    unsigned long key;
    int oldsize, i;
    xmlHashEntryPtr iter, next;
    struct _xmlHashEntry *oldtable;
#ifdef DEBUG_GROW
    unsigned long nbElem = 0;
#endif
  
    if (table == KD_NULL)
	return(-1);
    if (size < 8)
        return(-1);
    if (size > 8 * 2048)
	return(-1);

    oldsize = table->size;
    oldtable = table->table;
    if (oldtable == KD_NULL)
        return(-1);
  
    table->table = (xmlHashEntryPtr) xmlMalloc(size * sizeof(xmlHashEntry));
    if (table->table == KD_NULL) {
	table->table = oldtable;
	return(-1);
    }
    kdMemset(table->table, 0, size * sizeof(xmlHashEntry));
    table->size = size;

    /*	If the two loops are merged, there would be situations where
	a new entry needs to allocated and data copied into it from 
	the main table. So instead, we run through the array twice, first
	copying all the elements in the main array (where we can't get
	conflicts) and then the rest, so we only free (and don't allocate)
    */
    for (i = 0; i < oldsize; i++) {
	if (oldtable[i].valid == 0) 
	    continue;
	key = xmlHashComputeKey(table, oldtable[i].name, oldtable[i].name2,
				oldtable[i].name3);
	kdMemcpy(&(table->table[key]), &(oldtable[i]), sizeof(xmlHashEntry));
	table->table[key].next = KD_NULL;
    }

    for (i = 0; i < oldsize; i++) {
	iter = oldtable[i].next;
	while (iter) {
	    next = iter->next;

	    /*
	     * put back the entry in the new table
	     */

	    key = xmlHashComputeKey(table, iter->name, iter->name2,
		                    iter->name3);
	    if (table->table[key].valid == 0) {
		kdMemcpy(&(table->table[key]), iter, sizeof(xmlHashEntry));
		table->table[key].next = KD_NULL;
		xmlFree(iter);
	    } else {
	    	iter->next = table->table[key].next;
	    	table->table[key].next = iter;
	    }

#ifdef DEBUG_GROW
	    nbElem++;
#endif

	    iter = next;
	}
    }

    xmlFree(oldtable);

#ifdef DEBUG_GROW
    xmlGenericError(xmlGenericErrorContext,
	    "xmlHashGrow : from %d to %d, %d elems\n", oldsize, size, nbElem);
#endif

    return(0);
}

/**
 * xmlHashFree:
 * @table: the hash table
 * @f:  the deallocator function for items in the hash
 *
 * Free the hash @table and its contents. The userdata is
 * deallocated with @f if provided.
 */
void
xmlHashFree(xmlHashTablePtr table, xmlHashDeallocator f) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;
    int inside_table = 0;
    int nbElems;

    if (table == KD_NULL)
	return;
    if (table->table) {
	nbElems = table->nbElems;
	for(i = 0; (i < table->size) && (nbElems > 0); i++) {
	    iter = &(table->table[i]);
	    if (iter->valid == 0)
		continue;
	    inside_table = 1;
	    while (iter) {
		next = iter->next;
		if ((f != KD_NULL) && (iter->payload != KD_NULL))
		    f(iter->payload, iter->name);
		if (table->dict == KD_NULL) {
		    if (iter->name)
			xmlFree(iter->name);
		    if (iter->name2)
			xmlFree(iter->name2);
		    if (iter->name3)
			xmlFree(iter->name3);
		}
		iter->payload = KD_NULL;
		if (!inside_table)
		    xmlFree(iter);
		nbElems--;
		inside_table = 0;
		iter = next;
	    }
	}
	xmlFree(table->table);
    }
    if (table->dict)
        xmlDictFree(table->dict);
    xmlFree(table);
}

/**
 * xmlHashAddEntry:
 * @table: the hash table
 * @name: the name of the userdata
 * @userdata: a pointer to the userdata
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the @name. Duplicate names generate errors.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashAddEntry(xmlHashTablePtr table, const xmlChar *name, void *userdata) {
    return(xmlHashAddEntry3(table, name, KD_NULL, KD_NULL, userdata));
}

/**
 * xmlHashAddEntry2:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @userdata: a pointer to the userdata
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the (@name, @name2) tuple. Duplicate tuples generate errors.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashAddEntry2(xmlHashTablePtr table, const xmlChar *name,
	        const xmlChar *name2, void *userdata) {
    return(xmlHashAddEntry3(table, name, name2, KD_NULL, userdata));
}

/**
 * xmlHashUpdateEntry:
 * @table: the hash table
 * @name: the name of the userdata
 * @userdata: a pointer to the userdata
 * @f: the deallocator function for replaced item (if any)
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the @name. Existing entry for this @name will be removed
 * and freed with @f if found.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashUpdateEntry(xmlHashTablePtr table, const xmlChar *name,
	           void *userdata, xmlHashDeallocator f) {
    return(xmlHashUpdateEntry3(table, name, KD_NULL, KD_NULL, userdata, f));
}

/**
 * xmlHashUpdateEntry2:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @userdata: a pointer to the userdata
 * @f: the deallocator function for replaced item (if any)
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the (@name, @name2) tuple. Existing entry for this tuple will
 * be removed and freed with @f if found.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashUpdateEntry2(xmlHashTablePtr table, const xmlChar *name,
	           const xmlChar *name2, void *userdata,
		   xmlHashDeallocator f) {
    return(xmlHashUpdateEntry3(table, name, name2, KD_NULL, userdata, f));
}

/**
 * xmlHashLookup:
 * @table: the hash table
 * @name: the name of the userdata
 *
 * Find the userdata specified by the @name.
 *
 * Returns the pointer to the userdata
 */
void *
xmlHashLookup(xmlHashTablePtr table, const xmlChar *name) {
    return(xmlHashLookup3(table, name, KD_NULL, KD_NULL));
}

/**
 * xmlHashLookup2:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 *
 * Find the userdata specified by the (@name, @name2) tuple.
 *
 * Returns the pointer to the userdata
 */
void *
xmlHashLookup2(xmlHashTablePtr table, const xmlChar *name,
	      const xmlChar *name2) {
    return(xmlHashLookup3(table, name, name2, KD_NULL));
}

/**
 * xmlHashQLookup:
 * @table: the hash table
 * @prefix: the prefix of the userdata
 * @name: the name of the userdata
 *
 * Find the userdata specified by the QName @prefix:@name/@name.
 *
 * Returns the pointer to the userdata
 */
void *
xmlHashQLookup(xmlHashTablePtr table, const xmlChar *prefix,
               const xmlChar *name) {
    return(xmlHashQLookup3(table, prefix, name, KD_NULL, KD_NULL, KD_NULL, KD_NULL));
}

/**
 * xmlHashQLookup2:
 * @table: the hash table
 * @prefix: the prefix of the userdata
 * @name: the name of the userdata
 * @prefix2: the second prefix of the userdata
 * @name2: a second name of the userdata
 *
 * Find the userdata specified by the QNames tuple
 *
 * Returns the pointer to the userdata
 */
void *
xmlHashQLookup2(xmlHashTablePtr table, const xmlChar *prefix,
                const xmlChar *name, const xmlChar *prefix2,
	        const xmlChar *name2) {
    return(xmlHashQLookup3(table, prefix, name, prefix2, name2, KD_NULL, KD_NULL));
}

/**
 * xmlHashAddEntry3:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @name3: a third name of the userdata
 * @userdata: a pointer to the userdata
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the tuple (@name, @name2, @name3). Duplicate entries generate
 * errors.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashAddEntry3(xmlHashTablePtr table, const xmlChar *name,
	         const xmlChar *name2, const xmlChar *name3,
		 void *userdata) {
    unsigned long key, len = 0;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr insert;

    if ((table == KD_NULL) || (name == KD_NULL))
	return(-1);

    /*
     * If using a dict internalize if needed
     */
    if (table->dict) {
        if (!xmlDictOwns(table->dict, name)) {
	    name = xmlDictLookup(table->dict, name, -1);
	    if (name == KD_NULL)
	        return(-1);
	}
        if ((name2 != KD_NULL) && (!xmlDictOwns(table->dict, name2))) {
	    name2 = xmlDictLookup(table->dict, name2, -1);
	    if (name2 == KD_NULL)
	        return(-1);
	}
        if ((name3 != KD_NULL) && (!xmlDictOwns(table->dict, name3))) {
	    name3 = xmlDictLookup(table->dict, name3, -1);
	    if (name3 == KD_NULL)
	        return(-1);
	}
    }

    /*
     * Check for duplicate and insertion location.
     */
    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
	insert = KD_NULL;
    } else {
        if (table->dict) {
	    for (insert = &(table->table[key]); insert->next != KD_NULL;
		 insert = insert->next) {
		if ((insert->name == name) &&
		    (insert->name2 == name2) &&
		    (insert->name3 == name3))
		    return(-1);
		len++;
	    }
	    if ((insert->name == name) &&
		(insert->name2 == name2) &&
		(insert->name3 == name3))
		return(-1);
	} else {
	    for (insert = &(table->table[key]); insert->next != KD_NULL;
		 insert = insert->next) {
		if ((xmlStrEqual(insert->name, name)) &&
		    (xmlStrEqual(insert->name2, name2)) &&
		    (xmlStrEqual(insert->name3, name3)))
		    return(-1);
		len++;
	    }
	    if ((xmlStrEqual(insert->name, name)) &&
		(xmlStrEqual(insert->name2, name2)) &&
		(xmlStrEqual(insert->name3, name3)))
		return(-1);
	}
    }

    if (insert == KD_NULL) {
	entry = &(table->table[key]);
    } else {
	entry = (xmlHashEntry*) xmlMalloc(sizeof(xmlHashEntry));
	if (entry == KD_NULL)
	     return(-1);
    }

    if (table->dict != KD_NULL) {
        entry->name = (xmlChar *) name;
        entry->name2 = (xmlChar *) name2;
        entry->name3 = (xmlChar *) name3;
    } else {
	entry->name = xmlStrdup(name);
	entry->name2 = xmlStrdup(name2);
	entry->name3 = xmlStrdup(name3);
    }
    entry->payload = userdata;
    entry->next = KD_NULL;
    entry->valid = 1;


    if (insert != KD_NULL) 
	insert->next = entry;

    table->nbElems++;

    if (len > MAX_HASH_LEN)
	xmlHashGrow(table, MAX_HASH_LEN * table->size);

    return(0);
}

/**
 * xmlHashUpdateEntry3:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @name3: a third name of the userdata
 * @userdata: a pointer to the userdata
 * @f: the deallocator function for replaced item (if any)
 *
 * Add the @userdata to the hash @table. This can later be retrieved
 * by using the tuple (@name, @name2, @name3). Existing entry for this tuple
 * will be removed and freed with @f if found.
 *
 * Returns 0 the addition succeeded and -1 in case of error.
 */
int
xmlHashUpdateEntry3(xmlHashTablePtr table, const xmlChar *name,
	           const xmlChar *name2, const xmlChar *name3,
		   void *userdata, xmlHashDeallocator f) {
    unsigned long key;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr insert;

    if ((table == KD_NULL) || name == KD_NULL)
	return(-1);

    /*
     * If using a dict internalize if needed
     */
    if (table->dict) {
        if (!xmlDictOwns(table->dict, name)) {
	    name = xmlDictLookup(table->dict, name, -1);
	    if (name == KD_NULL)
	        return(-1);
	}
        if ((name2 != KD_NULL) && (!xmlDictOwns(table->dict, name2))) {
	    name2 = xmlDictLookup(table->dict, name2, -1);
	    if (name2 == KD_NULL)
	        return(-1);
	}
        if ((name3 != KD_NULL) && (!xmlDictOwns(table->dict, name3))) {
	    name3 = xmlDictLookup(table->dict, name3, -1);
	    if (name3 == KD_NULL)
	        return(-1);
	}
    }

    /*
     * Check for duplicate and insertion location.
     */
    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
	insert = KD_NULL;
    } else {
        if (table ->dict) {
	    for (insert = &(table->table[key]); insert->next != KD_NULL;
		 insert = insert->next) {
		if ((insert->name == name) &&
		    (insert->name2 == name2) &&
		    (insert->name3 == name3)) {
		    if (f)
			f(insert->payload, insert->name);
		    insert->payload = userdata;
		    return(0);
		}
	    }
	    if ((insert->name == name) &&
		(insert->name2 == name2) &&
		(insert->name3 == name3)) {
		if (f)
		    f(insert->payload, insert->name);
		insert->payload = userdata;
		return(0);
	    }
	} else {
	    for (insert = &(table->table[key]); insert->next != KD_NULL;
		 insert = insert->next) {
		if ((xmlStrEqual(insert->name, name)) &&
		    (xmlStrEqual(insert->name2, name2)) &&
		    (xmlStrEqual(insert->name3, name3))) {
		    if (f)
			f(insert->payload, insert->name);
		    insert->payload = userdata;
		    return(0);
		}
	    }
	    if ((xmlStrEqual(insert->name, name)) &&
		(xmlStrEqual(insert->name2, name2)) &&
		(xmlStrEqual(insert->name3, name3))) {
		if (f)
		    f(insert->payload, insert->name);
		insert->payload = userdata;
		return(0);
	    }
	}
    }

    if (insert == KD_NULL) {
	entry =  &(table->table[key]);
    } else {
	entry = (xmlHashEntry*) xmlMalloc(sizeof(xmlHashEntry));
	if (entry == KD_NULL)
	     return(-1);
    }

    if (table->dict != KD_NULL) {
        entry->name = (xmlChar *) name;
        entry->name2 = (xmlChar *) name2;
        entry->name3 = (xmlChar *) name3;
    } else {
	entry->name = xmlStrdup(name);
	entry->name2 = xmlStrdup(name2);
	entry->name3 = xmlStrdup(name3);
    }
    entry->payload = userdata;
    entry->next = KD_NULL;
    entry->valid = 1;
    table->nbElems++;


    if (insert != KD_NULL) {
	insert->next = entry;
    }
    return(0);
}

/**
 * xmlHashLookup3:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @name3: a third name of the userdata
 *
 * Find the userdata specified by the (@name, @name2, @name3) tuple.
 *
 * Returns the a pointer to the userdata
 */
void *
xmlHashLookup3(xmlHashTablePtr table, const xmlChar *name, 
	       const xmlChar *name2, const xmlChar *name3) {
    unsigned long key;
    xmlHashEntryPtr entry;

    if (table == KD_NULL)
	return(KD_NULL);
    if (name == KD_NULL)
	return(KD_NULL);
    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0)
	return(KD_NULL);
    if (table->dict) {
	for (entry = &(table->table[key]); entry != KD_NULL; entry = entry->next) {
	    if ((entry->name == name) &&
		(entry->name2 == name2) &&
		(entry->name3 == name3))
		return(entry->payload);
	}
    }
    for (entry = &(table->table[key]); entry != KD_NULL; entry = entry->next) {
	if ((xmlStrEqual(entry->name, name)) &&
	    (xmlStrEqual(entry->name2, name2)) &&
	    (xmlStrEqual(entry->name3, name3)))
	    return(entry->payload);
    }
    return(KD_NULL);
}

/**
 * xmlHashQLookup3:
 * @table: the hash table
 * @prefix: the prefix of the userdata
 * @name: the name of the userdata
 * @prefix2: the second prefix of the userdata
 * @name2: a second name of the userdata
 * @prefix3: the third prefix of the userdata
 * @name3: a third name of the userdata
 *
 * Find the userdata specified by the (@name, @name2, @name3) tuple.
 *
 * Returns the a pointer to the userdata
 */
void *
xmlHashQLookup3(xmlHashTablePtr table,
                const xmlChar *prefix, const xmlChar *name,
		const xmlChar *prefix2, const xmlChar *name2,
		const xmlChar *prefix3, const xmlChar *name3) {
    unsigned long key;
    xmlHashEntryPtr entry;

    if (table == KD_NULL)
	return(KD_NULL);
    if (name == KD_NULL)
	return(KD_NULL);
    key = xmlHashComputeQKey(table, prefix, name, prefix2,
                             name2, prefix3, name3);
    if (table->table[key].valid == 0)
	return(KD_NULL);
    for (entry = &(table->table[key]); entry != KD_NULL; entry = entry->next) {
	if ((xmlStrQEqual(prefix, name, entry->name)) &&
	    (xmlStrQEqual(prefix2, name2, entry->name2)) &&
	    (xmlStrQEqual(prefix3, name3, entry->name3)))
	    return(entry->payload);
    }
    return(KD_NULL);
}

typedef struct {
    xmlHashScanner hashscanner;
    void *data;
} stubData;

static void 
stubHashScannerFull (void *payload, void *data, const xmlChar *name, 
                     const xmlChar *name2 ATTRIBUTE_UNUSED,
		     const xmlChar *name3 ATTRIBUTE_UNUSED) {
    stubData *stubdata = (stubData *) data;
    stubdata->hashscanner (payload, stubdata->data, (xmlChar *) name);
}                                  
 
/**
 * xmlHashScan:
 * @table: the hash table
 * @f:  the scanner function for items in the hash
 * @data:  extra data passed to f
 *
 * Scan the hash @table and applied @f to each value.
 */
void
xmlHashScan(xmlHashTablePtr table, xmlHashScanner f, void *data) {
    stubData stubdata;
    stubdata.data = data;
    stubdata.hashscanner = f; 
    xmlHashScanFull (table, stubHashScannerFull, &stubdata);
}

/**
 * xmlHashScanFull:
 * @table: the hash table
 * @f:  the scanner function for items in the hash
 * @data:  extra data passed to f
 *
 * Scan the hash @table and applied @f to each value.
 */
void
xmlHashScanFull(xmlHashTablePtr table, xmlHashScannerFull f, void *data) {
    int i, nb;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;

    if (table == KD_NULL)
	return;
    if (f == KD_NULL)
	return;

    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0) 
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
                nb = table->nbElems;
		if ((f != KD_NULL) && (iter->payload != KD_NULL))
		    f(iter->payload, data, iter->name,
		      iter->name2, iter->name3);
                if (nb != table->nbElems) {
                    /* table was modified by the callback, be careful */
                    if (iter == &(table->table[i])) {
                        if (table->table[i].valid == 0)
                            iter = KD_NULL;
                        if (table->table[i].next != next)
			    iter = &(table->table[i]);
                    } else
		        iter = next;
                } else
		    iter = next;
	    }
	}
    }
}

/**
 * xmlHashScan3:
 * @table: the hash table
 * @name: the name of the userdata or KD_NULL
 * @name2: a second name of the userdata or KD_NULL
 * @name3: a third name of the userdata or KD_NULL
 * @f:  the scanner function for items in the hash
 * @data:  extra data passed to f
 *
 * Scan the hash @table and applied @f to each value matching
 * (@name, @name2, @name3) tuple. If one of the names is null,
 * the comparison is considered to match.
 */
void
xmlHashScan3(xmlHashTablePtr table, const xmlChar *name, 
	     const xmlChar *name2, const xmlChar *name3,
	     xmlHashScanner f, void *data) {
    xmlHashScanFull3 (table, name, name2, name3,
		      (xmlHashScannerFull) f, data);
}

/**
 * xmlHashScanFull3:
 * @table: the hash table
 * @name: the name of the userdata or KD_NULL
 * @name2: a second name of the userdata or KD_NULL
 * @name3: a third name of the userdata or KD_NULL
 * @f:  the scanner function for items in the hash
 * @data:  extra data passed to f
 *
 * Scan the hash @table and applied @f to each value matching
 * (@name, @name2, @name3) tuple. If one of the names is null,
 * the comparison is considered to match.
 */
void
xmlHashScanFull3(xmlHashTablePtr table, const xmlChar *name, 
		 const xmlChar *name2, const xmlChar *name3,
		 xmlHashScannerFull f, void *data) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;

    if (table == KD_NULL)
	return;
    if (f == KD_NULL)
	return;

    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0)
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
		if (((name == KD_NULL) || (xmlStrEqual(name, iter->name))) &&
		    ((name2 == KD_NULL) || (xmlStrEqual(name2, iter->name2))) &&
		    ((name3 == KD_NULL) || (xmlStrEqual(name3, iter->name3))) &&
		    (iter->payload != KD_NULL)) {
		    f(iter->payload, data, iter->name,
		      iter->name2, iter->name3);
		}
		iter = next;
	    }
	}
    }
}

/**
 * xmlHashCopy:
 * @table: the hash table
 * @f:  the copier function for items in the hash
 *
 * Scan the hash @table and applied @f to each value.
 *
 * Returns the new table or KD_NULL in case of error.
 */
xmlHashTablePtr
xmlHashCopy(xmlHashTablePtr table, xmlHashCopier f) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;
    xmlHashTablePtr ret;

    if (table == KD_NULL)
	return(KD_NULL);
    if (f == KD_NULL)
	return(KD_NULL);

    ret = xmlHashCreate(table->size);
    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0)
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
		xmlHashAddEntry3(ret, iter->name, iter->name2,
			         iter->name3, f(iter->payload, iter->name));
		iter = next;
	    }
	}
    }
    ret->nbElems = table->nbElems;
    return(ret);
}

/**
 * xmlHashSize:
 * @table: the hash table
 *
 * Query the number of elements installed in the hash @table.
 *
 * Returns the number of elements in the hash table or
 * -1 in case of error
 */
int
xmlHashSize(xmlHashTablePtr table) {
    if (table == KD_NULL)
	return(-1);
    return(table->nbElems);
}

/**
 * xmlHashRemoveEntry:
 * @table: the hash table
 * @name: the name of the userdata
 * @f: the deallocator function for removed item (if any)
 *
 * Find the userdata specified by the @name and remove
 * it from the hash @table. Existing userdata for this tuple will be removed
 * and freed with @f.
 *
 * Returns 0 if the removal succeeded and -1 in case of error or not found.
 */
int xmlHashRemoveEntry(xmlHashTablePtr table, const xmlChar *name,
		       xmlHashDeallocator f) {
    return(xmlHashRemoveEntry3(table, name, KD_NULL, KD_NULL, f));
}

/**
 * xmlHashRemoveEntry2:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @f: the deallocator function for removed item (if any)
 *
 * Find the userdata specified by the (@name, @name2) tuple and remove
 * it from the hash @table. Existing userdata for this tuple will be removed
 * and freed with @f.
 *
 * Returns 0 if the removal succeeded and -1 in case of error or not found.
 */
int
xmlHashRemoveEntry2(xmlHashTablePtr table, const xmlChar *name,
			const xmlChar *name2, xmlHashDeallocator f) {
    return(xmlHashRemoveEntry3(table, name, name2, KD_NULL, f));
}

/**
 * xmlHashRemoveEntry3:
 * @table: the hash table
 * @name: the name of the userdata
 * @name2: a second name of the userdata
 * @name3: a third name of the userdata
 * @f: the deallocator function for removed item (if any)
 *
 * Find the userdata specified by the (@name, @name2, @name3) tuple and remove
 * it from the hash @table. Existing userdata for this tuple will be removed
 * and freed with @f.
 *
 * Returns 0 if the removal succeeded and -1 in case of error or not found.
 */
int
xmlHashRemoveEntry3(xmlHashTablePtr table, const xmlChar *name,
    const xmlChar *name2, const xmlChar *name3, xmlHashDeallocator f) {
    unsigned long key;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr prev = KD_NULL;

    if (table == KD_NULL || name == KD_NULL)
        return(-1);

    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
        return(-1);
    } else {
        for (entry = &(table->table[key]); entry != KD_NULL; entry = entry->next) {
            if (xmlStrEqual(entry->name, name) &&
                    xmlStrEqual(entry->name2, name2) &&
                    xmlStrEqual(entry->name3, name3)) {
                if ((f != KD_NULL) && (entry->payload != KD_NULL))
                    f(entry->payload, entry->name);
                entry->payload = KD_NULL;
		if (table->dict == KD_NULL) {
		    if(entry->name)
			xmlFree(entry->name);
		    if(entry->name2)
			xmlFree(entry->name2);
		    if(entry->name3)
			xmlFree(entry->name3);
		}
                if(prev) {
                    prev->next = entry->next;
		    xmlFree(entry);
		} else {
		    if (entry->next == KD_NULL) {
			entry->valid = 0;
		    } else {
			entry = entry->next;
			kdMemcpy(&(table->table[key]), entry, sizeof(xmlHashEntry));
			xmlFree(entry);
		    }
		}
                table->nbElems--;
                return(0);
            }
            prev = entry;
        }
        return(-1);
    }
}

#define bottom_hash
#include "elfgcchack.h"
