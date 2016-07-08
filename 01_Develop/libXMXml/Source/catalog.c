/**
 * catalog.c: set of generic Catalog related routines 
 *
 * Reference:  SGML Open Technical Resolution TR9401:1997.
 *             http://www.jclark.com/sp/catalog.htm
 *
 *             XML Catalogs Working Draft 06 August 2001
 *             http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * See Copyright for the status of this software.
 *
 * Daniel.Veillard@imag.fr
 */

#define IN_LIBXML
#include "libxml.h"

#ifdef LIBXML_CATALOG_ENABLED
#include <XMXML/xmlmemory.h>
#include <XMXML/hash.h>
#include <XMXML/uri.h>
#include <XMXML/parserInternals.h>
#include <XMXML/catalog.h>
#include <XMXML/xmlerror.h>
#include <XMXML/threads.h>
#include <XMXML/globals.h>

#define MAX_DELEGATE	50
#define MAX_CATAL_DEPTH	50

#ifdef _WIN32
# define PATH_SEAPARATOR ';'
#else
# define PATH_SEAPARATOR ':'
#endif

/**
 * TODO:
 *
 * macro to flag unimplemented blocks
 * XML_CATALOG_PREFER user env to select between system/public prefered
 * option. C.f. Richard Tobin <richard@cogsci.ed.ac.uk>
 *> Just FYI, I am using an environment variable XML_CATALOG_PREFER with
 *> values "system" and "public".  I have made the default be "system" to
 *> match yours.
 */
#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);

#define XML_URN_PUBID "urn:publicid:"
#define XML_CATAL_BREAK ((xmlChar *) -1)
#ifndef XML_XML_DEFAULT_CATALOG
#define XML_XML_DEFAULT_CATALOG "file:///etc/xml/catalog"
#endif
#ifndef XML_SGML_DEFAULT_CATALOG
#define XML_SGML_DEFAULT_CATALOG "file:///etc/sgml/catalog"
#endif

static xmlChar *xmlCatalogNormalizePublic(const xmlChar *pubID);
static int xmlExpandCatalog(xmlCatalogPtr catal, const char *filename);

/************************************************************************
 *									*
 *			Types, all private				*
 *									*
 ************************************************************************/

typedef enum {
    XML_CATA_REMOVED = -1,
    XML_CATA_NONE = 0,
    XML_CATA_CATALOG,
    XML_CATA_BROKEN_CATALOG,
    XML_CATA_NEXT_CATALOG,
    XML_CATA_GROUP,
    XML_CATA_PUBLIC,
    XML_CATA_SYSTEM,
    XML_CATA_REWRITE_SYSTEM,
    XML_CATA_DELEGATE_PUBLIC,
    XML_CATA_DELEGATE_SYSTEM,
    XML_CATA_URI,
    XML_CATA_REWRITE_URI,
    XML_CATA_DELEGATE_URI,
    SGML_CATA_SYSTEM,
    SGML_CATA_PUBLIC,
    SGML_CATA_ENTITY,
    SGML_CATA_PENTITY,
    SGML_CATA_DOCTYPE,
    SGML_CATA_LINKTYPE,
    SGML_CATA_NOTATION,
    SGML_CATA_DELEGATE,
    SGML_CATA_BASE,
    SGML_CATA_CATALOG,
    SGML_CATA_DOCUMENT,
    SGML_CATA_SGMLDECL
} xmlCatalogEntryType;

typedef struct _xmlCatalogEntry xmlCatalogEntry;
typedef xmlCatalogEntry *xmlCatalogEntryPtr;
struct _xmlCatalogEntry {
    struct _xmlCatalogEntry *next;
    struct _xmlCatalogEntry *parent;
    struct _xmlCatalogEntry *children;
    xmlCatalogEntryType type;
    xmlChar *name;
    xmlChar *value;
    xmlChar *URL;  /* The expanded URL using the base */
    xmlCatalogPrefer prefer;
    int dealloc;
    int depth;
    struct _xmlCatalogEntry *group;
};

typedef enum {
    XML_XML_CATALOG_TYPE = 1,
    XML_SGML_CATALOG_TYPE
} xmlCatalogType;

#define XML_MAX_SGML_CATA_DEPTH 10
struct _xmlCatalog {
    xmlCatalogType type;	/* either XML or SGML */

    /*
     * SGML Catalogs are stored as a simple hash table of catalog entries
     * Catalog stack to check against overflows when building the
     * SGML catalog
     */
    char *catalTab[XML_MAX_SGML_CATA_DEPTH];	/* stack of catals */
    int          catalNr;	/* Number of current catal streams */
    int          catalMax;	/* Max number of catal streams */
    xmlHashTablePtr sgml;

    /*
     * XML Catalogs are stored as a tree of Catalog entries
     */
    xmlCatalogPrefer prefer;
    xmlCatalogEntryPtr xml;
};

/************************************************************************
 *									*
 *			Global variables				*
 *									*
 ************************************************************************/

/*
 * Those are preferences
 */
static int xmlDebugCatalogs = 0;   /* used for debugging */
static xmlCatalogAllow xmlCatalogDefaultAllow = XML_CATA_ALLOW_ALL;
static xmlCatalogPrefer xmlCatalogDefaultPrefer = XML_CATA_PREFER_PUBLIC;

/*
 * Hash table containing all the trees of XML catalogs parsed by
 * the application.
 */
static xmlHashTablePtr xmlCatalogXMLFiles = KD_NULL;

/*
 * The default catalog in use by the application
 */
static xmlCatalogPtr xmlDefaultCatalog = KD_NULL;

/*
 * A mutex for modifying the shared global catalog(s)
 * xmlDefaultCatalog tree.
 * It also protects xmlCatalogXMLFiles
 * The core of this readers/writer scheme is in xmlFetchXMLCatalogFile()
 */
static xmlRMutexPtr xmlCatalogMutex = KD_NULL;

/*
 * Whether the catalog support was initialized.
 */
static int xmlCatalogInitialized = 0;

/************************************************************************
 *									*
 * 			Catalog error handlers				*
 *									*
 ************************************************************************/

/**
 * xmlCatalogErrMemory:
 * @extra:  extra informations
 *
 * Handle an out of memory condition
 */
static void
xmlCatalogErrMemory(const char *extra)
{
    __xmlRaiseError(KD_NULL, KD_NULL, KD_NULL, KD_NULL, KD_NULL, XML_FROM_CATALOG,
                    XML_ERR_NO_MEMORY, XML_ERR_ERROR, KD_NULL, 0,
		    extra, KD_NULL, KD_NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}

/**
 * xmlCatalogErr:
 * @catal: the Catalog entry
 * @node: the context node
 * @msg:  the error message
 * @extra:  extra informations
 *
 * Handle a catalog error
 */
static void
xmlCatalogErr(xmlCatalogEntryPtr catal, xmlNodePtr node, int error,
               const char *msg, const xmlChar *str1, const xmlChar *str2,
	       const xmlChar *str3)
{
    __xmlRaiseError(KD_NULL, KD_NULL, KD_NULL, catal, node, XML_FROM_CATALOG,
                    error, XML_ERR_ERROR, KD_NULL, 0,
		    (const char *) str1, (const char *) str2,
		    (const char *) str3, 0, 0,
		    msg, str1, str2, str3);
}


/************************************************************************
 *									*
 *			Allocation and Freeing				*
 *									*
 ************************************************************************/

/**
 * xmlNewCatalogEntry:
 * @type:  type of entry
 * @name:  name of the entry
 * @value:  value of the entry
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 * @group:  for members of a group, the group entry 
 *
 * create a new Catalog entry, this type is shared both by XML and 
 * SGML catalogs, but the acceptable types values differs.
 *
 * Returns the xmlCatalogEntryPtr or KD_NULL in case of error
 */
static xmlCatalogEntryPtr
xmlNewCatalogEntry(xmlCatalogEntryType type, const xmlChar *name,
	   const xmlChar *value, const xmlChar *URL, xmlCatalogPrefer prefer,
	   xmlCatalogEntryPtr group) {
    xmlCatalogEntryPtr ret;
    xmlChar *normid = KD_NULL;

    ret = (xmlCatalogEntryPtr) xmlMalloc(sizeof(xmlCatalogEntry));
    if (ret == KD_NULL) {
        xmlCatalogErrMemory("allocating catalog entry");
	return(KD_NULL);
    }
    ret->next = KD_NULL;
    ret->parent = KD_NULL;
    ret->children = KD_NULL;
    ret->type = type;
    if (type == XML_CATA_PUBLIC || type == XML_CATA_DELEGATE_PUBLIC) {
        normid = xmlCatalogNormalizePublic(name);
        if (normid != KD_NULL)
            name = (*normid != 0 ? normid : KD_NULL);
    }
    if (name != KD_NULL)
	ret->name = xmlStrdup(name);
    else
	ret->name = KD_NULL;
    if (normid != KD_NULL)
        xmlFree(normid);
    if (value != KD_NULL)
	ret->value = xmlStrdup(value);
    else
	ret->value = KD_NULL;
    if (URL == KD_NULL)
	URL = value;
    if (URL != KD_NULL)
	ret->URL = xmlStrdup(URL);
    else
	ret->URL = KD_NULL;
    ret->prefer = prefer;
    ret->dealloc = 0;
    ret->depth = 0;
    ret->group = group;
    return(ret);
}

static void
xmlFreeCatalogEntryList(xmlCatalogEntryPtr ret);

/**
 * xmlFreeCatalogEntry:
 * @ret:  a Catalog entry
 *
 * Free the memory allocated to a Catalog entry
 */
static void
xmlFreeCatalogEntry(xmlCatalogEntryPtr ret) {
    if (ret == KD_NULL)
	return;
    /*
     * Entries stored in the file hash must be deallocated
     * only by the file hash cleaner !
     */
    if (ret->dealloc == 1)
	return;

    if (xmlDebugCatalogs) {
	if (ret->name != KD_NULL)
	    xmlGenericError(xmlGenericErrorContext,
		    "Free catalog entry %s\n", ret->name);
	else if (ret->value != KD_NULL)
	    xmlGenericError(xmlGenericErrorContext,
		    "Free catalog entry %s\n", ret->value);
	else
	    xmlGenericError(xmlGenericErrorContext,
		    "Free catalog entry\n");
    }

    if (ret->name != KD_NULL)
	xmlFree(ret->name);
    if (ret->value != KD_NULL)
	xmlFree(ret->value);
    if (ret->URL != KD_NULL)
	xmlFree(ret->URL);
    xmlFree(ret);
}

/**
 * xmlFreeCatalogEntryList:
 * @ret:  a Catalog entry list
 *
 * Free the memory allocated to a full chained list of Catalog entries
 */
static void
xmlFreeCatalogEntryList(xmlCatalogEntryPtr ret) {
    xmlCatalogEntryPtr next;

    while (ret != KD_NULL) {
	next = ret->next;
	xmlFreeCatalogEntry(ret);
	ret = next;
    }
}

/**
 * xmlFreeCatalogHashEntryList:
 * @ret:  a Catalog entry list
 *
 * Free the memory allocated to list of Catalog entries from the
 * catalog file hash.
 */
static void
xmlFreeCatalogHashEntryList(xmlCatalogEntryPtr catal) {
    xmlCatalogEntryPtr children, next;

    if (catal == KD_NULL)
	return;

    children = catal->children;
    while (children != KD_NULL) {
	next = children->next;
	children->dealloc = 0;
	children->children = KD_NULL;
	xmlFreeCatalogEntry(children);
	children = next;
    }
    catal->dealloc = 0;
    xmlFreeCatalogEntry(catal);
}

/**
 * xmlCreateNewCatalog:
 * @type:  type of catalog
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 *
 * create a new Catalog, this type is shared both by XML and 
 * SGML catalogs, but the acceptable types values differs.
 *
 * Returns the xmlCatalogPtr or KD_NULL in case of error
 */
static xmlCatalogPtr
xmlCreateNewCatalog(xmlCatalogType type, xmlCatalogPrefer prefer) {
    xmlCatalogPtr ret;

    ret = (xmlCatalogPtr) xmlMalloc(sizeof(xmlCatalog));
    if (ret == KD_NULL) {
        xmlCatalogErrMemory("allocating catalog");
	return(KD_NULL);
    }
    kdMemset (ret, 0, sizeof(xmlCatalog));
    ret->type = type;
    ret->catalNr = 0;
    ret->catalMax = XML_MAX_SGML_CATA_DEPTH;
    ret->prefer = prefer;
    if (ret->type == XML_SGML_CATALOG_TYPE)
	ret->sgml = xmlHashCreate(10);
    return(ret);
}

/**
 * xmlFreeCatalog:
 * @catal:  a Catalog
 *
 * Free the memory allocated to a Catalog
 */
void
xmlFreeCatalog(xmlCatalogPtr catal) {
    if (catal == KD_NULL)
	return;
    if (catal->xml != KD_NULL)
	xmlFreeCatalogEntryList(catal->xml);
    if (catal->sgml != KD_NULL)
	xmlHashFree(catal->sgml,
		(xmlHashDeallocator) xmlFreeCatalogEntry);
    xmlFree(catal);
}

/************************************************************************
 *									*
 *			Serializing Catalogs				*
 *									*
 ************************************************************************/

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlCatalogDumpEntry:
 * @entry:  the catalog entry
 * @out:  the file.
 *
 * Serialize an SGML Catalog entry
 */
static void
xmlCatalogDumpEntry(xmlCatalogEntryPtr entry, KDFile *out) {
    if ((entry == KD_NULL) || (out == KD_NULL))
	return;
    switch (entry->type) {
	case SGML_CATA_ENTITY:
	    kdFprintfKHR (out, "ENTITY "); break;
	case SGML_CATA_PENTITY:
	    kdFprintfKHR (out, "ENTITY %%"); break;
	case SGML_CATA_DOCTYPE:
	    kdFprintfKHR (out, "DOCTYPE "); break;
	case SGML_CATA_LINKTYPE:
	    kdFprintfKHR (out, "LINKTYPE "); break;
	case SGML_CATA_NOTATION:
	    kdFprintfKHR (out, "NOTATION "); break;
	case SGML_CATA_PUBLIC:
	    kdFprintfKHR (out, "PUBLIC "); break;
	case SGML_CATA_SYSTEM:
	    kdFprintfKHR (out, "SYSTEM "); break;
	case SGML_CATA_DELEGATE:
	    kdFprintfKHR (out, "DELEGATE "); break;
	case SGML_CATA_BASE:
	    kdFprintfKHR (out, "BASE "); break;
	case SGML_CATA_CATALOG:
	    kdFprintfKHR (out, "CATALOG "); break;
	case SGML_CATA_DOCUMENT:
	    kdFprintfKHR (out, "DOCUMENT "); break;
	case SGML_CATA_SGMLDECL:
	    kdFprintfKHR (out, "SGMLDECL "); break;
	default:
	    return;
    }
    switch (entry->type) {
	case SGML_CATA_ENTITY:
	case SGML_CATA_PENTITY:
	case SGML_CATA_DOCTYPE:
	case SGML_CATA_LINKTYPE:
	case SGML_CATA_NOTATION:
	    kdFprintfKHR (out, "%s", (const char *) entry->name); break;
	case SGML_CATA_PUBLIC:
	case SGML_CATA_SYSTEM:
	case SGML_CATA_SGMLDECL:
	case SGML_CATA_DOCUMENT:
	case SGML_CATA_CATALOG:
	case SGML_CATA_BASE:
	case SGML_CATA_DELEGATE:
	    kdFprintfKHR (out, "\"%s\"", entry->name); break;
	default:
	    break;
    }
    switch (entry->type) {
	case SGML_CATA_ENTITY:
	case SGML_CATA_PENTITY:
	case SGML_CATA_DOCTYPE:
	case SGML_CATA_LINKTYPE:
	case SGML_CATA_NOTATION:
	case SGML_CATA_PUBLIC:
	case SGML_CATA_SYSTEM:
	case SGML_CATA_DELEGATE:
	    kdFprintfKHR (out, " \"%s\"", entry->value); break;
	default:
	    break;
    }
    kdFprintfKHR (out, "\n");
}

/**
 * xmlDumpXMLCatalogNode:
 * @catal:  top catalog entry
 * @catalog: pointer to the xml tree
 * @doc: the containing document
 * @ns: the current namespace
 * @cgroup: group node for group members
 *
 * Serializes a Catalog entry, called by xmlDumpXMLCatalog and recursively
 * for group entries
 */
static void xmlDumpXMLCatalogNode(xmlCatalogEntryPtr catal, xmlNodePtr catalog,
		    xmlDocPtr doc, xmlNsPtr ns, xmlCatalogEntryPtr cgroup) {
    xmlNodePtr node;
    xmlCatalogEntryPtr cur;
    /*
     * add all the catalog entries
     */
    cur = catal;
    while (cur != KD_NULL) {
        if (cur->group == cgroup) {
	    switch (cur->type) {
	        case XML_CATA_REMOVED:
		    break;
	        case XML_CATA_BROKEN_CATALOG:
	        case XML_CATA_CATALOG:
		    if (cur == catal) {
			cur = cur->children;
		        continue;
		    }
		    break;
		case XML_CATA_NEXT_CATALOG:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "nextCatalog", KD_NULL);
		    xmlSetProp(node, BAD_CAST "catalog", cur->value);
		    xmlAddChild(catalog, node);
                    break;
		case XML_CATA_NONE:
		    break;
		case XML_CATA_GROUP:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "group", KD_NULL);
		    xmlSetProp(node, BAD_CAST "id", cur->name);
		    if (cur->value != KD_NULL) {
		        xmlNsPtr xns;
			xns = xmlSearchNsByHref(doc, node, XML_XML_NAMESPACE);
			if (xns != KD_NULL)
			    xmlSetNsProp(node, xns, BAD_CAST "base",
			    		 cur->value);
		    }
		    switch (cur->prefer) {
			case XML_CATA_PREFER_NONE:
		            break;
			case XML_CATA_PREFER_PUBLIC:
		            xmlSetProp(node, BAD_CAST "prefer", BAD_CAST "public");
			    break;
			case XML_CATA_PREFER_SYSTEM:
		            xmlSetProp(node, BAD_CAST "prefer", BAD_CAST "system");
			    break;
		    }
		    xmlDumpXMLCatalogNode(cur->next, node, doc, ns, cur);
		    xmlAddChild(catalog, node);
	            break;
		case XML_CATA_PUBLIC:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "public", KD_NULL);
		    xmlSetProp(node, BAD_CAST "publicId", cur->name);
		    xmlSetProp(node, BAD_CAST "uri", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_SYSTEM:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "system", KD_NULL);
		    xmlSetProp(node, BAD_CAST "systemId", cur->name);
		    xmlSetProp(node, BAD_CAST "uri", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_REWRITE_SYSTEM:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "rewriteSystem", KD_NULL);
		    xmlSetProp(node, BAD_CAST "systemIdStartString", cur->name);
		    xmlSetProp(node, BAD_CAST "rewritePrefix", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_DELEGATE_PUBLIC:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "delegatePublic", KD_NULL);
		    xmlSetProp(node, BAD_CAST "publicIdStartString", cur->name);
		    xmlSetProp(node, BAD_CAST "catalog", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_DELEGATE_SYSTEM:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "delegateSystem", KD_NULL);
		    xmlSetProp(node, BAD_CAST "systemIdStartString", cur->name);
		    xmlSetProp(node, BAD_CAST "catalog", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_URI:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "uri", KD_NULL);
		    xmlSetProp(node, BAD_CAST "name", cur->name);
		    xmlSetProp(node, BAD_CAST "uri", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_REWRITE_URI:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "rewriteURI", KD_NULL);
		    xmlSetProp(node, BAD_CAST "uriStartString", cur->name);
		    xmlSetProp(node, BAD_CAST "rewritePrefix", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case XML_CATA_DELEGATE_URI:
		    node = xmlNewDocNode(doc, ns, BAD_CAST "delegateURI", KD_NULL);
		    xmlSetProp(node, BAD_CAST "uriStartString", cur->name);
		    xmlSetProp(node, BAD_CAST "catalog", cur->value);
		    xmlAddChild(catalog, node);
		    break;
		case SGML_CATA_SYSTEM:
		case SGML_CATA_PUBLIC:
		case SGML_CATA_ENTITY:
		case SGML_CATA_PENTITY:
		case SGML_CATA_DOCTYPE:
		case SGML_CATA_LINKTYPE:
		case SGML_CATA_NOTATION:
		case SGML_CATA_DELEGATE:
		case SGML_CATA_BASE:
		case SGML_CATA_CATALOG:
		case SGML_CATA_DOCUMENT:
		case SGML_CATA_SGMLDECL:
		    break;
	    }
        }
	cur = cur->next;
    }
}

