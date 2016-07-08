/*
 * SAX.c : Old SAX v1 handlers to build a tree.
 *         Deprecated except for compatibility
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 */


#define IN_LIBXML
#include "libxml.h"

#include <XMXML/xmlmemory.h>
#include <XMXML/tree.h>
#include <XMXML/parser.h>
#include <XMXML/parserInternals.h>
#include <XMXML/valid.h>
#include <XMXML/entities.h>
#include <XMXML/xmlerror.h>
#include <XMXML/debugXML.h>
#include <XMXML/xmlIO.h>
#include <XMXML/SAX.h>
#include <XMXML/uri.h>
#include <XMXML/valid.h>
#include <XMXML/HTMLtree.h>
#include <XMXML/globals.h>
#include <XMXML/SAX2.h>

#ifdef LIBXML_LEGACY_ENABLED
#ifdef LIBXML_SAX1_ENABLED
/**
 * initxmlDefaultSAXHandler:
 * @hdlr:  the SAX handler
 * @warning:  flag if non-zero sets the handler warning procedure
 *
 * Initialize the default XML SAX version 1 handler
 * DEPRECATED: use xmlSAX2InitDefaultSAXHandler() for the new SAX2 blocks
 */
void
initxmlDefaultSAXHandler(xmlSAXHandlerV1 *hdlr, int warning)
{
    
    if(hdlr->initialized == 1)
	return;

    hdlr->internalSubset = xmlSAX2InternalSubset;
    hdlr->externalSubset = xmlSAX2ExternalSubset;
    hdlr->isStandalone = xmlSAX2IsStandalone;
    hdlr->hasInternalSubset = xmlSAX2HasInternalSubset;
    hdlr->hasExternalSubset = xmlSAX2HasExternalSubset;
    hdlr->resolveEntity = xmlSAX2ResolveEntity;
    hdlr->getEntity = xmlSAX2GetEntity;
    hdlr->getParameterEntity = xmlSAX2GetParameterEntity;
    hdlr->entityDecl = xmlSAX2EntityDecl;
    hdlr->attributeDecl = xmlSAX2AttributeDecl;
    hdlr->elementDecl = xmlSAX2ElementDecl;
    hdlr->notationDecl = xmlSAX2NotationDecl;
    hdlr->unparsedEntityDecl = xmlSAX2UnparsedEntityDecl;
    hdlr->setDocumentLocator = xmlSAX2SetDocumentLocator;
    hdlr->startDocument = xmlSAX2StartDocument;
    hdlr->endDocument = xmlSAX2EndDocument;
    hdlr->startElement = xmlSAX2StartElement;
    hdlr->endElement = xmlSAX2EndElement;
    hdlr->reference = xmlSAX2Reference;
    hdlr->characters = xmlSAX2Characters;
    hdlr->cdataBlock = xmlSAX2CDataBlock;
    hdlr->ignorableWhitespace = xmlSAX2Characters;
    hdlr->processingInstruction = xmlSAX2ProcessingInstruction;
    if (warning == 0)
	hdlr->warning = KD_NULL;
    else
	hdlr->warning = xmlParserWarning;
    hdlr->error = xmlParserError;
    hdlr->fatalError = xmlParserError;

    hdlr->initialized = 1;
}

#ifdef LIBXML_HTML_ENABLED

/**
 * inithtmlDefaultSAXHandler:
 * @hdlr:  the SAX handler
 *
 * Initialize the default HTML SAX version 1 handler
 * DEPRECATED: use xmlSAX2InitHtmlDefaultSAXHandler() for the new SAX2 blocks
 */
