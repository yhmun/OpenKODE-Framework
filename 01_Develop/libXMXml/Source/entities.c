/*
 * entities.c : implementation for the XML entities handling
 *
 * See Copyright for the status of this software.
 *
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <XMXML/xmlmemory.h>
#include <XMXML/hash.h>
#include <XMXML/entities.h>
#include <XMXML/parser.h>
#include <XMXML/parserInternals.h>
#include <XMXML/xmlerror.h>
#include <XMXML/globals.h>
#include <XMXML/dict.h>

/*
 * The XML predefined entities.
 */

static xmlEntity xmlEntityLt = {
    KD_NULL, XML_ENTITY_DECL, BAD_CAST "lt",
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, 
    BAD_CAST "<", BAD_CAST "<", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, 0, 1
};
static xmlEntity xmlEntityGt = {
    KD_NULL, XML_ENTITY_DECL, BAD_CAST "gt",
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, 
    BAD_CAST ">", BAD_CAST ">", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, 0, 1
};
static xmlEntity xmlEntityAmp = {
    KD_NULL, XML_ENTITY_DECL, BAD_CAST "amp",
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, 
    BAD_CAST "&", BAD_CAST "&", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, 0, 1
};
static xmlEntity xmlEntityQuot = {
    KD_NULL, XML_ENTITY_DECL, BAD_CAST "quot",
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, 
    BAD_CAST "\"", BAD_CAST "\"", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, 0, 1
};
static xmlEntity xmlEntityApos = {
    KD_NULL, XML_ENTITY_DECL, BAD_CAST "apos",
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, 
    BAD_CAST "'", BAD_CAST "'", 1,
    XML_INTERNAL_PREDEFINED_ENTITY,
    KD_NULL, KD_NULL, KD_NULL, KD_NULL, 0, 1
};

/**
 * xmlEntitiesErrMemory:
 * @extra:  extra informations
 *
 * Handle an out of memory condition
 */
static void
xmlEntitiesErrMemory(const char *extra)
{
    __xmlSimpleError(XML_FROM_TREE, XML_ERR_NO_MEMORY, KD_NULL, KD_NULL, extra);
}

/**
 * xmlEntitiesErr:
 * @code:  the error code
 * @msg:  the message
 *
 * Handle an out of memory condition
 */
static void
xmlEntitiesErr(xmlParserErrors code, const char *msg)
{
    __xmlSimpleError(XML_FROM_TREE, code, KD_NULL, msg, KD_NULL);
}

/*
 * xmlFreeEntity : clean-up an entity record.
 */
static void
xmlFreeEntity(xmlEntityPtr entity)
{
    xmlDictPtr dict = KD_NULL;

    if (entity == KD_NULL)
        return;

    if (entity->doc != KD_NULL)
        dict = entity->doc->dict;


    if ((entity->children) && (entity->owner == 1) &&
        (entity == (xmlEntityPtr) entity->children->parent))
        xmlFreeNodeList(entity->children);
    if (dict != KD_NULL) {
        if ((entity->name != KD_NULL) && (!xmlDictOwns(dict, entity->name)))
            xmlFree((char *) entity->name);
        if ((entity->ExternalID != KD_NULL) &&
	    (!xmlDictOwns(dict, entity->ExternalID)))
            xmlFree((char *) entity->ExternalID);
        if ((entity->SystemID != KD_NULL) &&
	    (!xmlDictOwns(dict, entity->SystemID)))
            xmlFree((char *) entity->SystemID);
        if ((entity->URI != KD_NULL) && (!xmlDictOwns(dict, entity->URI)))
            xmlFree((char *) entity->URI);
        if ((entity->content != KD_NULL)
            && (!xmlDictOwns(dict, entity->content)))
            xmlFree((char *) entity->content);
        if ((entity->orig != KD_NULL) && (!xmlDictOwns(dict, entity->orig)))
            xmlFree((char *) entity->orig);
    } else {
        if (entity->name != KD_NULL)
            xmlFree((char *) entity->name);
        if (entity->ExternalID != KD_NULL)
            xmlFree((char *) entity->ExternalID);
        if (entity->SystemID != KD_NULL)
            xmlFree((char *) entity->SystemID);
        if (entity->URI != KD_NULL)
            xmlFree((char *) entity->URI);
        if (entity->content != KD_NULL)
            xmlFree((char *) entity->content);
        if (entity->orig != KD_NULL)
            xmlFree((char *) entity->orig);
    }
    xmlFree(entity);
}