static int
xmlDumpXMLCatalog(KDFile *out, xmlCatalogEntryPtr catal) {
    int ret;
    xmlDocPtr doc;
    xmlNsPtr ns;
    xmlDtdPtr dtd;
    xmlNodePtr catalog;
    xmlOutputBufferPtr buf;

    /*
     * Rebuild a catalog
     */
    doc = xmlNewDoc(KD_NULL);
    if (doc == KD_NULL)
	return(-1);
    dtd = xmlNewDtd(doc, BAD_CAST "catalog",
	       BAD_CAST "-//OASIS//DTD Entity Resolution XML Catalog V1.0//EN",
BAD_CAST "http://www.oasis-open.org/committees/entity/release/1.0/catalog.dtd");

    xmlAddChild((xmlNodePtr) doc, (xmlNodePtr) dtd);

    ns = xmlNewNs(KD_NULL, XML_CATALOGS_NAMESPACE, KD_NULL);
    if (ns == KD_NULL) {
	xmlFreeDoc(doc);
	return(-1);
    }
    catalog = xmlNewDocNode(doc, ns, BAD_CAST "catalog", KD_NULL);
    if (catalog == KD_NULL) {
	xmlFreeNs(ns);
	xmlFreeDoc(doc);
	return(-1);
    }
    catalog->nsDef = ns;
    xmlAddChild((xmlNodePtr) doc, catalog);

    xmlDumpXMLCatalogNode(catal, catalog, doc, ns, KD_NULL);
    
    /*
     * reserialize it
     */
    buf = xmlOutputBufferCreateFile(out, KD_NULL);
    if (buf == KD_NULL) {
	xmlFreeDoc(doc);
	return(-1);
    }
    ret = xmlSaveFormatFileTo(buf, doc, KD_NULL, 1);

    /*
     * Free it
     */
    xmlFreeDoc(doc);

    return(ret);
}
#endif /* LIBXML_OUTPUT_ENABLED */

/************************************************************************
 *									*
 *			Converting SGML Catalogs to XML			*
 *									*
 ************************************************************************/

/**
 * xmlCatalogConvertEntry:
 * @entry:  the entry
 * @catal:  pointer to the catalog being converted
 *
 * Convert one entry from the catalog
 */
static void
xmlCatalogConvertEntry(xmlCatalogEntryPtr entry, xmlCatalogPtr catal) {
    if ((entry == KD_NULL) || (catal == KD_NULL) || (catal->sgml == KD_NULL) ||
	(catal->xml == KD_NULL))
	return;
    switch (entry->type) {
	case SGML_CATA_ENTITY:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_PENTITY:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_DOCTYPE:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_LINKTYPE:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_NOTATION:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_PUBLIC:
	    entry->type = XML_CATA_PUBLIC;
	    break;
	case SGML_CATA_SYSTEM:
	    entry->type = XML_CATA_SYSTEM;
	    break;
	case SGML_CATA_DELEGATE:
	    entry->type = XML_CATA_DELEGATE_PUBLIC;
	    break;
	case SGML_CATA_CATALOG:
	    entry->type = XML_CATA_CATALOG;
	    break;
	default:
	    xmlHashRemoveEntry(catal->sgml, entry->name,
		               (xmlHashDeallocator) xmlFreeCatalogEntry);
	    return;
    }
    /*
     * Conversion successful, remove from the SGML catalog
     * and add it to the default XML one
     */
    xmlHashRemoveEntry(catal->sgml, entry->name, KD_NULL);
    entry->parent = catal->xml;
    entry->next = KD_NULL;
    if (catal->xml->children == KD_NULL)
	catal->xml->children = entry;
    else {
	xmlCatalogEntryPtr prev;

	prev = catal->xml->children;
	while (prev->next != KD_NULL)
	    prev = prev->next;
	prev->next = entry;
    }
}

/**
 * xmlConvertSGMLCatalog:
 * @catal: the catalog
 *
 * Convert all the SGML catalog entries as XML ones
 *
 * Returns the number of entries converted if successful, -1 otherwise
 */
int
xmlConvertSGMLCatalog(xmlCatalogPtr catal) {

    if ((catal == KD_NULL) || (catal->type != XML_SGML_CATALOG_TYPE))
	return(-1);

    if (xmlDebugCatalogs) {
	xmlGenericError(xmlGenericErrorContext,
		"Converting SGML catalog to XML\n");
    }
    xmlHashScan(catal->sgml,
		(xmlHashScanner) xmlCatalogConvertEntry,
		&catal);
    return(0);
}

/************************************************************************
 *									*
 *			Helper function					*
 *									*
 ************************************************************************/

/**
 * xmlCatalogUnWrapURN:
 * @urn:  an "urn:publicid:" to unwrap
 *
 * Expand the URN into the equivalent Public Identifier
 *
 * Returns the new identifier or KD_NULL, the string must be deallocated
 *         by the caller.
 */
static xmlChar *
xmlCatalogUnWrapURN(const xmlChar *urn) {
    xmlChar result[2000];
    unsigned int i = 0;

    if (xmlStrncmp(urn, BAD_CAST XML_URN_PUBID, sizeof(XML_URN_PUBID) - 1))
	return(KD_NULL);
    urn += sizeof(XML_URN_PUBID) - 1;
    
    while (*urn != 0) {
	if (i > sizeof(result) - 4)
	    break;
	if (*urn == '+') {
	    result[i++] = ' ';
	    urn++;
	} else if (*urn == ':') {
	    result[i++] = '/';
	    result[i++] = '/';
	    urn++;
	} else if (*urn == ';') {
	    result[i++] = ':';
	    result[i++] = ':';
	    urn++;
	} else if (*urn == '%') {
	    if ((urn[1] == '2') && (urn[2] == 'B'))
		result[i++] = '+';
	    else if ((urn[1] == '3') && (urn[2] == 'A'))
		result[i++] = ':';
	    else if ((urn[1] == '2') && (urn[2] == 'F'))
		result[i++] = '/';
	    else if ((urn[1] == '3') && (urn[2] == 'B'))
		result[i++] = ';';
	    else if ((urn[1] == '2') && (urn[2] == '7'))
		result[i++] = '\'';
	    else if ((urn[1] == '3') && (urn[2] == 'F'))
		result[i++] = '?';
	    else if ((urn[1] == '2') && (urn[2] == '3'))
		result[i++] = '#';
	    else if ((urn[1] == '2') && (urn[2] == '5'))
		result[i++] = '%';
	    else {
		result[i++] = *urn;
		urn++;
		continue;
	    }
	    urn += 3;
	} else {
	    result[i++] = *urn;
	    urn++;
	}
    }
    result[i] = 0;

    return(xmlStrdup(result));
}

/**
 * xmlParseCatalogFile:
 * @filename:  the filename
 *
 * parse an XML file and build a tree. It's like xmlParseFile()
 * except it bypass all catalog lookups.
 *
 * Returns the resulting document tree or KD_NULL in case of error
 */

xmlDocPtr
xmlParseCatalogFile(const char *filename) {
    xmlDocPtr ret;
    xmlParserCtxtPtr ctxt;
    char *directory = KD_NULL;
    xmlParserInputPtr inputStream;
    xmlParserInputBufferPtr buf;

    ctxt = xmlNewParserCtxt();
    if (ctxt == KD_NULL) {
#ifdef LIBXML_SAX1_ENABLED
	if (xmlDefaultSAXHandler.error != KD_NULL) {
	    xmlDefaultSAXHandler.error(KD_NULL, "out of memory\n");
	}
#endif
	return(KD_NULL);
    }

    buf = xmlParserInputBufferCreateFilename(filename, XML_CHAR_ENCODING_NONE);
    if (buf == KD_NULL) {
	xmlFreeParserCtxt(ctxt);
	return(KD_NULL);
    }

    inputStream = xmlNewInputStream(ctxt);
    if (inputStream == KD_NULL) {
	xmlFreeParserCtxt(ctxt);
	return(KD_NULL);
    }

    inputStream->filename = (char *) xmlCanonicPath((const xmlChar *)filename);
    inputStream->buf = buf;
    inputStream->base = inputStream->buf->buffer->content;
    inputStream->cur = inputStream->buf->buffer->content;
    inputStream->end = 
	&inputStream->buf->buffer->content[inputStream->buf->buffer->use];

    inputPush(ctxt, inputStream);
    if ((ctxt->directory == KD_NULL) && (directory == KD_NULL))
        directory = xmlParserGetDirectory(filename);
    if ((ctxt->directory == KD_NULL) && (directory != KD_NULL))
        ctxt->directory = directory;
    ctxt->valid = 0;
    ctxt->validate = 0;
    ctxt->loadsubset = 0;
    ctxt->pedantic = 0;
    ctxt->dictNames = 1;

    xmlParseDocument(ctxt);

    if (ctxt->wellFormed)
	ret = ctxt->myDoc;
    else {
        ret = KD_NULL;
        xmlFreeDoc(ctxt->myDoc);
        ctxt->myDoc = KD_NULL;
    }
    xmlFreeParserCtxt(ctxt);
    
    return(ret);
}