void
inithtmlDefaultSAXHandler(xmlSAXHandlerV1 *hdlr)
{
    if(hdlr->initialized == 1)
	return;

    hdlr->internalSubset = xmlSAX2InternalSubset;
    hdlr->externalSubset = KD_NULL;
    hdlr->isStandalone = KD_NULL;
    hdlr->hasInternalSubset = KD_NULL;
    hdlr->hasExternalSubset = KD_NULL;
    hdlr->resolveEntity = KD_NULL;
    hdlr->getEntity = xmlSAX2GetEntity;
    hdlr->getParameterEntity = KD_NULL;
    hdlr->entityDecl = KD_NULL;
    hdlr->attributeDecl = KD_NULL;
    hdlr->elementDecl = KD_NULL;
    hdlr->notationDecl = KD_NULL;
    hdlr->unparsedEntityDecl = KD_NULL;
    hdlr->setDocumentLocator = xmlSAX2SetDocumentLocator;
    hdlr->startDocument = xmlSAX2StartDocument;
    hdlr->endDocument = xmlSAX2EndDocument;
    hdlr->startElement = xmlSAX2StartElement;
    hdlr->endElement = xmlSAX2EndElement;
    hdlr->reference = KD_NULL;
    hdlr->characters = xmlSAX2Characters;
    hdlr->cdataBlock = xmlSAX2CDataBlock;
    hdlr->ignorableWhitespace = xmlSAX2IgnorableWhitespace;
    hdlr->processingInstruction = xmlSAX2ProcessingInstruction;
    hdlr->comment = xmlSAX2Comment;
    hdlr->warning = xmlParserWarning;
    hdlr->error = xmlParserError;
    hdlr->fatalError = xmlParserError;

    hdlr->initialized = 1;
}

#endif /* LIBXML_HTML_ENABLED */

#ifdef LIBXML_DOCB_ENABLED
/**
 * initdocbDefaultSAXHandler:
 * @hdlr:  the SAX handler
 *
 * Initialize the default DocBook SAX version 1 handler
 * DEPRECATED: use xmlSAX2InitDocbDefaultSAXHandler() for the new SAX2 blocks
 */
void
initdocbDefaultSAXHandler(xmlSAXHandlerV1 *hdlr)
{
    if(hdlr->initialized == 1)
	return;

    hdlr->internalSubset = xmlSAX2InternalSubset;
    hdlr->externalSubset = KD_NULL;
    hdlr->isStandalone = xmlSAX2IsStandalone;
    hdlr->hasInternalSubset = xmlSAX2HasInternalSubset;
    hdlr->hasExternalSubset = xmlSAX2HasExternalSubset;
    hdlr->resolveEntity = xmlSAX2ResolveEntity;
    hdlr->getEntity = xmlSAX2GetEntity;
    hdlr->getParameterEntity = KD_NULL;
    hdlr->entityDecl = xmlSAX2EntityDecl;
    hdlr->attributeDecl = KD_NULL;
    hdlr->elementDecl = KD_NULL;
    hdlr->notationDecl = KD_NULL;
    hdlr->unparsedEntityDecl = KD_NULL;
    hdlr->setDocumentLocator = xmlSAX2SetDocumentLocator;
    hdlr->startDocument = xmlSAX2StartDocument;
    hdlr->endDocument = xmlSAX2EndDocument;
    hdlr->startElement = xmlSAX2StartElement;
    hdlr->endElement = xmlSAX2EndElement;
    hdlr->reference = xmlSAX2Reference;
    hdlr->characters = xmlSAX2Characters;
    hdlr->cdataBlock = KD_NULL;
    hdlr->ignorableWhitespace = xmlSAX2IgnorableWhitespace;
    hdlr->processingInstruction = KD_NULL;
    hdlr->comment = xmlSAX2Comment;
    hdlr->warning = xmlParserWarning;
    hdlr->error = xmlParserError;
    hdlr->fatalError = xmlParserError;

    hdlr->initialized = 1;
}

#endif /* LIBXML_DOCB_ENABLED */

#endif /* LIBXML_SAX1_ENABLED */

#define bottom_SAX
#include "elfgcchack.h"
#endif /* LIBXML_LEGACY_ENABLED */