/*
 * xmlCreateEntity:
 *
 * internal routine doing the entity node strutures allocations
 */
static xmlEntityPtr
xmlCreateEntity(xmlDictPtr dict, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
	        const xmlChar *content) {
    xmlEntityPtr ret;

    ret = (xmlEntityPtr) xmlMalloc(sizeof(xmlEntity));
    if (ret == KD_NULL) {
        xmlEntitiesErrMemory("xmlCreateEntity: malloc failed");
	return(KD_NULL);
    }
    kdMemset (ret, 0, sizeof(xmlEntity));
    ret->type = XML_ENTITY_DECL;
    ret->checked = 0;

    /*
     * fill the structure.
     */
    ret->etype = (xmlEntityType) type;
    if (dict == KD_NULL) {
	ret->name = xmlStrdup(name);
	if (ExternalID != KD_NULL)
	    ret->ExternalID = xmlStrdup(ExternalID);
	if (SystemID != KD_NULL)
	    ret->SystemID = xmlStrdup(SystemID);
    } else {
        ret->name = xmlDictLookup(dict, name, -1);
	if (ExternalID != KD_NULL)
	    ret->ExternalID = xmlDictLookup(dict, ExternalID, -1);
	if (SystemID != KD_NULL)
	    ret->SystemID = xmlDictLookup(dict, SystemID, -1);
    }
    if (content != KD_NULL) {
        ret->length = xmlStrlen(content);
	if ((dict != KD_NULL) && (ret->length < 5))
	    ret->content = (xmlChar *)
	                   xmlDictLookup(dict, content, ret->length);
	else
	    ret->content = xmlStrndup(content, ret->length);
     } else {
        ret->length = 0;
        ret->content = KD_NULL;
    }
    ret->URI = KD_NULL; /* to be computed by the layer knowing
			the defining entity */
    ret->orig = KD_NULL;
    ret->owner = 0;

    return(ret);
}

/*
 * xmlAddEntity : register a new entity for an entities table.
 */
static xmlEntityPtr
xmlAddEntity(xmlDtdPtr dtd, const xmlChar *name, int type,
	  const xmlChar *ExternalID, const xmlChar *SystemID,
	  const xmlChar *content) {
    xmlDictPtr dict = KD_NULL;
    xmlEntitiesTablePtr table = KD_NULL;
    xmlEntityPtr ret;

    if (name == KD_NULL)
	return(KD_NULL);
    if (dtd == KD_NULL)
	return(KD_NULL);
    if (dtd->doc != KD_NULL)
        dict = dtd->doc->dict;

    switch (type) {
        case XML_INTERNAL_GENERAL_ENTITY:
        case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
        case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    if (dtd->entities == KD_NULL)
		dtd->entities = (xmlEntitiesTable*) xmlHashCreateDict(0, dict);
	    table = (xmlEntitiesTable*) dtd->entities;
	    break;
        case XML_INTERNAL_PARAMETER_ENTITY:
        case XML_EXTERNAL_PARAMETER_ENTITY:
	    if (dtd->pentities == KD_NULL)
		dtd->pentities = xmlHashCreateDict(0, dict);
	    table = (xmlEntitiesTable*) dtd->pentities;
	    break;
        case XML_INTERNAL_PREDEFINED_ENTITY:
	    return(KD_NULL);
    }
    if (table == KD_NULL)
	return(KD_NULL);
    ret = xmlCreateEntity(dict, name, type, ExternalID, SystemID, content);
    if (ret == KD_NULL)
        return(KD_NULL);
    ret->doc = dtd->doc;

    if (xmlHashAddEntry(table, name, ret)) {
	/*
	 * entity was already defined at another level.
	 */
        xmlFreeEntity(ret);
	return(KD_NULL);
    }
    return(ret);
}