/**
 * xmlLoadFileContent:
 * @filename:  a file path
 *
 * Load a file content into memory.
 *
 * Returns a pointer to the 0 terminated string or KD_NULL in case of error
 */
static xmlChar *
xmlLoadFileContent(const char *filename)
{
	KDFile*         fd;
    KDint           len;
    KDsize          size;
	struct KDStat   info;

    xmlChar*        content;

    if ( filename == KD_NULL )
	{
        return ( KD_NULL );
	}

    if ( kdStat ( filename, &info ) < 0 )
	{
        return ( KD_NULL );
	}

	size = info.st_size;

	if ( ( fd = kdFopen ( filename, "rb" ) ) == KD_NULL )
	{
		return ( KD_NULL );
	}	

    content = (xmlChar*) xmlMallocAtomic ( size + 10 );
    if ( content == KD_NULL )
	{
        xmlCatalogErrMemory ( "allocating catalog data" );
        return ( KD_NULL );
    }

    len = kdFread ( content, 1, size, fd );
    kdFclose ( fd );

    if ( len < 0 ) 
	{
        xmlFree ( content );
        return ( KD_NULL );
    }
    content[ len ] = 0;

    return ( content );
}

/**
 * xmlCatalogNormalizePublic:
 * @pubID:  the public ID string
 *
 *  Normalizes the Public Identifier
 *
 * Implements 6.2. Public Identifier Normalization
 * from http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * Returns the new string or KD_NULL, the string must be deallocated
 *         by the caller.
 */
static xmlChar *
xmlCatalogNormalizePublic(const xmlChar *pubID)
{
    int ok = 1;
    int white;
    const xmlChar *p;
    xmlChar *ret;
    xmlChar *q;

    if (pubID == KD_NULL )
        return ( KD_NULL );

    white = 1;
    for (p = pubID;*p != 0 && ok;p++) {
        if (!xmlIsBlank_ch(*p))
            white = 0;
        else if (*p == 0x20 && !white)
            white = 1;
        else
            ok = 0;
    }
    if (ok && !white)	/* is normalized */
        return(KD_NULL);

    ret = xmlStrdup(pubID);
    q = ret;
    white = 0;
    for (p = pubID;*p != 0;p++) {
        if (xmlIsBlank_ch(*p)) {
            if (q != ret)
                white = 1;
        } else {
            if (white) {
                *(q++) = 0x20;
                white = 0;
            }
            *(q++) = *p;
        }
    }
    *q = 0;
    return(ret);
}

/************************************************************************
 *									*
 *			The XML Catalog parser				*
 *									*
 ************************************************************************/

static xmlCatalogEntryPtr
xmlParseXMLCatalogFile(xmlCatalogPrefer prefer, const xmlChar *filename);
static void
xmlParseXMLCatalogNodeList(xmlNodePtr cur, xmlCatalogPrefer prefer,
	                   xmlCatalogEntryPtr parent, xmlCatalogEntryPtr cgroup);
static xmlChar *
xmlCatalogListXMLResolve(xmlCatalogEntryPtr catal, const xmlChar *pubID,
	              const xmlChar *sysID);
static xmlChar *
xmlCatalogListXMLResolveURI(xmlCatalogEntryPtr catal, const xmlChar *URI);


/**
 * xmlGetXMLCatalogEntryType:
 * @name:  the name
 *
 * lookup the internal type associated to an XML catalog entry name
 *
 * Returns the type associated with that name
 */
static xmlCatalogEntryType
xmlGetXMLCatalogEntryType(const xmlChar *name) {
    xmlCatalogEntryType type = XML_CATA_NONE;
    if (xmlStrEqual(name, (const xmlChar *) "system"))
	type = XML_CATA_SYSTEM;
    else if (xmlStrEqual(name, (const xmlChar *) "public"))
	type = XML_CATA_PUBLIC;
    else if (xmlStrEqual(name, (const xmlChar *) "rewriteSystem"))
	type = XML_CATA_REWRITE_SYSTEM;
    else if (xmlStrEqual(name, (const xmlChar *) "delegatePublic"))
	type = XML_CATA_DELEGATE_PUBLIC;
    else if (xmlStrEqual(name, (const xmlChar *) "delegateSystem"))
	type = XML_CATA_DELEGATE_SYSTEM;
    else if (xmlStrEqual(name, (const xmlChar *) "uri"))
	type = XML_CATA_URI;
    else if (xmlStrEqual(name, (const xmlChar *) "rewriteURI"))
	type = XML_CATA_REWRITE_URI;
    else if (xmlStrEqual(name, (const xmlChar *) "delegateURI"))
	type = XML_CATA_DELEGATE_URI;
    else if (xmlStrEqual(name, (const xmlChar *) "nextCatalog"))
	type = XML_CATA_NEXT_CATALOG;
    else if (xmlStrEqual(name, (const xmlChar *) "catalog"))
	type = XML_CATA_CATALOG;
    return(type);
}

/**
 * xmlParseXMLCatalogOneNode:
 * @cur:  the XML node
 * @type:  the type of Catalog entry
 * @name:  the name of the node
 * @attrName:  the attribute holding the value
 * @uriAttrName:  the attribute holding the URI-Reference
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 * @cgroup:  the group which includes this node
 *
 * Finishes the examination of an XML tree node of a catalog and build
 * a Catalog entry from it.
 *
 * Returns the new Catalog entry node or KD_NULL in case of error.
 */
static xmlCatalogEntryPtr
xmlParseXMLCatalogOneNode(xmlNodePtr cur, xmlCatalogEntryType type,
			  const xmlChar *name, const xmlChar *attrName,
			  const xmlChar *uriAttrName, xmlCatalogPrefer prefer,
			  xmlCatalogEntryPtr cgroup) {
    int ok = 1;
    xmlChar *uriValue;
    xmlChar *nameValue = KD_NULL;
    xmlChar *base = KD_NULL;
    xmlChar *URL = KD_NULL;
    xmlCatalogEntryPtr ret = KD_NULL;

    if (attrName != KD_NULL) {
	nameValue = xmlGetProp(cur, attrName);
	if (nameValue == KD_NULL) {
	    xmlCatalogErr(ret, cur, XML_CATALOG_MISSING_ATTR,
			  "%s entry lacks '%s'\n", name, attrName, KD_NULL);
	    ok = 0;
	}
    }
    uriValue = xmlGetProp(cur, uriAttrName);
    if (uriValue == KD_NULL) {
	xmlCatalogErr(ret, cur, XML_CATALOG_MISSING_ATTR,
		"%s entry lacks '%s'\n", name, uriAttrName, KD_NULL);
	ok = 0;
    }
    if (!ok) {
	if (nameValue != KD_NULL)
	    xmlFree(nameValue);
	if (uriValue != KD_NULL)
	    xmlFree(uriValue);
	return(KD_NULL);
    }

    base = xmlNodeGetBase(cur->doc, cur);
    URL = xmlBuildURI(uriValue, base);
    if (URL != KD_NULL) {
	if (xmlDebugCatalogs > 1) {
	    if (nameValue != KD_NULL)
		xmlGenericError(xmlGenericErrorContext,
			"Found %s: '%s' '%s'\n", name, nameValue, URL);
	    else
		xmlGenericError(xmlGenericErrorContext,
			"Found %s: '%s'\n", name, URL);
	}
	ret = xmlNewCatalogEntry(type, nameValue, uriValue, URL, prefer, cgroup);
    } else {
	xmlCatalogErr(ret, cur, XML_CATALOG_ENTRY_BROKEN,
		"%s entry '%s' broken ?: %s\n", name, uriAttrName, uriValue);
    }
    if (nameValue != KD_NULL)
	xmlFree(nameValue);
    if (uriValue != KD_NULL)
	xmlFree(uriValue);
    if (base != KD_NULL)
	xmlFree(base);
    if (URL != KD_NULL)
	xmlFree(URL);
    return(ret);
}

/**
 * xmlParseXMLCatalogNode:
 * @cur:  the XML node
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 * @parent:  the parent Catalog entry
 * @cgroup:  the group which includes this node
 *
 * Examines an XML tree node of a catalog and build
 * a Catalog entry from it adding it to its parent. The examination can
 * be recursive.
 */
static void
xmlParseXMLCatalogNode(xmlNodePtr cur, xmlCatalogPrefer prefer,
	               xmlCatalogEntryPtr parent, xmlCatalogEntryPtr cgroup)
{
    xmlChar *base = KD_NULL;
    xmlCatalogEntryPtr entry = KD_NULL;

    if (cur == KD_NULL)
        return;
    if (xmlStrEqual(cur->name, BAD_CAST "group")) {
        xmlChar *prop;
	xmlCatalogPrefer pref = XML_CATA_PREFER_NONE;

        prop = xmlGetProp(cur, BAD_CAST "prefer");
        if (prop != KD_NULL) {
            if (xmlStrEqual(prop, BAD_CAST "system")) {
                prefer = XML_CATA_PREFER_SYSTEM;
            } else if (xmlStrEqual(prop, BAD_CAST "public")) {
                prefer = XML_CATA_PREFER_PUBLIC;
            } else {
		xmlCatalogErr(parent, cur, XML_CATALOG_PREFER_VALUE,
                              "Invalid value for prefer: '%s'\n",
			      prop, KD_NULL, KD_NULL);
            }
            xmlFree(prop);
	    pref = prefer;
        }
	prop = xmlGetProp(cur, BAD_CAST "id");
	base = xmlGetNsProp(cur, BAD_CAST "base", XML_XML_NAMESPACE);
	entry = xmlNewCatalogEntry(XML_CATA_GROUP, prop, base, KD_NULL, pref, cgroup);
	xmlFree(prop);
    } else if (xmlStrEqual(cur->name, BAD_CAST "public")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_PUBLIC,
		BAD_CAST "public", BAD_CAST "publicId", BAD_CAST "uri", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "system")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_SYSTEM,
		BAD_CAST "system", BAD_CAST "systemId", BAD_CAST "uri", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "rewriteSystem")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_REWRITE_SYSTEM,
		BAD_CAST "rewriteSystem", BAD_CAST "systemIdStartString",
		BAD_CAST "rewritePrefix", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "delegatePublic")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_DELEGATE_PUBLIC,
		BAD_CAST "delegatePublic", BAD_CAST "publicIdStartString",
		BAD_CAST "catalog", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "delegateSystem")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_DELEGATE_SYSTEM,
		BAD_CAST "delegateSystem", BAD_CAST "systemIdStartString",
		BAD_CAST "catalog", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "uri")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_URI,
		BAD_CAST "uri", BAD_CAST "name",
		BAD_CAST "uri", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "rewriteURI")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_REWRITE_URI,
		BAD_CAST "rewriteURI", BAD_CAST "uriStartString",
		BAD_CAST "rewritePrefix", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "delegateURI")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_DELEGATE_URI,
		BAD_CAST "delegateURI", BAD_CAST "uriStartString",
		BAD_CAST "catalog", prefer, cgroup);
    } else if (xmlStrEqual(cur->name, BAD_CAST "nextCatalog")) {
	entry = xmlParseXMLCatalogOneNode(cur, XML_CATA_NEXT_CATALOG,
		BAD_CAST "nextCatalog", KD_NULL,
		BAD_CAST "catalog", prefer, cgroup);
    }
    if (entry != KD_NULL) {
        if (parent != KD_NULL) {
	    entry->parent = parent;
	    if (parent->children == KD_NULL)
		parent->children = entry;
	    else {
		xmlCatalogEntryPtr prev;

		prev = parent->children;
		while (prev->next != KD_NULL)
		    prev = prev->next;
		prev->next = entry;
	    }
	}
	if (entry->type == XML_CATA_GROUP) {
	    /*
	     * Recurse to propagate prefer to the subtree
	     * (xml:base handling is automated)
	     */
            xmlParseXMLCatalogNodeList(cur->children, prefer, parent, entry);
	}
    }
    if (base != KD_NULL)
	xmlFree(base);
}

/**
 * xmlParseXMLCatalogNodeList:
 * @cur:  the XML node list of siblings
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 * @parent:  the parent Catalog entry
 * @cgroup:  the group which includes this list
 *
 * Examines a list of XML sibling nodes of a catalog and build
 * a list of Catalog entry from it adding it to the parent.
 * The examination will recurse to examine node subtrees.
 */
static void
xmlParseXMLCatalogNodeList(xmlNodePtr cur, xmlCatalogPrefer prefer,
	                   xmlCatalogEntryPtr parent, xmlCatalogEntryPtr cgroup) {
    while (cur != KD_NULL) {
	if ((cur->ns != KD_NULL) && (cur->ns->href != KD_NULL) &&
	    (xmlStrEqual(cur->ns->href, XML_CATALOGS_NAMESPACE))) {
	    xmlParseXMLCatalogNode(cur, prefer, parent, cgroup);
	}
	cur = cur->next;
    }
    /* TODO: sort the list according to REWRITE lengths and prefer value */
}

/**
 * xmlParseXMLCatalogFile:
 * @prefer:  the PUBLIC vs. SYSTEM current preference value
 * @filename:  the filename for the catalog
 *
 * Parses the catalog file to extract the XML tree and then analyze the
 * tree to build a list of Catalog entries corresponding to this catalog
 * 
 * Returns the resulting Catalog entries list
 */
static xmlCatalogEntryPtr
xmlParseXMLCatalogFile(xmlCatalogPrefer prefer, const xmlChar *filename) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *prop;
    xmlCatalogEntryPtr parent = KD_NULL;

    if (filename == KD_NULL)
        return(KD_NULL);

    doc = xmlParseCatalogFile((const char *) filename);
    if (doc == KD_NULL) {
	if (xmlDebugCatalogs)
	    xmlGenericError(xmlGenericErrorContext,
		    "Failed to parse catalog %s\n", filename);
	return(KD_NULL);
    }

    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"%d Parsing catalog %s\n", xmlGetThreadId(), filename);

    cur = xmlDocGetRootElement(doc);
    if ((cur != KD_NULL) && (xmlStrEqual(cur->name, BAD_CAST "catalog")) &&
	(cur->ns != KD_NULL) && (cur->ns->href != KD_NULL) &&
	(xmlStrEqual(cur->ns->href, XML_CATALOGS_NAMESPACE))) {

	parent = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL,
				    (const xmlChar *)filename, KD_NULL, prefer, KD_NULL);
        if (parent == KD_NULL) {
	    xmlFreeDoc(doc);
	    return(KD_NULL);
	}

	prop = xmlGetProp(cur, BAD_CAST "prefer");
	if (prop != KD_NULL) {
	    if (xmlStrEqual(prop, BAD_CAST "system")) {
		prefer = XML_CATA_PREFER_SYSTEM;
	    } else if (xmlStrEqual(prop, BAD_CAST "public")) {
		prefer = XML_CATA_PREFER_PUBLIC;
	    } else {
		xmlCatalogErr(KD_NULL, cur, XML_CATALOG_PREFER_VALUE,
			      "Invalid value for prefer: '%s'\n",
			      prop, KD_NULL, KD_NULL);
	    }
	    xmlFree(prop);
	}
	cur = cur->children;
	xmlParseXMLCatalogNodeList(cur, prefer, parent, KD_NULL);
    } else {
	xmlCatalogErr(KD_NULL, (xmlNodePtr) doc, XML_CATALOG_NOT_CATALOG,
		      "File %s is not an XML Catalog\n",
		      filename, KD_NULL, KD_NULL);
	xmlFreeDoc(doc);
	return(KD_NULL);
    }
    xmlFreeDoc(doc);
    return(parent);
}

