/*
 * Summary: Tree debugging APIs
 * Description: Interfaces to a set of routines used for debugging the tree
 *              produced by the XML parser.
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __DEBUG_XML__
#define __DEBUG_XML__

#include <XMXml/xmlversion.h>
#include <XMXml/tree.h>

#ifdef LIBXML_DEBUG_ENABLED

#include <XMXml/xpath.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The standard Dump routines.
 */
XMLPUBFUN void XMLCALL	
	xmlDebugDumpString	(KDFile *output,
				 const xmlChar *str);
XMLPUBFUN void XMLCALL	
	xmlDebugDumpAttr	(KDFile *output,
				 xmlAttrPtr attr,
				 int depth);
XMLPUBFUN void XMLCALL	
	xmlDebugDumpAttrList	(KDFile *output,
				 xmlAttrPtr attr,
				 int depth);
XMLPUBFUN void XMLCALL	
	xmlDebugDumpOneNode	(KDFile *output,
				 xmlNodePtr node,
				 int depth);
XMLPUBFUN void XMLCALL
	xmlDebugDumpNode	(KDFile *output,
				 xmlNodePtr node,
				 int depth);
XMLPUBFUN void XMLCALL
	xmlDebugDumpNodeList	(KDFile *output,
				 xmlNodePtr node,
				 int depth);
XMLPUBFUN void XMLCALL
	xmlDebugDumpDocumentHead(KDFile *output,
				 xmlDocPtr doc);
XMLPUBFUN void XMLCALL
	xmlDebugDumpDocument	(KDFile *output,
				 xmlDocPtr doc);
XMLPUBFUN void XMLCALL	
	xmlDebugDumpDTD		(KDFile *output,
				 xmlDtdPtr dtd);
XMLPUBFUN void XMLCALL	
	xmlDebugDumpEntities	(KDFile *output,
				 xmlDocPtr doc);

/****************************************************************
 *								*
 *	 		Checking routines			*
 *								*
 ****************************************************************/

XMLPUBFUN int XMLCALL
	xmlDebugCheckDocument	(KDFile * output,
				 xmlDocPtr doc);

/****************************************************************
 *								*
 *	 		XML shell helpers			*
 *								*
 ****************************************************************/

XMLPUBFUN void XMLCALL	
	xmlLsOneNode		(KDFile *output, xmlNodePtr node);
XMLPUBFUN int XMLCALL	
	xmlLsCountNode		(xmlNodePtr node);

XMLPUBFUN const char * XMLCALL 
	xmlBoolToText		(int boolval);

/****************************************************************
 *								*
 *	 The XML shell related structures and functions		*
 *								*
 ****************************************************************/

#ifdef LIBXML_XPATH_ENABLED
/**
 * xmlShellReadlineFunc:
 * @prompt:  a string prompt
 *
 * This is a generic signature for the XML shell input function.
 *
 * Returns a string which will be freed by the Shell.
 */
typedef char * (* xmlShellReadlineFunc)(char *prompt);

/**
 * xmlShellCtxt:
 *
 * A debugging shell context.
 * TODO: add the defined function tables.
 */
typedef struct _xmlShellCtxt xmlShellCtxt;
typedef xmlShellCtxt *xmlShellCtxtPtr;
struct _xmlShellCtxt {
    char *filename;
    xmlDocPtr doc;
    xmlNodePtr node;
    xmlXPathContextPtr pctxt;
    int loaded;
    KDFile *output;
    xmlShellReadlineFunc input;
};

/**
 * xmlShellCmd:
 * @ctxt:  a shell context
 * @arg:  a string argument
 * @node:  a first node
 * @node2:  a second node
 *
 * This is a generic signature for the XML shell functions.
 *
 * Returns an int, negative returns indicating errors.
 */
typedef int (* xmlShellCmd) (xmlShellCtxtPtr ctxt,
                             char *arg,
			     xmlNodePtr node,
			     xmlNodePtr node2);

XMLPUBFUN void XMLCALL	
	xmlShellPrintXPathError	(int errorType,
				 const char *arg);
XMLPUBFUN void XMLCALL	
	xmlShellPrintXPathResult(xmlXPathObjectPtr list);
XMLPUBFUN int XMLCALL	
	xmlShellList		(xmlShellCtxtPtr ctxt,
				 char *arg,
				 xmlNodePtr node,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellBase		(xmlShellCtxtPtr ctxt,
				 char *arg,
				 xmlNodePtr node,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellDir		(xmlShellCtxtPtr ctxt,
				 char *arg,
				 xmlNodePtr node,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellLoad		(xmlShellCtxtPtr ctxt,
				 char *filename,
				 xmlNodePtr node,
				 xmlNodePtr node2);
#ifdef LIBXML_OUTPUT_ENABLED
XMLPUBFUN void XMLCALL	
	xmlShellPrintNode	(xmlNodePtr node);
XMLPUBFUN int XMLCALL	
	xmlShellCat		(xmlShellCtxtPtr ctxt,
				 char *arg,
				 xmlNodePtr node,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellWrite		(xmlShellCtxtPtr ctxt,
				 char *filename,
				 xmlNodePtr node,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellSave		(xmlShellCtxtPtr ctxt,
				 char *filename,
				 xmlNodePtr node,
				 xmlNodePtr node2);
#endif /* LIBXML_OUTPUT_ENABLED */
#ifdef LIBXML_VALID_ENABLED
XMLPUBFUN int XMLCALL	
	xmlShellValidate	(xmlShellCtxtPtr ctxt,
				 char *dtd,
				 xmlNodePtr node,
				 xmlNodePtr node2);
#endif /* LIBXML_VALID_ENABLED */
XMLPUBFUN int XMLCALL	
	xmlShellDu		(xmlShellCtxtPtr ctxt,
				 char *arg,
				 xmlNodePtr tree,
				 xmlNodePtr node2);
XMLPUBFUN int XMLCALL	
	xmlShellPwd		(xmlShellCtxtPtr ctxt,
				 char *buffer,
				 xmlNodePtr node,
				 xmlNodePtr node2);

/*
 * The Shell interface.
 */
XMLPUBFUN void XMLCALL	
	xmlShell		(xmlDocPtr doc,
				 char *filename,
				 xmlShellReadlineFunc input,
				 KDFile *output);
			 
#endif /* LIBXML_XPATH_ENABLED */

#ifdef __cplusplus
}
#endif

#endif /* LIBXML_DEBUG_ENABLED */
#endif /* __DEBUG_XML__ */