/**
 * xmlGetPredefinedEntity:
 * @name:  the entity name
 *
 * Check whether this name is an predefined entity.
 *
 * Returns KD_NULL if not, otherwise the entity
 */
xmlEntityPtr
xmlGetPredefinedEntity(const xmlChar *name) {
    if (name == KD_NULL) return(KD_NULL);
    switch (name[0]) {
        case 'l':
	    if (xmlStrEqual(name, BAD_CAST "lt"))
	        return(&xmlEntityLt);
	    break;
        case 'g':
	    if (xmlStrEqual(name, BAD_CAST "gt"))
	        return(&xmlEntityGt);
	    break;
        case 'a':
	    if (xmlStrEqual(name, BAD_CAST "amp"))
	        return(&xmlEntityAmp);
	    if (xmlStrEqual(name, BAD_CAST "apos"))
	        return(&xmlEntityApos);
	    break;
        case 'q':
	    if (xmlStrEqual(name, BAD_CAST "quot"))
	        return(&xmlEntityQuot);
	    break;
	default:
	    break;
    }
    return(KD_NULL);
}

/**
 * xmlAddDtdEntity:
 * @doc:  the document
 * @name:  the entity name
 * @type:  the entity type XML_xxx_yyy_ENTITY
 * @ExternalID:  the entity external ID if available
 * @SystemID:  the entity system ID if available
 * @content:  the entity content
 *
 * Register a new entity for this document DTD external subset.
 *
 * Returns a pointer to the entity or KD_NULL in case of error
 */
xmlEntityPtr
xmlAddDtdEntity(xmlDocPtr doc, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
		const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDtdPtr dtd;

    if (doc == KD_NULL) {
	xmlEntitiesErr(XML_DTD_NO_DOC,
	        "xmlAddDtdEntity: document is KD_NULL");
	return(KD_NULL);
    }
    if (doc->extSubset == KD_NULL) {
	xmlEntitiesErr(XML_DTD_NO_DTD,
	        "xmlAddDtdEntity: document without external subset");
	return(KD_NULL);
    }
    dtd = doc->extSubset;
    ret = xmlAddEntity(dtd, name, type, ExternalID, SystemID, content);
    if (ret == KD_NULL) return(KD_NULL);

    /*
     * Link it to the DTD
     */
    ret->parent = dtd;
    ret->doc = dtd->doc;
    if (dtd->last == KD_NULL) {
	dtd->children = dtd->last = (xmlNodePtr) ret;
    } else {
        dtd->last->next = (xmlNodePtr) ret;
	ret->prev = dtd->last;
	dtd->last = (xmlNodePtr) ret;
    }
    return(ret);
}

/**
 * xmlAddDocEntity:
 * @doc:  the document
 * @name:  the entity name
 * @type:  the entity type XML_xxx_yyy_ENTITY
 * @ExternalID:  the entity external ID if available
 * @SystemID:  the entity system ID if available
 * @content:  the entity content
 *
 * Register a new entity for this document.
 *
 * Returns a pointer to the entity or KD_NULL in case of error
 */
xmlEntityPtr
xmlAddDocEntity(xmlDocPtr doc, const xmlChar *name, int type,
	        const xmlChar *ExternalID, const xmlChar *SystemID,
	        const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDtdPtr dtd;

    if (doc == KD_NULL) {
	xmlEntitiesErr(XML_DTD_NO_DOC,
	        "xmlAddDocEntity: document is KD_NULL");
	return(KD_NULL);
    }
    if (doc->intSubset == KD_NULL) {
	xmlEntitiesErr(XML_DTD_NO_DTD,
	        "xmlAddDocEntity: document without internal subset");
	return(KD_NULL);
    }
    dtd = doc->intSubset;
    ret = xmlAddEntity(dtd, name, type, ExternalID, SystemID, content);
    if (ret == KD_NULL) return(KD_NULL);

    /*
     * Link it to the DTD
     */
    ret->parent = dtd;
    ret->doc = dtd->doc;
    if (dtd->last == KD_NULL) {
	dtd->children = dtd->last = (xmlNodePtr) ret;
    } else {
	dtd->last->next = (xmlNodePtr) ret;
	ret->prev = dtd->last;
	dtd->last = (xmlNodePtr) ret;
    }
    return(ret);
}