/**
 * xmlFetchXMLCatalogFile:
 * @catal:  an existing but incomplete catalog entry
 *
 * Fetch and parse the subcatalog referenced by an entry
 * 
 * Returns 0 in case of success, -1 otherwise
 */
static int
xmlFetchXMLCatalogFile(xmlCatalogEntryPtr catal) {
    xmlCatalogEntryPtr doc;

    if (catal == KD_NULL) 
	return(-1);
    if (catal->URL == KD_NULL)
	return(-1);
    if (catal->children != KD_NULL)
	return(-1);

    /*
     * lock the whole catalog for modification
     */
    xmlRMutexLock(xmlCatalogMutex);
    if (catal->children != KD_NULL) {
	/* Okay someone else did it in the meantime */
	xmlRMutexUnlock(xmlCatalogMutex);
	return(0);
    }

    if (xmlCatalogXMLFiles != KD_NULL) {
	doc = (xmlCatalogEntryPtr)
	    xmlHashLookup(xmlCatalogXMLFiles, catal->URL);
	if (doc != KD_NULL) {
	    if (xmlDebugCatalogs)
		xmlGenericError(xmlGenericErrorContext,
		    "Found %s in file hash\n", catal->URL);

	    if (catal->type == XML_CATA_CATALOG)
		catal->children = doc->children;
	    else
		catal->children = doc;
	    catal->dealloc = 0;
	    xmlRMutexUnlock(xmlCatalogMutex);
	    return(0);
	}
	if (xmlDebugCatalogs)
	    xmlGenericError(xmlGenericErrorContext,
		"%s not found in file hash\n", catal->URL);
    }

    /*
     * Fetch and parse. Note that xmlParseXMLCatalogFile does not
     * use the existing catalog, there is no recursion allowed at
     * that level.
     */
    doc = xmlParseXMLCatalogFile(catal->prefer, catal->URL);
    if (doc == KD_NULL) {
	catal->type = XML_CATA_BROKEN_CATALOG;
	xmlRMutexUnlock(xmlCatalogMutex);
	return(-1);
    }

    if (catal->type == XML_CATA_CATALOG)
	catal->children = doc->children;
    else
	catal->children = doc;

    doc->dealloc = 1;

    if (xmlCatalogXMLFiles == KD_NULL)
	xmlCatalogXMLFiles = xmlHashCreate(10);
    if (xmlCatalogXMLFiles != KD_NULL) {
	if (xmlDebugCatalogs)
	    xmlGenericError(xmlGenericErrorContext,
		"%s added to file hash\n", catal->URL);
	xmlHashAddEntry(xmlCatalogXMLFiles, catal->URL, doc);
    }
    xmlRMutexUnlock(xmlCatalogMutex);
    return(0);
}

/************************************************************************
 *									*
 *			XML Catalog handling				*
 *									*
 ************************************************************************/

/**
 * xmlAddXMLCatalog:
 * @catal:  top of an XML catalog
 * @type:  the type of record to add to the catalog
 * @orig:  the system, public or prefix to match (or KD_NULL)
 * @replace:  the replacement value for the match
 *
 * Add an entry in the XML catalog, it may overwrite existing but
 * different entries.
 *
 * Returns 0 if successful, -1 otherwise
 */
static int
xmlAddXMLCatalog(xmlCatalogEntryPtr catal, const xmlChar *type,
	      const xmlChar *orig, const xmlChar *replace) {
    xmlCatalogEntryPtr cur;
    xmlCatalogEntryType typ;
    int doregister = 0;

    if ((catal == KD_NULL) || 
	((catal->type != XML_CATA_CATALOG) &&
	 (catal->type != XML_CATA_BROKEN_CATALOG)))
	return(-1);
    if (catal->children == KD_NULL) {
	xmlFetchXMLCatalogFile(catal);
    }
    if (catal->children == KD_NULL)
	doregister = 1;

    typ = xmlGetXMLCatalogEntryType(type);
    if (typ == XML_CATA_NONE) {
	if (xmlDebugCatalogs)
	    xmlGenericError(xmlGenericErrorContext,
		    "Failed to add unknown element %s to catalog\n", type);
	return(-1);
    }

    cur = catal->children;
    /*
     * Might be a simple "update in place"
     */
    if (cur != KD_NULL) {
	while (cur != KD_NULL) {
	    if ((orig != KD_NULL) && (cur->type == typ) &&
		(xmlStrEqual(orig, cur->name))) {
		if (xmlDebugCatalogs)
		    xmlGenericError(xmlGenericErrorContext,
			    "Updating element %s to catalog\n", type);
		if (cur->value != KD_NULL)
		    xmlFree(cur->value);
		if (cur->URL != KD_NULL)
		    xmlFree(cur->URL);
		cur->value = xmlStrdup(replace);
		cur->URL = xmlStrdup(replace);
		return(0);
	    }
	    if (cur->next == KD_NULL)
		break;
	    cur = cur->next;
	}
    }
    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Adding element %s to catalog\n", type);
    if (cur == KD_NULL)
	catal->children = xmlNewCatalogEntry(typ, orig, replace,
		                             KD_NULL, catal->prefer, KD_NULL);
    else
	cur->next = xmlNewCatalogEntry(typ, orig, replace,
		                       KD_NULL, catal->prefer, KD_NULL);
    if (doregister) {
        catal->type = XML_CATA_CATALOG;
	cur = (xmlCatalogEntry*) xmlHashLookup(xmlCatalogXMLFiles, catal->URL);
	if (cur != KD_NULL)
	    cur->children = catal->children;
    }

    return(0);
}

/**
 * xmlDelXMLCatalog:
 * @catal:  top of an XML catalog
 * @value:  the value to remove from the catalog
 *
 * Remove entries in the XML catalog where the value or the URI
 * is equal to @value
 *
 * Returns the number of entries removed if successful, -1 otherwise
 */
static int
xmlDelXMLCatalog(xmlCatalogEntryPtr catal, const xmlChar *value) {
    xmlCatalogEntryPtr cur;
    int ret = 0;

    if ((catal == KD_NULL) || 
	((catal->type != XML_CATA_CATALOG) &&
	 (catal->type != XML_CATA_BROKEN_CATALOG)))
	return(-1);
    if (value == KD_NULL)
	return(-1);
    if (catal->children == KD_NULL) {
	xmlFetchXMLCatalogFile(catal);
    }

    /*
     * Scan the children
     */
    cur = catal->children;
    while (cur != KD_NULL) {
	if (((cur->name != KD_NULL) && (xmlStrEqual(value, cur->name))) ||
	    (xmlStrEqual(value, cur->value))) {
	    if (xmlDebugCatalogs) {
		if (cur->name != KD_NULL)
		    xmlGenericError(xmlGenericErrorContext,
			    "Removing element %s from catalog\n", cur->name);
		else
		    xmlGenericError(xmlGenericErrorContext,
			    "Removing element %s from catalog\n", cur->value);
	    }
	    cur->type = XML_CATA_REMOVED;
	}
	cur = cur->next;
    }
    return(ret);
}

/**
 * xmlCatalogXMLResolve:
 * @catal:  a catalog list
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier for a
 * list of catalog entries.
 *
 * Implements (or tries to) 7.1. External Identifier Resolution
 * from http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * Returns the URI of the resource or KD_NULL if not found
 */
static xmlChar *
xmlCatalogXMLResolve(xmlCatalogEntryPtr catal, const xmlChar *pubID,
	              const xmlChar *sysID) {
    xmlChar *ret = KD_NULL;
    xmlCatalogEntryPtr cur;
    int haveDelegate = 0;
    int haveNext = 0;

    /*
     * protection against loops
     */
    if (catal->depth > MAX_CATAL_DEPTH) {
	xmlCatalogErr(catal, KD_NULL, XML_CATALOG_RECURSION,
		      "Detected recursion in catalog %s\n",
		      catal->name, KD_NULL, KD_NULL);
	return(KD_NULL);
    }
    catal->depth++;

    /*
     * First tries steps 2/ 3/ 4/ if a system ID is provided.
     */
    if (sysID != KD_NULL) {
	xmlCatalogEntryPtr rewrite = KD_NULL;
	int lenrewrite = 0, len;
	cur = catal;
	haveDelegate = 0;
	while (cur != KD_NULL) {
	    switch (cur->type) {
		case XML_CATA_SYSTEM:
		    if (xmlStrEqual(sysID, cur->name)) {
			if (xmlDebugCatalogs)
			    xmlGenericError(xmlGenericErrorContext,
				    "Found system match %s, using %s\n",
				            cur->name, cur->URL);
			catal->depth--;
			return(xmlStrdup(cur->URL));
		    }
		    break;
		case XML_CATA_REWRITE_SYSTEM:
		    len = xmlStrlen(cur->name);
		    if ((len > lenrewrite) &&
			(!xmlStrncmp(sysID, cur->name, len))) {
			lenrewrite = len;
			rewrite = cur;
		    }
		    break;
		case XML_CATA_DELEGATE_SYSTEM:
		    if (!xmlStrncmp(sysID, cur->name, xmlStrlen(cur->name)))
			haveDelegate++;
		    break;
		case XML_CATA_NEXT_CATALOG:
		    haveNext++;
		    break;
		default:
		    break;
	    }
	    cur = cur->next;
	}
	if (rewrite != KD_NULL) {
	    if (xmlDebugCatalogs)
		xmlGenericError(xmlGenericErrorContext,
			"Using rewriting rule %s\n", rewrite->name);
	    ret = xmlStrdup(rewrite->URL);
	    if (ret != KD_NULL)
		ret = xmlStrcat(ret, &sysID[lenrewrite]);
	    catal->depth--;
	    return(ret);
	}
	if (haveDelegate) {
	    const xmlChar *delegates[MAX_DELEGATE];
	    int nbList = 0, i;

	    /*
	     * Assume the entries have been sorted by decreasing substring
	     * matches when the list was produced.
	     */
	    cur = catal;
	    while (cur != KD_NULL) {
		if ((cur->type == XML_CATA_DELEGATE_SYSTEM) &&
		    (!xmlStrncmp(sysID, cur->name, xmlStrlen(cur->name)))) {
		    for (i = 0;i < nbList;i++)
			if (xmlStrEqual(cur->URL, delegates[i]))
			    break;
		    if (i < nbList) {
			cur = cur->next;
			continue;
		    }
		    if (nbList < MAX_DELEGATE)
			delegates[nbList++] = cur->URL;

		    if (cur->children == KD_NULL) {
			xmlFetchXMLCatalogFile(cur);
		    }
		    if (cur->children != KD_NULL) {
			if (xmlDebugCatalogs)
			    xmlGenericError(xmlGenericErrorContext,
				    "Trying system delegate %s\n", cur->URL);
			ret = xmlCatalogListXMLResolve(
				cur->children, KD_NULL, sysID);
			if (ret != KD_NULL) {
			    catal->depth--;
			    return(ret);
			}
		    }
		}
		cur = cur->next;
	    }
	    /*
	     * Apply the cut algorithm explained in 4/
	     */
	    catal->depth--;
	    return(XML_CATAL_BREAK);
	}
    }
    /*
     * Then tries 5/ 6/ if a public ID is provided
     */
    if (pubID != KD_NULL) {
	cur = catal;
	haveDelegate = 0;
	while (cur != KD_NULL) {
	    switch (cur->type) {
		case XML_CATA_PUBLIC:
		    if (xmlStrEqual(pubID, cur->name)) {
			if (xmlDebugCatalogs)
			    xmlGenericError(xmlGenericErrorContext,
				    "Found public match %s\n", cur->name);
			catal->depth--;
			return(xmlStrdup(cur->URL));
		    }
		    break;
		case XML_CATA_DELEGATE_PUBLIC:
		    if (!xmlStrncmp(pubID, cur->name, xmlStrlen(cur->name)) &&
			(cur->prefer == XML_CATA_PREFER_PUBLIC))
			haveDelegate++;
		    break;
		case XML_CATA_NEXT_CATALOG:
		    if (sysID == KD_NULL)
			haveNext++;
		    break;
		default:
		    break;
	    }
	    cur = cur->next;
	}
	if (haveDelegate) {
	    const xmlChar *delegates[MAX_DELEGATE];
	    int nbList = 0, i;

	    /*
	     * Assume the entries have been sorted by decreasing substring
	     * matches when the list was produced.
	     */
	    cur = catal;
	    while (cur != KD_NULL) {
		if ((cur->type == XML_CATA_DELEGATE_PUBLIC) &&
		    (cur->prefer == XML_CATA_PREFER_PUBLIC) &&
		    (!xmlStrncmp(pubID, cur->name, xmlStrlen(cur->name)))) {

		    for (i = 0;i < nbList;i++)
			if (xmlStrEqual(cur->URL, delegates[i]))
			    break;
		    if (i < nbList) {
			cur = cur->next;
			continue;
		    }
		    if (nbList < MAX_DELEGATE)
			delegates[nbList++] = cur->URL;
			    
		    if (cur->children == KD_NULL) {
			xmlFetchXMLCatalogFile(cur);
		    }
		    if (cur->children != KD_NULL) {
			if (xmlDebugCatalogs)
			    xmlGenericError(xmlGenericErrorContext,
				    "Trying public delegate %s\n", cur->URL);
			ret = xmlCatalogListXMLResolve(
				cur->children, pubID, KD_NULL);
			if (ret != KD_NULL) {
			    catal->depth--;
			    return(ret);
			}
		    }
		}
		cur = cur->next;
	    }
	    /*
	     * Apply the cut algorithm explained in 4/
	     */
	    catal->depth--;
	    return(XML_CATAL_BREAK);
	}
    }
    if (haveNext) {
	cur = catal;
	while (cur != KD_NULL) {
	    if (cur->type == XML_CATA_NEXT_CATALOG) {
		if (cur->children == KD_NULL) {
		    xmlFetchXMLCatalogFile(cur);
		}
		if (cur->children != KD_NULL) {
		    ret = xmlCatalogListXMLResolve(cur->children, pubID, sysID);
		    if (ret != KD_NULL) {
			catal->depth--;
			return(ret);
		    } else if (catal->depth > MAX_CATAL_DEPTH) {
		        return(KD_NULL);
		    }
		}
	    }
	    cur = cur->next;
	}
    }

    catal->depth--;
    return(KD_NULL);
}

