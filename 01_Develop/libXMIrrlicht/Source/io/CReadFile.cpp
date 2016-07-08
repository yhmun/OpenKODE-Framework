// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CReadFile.h"

namespace irr
{
namespace io
{


CReadFile::CReadFile(const io::path& fileName)
: m_pFile(0), m_nFileSize(0), m_sFilename(fileName)
{
	#ifdef _DEBUG
	setDebugName("CReadFile");
	#endif

	openFile();
}


CReadFile::~CReadFile()
{
	if ( m_pFile )
	{
		kdFclose ( m_pFile );
	}
}


//! returns how much was read
s32 CReadFile::read(void* buffer, u32 sizeToRead)
{
	if ( !isOpen ( ) )
	{
		return false;
	}

	return (s32) kdFread ( buffer, 1, sizeToRead, m_pFile );
}


//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CReadFile::seek(long finalPos, bool relativeMovement)
{
	if ( !isOpen ( ) )
	{
		return false;
	}

	return kdFseek ( m_pFile, finalPos, relativeMovement ? KD_SEEK_CUR : KD_SEEK_SET ) == 0;
}


//! returns size of file
long CReadFile::getSize() const
{
	return m_nFileSize;
}


//! returns where in the file we are.
long CReadFile::getPos() const
{
	return kdFtell ( m_pFile );
}

KDFile* CReadFile::getFile() const
{
	return m_pFile;
}

//! opens the file
void CReadFile::openFile()
{
	if ( m_sFilename.size ( ) == 0 ) // bugfix posted by rt
	{
		m_pFile = 0;
		return;
	}

	m_pFile = kdFopen ( m_sFilename.c_str ( ), "rb") ;

	if ( m_pFile )
	{
		// get FileSize

		kdFseek ( m_pFile, 0, KD_SEEK_END );
		m_nFileSize = getPos ( );
		kdFseek ( m_pFile, 0, KD_SEEK_SET );
	}
}


//! returns name of file
const io::path& CReadFile::getFileName() const
{
	return m_sFilename;
}



IReadFile* createReadFile(const io::path& fileName)
{
	CReadFile* file = new CReadFile(fileName);
	if (file->isOpen())
		return file;

	file->drop();
	return 0;
}


} // end namespace io
} // end namespace irr