/**
 * xmlNewEntity:
 * @doc:  the document
 * @name:  the entity name
 * @type:  the entity type XML_xxx_yyy_ENTITY
 * @ExternalID:  the entity external ID if available
 * @SystemID:  the entity system ID if available
 * @content:  the entity content
 *
 * Create a new entity, this differs from xmlAddDocEntity() that if
 * the document is KD_NULL or has no internal subset defined, then an
 * unlinked entity structure will be returned, it is then the responsability
 * of the caller to link it to the document later or free it when not needed
 * anymore.
 *
 * Returns a pointer to the entity or KD_NULL in case of error
 */
xmlEntityPtr
xmlNewEntity(xmlDocPtr doc, const xmlChar *name, int type,
	     const xmlChar *ExternalID, const xmlChar *SystemID,
	     const xmlChar *content) {
    xmlEntityPtr ret;
    xmlDictPtr dict;

    if ((doc != KD_NULL) && (doc->intSubset != KD_NULL)) {
	return(xmlAddDocEntity(doc, name, type, ExternalID, SystemID, content));
    }
    if (doc != KD_NULL)
        dict = doc->dict;
    else
        dict = KD_NULL;
    ret = xmlCreateEntity(dict, name, type, ExternalID, SystemID, content);
    if (ret == KD_NULL)
        return(KD_NULL);
    ret->doc = doc;
    return(ret);
}

/**
 * xmlGetEntityFromTable:
 * @table:  an entity table
 * @name:  the entity name
 * @parameter:  look for parameter entities
 *
 * Do an entity lookup in the table.
 * returns the corresponding parameter entity, if found.
 * 
 * Returns A pointer to the entity structure or KD_NULL if not found.
 */
static xmlEntityPtr
xmlGetEntityFromTable(xmlEntitiesTablePtr table, const xmlChar *name) {
    return((xmlEntityPtr) xmlHashLookup(table, name));
}

/**
 * xmlGetParameterEntity:
 * @doc:  the document referencing the entity
 * @name:  the entity name
 *
 * Do an entity lookup in the internal and external subsets and
 * returns the corresponding parameter entity, if found.
 * 
 * Returns A pointer to the entity structure or KD_NULL if not found.
 */
xmlEntityPtr
xmlGetParameterEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntitiesTablePtr table;
    xmlEntityPtr ret;

    if (doc == KD_NULL)
	return(KD_NULL);
    if ((doc->intSubset != KD_NULL) && (doc->intSubset->pentities != KD_NULL)) {
	table = (xmlEntitiesTablePtr) doc->intSubset->pentities;
	ret = xmlGetEntityFromTable(table, name);
	if (ret != KD_NULL)
	    return(ret);
    }
    if ((doc->extSubset != KD_NULL) && (doc->extSubset->pentities != KD_NULL)) {
	table = (xmlEntitiesTablePtr) doc->extSubset->pentities;
	return(xmlGetEntityFromTable(table, name));
    }
    return(KD_NULL);
}

/**
 * xmlGetDtdEntity:
 * @doc:  the document referencing the entity
 * @name:  the entity name
 *
 * Do an entity lookup in the DTD entity hash table and
 * returns the corresponding entity, if found.
 * Note: the first argument is the document node, not the DTD node.
 * 
 * Returns A pointer to the entity structure or KD_NULL if not found.
 */
xmlEntityPtr
xmlGetDtdEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntitiesTablePtr table;

    if (doc == KD_NULL)
	return(KD_NULL);
    if ((doc->extSubset != KD_NULL) && (doc->extSubset->entities != KD_NULL)) {
	table = (xmlEntitiesTablePtr) doc->extSubset->entities;
	return(xmlGetEntityFromTable(table, name));
    }
    return(KD_NULL);
}