/**
 * xmlCatalogXMLResolveURI:
 * @catal:  a catalog list
 * @URI:  the URI
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier for a
 * list of catalog entries.
 *
 * Implements (or tries to) 7.2.2. URI Resolution
 * from http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * Returns the URI of the resource or KD_NULL if not found
 */
static xmlChar *
xmlCatalogXMLResolveURI(xmlCatalogEntryPtr catal, const xmlChar *URI) {
    xmlChar *ret = KD_NULL;
    xmlCatalogEntryPtr cur;
    int haveDelegate = 0;
    int haveNext = 0;
    xmlCatalogEntryPtr rewrite = KD_NULL;
    int lenrewrite = 0, len;

    if (catal == KD_NULL)
	return(KD_NULL);

    if (URI == KD_NULL)
	return(KD_NULL);

    if (catal->depth > MAX_CATAL_DEPTH) {
	xmlCatalogErr(catal, KD_NULL, XML_CATALOG_RECURSION,
		      "Detected recursion in catalog %s\n",
		      catal->name, KD_NULL, KD_NULL);
	return(KD_NULL);
    }

    /*
     * First tries steps 2/ 3/ 4/ if a system ID is provided.
     */
    cur = catal;
    haveDelegate = 0;
    while (cur != KD_NULL) {
	switch (cur->type) {
	    case XML_CATA_URI:
		if (xmlStrEqual(URI, cur->name)) {
		    if (xmlDebugCatalogs)
			xmlGenericError(xmlGenericErrorContext,
				"Found URI match %s\n", cur->name);
		    return(xmlStrdup(cur->URL));
		}
		break;
	    case XML_CATA_REWRITE_URI:
		len = xmlStrlen(cur->name);
		if ((len > lenrewrite) &&
		    (!xmlStrncmp(URI, cur->name, len))) {
		    lenrewrite = len;
		    rewrite = cur;
		}
		break;
	    case XML_CATA_DELEGATE_URI:
		if (!xmlStrncmp(URI, cur->name, xmlStrlen(cur->name)))
		    haveDelegate++;
		break;
	    case XML_CATA_NEXT_CATALOG:
		haveNext++;
		break;
	    default:
		break;
	}
	cur = cur->next;
    }
    if (rewrite != KD_NULL) {
	if (xmlDebugCatalogs)
	    xmlGenericError(xmlGenericErrorContext,
		    "Using rewriting rule %s\n", rewrite->name);
	ret = xmlStrdup(rewrite->URL);
	if (ret != KD_NULL)
	    ret = xmlStrcat(ret, &URI[lenrewrite]);
	return(ret);
    }
    if (haveDelegate) {
	const xmlChar *delegates[MAX_DELEGATE];
	int nbList = 0, i;

	/*
	 * Assume the entries have been sorted by decreasing substring
	 * matches when the list was produced.
	 */
	cur = catal;
	while (cur != KD_NULL) {
	    if (((cur->type == XML_CATA_DELEGATE_SYSTEM) ||
	         (cur->type == XML_CATA_DELEGATE_URI)) &&
		(!xmlStrncmp(URI, cur->name, xmlStrlen(cur->name)))) {
		for (i = 0;i < nbList;i++)
		    if (xmlStrEqual(cur->URL, delegates[i]))
			break;
		if (i < nbList) {
		    cur = cur->next;
		    continue;
		}
		if (nbList < MAX_DELEGATE)
		    delegates[nbList++] = cur->URL;

		if (cur->children == KD_NULL) {
		    xmlFetchXMLCatalogFile(cur);
		}
		if (cur->children != KD_NULL) {
		    if (xmlDebugCatalogs)
			xmlGenericError(xmlGenericErrorContext,
				"Trying URI delegate %s\n", cur->URL);
		    ret = xmlCatalogListXMLResolveURI(
			    cur->children, URI);
		    if (ret != KD_NULL)
			return(ret);
		}
	    }
	    cur = cur->next;
	}
	/*
	 * Apply the cut algorithm explained in 4/
	 */
	return(XML_CATAL_BREAK);
    }
    if (haveNext) {
	cur = catal;
	while (cur != KD_NULL) {
	    if (cur->type == XML_CATA_NEXT_CATALOG) {
		if (cur->children == KD_NULL) {
		    xmlFetchXMLCatalogFile(cur);
		}
		if (cur->children != KD_NULL) {
		    ret = xmlCatalogListXMLResolveURI(cur->children, URI);
		    if (ret != KD_NULL)
			return(ret);
		}
	    }
	    cur = cur->next;
	}
    }

    return(KD_NULL);
}

/**
 * xmlCatalogListXMLResolve:
 * @catal:  a catalog list
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier for a
 * list of catalogs
 *
 * Implements (or tries to) 7.1. External Identifier Resolution
 * from http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * Returns the URI of the resource or KD_NULL if not found
 */
static xmlChar *
xmlCatalogListXMLResolve(xmlCatalogEntryPtr catal, const xmlChar *pubID,
	              const xmlChar *sysID) {
    xmlChar *ret = KD_NULL;
    xmlChar *urnID = KD_NULL;
    xmlChar *normid;
    
    if (catal == KD_NULL)
        return(KD_NULL);
    if ((pubID == KD_NULL) && (sysID == KD_NULL))
	return(KD_NULL);

    normid = xmlCatalogNormalizePublic(pubID);
    if (normid != KD_NULL)
        pubID = (*normid != 0 ? normid : KD_NULL);
    
    if (!xmlStrncmp(pubID, BAD_CAST XML_URN_PUBID, sizeof(XML_URN_PUBID) - 1)) {
	urnID = xmlCatalogUnWrapURN(pubID);
	if (xmlDebugCatalogs) {
	    if (urnID == KD_NULL)
		xmlGenericError(xmlGenericErrorContext,
			"Public URN ID %s expanded to KD_NULL\n", pubID);
	    else
		xmlGenericError(xmlGenericErrorContext,
			"Public URN ID expanded to %s\n", urnID);
	}
	ret = xmlCatalogListXMLResolve(catal, urnID, sysID);
	if (urnID != KD_NULL)
	    xmlFree(urnID);
	if (normid != KD_NULL)
	    xmlFree(normid);
	return(ret);
    }
    if (!xmlStrncmp(sysID, BAD_CAST XML_URN_PUBID, sizeof(XML_URN_PUBID) - 1)) {
	urnID = xmlCatalogUnWrapURN(sysID);
	if (xmlDebugCatalogs) {
	    if (urnID == KD_NULL)
		xmlGenericError(xmlGenericErrorContext,
			"System URN ID %s expanded to KD_NULL\n", sysID);
	    else
		xmlGenericError(xmlGenericErrorContext,
			"System URN ID expanded to %s\n", urnID);
	}
	if (pubID == KD_NULL)
	    ret = xmlCatalogListXMLResolve(catal, urnID, KD_NULL);
	else if (xmlStrEqual(pubID, urnID))
	    ret = xmlCatalogListXMLResolve(catal, pubID, KD_NULL);
	else {
	    ret = xmlCatalogListXMLResolve(catal, pubID, urnID);
	}
	if (urnID != KD_NULL)
	    xmlFree(urnID);
	if (normid != KD_NULL)
	    xmlFree(normid);
	return(ret);
    }
    while (catal != KD_NULL) {
	if (catal->type == XML_CATA_CATALOG) {
	    if (catal->children == KD_NULL) {
		xmlFetchXMLCatalogFile(catal);
	    }
	    if (catal->children != KD_NULL) {
		ret = xmlCatalogXMLResolve(catal->children, pubID, sysID);
		if (ret != KD_NULL) {
		    break;
                } else if ((catal->children != KD_NULL) &&
		           (catal->children->depth > MAX_CATAL_DEPTH)) {
	            ret = KD_NULL;
		    break;
	        }
	    }
	}
	catal = catal->next;
    }
    if (normid != KD_NULL)
	xmlFree(normid);
    return(ret);
}

/**
 * xmlCatalogListXMLResolveURI:
 * @catal:  a catalog list
 * @URI:  the URI
 *
 * Do a complete resolution lookup of an URI for a list of catalogs
 *
 * Implements (or tries to) 7.2. URI Resolution
 * from http://www.oasis-open.org/committees/entity/spec-2001-08-06.html
 *
 * Returns the URI of the resource or KD_NULL if not found
 */
static xmlChar *
xmlCatalogListXMLResolveURI(xmlCatalogEntryPtr catal, const xmlChar *URI) {
    xmlChar *ret = KD_NULL;
    xmlChar *urnID = KD_NULL;
    
    if (catal == KD_NULL)
        return(KD_NULL);
    if (URI == KD_NULL)
	return(KD_NULL);

    if (!xmlStrncmp(URI, BAD_CAST XML_URN_PUBID, sizeof(XML_URN_PUBID) - 1)) {
	urnID = xmlCatalogUnWrapURN(URI);
	if (xmlDebugCatalogs) {
	    if (urnID == KD_NULL)
		xmlGenericError(xmlGenericErrorContext,
			"URN ID %s expanded to KD_NULL\n", URI);
	    else
		xmlGenericError(xmlGenericErrorContext,
			"URN ID expanded to %s\n", urnID);
	}
	ret = xmlCatalogListXMLResolve(catal, urnID, KD_NULL);
	if (urnID != KD_NULL)
	    xmlFree(urnID);
	return(ret);
    }
    while (catal != KD_NULL) {
	if (catal->type == XML_CATA_CATALOG) {
	    if (catal->children == KD_NULL) {
		xmlFetchXMLCatalogFile(catal);
	    }
	    if (catal->children != KD_NULL) {
		ret = xmlCatalogXMLResolveURI(catal->children, URI);
		if (ret != KD_NULL)
		    return(ret);
	    }
	}
	catal = catal->next;
    }
    return(ret);
}

/************************************************************************
 *									*
 *			The SGML Catalog parser				*
 *									*
 ************************************************************************/


#define RAW *cur
#define NEXT cur++;
#define SKIP(x) cur += x;

#define SKIP_BLANKS while (IS_BLANK_CH(*cur)) NEXT;

/**
 * xmlParseSGMLCatalogComment:
 * @cur:  the current character
 *
 * Skip a comment in an SGML catalog
 *
 * Returns new current character
 */
static const xmlChar *
xmlParseSGMLCatalogComment(const xmlChar *cur) {
    if ((cur[0] != '-') || (cur[1] != '-')) 
	return(cur);
    SKIP(2);
    while ((cur[0] != 0) && ((cur[0] != '-') || ((cur[1] != '-'))))
	NEXT;
    if (cur[0] == 0) {
	return(KD_NULL);
    }
    return(cur + 2);
}

/**
 * xmlParseSGMLCatalogPubid:
 * @cur:  the current character
 * @id:  the return location
 *
 * Parse an SGML catalog ID
 *
 * Returns new current character and store the value in @id
 */
static const xmlChar *
xmlParseSGMLCatalogPubid(const xmlChar *cur, xmlChar **id) {
    xmlChar *buf = KD_NULL, *tmp;
    int len = 0;
    int size = 50;
    xmlChar stop;
    int count = 0;

    *id = KD_NULL;

    if (RAW == '"') {
        NEXT;
	stop = '"';
    } else if (RAW == '\'') {
        NEXT;
	stop = '\'';
    } else {
	stop = ' ';
    }
    buf = (xmlChar *) xmlMallocAtomic(size * sizeof(xmlChar));
    if (buf == KD_NULL) {
        xmlCatalogErrMemory("allocating public ID");
	return(KD_NULL);
    }
    while (IS_PUBIDCHAR_CH(*cur) || (*cur == '?')) {
	if ((*cur == stop) && (stop != ' '))
	    break;
	if ((stop == ' ') && (IS_BLANK_CH(*cur)))
	    break;
	if (len + 1 >= size) {
	    size *= 2;
	    tmp = (xmlChar *) xmlRealloc(buf, size * sizeof(xmlChar));
	    if (tmp == KD_NULL) {
		xmlCatalogErrMemory("allocating public ID");
		xmlFree(buf);
		return(KD_NULL);
	    }
	    buf = tmp;
	}
	buf[len++] = *cur;
	count++;
	NEXT;
    }
    buf[len] = 0;
    if (stop == ' ') {
	if (!IS_BLANK_CH(*cur)) {
	    xmlFree(buf);
	    return(KD_NULL);
	}
    } else {
	if (*cur != stop) {
	    xmlFree(buf);
	    return(KD_NULL);
	}
	NEXT;
    }
    *id = buf;
    return(cur);
}

/**
 * xmlParseSGMLCatalogName:
 * @cur:  the current character
 * @name:  the return location
 *
 * Parse an SGML catalog name
 *
 * Returns new current character and store the value in @name
 */
static const xmlChar *
xmlParseSGMLCatalogName(const xmlChar *cur, xmlChar **name) {
    xmlChar buf[XML_MAX_NAMELEN + 5];
    int len = 0;
    int c;

    *name = KD_NULL;

    /*
     * Handler for more complex cases
     */
    c = *cur;
    if ((!IS_LETTER(c) && (c != '_') && (c != ':'))) {
	return(KD_NULL);
    }

    while (((IS_LETTER(c)) || (IS_DIGIT(c)) ||
            (c == '.') || (c == '-') ||
	    (c == '_') || (c == ':'))) {
	buf[len++] = c;
	cur++;
	c = *cur;
	if (len >= XML_MAX_NAMELEN)
	    return(KD_NULL);
    }
    *name = xmlStrndup(buf, len);
    return(cur);
}

/**
 * xmlGetSGMLCatalogEntryType:
 * @name:  the entry name
 *
 * Get the Catalog entry type for a given SGML Catalog name
 *
 * Returns Catalog entry type
 */
static xmlCatalogEntryType
xmlGetSGMLCatalogEntryType(const xmlChar *name) {
    xmlCatalogEntryType type = XML_CATA_NONE;
    if (xmlStrEqual(name, (const xmlChar *) "SYSTEM"))
	type = SGML_CATA_SYSTEM;
    else if (xmlStrEqual(name, (const xmlChar *) "PUBLIC"))
	type = SGML_CATA_PUBLIC;
    else if (xmlStrEqual(name, (const xmlChar *) "DELEGATE"))
	type = SGML_CATA_DELEGATE;
    else if (xmlStrEqual(name, (const xmlChar *) "ENTITY"))
	type = SGML_CATA_ENTITY;
    else if (xmlStrEqual(name, (const xmlChar *) "DOCTYPE"))
	type = SGML_CATA_DOCTYPE;
    else if (xmlStrEqual(name, (const xmlChar *) "LINKTYPE"))
	type = SGML_CATA_LINKTYPE;
    else if (xmlStrEqual(name, (const xmlChar *) "NOTATION"))
	type = SGML_CATA_NOTATION;
    else if (xmlStrEqual(name, (const xmlChar *) "SGMLDECL"))
	type = SGML_CATA_SGMLDECL;
    else if (xmlStrEqual(name, (const xmlChar *) "DOCUMENT"))
	type = SGML_CATA_DOCUMENT;
    else if (xmlStrEqual(name, (const xmlChar *) "CATALOG"))
	type = SGML_CATA_CATALOG;
    else if (xmlStrEqual(name, (const xmlChar *) "BASE"))
	type = SGML_CATA_BASE;
    return(type);
}

