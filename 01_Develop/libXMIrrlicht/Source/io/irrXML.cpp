// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine" and the "irrXML" project.
// For conditions of distribution and use, see copyright notice in irrlicht.h and/or irrXML.h

#include "irrXML.h"
#include "irrString.h"
#include "irrArray.h"
#include "fast_atof.h"
#include "CXMLReaderImpl.h"

namespace irr
{
namespace io
{

//! Implementation of the file read callback for ordinary files
class CFileReadCallBack : public IFileReadCallBack
{
public:

	//! construct from filename
	CFileReadCallBack(const char* filename)
		: m_pFile ( 0 ), m_nSize ( -1 ), m_bClose ( true )
	{
		// open file
		m_pFile = kdFopen ( filename, "rb" );

		if ( m_pFile )
		{
			getFileSize ( );
		}
	}

	//! construct from FILE pointer
	CFileReadCallBack ( KDFile* file )
		: m_pFile ( file ), m_nSize ( -1 ), m_bClose ( false )
	{
		if ( m_pFile )
		{
			getFileSize ( );
		}
	}

	//! destructor
	virtual ~CFileReadCallBack ( )
	{
		if ( m_bClose && m_pFile )
		{
			kdFclose ( m_pFile );
		}
	}

	//! Reads an amount of bytes from the file.
	virtual int read(void* buffer, int sizeToRead)
	{
		if ( !m_pFile )
		{
			return 0;
		}

		return (int) kdFread ( buffer, 1, sizeToRead, m_pFile );
	}

	//! Returns size of file in bytes
	virtual long getSize ( ) const
	{
		return m_nSize;
	}

private:

	//! retrieves the file size of the open file
	void getFileSize ( )
	{
		kdFseek ( m_pFile, 0, KD_SEEK_END );
		m_nSize = kdFtell ( m_pFile );
		kdFseek ( m_pFile, 0, KD_SEEK_SET );
	}

	KDFile*   m_pFile;
	long      m_nSize;
	bool      m_bClose;

}; // end class CFileReadCallBack



// FACTORY FUNCTIONS:


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IRRLICHT_API IrrXMLReader* IRRCALLCONV createIrrXMLReader(const char* filename)
{
	return createIrrXMLReader(new CFileReadCallBack(filename), true); 
}


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IRRLICHT_API IrrXMLReader* IRRCALLCONV createIrrXMLReader(KDFile* file)
{
	return createIrrXMLReader(new CFileReadCallBack(file), true); 
}


//! Creates an instance of an UFT-8 or ASCII character xml parser. 
IRRLICHT_API IrrXMLReader* IRRCALLCONV createIrrXMLReader(IFileReadCallBack* callback,
														  bool deleteCallback)
{
	if (callback && (callback->getSize() >= 0))
	{
		return new CXMLReaderImpl<char, IXMLBase>(callback, deleteCallback); 
	}
	else
	{
		if(callback && deleteCallback)
			delete callback;

		return 0;
	}
}


//! Creates an instance of an UTF-16 xml parser. 
IRRLICHT_API IrrXMLReaderUTF16* IRRCALLCONV createIrrXMLReaderUTF16(const char* filename)
{
	return createIrrXMLReaderUTF16(new CFileReadCallBack(filename), true); 
}


//! Creates an instance of an UTF-16 xml parser. 
IRRLICHT_API IrrXMLReaderUTF16* IRRCALLCONV createIrrXMLReaderUTF16(KDFile* file)
{
	return createIrrXMLReaderUTF16(new CFileReadCallBack(file), true); 
}


//! Creates an instance of an UTF-16 xml parser. 
IRRLICHT_API IrrXMLReaderUTF16* IRRCALLCONV createIrrXMLReaderUTF16(IFileReadCallBack* callback,
																	bool deleteCallback)
{
	if (callback && (callback->getSize() >= 0))
	{
		return new CXMLReaderImpl<char16, IXMLBase>(callback, deleteCallback); 
	}
	else
	{
		if(callback && deleteCallback)
			delete callback;

		return 0;
	}
}


//! Creates an instance of an UTF-32 xml parser. 
IRRLICHT_API IrrXMLReaderUTF32* IRRCALLCONV createIrrXMLReaderUTF32(const char* filename)
{
	return createIrrXMLReaderUTF32(new CFileReadCallBack(filename), true); 
}


//! Creates an instance of an UTF-32 xml parser. 
IRRLICHT_API IrrXMLReaderUTF32* IRRCALLCONV createIrrXMLReaderUTF32(KDFile* file)
{
	return createIrrXMLReaderUTF32(new CFileReadCallBack(file), true); 
}


//! Creates an instance of an UTF-32 xml parser. 
IRRLICHT_API IrrXMLReaderUTF32* IRRCALLCONV createIrrXMLReaderUTF32(
		IFileReadCallBack* callback, bool deleteCallback)
{
	if (callback && (callback->getSize() >= 0))
	{
		return new CXMLReaderImpl<char32, IXMLBase>(callback, deleteCallback); 
	}
	else
	{
		if(callback && deleteCallback)
			delete callback;

		return 0;
	}
}


} // end namespace io
} // end namespace irr