/**
 * xmlGetDocEntity:
 * @doc:  the document referencing the entity
 * @name:  the entity name
 *
 * Do an entity lookup in the document entity hash table and
 * returns the corresponding entity, otherwise a lookup is done
 * in the predefined entities too.
 * 
 * Returns A pointer to the entity structure or KD_NULL if not found.
 */
xmlEntityPtr
xmlGetDocEntity(xmlDocPtr doc, const xmlChar *name) {
    xmlEntityPtr cur;
    xmlEntitiesTablePtr table;

    if (doc != KD_NULL) {
	if ((doc->intSubset != KD_NULL) && (doc->intSubset->entities != KD_NULL)) {
	    table = (xmlEntitiesTablePtr) doc->intSubset->entities;
	    cur = xmlGetEntityFromTable(table, name);
	    if (cur != KD_NULL)
		return(cur);
	}
	if (doc->standalone != 1) {
	    if ((doc->extSubset != KD_NULL) &&
		(doc->extSubset->entities != KD_NULL)) {
		table = (xmlEntitiesTablePtr) doc->extSubset->entities;
		cur = xmlGetEntityFromTable(table, name);
		if (cur != KD_NULL)
		    return(cur);
	    }
	}
    }
    return(xmlGetPredefinedEntity(name));
}

/*
 * Macro used to grow the current buffer.
 */
#define growBufferReentrant() {						\
    buffer_size *= 2;							\
    buffer = (xmlChar *)						\
    		xmlRealloc(buffer, buffer_size * sizeof(xmlChar));	\
    if (buffer == KD_NULL) {						\
        xmlEntitiesErrMemory("xmlEncodeEntitiesReentrant: realloc failed");\
	return(KD_NULL);							\
    }									\
}


/**
 * xmlEncodeEntitiesReentrant:
 * @doc:  the document containing the string
 * @input:  A string to convert to XML.
 *
 * Do a global encoding of a string, replacing the predefined entities
 * and non ASCII values with their entities and CharRef counterparts.
 * Contrary to xmlEncodeEntities, this routine is reentrant, and result
 * must be deallocated.
 *
 * Returns A newly allocated string with the substitution done.
 */