/**
 * xmlParseSGMLCatalog:
 * @catal:  the SGML Catalog
 * @value:  the content of the SGML Catalog serialization
 * @file:  the filepath for the catalog
 * @super:  should this be handled as a Super Catalog in which case
 *          parsing is not recursive
 *
 * Parse an SGML catalog content and fill up the @catal hash table with
 * the new entries found.
 *
 * Returns 0 in case of success, -1 in case of error.
 */
static int
xmlParseSGMLCatalog(xmlCatalogPtr catal, const xmlChar *value,
	            const char *file, int super) {
    const xmlChar *cur = value;
    xmlChar *base = KD_NULL;
    int res;

    if ((cur == KD_NULL) || (file == KD_NULL))
        return(-1);
    base = xmlStrdup((const xmlChar *) file);

    while ((cur != KD_NULL) && (cur[0] != 0)) {
	SKIP_BLANKS;
	if (cur[0] == 0)
	    break;
	if ((cur[0] == '-') && (cur[1] == '-')) {
	    cur = xmlParseSGMLCatalogComment(cur);
	    if (cur == KD_NULL) {
		/* error */
		break;
	    }
	} else {
	    xmlChar *sysid = KD_NULL;
	    xmlChar *name = KD_NULL;
	    xmlCatalogEntryType type = XML_CATA_NONE;

	    cur = xmlParseSGMLCatalogName(cur, &name);
	    if (name == KD_NULL) {
		/* error */
		break;
	    }
	    if (!IS_BLANK_CH(*cur)) {
		/* error */
		break;
	    }
	    SKIP_BLANKS;
	    if (xmlStrEqual(name, (const xmlChar *) "SYSTEM"))
                type = SGML_CATA_SYSTEM;
	    else if (xmlStrEqual(name, (const xmlChar *) "PUBLIC"))
                type = SGML_CATA_PUBLIC;
	    else if (xmlStrEqual(name, (const xmlChar *) "DELEGATE"))
                type = SGML_CATA_DELEGATE;
	    else if (xmlStrEqual(name, (const xmlChar *) "ENTITY"))
                type = SGML_CATA_ENTITY;
	    else if (xmlStrEqual(name, (const xmlChar *) "DOCTYPE"))
                type = SGML_CATA_DOCTYPE;
	    else if (xmlStrEqual(name, (const xmlChar *) "LINKTYPE"))
                type = SGML_CATA_LINKTYPE;
	    else if (xmlStrEqual(name, (const xmlChar *) "NOTATION"))
                type = SGML_CATA_NOTATION;
	    else if (xmlStrEqual(name, (const xmlChar *) "SGMLDECL"))
                type = SGML_CATA_SGMLDECL;
	    else if (xmlStrEqual(name, (const xmlChar *) "DOCUMENT"))
                type = SGML_CATA_DOCUMENT;
	    else if (xmlStrEqual(name, (const xmlChar *) "CATALOG"))
                type = SGML_CATA_CATALOG;
	    else if (xmlStrEqual(name, (const xmlChar *) "BASE"))
                type = SGML_CATA_BASE;
	    else if (xmlStrEqual(name, (const xmlChar *) "OVERRIDE")) {
		xmlFree(name);
		cur = xmlParseSGMLCatalogName(cur, &name);
		if (name == KD_NULL) {
		    /* error */
		    break;
		}
		xmlFree(name);
		continue;
	    }
	    xmlFree(name);
	    name = KD_NULL;

	    switch(type) {
		case SGML_CATA_ENTITY:
		    if (*cur == '%')
			type = SGML_CATA_PENTITY;
		case SGML_CATA_PENTITY:
		case SGML_CATA_DOCTYPE:
		case SGML_CATA_LINKTYPE:
		case SGML_CATA_NOTATION:
		    cur = xmlParseSGMLCatalogName(cur, &name);
		    if (cur == KD_NULL) {
			/* error */
			break;
		    }
		    if (!IS_BLANK_CH(*cur)) {
			/* error */
			break;
		    }
		    SKIP_BLANKS;
		    cur = xmlParseSGMLCatalogPubid(cur, &sysid);
		    if (cur == KD_NULL) {
			/* error */
			break;
		    }
		    break;
		case SGML_CATA_PUBLIC:
		case SGML_CATA_SYSTEM:
		case SGML_CATA_DELEGATE:
		    cur = xmlParseSGMLCatalogPubid(cur, &name);
		    if (cur == KD_NULL) {
			/* error */
			break;
		    }
		    if (type != SGML_CATA_SYSTEM) {
		        xmlChar *normid;

		        normid = xmlCatalogNormalizePublic(name);
		        if (normid != KD_NULL) {
		            if (name != KD_NULL)
		                xmlFree(name);
		            if (*normid != 0)
		                name = normid;
		            else {
		                xmlFree(normid);
		                name = KD_NULL;
		            }
		        }
		    }
		    if (!IS_BLANK_CH(*cur)) {
			/* error */
			break;
		    }
		    SKIP_BLANKS;
		    cur = xmlParseSGMLCatalogPubid(cur, &sysid);
		    if (cur == KD_NULL) {
			/* error */
			break;
		    }
		    break;
		case SGML_CATA_BASE:
		case SGML_CATA_CATALOG:
		case SGML_CATA_DOCUMENT:
		case SGML_CATA_SGMLDECL:
		    cur = xmlParseSGMLCatalogPubid(cur, &sysid);
		    if (cur == KD_NULL) {
			/* error */
			break;
		    }
		    break;
		default:
		    break;
	    }
	    if (cur == KD_NULL) {
		if (name != KD_NULL)
		    xmlFree(name);
		if (sysid != KD_NULL)
		    xmlFree(sysid);
		break;
	    } else if (type == SGML_CATA_BASE) {
		if (base != KD_NULL)
		    xmlFree(base);
		base = xmlStrdup(sysid);
	    } else if ((type == SGML_CATA_PUBLIC) ||
		       (type == SGML_CATA_SYSTEM)) {
		xmlChar *filename;

		filename = xmlBuildURI(sysid, base);
		if (filename != KD_NULL) {
		    xmlCatalogEntryPtr entry;

		    entry = xmlNewCatalogEntry(type, name, filename,
			                       KD_NULL, XML_CATA_PREFER_NONE, KD_NULL);
		    res = xmlHashAddEntry(catal->sgml, name, entry);
		    if (res < 0) {
			xmlFreeCatalogEntry(entry);
		    }
		    xmlFree(filename);
		}

	    } else if (type == SGML_CATA_CATALOG) {
		if (super) {
		    xmlCatalogEntryPtr entry;

		    entry = xmlNewCatalogEntry(type, sysid, KD_NULL, KD_NULL,
			                       XML_CATA_PREFER_NONE, KD_NULL);
		    res = xmlHashAddEntry(catal->sgml, sysid, entry);
		    if (res < 0) {
			xmlFreeCatalogEntry(entry);
		    }
		} else {
		    xmlChar *filename;

		    filename = xmlBuildURI(sysid, base);
		    if (filename != KD_NULL) {
			xmlExpandCatalog(catal, (const char *)filename);
			xmlFree(filename);
		    }
		}
	    }
	    /*
	     * drop anything else we won't handle it
	     */
	    if (name != KD_NULL)
		xmlFree(name);
	    if (sysid != KD_NULL)
		xmlFree(sysid);
	}
    }
    if (base != KD_NULL)
	xmlFree(base);
    if (cur == KD_NULL)
	return(-1);
    return(0);
}

/************************************************************************
 *									*
 *			SGML Catalog handling				*
 *									*
 ************************************************************************/

/**
 * xmlCatalogGetSGMLPublic:
 * @catal:  an SGML catalog hash
 * @pubID:  the public ID string
 *
 * Try to lookup the catalog local reference associated to a public ID
 *
 * Returns the local resource if found or KD_NULL otherwise.
 */
static const xmlChar *
xmlCatalogGetSGMLPublic(xmlHashTablePtr catal, const xmlChar *pubID) {
    xmlCatalogEntryPtr entry;
    xmlChar *normid;

    if (catal == KD_NULL)
	return(KD_NULL);

    normid = xmlCatalogNormalizePublic(pubID);
    if (normid != KD_NULL)
        pubID = (*normid != 0 ? normid : KD_NULL);

    entry = (xmlCatalogEntryPtr) xmlHashLookup(catal, pubID);
    if (entry == KD_NULL) {
	if (normid != KD_NULL)
	    xmlFree(normid);
	return(KD_NULL);
    }
    if (entry->type == SGML_CATA_PUBLIC) {
	if (normid != KD_NULL)
	    xmlFree(normid);
	return(entry->URL);
    }
    if (normid != KD_NULL)
        xmlFree(normid);
    return(KD_NULL);
}

/**
 * xmlCatalogGetSGMLSystem:
 * @catal:  an SGML catalog hash
 * @sysID:  the system ID string
 *
 * Try to lookup the catalog local reference for a system ID
 *
 * Returns the local resource if found or KD_NULL otherwise.
 */
static const xmlChar *
xmlCatalogGetSGMLSystem(xmlHashTablePtr catal, const xmlChar *sysID) {
    xmlCatalogEntryPtr entry;

    if (catal == KD_NULL)
	return(KD_NULL);

    entry = (xmlCatalogEntryPtr) xmlHashLookup(catal, sysID);
    if (entry == KD_NULL)
	return(KD_NULL);
    if (entry->type == SGML_CATA_SYSTEM)
	return(entry->URL);
    return(KD_NULL);
}

/**
 * xmlCatalogSGMLResolve:
 * @catal:  the SGML catalog
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier
 *
 * Returns the URI of the resource or KD_NULL if not found
 */
static const xmlChar *
xmlCatalogSGMLResolve(xmlCatalogPtr catal, const xmlChar *pubID,
	              const xmlChar *sysID) {
    const xmlChar *ret = KD_NULL;

    if (catal->sgml == KD_NULL)
	return(KD_NULL);

    if (pubID != KD_NULL)
	ret = xmlCatalogGetSGMLPublic(catal->sgml, pubID);
    if (ret != KD_NULL)
	return(ret);
    if (sysID != KD_NULL)
	ret = xmlCatalogGetSGMLSystem(catal->sgml, sysID);
    if (ret != KD_NULL)
	return(ret);
    return(KD_NULL);
}

/************************************************************************
 *									*
 *			Specific Public interfaces			*
 *									*
 ************************************************************************/

/**
 * xmlLoadSGMLSuperCatalog:
 * @filename:  a file path
 *
 * Load an SGML super catalog. It won't expand CATALOG or DELEGATE
 * references. This is only needed for manipulating SGML Super Catalogs
 * like adding and removing CATALOG or DELEGATE entries.
 *
 * Returns the catalog parsed or KD_NULL in case of error
 */
xmlCatalogPtr
xmlLoadSGMLSuperCatalog(const char *filename)
{
    xmlChar *content;
    xmlCatalogPtr catal;
    int ret;

    content = xmlLoadFileContent(filename);
    if (content == KD_NULL)
        return(KD_NULL);

    catal = xmlCreateNewCatalog(XML_SGML_CATALOG_TYPE, xmlCatalogDefaultPrefer);
    if (catal == KD_NULL) {
	xmlFree(content);
	return(KD_NULL);
    }

    ret = xmlParseSGMLCatalog(catal, content, filename, 1);
    xmlFree(content);
    if (ret < 0) {
	xmlFreeCatalog(catal);
	return(KD_NULL);
    }
    return (catal);
}

/**
 * xmlLoadACatalog:
 * @filename:  a file path
 *
 * Load the catalog and build the associated data structures.
 * This can be either an XML Catalog or an SGML Catalog
 * It will recurse in SGML CATALOG entries. On the other hand XML
 * Catalogs are not handled recursively.
 *
 * Returns the catalog parsed or KD_NULL in case of error
 */
xmlCatalogPtr
xmlLoadACatalog(const char *filename)
{
    xmlChar *content;
    xmlChar *first;
    xmlCatalogPtr catal;
    int ret;

    content = xmlLoadFileContent(filename);
    if (content == KD_NULL)
        return(KD_NULL);


    first = content;
   
    while ((*first != 0) && (*first != '-') && (*first != '<') &&
	   (!(((*first >= 'A') && (*first <= 'Z')) ||
	      ((*first >= 'a') && (*first <= 'z')))))
	first++;

    if (*first != '<') {
	catal = xmlCreateNewCatalog(XML_SGML_CATALOG_TYPE, xmlCatalogDefaultPrefer);
	if (catal == KD_NULL) {
	    xmlFree(content);
	    return(KD_NULL);
	}
        ret = xmlParseSGMLCatalog(catal, content, filename, 0);
	if (ret < 0) {
	    xmlFreeCatalog(catal);
	    xmlFree(content);
	    return(KD_NULL);
	}
    } else {
	catal = xmlCreateNewCatalog(XML_XML_CATALOG_TYPE, xmlCatalogDefaultPrefer);
	if (catal == KD_NULL) {
	    xmlFree(content);
	    return(KD_NULL);
	}
        catal->xml = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL,
		       KD_NULL, BAD_CAST filename, xmlCatalogDefaultPrefer, KD_NULL);
    }
    xmlFree(content);
    return (catal);
}

/**
 * xmlExpandCatalog:
 * @catal:  a catalog
 * @filename:  a file path
 *
 * Load the catalog and expand the existing catal structure.
 * This can be either an XML Catalog or an SGML Catalog
 *
 * Returns 0 in case of success, -1 in case of error
 */
static int
xmlExpandCatalog(xmlCatalogPtr catal, const char *filename)
{
    int ret;

    if ((catal == KD_NULL) || (filename == KD_NULL))
	return(-1);


    if (catal->type == XML_SGML_CATALOG_TYPE) {
	xmlChar *content;

	content = xmlLoadFileContent(filename);
	if (content == KD_NULL)
	    return(-1);

        ret = xmlParseSGMLCatalog(catal, content, filename, 0);
	if (ret < 0) {
	    xmlFree(content);
	    return(-1);
	}
	xmlFree(content);
    } else {
	xmlCatalogEntryPtr tmp, cur;
	tmp = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL,
		       KD_NULL, BAD_CAST filename, xmlCatalogDefaultPrefer, KD_NULL);

	cur = catal->xml;
	if (cur == KD_NULL) {
	    catal->xml = tmp;
	} else {
	    while (cur->next != KD_NULL) cur = cur->next;
	    cur->next = tmp;
	}
    }
    return (0);
}