xmlChar *
xmlEncodeEntitiesReentrant(xmlDocPtr doc, const xmlChar *input) {
    const xmlChar *cur = input;
    xmlChar *buffer = KD_NULL;
    xmlChar *out = KD_NULL;
    int buffer_size = 0;
    int html = 0;

    if (input == KD_NULL) return(KD_NULL);
    if (doc != KD_NULL)
        html = (doc->type == XML_HTML_DOCUMENT_NODE);

    /*
     * allocate an translation buffer.
     */
    buffer_size = 1000;
    buffer = (xmlChar *) xmlMalloc(buffer_size * sizeof(xmlChar));
    if (buffer == KD_NULL) {
        xmlEntitiesErrMemory("xmlEncodeEntitiesReentrant: malloc failed");
	return(KD_NULL);
    }
    out = buffer;

    while (*cur != '\0') {
        if (out - buffer > buffer_size - 100) {
	    int indx = out - buffer;

	    growBufferReentrant();
	    out = &buffer[indx];
	}

	/*
	 * By default one have to encode at least '<', '>', '"' and '&' !
	 */
	if (*cur == '<') {
	    *out++ = '&';
	    *out++ = 'l';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '>') {
	    *out++ = '&';
	    *out++ = 'g';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '&') {
	    *out++ = '&';
	    *out++ = 'a';
	    *out++ = 'm';
	    *out++ = 'p';
	    *out++ = ';';
	} else if (((*cur >= 0x20) && (*cur < 0x80)) ||
	    (*cur == '\n') || (*cur == '\t') || ((html) && (*cur == '\r'))) {
	    /*
	     * default case, just copy !
	     */
	    *out++ = *cur;
	} else if (*cur >= 0x80) {
	    if (((doc != KD_NULL) && (doc->encoding != KD_NULL)) || (html)) {
		/*
		 * Bj?n Reese <br@sseusa.com> provided the patch
	        xmlChar xc;
	        xc = (*cur & 0x3F) << 6;
	        if (cur[1] != 0) {
		    xc += *(++cur) & 0x3F;
		    *out++ = xc;
	        } else
		 */
		*out++ = *cur;
	    } else {
		/*
		 * We assume we have UTF-8 input.
		 */
		char buf[11], *ptr;
		int val = 0, l = 1;

		if (*cur < 0xC0) {
		    xmlEntitiesErr(XML_CHECK_NOT_UTF8,
			    "xmlEncodeEntitiesReentrant : input not UTF-8");
		    if (doc != KD_NULL)
			doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
		    kdSnprintfKHR (buf, sizeof(buf), "&#%d;", *cur);
		    buf[sizeof(buf) - 1] = 0;
		    ptr = buf;
		    while (*ptr != 0) *out++ = *ptr++;
		    cur++;
		    continue;
		} else if (*cur < 0xE0) {
                    val = (cur[0]) & 0x1F;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    l = 2;
		} else if (*cur < 0xF0) {
                    val = (cur[0]) & 0x0F;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    val <<= 6;
		    val |= (cur[2]) & 0x3F;
		    l = 3;
		} else if (*cur < 0xF8) {
                    val = (cur[0]) & 0x07;
		    val <<= 6;
		    val |= (cur[1]) & 0x3F;
		    val <<= 6;
		    val |= (cur[2]) & 0x3F;
		    val <<= 6;
		    val |= (cur[3]) & 0x3F;
		    l = 4;
		}
		if ((l == 1) || (!IS_CHAR(val))) {
		    xmlEntitiesErr(XML_ERR_INVALID_CHAR,
			"xmlEncodeEntitiesReentrant : char out of range\n");
		    if (doc != KD_NULL)
			doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
		    kdSnprintfKHR (buf, sizeof(buf), "&#%d;", *cur);
		    buf[sizeof(buf) - 1] = 0;
		    ptr = buf;
		    while (*ptr != 0) *out++ = *ptr++;
		    cur++;
		    continue;
		}
		/*
		 * We could do multiple things here. Just save as a char ref
		 */
		kdSnprintfKHR (buf, sizeof(buf), "&#x%X;", val);
		buf[sizeof(buf) - 1] = 0;
		ptr = buf;
		while (*ptr != 0) *out++ = *ptr++;
		cur += l;
		continue;
	    }
	} else if (IS_BYTE_CHAR(*cur)) {
	    char buf[11], *ptr;

	    kdSnprintfKHR (buf, sizeof(buf), "&#%d;", *cur);
	    buf[sizeof(buf) - 1] = 0;
            ptr = buf;
	    while (*ptr != 0) *out++ = *ptr++;
	}
	cur++;
    }
    *out = 0;
    return(buffer);
}

/**
 * xmlEncodeSpecialChars:
 * @doc:  the document containing the string
 * @input:  A string to convert to XML.
 *
 * Do a global encoding of a string, replacing the predefined entities
 * this routine is reentrant, and result must be deallocated.
 *
 * Returns A newly allocated string with the substitution done.
 */
xmlChar *
xmlEncodeSpecialChars(xmlDocPtr doc ATTRIBUTE_UNUSED, const xmlChar *input) {
    const xmlChar *cur = input;
    xmlChar *buffer = KD_NULL;
    xmlChar *out = KD_NULL;
    int buffer_size = 0;
    if (input == KD_NULL) return(KD_NULL);

    /*
     * allocate an translation buffer.
     */
    buffer_size = 1000;
    buffer = (xmlChar *) xmlMalloc(buffer_size * sizeof(xmlChar));
    if (buffer == KD_NULL) {
        xmlEntitiesErrMemory("xmlEncodeSpecialChars: malloc failed");
	return(KD_NULL);
    }
    out = buffer;

    while (*cur != '\0') {
        if (out - buffer > buffer_size - 10) {
	    int indx = out - buffer;

	    growBufferReentrant();
	    out = &buffer[indx];
	}

	/*
	 * By default one have to encode at least '<', '>', '"' and '&' !
	 */
	if (*cur == '<') {
	    *out++ = '&';
	    *out++ = 'l';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '>') {
	    *out++ = '&';
	    *out++ = 'g';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '&') {
	    *out++ = '&';
	    *out++ = 'a';
	    *out++ = 'm';
	    *out++ = 'p';
	    *out++ = ';';
	} else if (*cur == '"') {
	    *out++ = '&';
	    *out++ = 'q';
	    *out++ = 'u';
	    *out++ = 'o';
	    *out++ = 't';
	    *out++ = ';';
	} else if (*cur == '\r') {
	    *out++ = '&';
	    *out++ = '#';
	    *out++ = '1';
	    *out++ = '3';
	    *out++ = ';';
	} else {
	    /*
	     * Works because on UTF-8, all extended sequences cannot
	     * result in bytes in the ASCII range.
	     */
	    *out++ = *cur;
	}
	cur++;
    }
    *out = 0;
    return(buffer);
}

/**
 * xmlCreateEntitiesTable:
 *
 * create and initialize an empty entities hash table.
 * This really doesn't make sense and should be deprecated
 *
 * Returns the xmlEntitiesTablePtr just created or KD_NULL in case of error.
 */
xmlEntitiesTablePtr
xmlCreateEntitiesTable(void) {
    return((xmlEntitiesTablePtr) xmlHashCreate(0));
}

/**
 * xmlFreeEntityWrapper:
 * @entity:  An entity
 * @name:  its name
 *
 * Deallocate the memory used by an entities in the hash table.
 */
static void
xmlFreeEntityWrapper(xmlEntityPtr entity,
	               const xmlChar *name ATTRIBUTE_UNUSED) {
    if (entity != KD_NULL)
	xmlFreeEntity(entity);
}

/**
 * xmlFreeEntitiesTable:
 * @table:  An entity table
 *
 * Deallocate the memory used by an entities hash table.
 */
void
xmlFreeEntitiesTable(xmlEntitiesTablePtr table) {
    xmlHashFree(table, (xmlHashDeallocator) xmlFreeEntityWrapper);
}

#ifdef LIBXML_TREE_ENABLED
/**
 * xmlCopyEntity:
 * @ent:  An entity
 *
 * Build a copy of an entity
 * 
 * Returns the new xmlEntitiesPtr or KD_NULL in case of error.
 */
static xmlEntityPtr
xmlCopyEntity(xmlEntityPtr ent) {
    xmlEntityPtr cur;

    cur = (xmlEntityPtr) xmlMalloc(sizeof(xmlEntity));
    if (cur == KD_NULL) {
        xmlEntitiesErrMemory("xmlCopyEntity:: malloc failed");
	return(KD_NULL);
    }
    kdMemset(cur, 0, sizeof(xmlEntity));
    cur->type = XML_ENTITY_DECL;

    cur->etype = ent->etype;
    if (ent->name != KD_NULL)
	cur->name = xmlStrdup(ent->name);
    if (ent->ExternalID != KD_NULL)
	cur->ExternalID = xmlStrdup(ent->ExternalID);
    if (ent->SystemID != KD_NULL)
	cur->SystemID = xmlStrdup(ent->SystemID);
    if (ent->content != KD_NULL)
	cur->content = xmlStrdup(ent->content);
    if (ent->orig != KD_NULL)
	cur->orig = xmlStrdup(ent->orig);
    if (ent->URI != KD_NULL)
	cur->URI = xmlStrdup(ent->URI);
    return(cur);
}

/**
 * xmlCopyEntitiesTable:
 * @table:  An entity table
 *
 * Build a copy of an entity table.
 * 
 * Returns the new xmlEntitiesTablePtr or KD_NULL in case of error.
 */
xmlEntitiesTablePtr
xmlCopyEntitiesTable(xmlEntitiesTablePtr table) {
    return(xmlHashCopy(table, (xmlHashCopier) xmlCopyEntity));
}
#endif /* LIBXML_TREE_ENABLED */

#ifdef LIBXML_OUTPUT_ENABLED

/**
 * xmlDumpEntityContent:
 * @buf:  An XML buffer.
 * @content:  The entity content.
 *
 * This will dump the quoted string value, taking care of the special
 * treatment required by %
 */