/**
 * xmlACatalogResolveSystem:
 * @catal:  a Catalog
 * @sysID:  the system ID string
 *
 * Try to lookup the catalog resource for a system ID
 *
 * Returns the resource if found or KD_NULL otherwise, the value returned
 *      must be freed by the caller.
 */
xmlChar *
xmlACatalogResolveSystem(xmlCatalogPtr catal, const xmlChar *sysID) {
    xmlChar *ret = KD_NULL;

    if ((sysID == KD_NULL) || (catal == KD_NULL))
	return(KD_NULL);
    
    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Resolve sysID %s\n", sysID);

    if (catal->type == XML_XML_CATALOG_TYPE) {
	ret = xmlCatalogListXMLResolve(catal->xml, KD_NULL, sysID);
	if (ret == XML_CATAL_BREAK)
	    ret = KD_NULL;
    } else {
	const xmlChar *sgml;

	sgml = xmlCatalogGetSGMLSystem(catal->sgml, sysID);
	if (sgml != KD_NULL)
	    ret = xmlStrdup(sgml);
    }
    return(ret);
}

/**
 * xmlACatalogResolvePublic:
 * @catal:  a Catalog
 * @pubID:  the public ID string
 *
 * Try to lookup the catalog local reference associated to a public ID in that catalog
 *
 * Returns the local resource if found or KD_NULL otherwise, the value returned
 *      must be freed by the caller.
 */
xmlChar *
xmlACatalogResolvePublic(xmlCatalogPtr catal, const xmlChar *pubID) {
    xmlChar *ret = KD_NULL;

    if ((pubID == KD_NULL) || (catal == KD_NULL))
	return(KD_NULL);
    
    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Resolve pubID %s\n", pubID);

    if (catal->type == XML_XML_CATALOG_TYPE) {
	ret = xmlCatalogListXMLResolve(catal->xml, pubID, KD_NULL);
	if (ret == XML_CATAL_BREAK)
	    ret = KD_NULL;
    } else {
	const xmlChar *sgml;

	sgml = xmlCatalogGetSGMLPublic(catal->sgml, pubID);
	if (sgml != KD_NULL)
	    ret = xmlStrdup(sgml);
    }
    return(ret);
}

/**
 * xmlACatalogResolve:
 * @catal:  a Catalog
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlACatalogResolve(xmlCatalogPtr catal, const xmlChar * pubID,
                   const xmlChar * sysID)
{
    xmlChar *ret = KD_NULL;

    if (((pubID == KD_NULL) && (sysID == KD_NULL)) || (catal == KD_NULL))
        return (KD_NULL);

    if (xmlDebugCatalogs) {
         if ((pubID != KD_NULL) && (sysID != KD_NULL)) {
             xmlGenericError(xmlGenericErrorContext,
                             "Resolve: pubID %s sysID %s\n", pubID, sysID);
         } else if (pubID != KD_NULL) {
             xmlGenericError(xmlGenericErrorContext,
                             "Resolve: pubID %s\n", pubID);
         } else {
             xmlGenericError(xmlGenericErrorContext,
                             "Resolve: sysID %s\n", sysID);
         }
    }

    if (catal->type == XML_XML_CATALOG_TYPE) {
        ret = xmlCatalogListXMLResolve(catal->xml, pubID, sysID);
	if (ret == XML_CATAL_BREAK)
	    ret = KD_NULL;
    } else {
        const xmlChar *sgml;

        sgml = xmlCatalogSGMLResolve(catal, pubID, sysID);
        if (sgml != KD_NULL)
            ret = xmlStrdup(sgml);
    }
    return (ret);
}

/**
 * xmlACatalogResolveURI:
 * @catal:  a Catalog
 * @URI:  the URI
 *
 * Do a complete resolution lookup of an URI
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlACatalogResolveURI(xmlCatalogPtr catal, const xmlChar *URI) {
    xmlChar *ret = KD_NULL;

    if ((URI == KD_NULL) || (catal == KD_NULL))
	return(KD_NULL);

    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Resolve URI %s\n", URI);

    if (catal->type == XML_XML_CATALOG_TYPE) {
	ret = xmlCatalogListXMLResolveURI(catal->xml, URI);
	if (ret == XML_CATAL_BREAK)
	    ret = KD_NULL;
    } else {
	const xmlChar *sgml;

	sgml = xmlCatalogSGMLResolve(catal, KD_NULL, URI);
	if (sgml != KD_NULL)
            ret = xmlStrdup(sgml);
    }
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlACatalogDump:
 * @catal:  a Catalog
 * @out:  the file.
 *
 * Dump the given catalog to the given file.
 */
void
xmlACatalogDump(xmlCatalogPtr catal, KDFile *out) {
    if ((out == KD_NULL) || (catal == KD_NULL))
	return;

    if (catal->type == XML_XML_CATALOG_TYPE) {
	xmlDumpXMLCatalog(out, catal->xml);
    } else {
	xmlHashScan(catal->sgml,
		    (xmlHashScanner) xmlCatalogDumpEntry, out);
    } 
}
#endif /* LIBXML_OUTPUT_ENABLED */

/**
 * xmlACatalogAdd:
 * @catal:  a Catalog
 * @type:  the type of record to add to the catalog
 * @orig:  the system, public or prefix to match 
 * @replace:  the replacement value for the match
 *
 * Add an entry in the catalog, it may overwrite existing but
 * different entries.
 *
 * Returns 0 if successful, -1 otherwise
 */
int
xmlACatalogAdd(xmlCatalogPtr catal, const xmlChar * type,
              const xmlChar * orig, const xmlChar * replace)
{
    int res = -1;

    if (catal == KD_NULL)
	return(-1);

    if (catal->type == XML_XML_CATALOG_TYPE) {
        res = xmlAddXMLCatalog(catal->xml, type, orig, replace);
    } else {
        xmlCatalogEntryType cattype;

        cattype = xmlGetSGMLCatalogEntryType(type);
        if (cattype != XML_CATA_NONE) {
            xmlCatalogEntryPtr entry;

            entry = xmlNewCatalogEntry(cattype, orig, replace, KD_NULL,
                                       XML_CATA_PREFER_NONE, KD_NULL);
	    if (catal->sgml == KD_NULL)
		catal->sgml = xmlHashCreate(10);
            res = xmlHashAddEntry(catal->sgml, orig, entry);
        }
    }
    return (res);
}

/**
 * xmlACatalogRemove:
 * @catal:  a Catalog
 * @value:  the value to remove
 *
 * Remove an entry from the catalog
 *
 * Returns the number of entries removed if successful, -1 otherwise
 */
int
xmlACatalogRemove(xmlCatalogPtr catal, const xmlChar *value) {
    int res = -1;

    if ((catal == KD_NULL) || (value == KD_NULL))
	return(-1);

    if (catal->type == XML_XML_CATALOG_TYPE) {
	res = xmlDelXMLCatalog(catal->xml, value);
    } else {
	res = xmlHashRemoveEntry(catal->sgml, value,
		(xmlHashDeallocator) xmlFreeCatalogEntry);
	if (res == 0)
	    res = 1;
    } 
    return(res);
}

/**
 * xmlNewCatalog:
 * @sgml:  should this create an SGML catalog
 *
 * create a new Catalog.
 *
 * Returns the xmlCatalogPtr or KD_NULL in case of error
 */
xmlCatalogPtr
xmlNewCatalog(int sgml) {
    xmlCatalogPtr catal = KD_NULL;

    if (sgml) {
	catal = xmlCreateNewCatalog(XML_SGML_CATALOG_TYPE,
		                    xmlCatalogDefaultPrefer);
        if ((catal != KD_NULL) && (catal->sgml == KD_NULL))
	    catal->sgml = xmlHashCreate(10);
    } else
	catal = xmlCreateNewCatalog(XML_XML_CATALOG_TYPE,
		                    xmlCatalogDefaultPrefer);
    return(catal);
}

/**
 * xmlCatalogIsEmpty:
 * @catal:  should this create an SGML catalog
 *
 * Check is a catalog is empty
 *
 * Returns 1 if the catalog is empty, 0 if not, amd -1 in case of error.
 */
int
xmlCatalogIsEmpty(xmlCatalogPtr catal) {
    if (catal == KD_NULL)
	return(-1);

    if (catal->type == XML_XML_CATALOG_TYPE) {
	if (catal->xml == KD_NULL)
	    return(1);
	if ((catal->xml->type != XML_CATA_CATALOG) &&
	    (catal->xml->type != XML_CATA_BROKEN_CATALOG))
	    return(-1);
	if (catal->xml->children == KD_NULL)
	    return(1);
        return(0);
    } else {
	int res;

	if (catal->sgml == KD_NULL)
	    return(1);
	res = xmlHashSize(catal->sgml);
	if (res == 0)
	    return(1);
	if (res < 0)
	    return(-1);
    } 
    return(0);
}

/************************************************************************
 *									*
 *   Public interfaces manipulating the global shared default catalog	*
 *									*
 ************************************************************************/

/**
 * xmlInitializeCatalogData:
 *
 * Do the catalog initialization only of global data, doesn't try to load
 * any catalog actually.
 * this function is not thread safe, catalog initialization should
 * preferably be done once at startup
 */
static void
xmlInitializeCatalogData(void) {
    if (xmlCatalogInitialized != 0)
	return;

    //if (getenv("XML_DEBUG_CATALOG"))
	//xmlDebugCatalogs = 1;
    xmlCatalogMutex = xmlNewRMutex();

    xmlCatalogInitialized = 1;
}
/**
 * xmlInitializeCatalog:
 *
 * Do the catalog initialization.
 * this function is not thread safe, catalog initialization should
 * preferably be done once at startup
 */
void
xmlInitializeCatalog(void) {
    if (xmlCatalogInitialized != 0)
	return;

    xmlInitializeCatalogData();
    xmlRMutexLock(xmlCatalogMutex);

    //if (getenv("XML_DEBUG_CATALOG"))
	//xmlDebugCatalogs = 1;

    if (xmlDefaultCatalog == KD_NULL) {
	const char *catalogs = KD_NULL;
	char *path;
	const char *cur, *paths;
	xmlCatalogPtr catal;
	xmlCatalogEntryPtr *nextent;

	//catalogs = (const char *) getenv("XML_CATALOG_FILES");
	if (catalogs == KD_NULL)
/*
#if defined(_WIN32) && defined(_MSC_VER)
    {
		void* hmodule;
		hmodule = GetModuleHandleA("libxml2.dll");
		if (hmodule == KD_NULL)
			hmodule = GetModuleHandleA(KD_NULL);
		if (hmodule != KD_NULL) {
			char buf[256];
			unsigned long len = GetModuleFileNameA(hmodule, buf, 255);
			if (len != 0) {
				char* p = &(buf[len]);
				while (*p != '\\' && p > buf) 
					p--;
				if (p != buf) {
					xmlChar* uri;
					strncpy(p, "\\..\\etc\\catalog", 255 - (p - buf));
					uri = xmlCanonicPath(buf);
					if (uri != KD_NULL) {
						strncpy(XML_XML_DEFAULT_CATALOG, uri, 255);
						xmlFree(uri);
					}
				}
			}
		}
		catalogs = XML_XML_DEFAULT_CATALOG;
    }
#else
	    catalogs = XML_XML_DEFAULT_CATALOG;
#endif
*/
	catalogs = XML_XML_DEFAULT_CATALOG;

	catal = xmlCreateNewCatalog(XML_XML_CATALOG_TYPE, 
		xmlCatalogDefaultPrefer);
	if (catal != KD_NULL) {
	    /* the XML_CATALOG_FILES envvar is allowed to contain a 
	       space-separated list of entries. */
	    cur = catalogs;
	    nextent = &catal->xml;
	    while (*cur != '\0') {
		while (xmlIsBlank_ch(*cur)) 
		    cur++;
		if (*cur != 0) {
		    paths = cur;
		    while ((*cur != 0) && (!xmlIsBlank_ch(*cur)))
			cur++;
		    path = (char *) xmlStrndup((const xmlChar *)paths, cur - paths);
		    if (path != KD_NULL) {
			*nextent = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL,
				KD_NULL, BAD_CAST path, xmlCatalogDefaultPrefer, KD_NULL);
			if (*nextent != KD_NULL)
			    nextent = &((*nextent)->next);
			xmlFree(path);
		    }
		}
	    }
	    xmlDefaultCatalog = catal;
	}
    }

    xmlRMutexUnlock(xmlCatalogMutex);
}


/**
 * xmlLoadCatalog:
 * @filename:  a file path
 *
 * Load the catalog and makes its definitions effective for the default
 * external entity loader. It will recurse in SGML CATALOG entries.
 * this function is not thread safe, catalog initialization should
 * preferably be done once at startup
 *
 * Returns 0 in case of success -1 in case of error
 */
int
xmlLoadCatalog(const char *filename)
{
    int ret;
    xmlCatalogPtr catal;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalogData();

    xmlRMutexLock(xmlCatalogMutex);

    if (xmlDefaultCatalog == KD_NULL) {
	catal = xmlLoadACatalog(filename);
	if (catal == KD_NULL) {
	    xmlRMutexUnlock(xmlCatalogMutex);
	    return(-1);
	}

	xmlDefaultCatalog = catal;
	xmlRMutexUnlock(xmlCatalogMutex);
	return(0);
    }

    ret = xmlExpandCatalog(xmlDefaultCatalog, filename);
    xmlRMutexUnlock(xmlCatalogMutex);
    return(ret);
}

/**
 * xmlLoadCatalogs:
 * @pathss:  a list of directories separated by a colon or a space.
 *
 * Load the catalogs and makes their definitions effective for the default
 * external entity loader.
 * this function is not thread safe, catalog initialization should
 * preferably be done once at startup
 */
void
xmlLoadCatalogs(const char *pathss) {
    const char *cur;
    const char *paths;
    xmlChar *path;
  /*
#ifdef _WIN32
    int i, iLen;
#endif
*/
    if (pathss == KD_NULL)
	return;

    cur = pathss;
    while (*cur != 0) {
	while (xmlIsBlank_ch(*cur)) cur++;
	if (*cur != 0) {
	    paths = cur;
	    while ((*cur != 0) && (*cur != PATH_SEAPARATOR) && (!xmlIsBlank_ch(*cur)))
		cur++;
	    path = xmlStrndup((const xmlChar *)paths, cur - paths);
	    /*
#ifdef _WIN32
        iLen = strlen(path);
        for(i = 0; i < iLen; i++) {
            if(path[i] == '\\') {
                path[i] = '/';
            }
        }
#endif
*/
	    if (path != KD_NULL) {
		xmlLoadCatalog((const char *) path);
		xmlFree(path);
	    }
	}
	while (*cur == PATH_SEAPARATOR)
	    cur++;
    }
}

/**
 * xmlCatalogCleanup:
 *
 * Free up all the memory associated with catalogs
 */
void
xmlCatalogCleanup(void) {
    if (xmlCatalogInitialized == 0)
        return;

    xmlRMutexLock(xmlCatalogMutex);
    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Catalogs cleanup\n");
    if (xmlCatalogXMLFiles != KD_NULL)
	xmlHashFree(xmlCatalogXMLFiles, 
		    (xmlHashDeallocator)xmlFreeCatalogHashEntryList);
    xmlCatalogXMLFiles = KD_NULL;
    if (xmlDefaultCatalog != KD_NULL)
	xmlFreeCatalog(xmlDefaultCatalog);
    xmlDefaultCatalog = KD_NULL;
    xmlDebugCatalogs = 0;
    xmlCatalogInitialized = 0;
    xmlRMutexUnlock(xmlCatalogMutex);
    xmlFreeRMutex(xmlCatalogMutex);
}

/**
 * xmlCatalogResolveSystem:
 * @sysID:  the system ID string
 *
 * Try to lookup the catalog resource for a system ID
 *
 * Returns the resource if found or KD_NULL otherwise, the value returned
 *      must be freed by the caller.
 */
xmlChar *
xmlCatalogResolveSystem(const xmlChar *sysID) {
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    ret = xmlACatalogResolveSystem(xmlDefaultCatalog, sysID);
    return(ret);
}

/**
 * xmlCatalogResolvePublic:
 * @pubID:  the public ID string
 *
 * Try to lookup the catalog reference associated to a public ID
 *
 * Returns the resource if found or KD_NULL otherwise, the value returned
 *      must be freed by the caller.
 */
xmlChar *
xmlCatalogResolvePublic(const xmlChar *pubID) {
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    ret = xmlACatalogResolvePublic(xmlDefaultCatalog, pubID);
    return(ret);
}

/**
 * xmlCatalogResolve:
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlCatalogResolve(const xmlChar *pubID, const xmlChar *sysID) {
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    ret = xmlACatalogResolve(xmlDefaultCatalog, pubID, sysID);
    return(ret);
}

/**
 * xmlCatalogResolveURI:
 * @URI:  the URI
 *
 * Do a complete resolution lookup of an URI
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlCatalogResolveURI(const xmlChar *URI) {
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    ret = xmlACatalogResolveURI(xmlDefaultCatalog, URI);
    return(ret);
}

#ifdef LIBXML_OUTPUT_ENABLED
/**
 * xmlCatalogDump:
 * @out:  the file.
 *
 * Dump all the global catalog content to the given file.
 */
void
xmlCatalogDump(KDFile *out) {
    if (out == KD_NULL)
	return;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    xmlACatalogDump(xmlDefaultCatalog, out);
}
#endif /* LIBXML_OUTPUT_ENABLED */

/**
 * xmlCatalogAdd:
 * @type:  the type of record to add to the catalog
 * @orig:  the system, public or prefix to match 
 * @replace:  the replacement value for the match
 *
 * Add an entry in the catalog, it may overwrite existing but
 * different entries.
 * If called before any other catalog routine, allows to override the
 * default shared catalog put in place by xmlInitializeCatalog();
 *
 * Returns 0 if successful, -1 otherwise
 */
int
xmlCatalogAdd(const xmlChar *type, const xmlChar *orig, const xmlChar *replace) {
    int res = -1;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalogData();

    xmlRMutexLock(xmlCatalogMutex);
    /*
     * Specific case where one want to override the default catalog
     * put in place by xmlInitializeCatalog();
     */
    if ((xmlDefaultCatalog == KD_NULL) &&
	(xmlStrEqual(type, BAD_CAST "catalog"))) {
	xmlDefaultCatalog = xmlCreateNewCatalog(XML_XML_CATALOG_TYPE,
		                          xmlCatalogDefaultPrefer);
	xmlDefaultCatalog->xml = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL,
				    orig, KD_NULL,  xmlCatalogDefaultPrefer, KD_NULL);

	xmlRMutexUnlock(xmlCatalogMutex);
	return(0);
    } 

    res = xmlACatalogAdd(xmlDefaultCatalog, type, orig, replace);
    xmlRMutexUnlock(xmlCatalogMutex);
    return(res);
}

/**
 * xmlCatalogRemove:
 * @value:  the value to remove
 *
 * Remove an entry from the catalog
 *
 * Returns the number of entries removed if successful, -1 otherwise
 */
int
xmlCatalogRemove(const xmlChar *value) {
    int res;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    xmlRMutexLock(xmlCatalogMutex);
    res = xmlACatalogRemove(xmlDefaultCatalog, value);
    xmlRMutexUnlock(xmlCatalogMutex);
    return(res);
}

/**
 * xmlCatalogConvert:
 *
 * Convert all the SGML catalog entries as XML ones
 *
 * Returns the number of entries converted if successful, -1 otherwise
 */
int
xmlCatalogConvert(void) {
    int res = -1;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    xmlRMutexLock(xmlCatalogMutex);
    res = xmlConvertSGMLCatalog(xmlDefaultCatalog);
    xmlRMutexUnlock(xmlCatalogMutex);
    return(res);
}

/************************************************************************
 *									*
 *	Public interface manipulating the common preferences		*
 *									*
 ************************************************************************/

/**
 * xmlCatalogGetDefaults:
 *
 * Used to get the user preference w.r.t. to what catalogs should
 * be accepted
 *
 * Returns the current xmlCatalogAllow value
 */
xmlCatalogAllow
xmlCatalogGetDefaults(void) {
    return(xmlCatalogDefaultAllow);
}

/**
 * xmlCatalogSetDefaults:
 * @allow:  what catalogs should be accepted
 *
 * Used to set the user preference w.r.t. to what catalogs should
 * be accepted
 */
void
xmlCatalogSetDefaults(xmlCatalogAllow allow) {
    if (xmlDebugCatalogs) {
	switch (allow) {
	    case XML_CATA_ALLOW_NONE:
		xmlGenericError(xmlGenericErrorContext,
			"Disabling catalog usage\n");
		break;
	    case XML_CATA_ALLOW_GLOBAL:
		xmlGenericError(xmlGenericErrorContext,
			"Allowing only global catalogs\n");
		break;
	    case XML_CATA_ALLOW_DOCUMENT:
		xmlGenericError(xmlGenericErrorContext,
			"Allowing only catalogs from the document\n");
		break;
	    case XML_CATA_ALLOW_ALL:
		xmlGenericError(xmlGenericErrorContext,
			"Allowing all catalogs\n");
		break;
	}
    }
    xmlCatalogDefaultAllow = allow;
}

/**
 * xmlCatalogSetDefaultPrefer:
 * @prefer:  the default preference for delegation
 *
 * Allows to set the preference between public and system for deletion
 * in XML Catalog resolution. C.f. section 4.1.1 of the spec
 * Values accepted are XML_CATA_PREFER_PUBLIC or XML_CATA_PREFER_SYSTEM
 *
 * Returns the previous value of the default preference for delegation
 */
xmlCatalogPrefer
xmlCatalogSetDefaultPrefer(xmlCatalogPrefer prefer) {
    xmlCatalogPrefer ret = xmlCatalogDefaultPrefer;

    if (prefer == XML_CATA_PREFER_NONE)
	return(ret);

    if (xmlDebugCatalogs) {
	switch (prefer) {
	    case XML_CATA_PREFER_PUBLIC:
		xmlGenericError(xmlGenericErrorContext,
			"Setting catalog preference to PUBLIC\n");
		break;
	    case XML_CATA_PREFER_SYSTEM:
		xmlGenericError(xmlGenericErrorContext,
			"Setting catalog preference to SYSTEM\n");
		break;
	    case XML_CATA_PREFER_NONE:
		break;
	}
    }
    xmlCatalogDefaultPrefer = prefer;
    return(ret);
}

/**
 * xmlCatalogSetDebug:
 * @level:  the debug level of catalogs required
 *
 * Used to set the debug level for catalog operation, 0 disable
 * debugging, 1 enable it
 *
 * Returns the previous value of the catalog debugging level
 */
int
xmlCatalogSetDebug(int level) {
    int ret = xmlDebugCatalogs;

    if (level <= 0)
        xmlDebugCatalogs = 0;
    else
	xmlDebugCatalogs = level;
    return(ret);
}

/************************************************************************
 *									*
 *   Minimal interfaces used for per-document catalogs by the parser	*
 *									*
 ************************************************************************/

/**
 * xmlCatalogFreeLocal:
 * @catalogs:  a document's list of catalogs
 *
 * Free up the memory associated to the catalog list
 */
void
xmlCatalogFreeLocal(void *catalogs) {
    xmlCatalogEntryPtr catal;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    catal = (xmlCatalogEntryPtr) catalogs;
    if (catal != KD_NULL)
	xmlFreeCatalogEntryList(catal);
}


/**
 * xmlCatalogAddLocal:
 * @catalogs:  a document's list of catalogs
 * @URL:  the URL to a new local catalog
 *
 * Add the new entry to the catalog list
 *
 * Returns the updated list
 */
void *	
xmlCatalogAddLocal(void *catalogs, const xmlChar *URL) {
    xmlCatalogEntryPtr catal, add;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    if (URL == KD_NULL)
	return(catalogs);

    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Adding document catalog %s\n", URL);

    add = xmlNewCatalogEntry(XML_CATA_CATALOG, KD_NULL, URL, KD_NULL,
	                     xmlCatalogDefaultPrefer, KD_NULL);
    if (add == KD_NULL)
	return(catalogs);

    catal = (xmlCatalogEntryPtr) catalogs;
    if (catal == KD_NULL) 
	return((void *) add);

    while (catal->next != KD_NULL)
	catal = catal->next;
    catal->next = add;
    return(catalogs);
}

/**
 * xmlCatalogLocalResolve:
 * @catalogs:  a document's list of catalogs
 * @pubID:  the public ID string
 * @sysID:  the system ID string
 *
 * Do a complete resolution lookup of an External Identifier using a 
 * document's private catalog list
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlCatalogLocalResolve(void *catalogs, const xmlChar *pubID,
	               const xmlChar *sysID) {
    xmlCatalogEntryPtr catal;
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    if ((pubID == KD_NULL) && (sysID == KD_NULL))
	return(KD_NULL);

    if (xmlDebugCatalogs) {
        if ((pubID != KD_NULL) && (sysID != KD_NULL)) {
            xmlGenericError(xmlGenericErrorContext,
                            "Local Resolve: pubID %s sysID %s\n", pubID, sysID);
        } else if (pubID != KD_NULL) {
            xmlGenericError(xmlGenericErrorContext,
                            "Local Resolve: pubID %s\n", pubID);
        } else {
            xmlGenericError(xmlGenericErrorContext,
                            "Local Resolve: sysID %s\n", sysID);
        }
    }

    catal = (xmlCatalogEntryPtr) catalogs;
    if (catal == KD_NULL)
	return(KD_NULL);
    ret = xmlCatalogListXMLResolve(catal, pubID, sysID);
    if ((ret != KD_NULL) && (ret != XML_CATAL_BREAK))
	return(ret);
    return(KD_NULL);
}

/**
 * xmlCatalogLocalResolveURI:
 * @catalogs:  a document's list of catalogs
 * @URI:  the URI
 *
 * Do a complete resolution lookup of an URI using a 
 * document's private catalog list
 *
 * Returns the URI of the resource or KD_NULL if not found, it must be freed
 *      by the caller.
 */
xmlChar *
xmlCatalogLocalResolveURI(void *catalogs, const xmlChar *URI) {
    xmlCatalogEntryPtr catal;
    xmlChar *ret;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    if (URI == KD_NULL)
	return(KD_NULL);

    if (xmlDebugCatalogs)
	xmlGenericError(xmlGenericErrorContext,
		"Resolve URI %s\n", URI);

    catal = (xmlCatalogEntryPtr) catalogs;
    if (catal == KD_NULL)
	return(KD_NULL);
    ret = xmlCatalogListXMLResolveURI(catal, URI);
    if ((ret != KD_NULL) && (ret != XML_CATAL_BREAK))
	return(ret);
    return(KD_NULL);
}

/************************************************************************
 *									*
 *			Deprecated interfaces				*
 *									*
 ************************************************************************/
/**
 * xmlCatalogGetSystem:
 * @sysID:  the system ID string
 *
 * Try to lookup the catalog reference associated to a system ID
 * DEPRECATED, use xmlCatalogResolveSystem()
 *
 * Returns the resource if found or KD_NULL otherwise.
 */
const xmlChar *
xmlCatalogGetSystem(const xmlChar *sysID) {
    xmlChar *ret;
    static xmlChar result[1000];
    static int msg = 0;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    if (msg == 0) {
	xmlGenericError(xmlGenericErrorContext,
		"Use of deprecated xmlCatalogGetSystem() call\n");
	msg++;
    }

    if (sysID == KD_NULL)
	return(KD_NULL);
    
    /*
     * Check first the XML catalogs
     */
    if (xmlDefaultCatalog != KD_NULL) {
	ret = xmlCatalogListXMLResolve(xmlDefaultCatalog->xml, KD_NULL, sysID);
	if ((ret != KD_NULL) && (ret != XML_CATAL_BREAK)) {
	    kdSnprintfKHR ( (char *) result, sizeof(result) - 1, "%s", (char *) ret);
	    result[sizeof(result) - 1] = 0;
	    return(result);
	}
    }

    if (xmlDefaultCatalog != KD_NULL)
	return(xmlCatalogGetSGMLSystem(xmlDefaultCatalog->sgml, sysID));
    return(KD_NULL);
}

/**
 * xmlCatalogGetPublic:
 * @pubID:  the public ID string
 *
 * Try to lookup the catalog reference associated to a public ID
 * DEPRECATED, use xmlCatalogResolvePublic()
 *
 * Returns the resource if found or KD_NULL otherwise.
 */
const xmlChar *
xmlCatalogGetPublic(const xmlChar *pubID) {
    xmlChar *ret;
    static xmlChar result[1000];
    static int msg = 0;

    if (!xmlCatalogInitialized)
	xmlInitializeCatalog();

    if (msg == 0) {
	xmlGenericError(xmlGenericErrorContext,
		"Use of deprecated xmlCatalogGetPublic() call\n");
	msg++;
    }

    if (pubID == KD_NULL)
	return(KD_NULL);
    
    /*
     * Check first the XML catalogs
     */
    if (xmlDefaultCatalog != KD_NULL) {
	ret = xmlCatalogListXMLResolve(xmlDefaultCatalog->xml, pubID, KD_NULL);
	if ((ret != KD_NULL) && (ret != XML_CATAL_BREAK)) {
	    kdSnprintfKHR ((char *) result, sizeof(result) - 1, "%s", (char *) ret);
	    result[sizeof(result) - 1] = 0;
	    return(result);
	}
    }

    if (xmlDefaultCatalog != KD_NULL)
	return(xmlCatalogGetSGMLPublic(xmlDefaultCatalog->sgml, pubID));
    return(KD_NULL);
}

#define bottom_catalog
#include "elfgcchack.h"
#endif /* LIBXML_CATALOG_ENABLED */