static void
xmlDumpEntityContent(xmlBufferPtr buf, const xmlChar *content) {
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE) return;
    if (xmlStrchr(content, '%')) {
        const xmlChar * base, *cur;

	xmlBufferCCat(buf, "\"");
	base = cur = content;
	while (*cur != 0) {
	    if (*cur == '"') {
		if (base != cur)
		    xmlBufferAdd(buf, base, cur - base);
		xmlBufferAdd(buf, BAD_CAST "&quot;", 6);
		cur++;
		base = cur;
	    } else if (*cur == '%') {
		if (base != cur)
		    xmlBufferAdd(buf, base, cur - base);
		xmlBufferAdd(buf, BAD_CAST "&#x25;", 6);
		cur++;
		base = cur;
	    } else {
		cur++;
	    }
	}
	if (base != cur)
	    xmlBufferAdd(buf, base, cur - base);
	xmlBufferCCat(buf, "\"");
    } else {
        xmlBufferWriteQuotedString(buf, content);
    }
}

/**
 * xmlDumpEntityDecl:
 * @buf:  An XML buffer.
 * @ent:  An entity table
 *
 * This will dump the content of the entity table as an XML DTD definition
 */
void
xmlDumpEntityDecl(xmlBufferPtr buf, xmlEntityPtr ent) {
    if ((buf == KD_NULL) || (ent == KD_NULL)) return;
    switch (ent->etype) {
	case XML_INTERNAL_GENERAL_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    xmlBufferWriteChar(buf, " ");
	    if (ent->orig != KD_NULL)
		xmlBufferWriteQuotedString(buf, ent->orig);
	    else
		xmlDumpEntityContent(buf, ent->content);
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_GENERAL_PARSED_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != KD_NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_GENERAL_UNPARSED_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != KD_NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    if (ent->content != KD_NULL) { /* Should be true ! */
		xmlBufferWriteChar(buf, " NDATA ");
		if (ent->orig != KD_NULL)
		    xmlBufferWriteCHAR(buf, ent->orig);
		else
		    xmlBufferWriteCHAR(buf, ent->content);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_INTERNAL_PARAMETER_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY % ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    xmlBufferWriteChar(buf, " ");
	    if (ent->orig == KD_NULL)
		xmlDumpEntityContent(buf, ent->content);
	    else
		xmlBufferWriteQuotedString(buf, ent->orig);
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	case XML_EXTERNAL_PARAMETER_ENTITY:
	    xmlBufferWriteChar(buf, "<!ENTITY % ");
	    xmlBufferWriteCHAR(buf, ent->name);
	    if (ent->ExternalID != KD_NULL) {
		 xmlBufferWriteChar(buf, " PUBLIC ");
		 xmlBufferWriteQuotedString(buf, ent->ExternalID);
		 xmlBufferWriteChar(buf, " ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    } else {
		 xmlBufferWriteChar(buf, " SYSTEM ");
		 xmlBufferWriteQuotedString(buf, ent->SystemID);
	    }
	    xmlBufferWriteChar(buf, ">\n");
	    break;
	default:
	    xmlEntitiesErr(XML_DTD_UNKNOWN_ENTITY,
		"xmlDumpEntitiesDecl: internal: unknown type entity type");
    }
}

/**
 * xmlDumpEntityDeclScan:
 * @ent:  An entity table
 * @buf:  An XML buffer.
 *
 * When using the hash table scan function, arguments need to be reversed
 */
static void
xmlDumpEntityDeclScan(xmlEntityPtr ent, xmlBufferPtr buf) {
    xmlDumpEntityDecl(buf, ent);
}
      
/**
 * xmlDumpEntitiesTable:
 * @buf:  An XML buffer.
 * @table:  An entity table
 *
 * This will dump the content of the entity table as an XML DTD definition
 */
void
xmlDumpEntitiesTable(xmlBufferPtr buf, xmlEntitiesTablePtr table) {
    xmlHashScan(table, (xmlHashScanner)xmlDumpEntityDeclScan, buf);
}
#endif /* LIBXML_OUTPUT_ENABLED */
#define bottom_entities
#include "elfgcchack.h"