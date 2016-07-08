// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CWriteFile.h"

namespace irr
{
namespace io
{


CWriteFile::CWriteFile(const io::path& fileName, bool append)
: m_pFile ( 0 ), m_nSize ( 0 )
{
	#ifdef _DEBUG
	setDebugName("CWriteFile");
	#endif

	m_sName = fileName;
	openFile ( append );
}



CWriteFile::~CWriteFile ( )
{
	if ( m_pFile )
	{
		kdFclose ( m_pFile );
	}
}



//! returns if file is open
inline bool CWriteFile::isOpen ( ) const
{
	return m_pFile != 0;
}



//! returns how much was read
s32 CWriteFile::write(const void* buffer, u32 sizeToWrite)
{
	if ( !isOpen ( ) )
	{
		return 0;
	}

	return (s32) kdFwrite ( buffer, 1, sizeToWrite, m_pFile );
}



//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CWriteFile::seek(long finalPos, bool relativeMovement)
{
	if ( !isOpen ( ) )
	{
		return 0;
	}

	return kdFseek ( m_pFile, finalPos, relativeMovement ? KD_SEEK_CUR : KD_SEEK_SET ) == 0;
}



//! returns where in the file we are.
long CWriteFile::getPos ( ) const
{
	return kdFtell ( m_pFile );
}



//! opens the file
void CWriteFile::openFile(bool append)
{
	if ( m_sName.size ( ) == 0 )
	{
		m_pFile = 0;
		return;
	}

	m_pFile = kdFopen ( m_sName.c_str ( ), append ? "ab" : "wb" );

	if ( m_pFile )
	{
		// get FileSize
		kdFseek ( m_pFile, 0, KD_SEEK_END );
		m_nSize = kdFtell ( m_pFile );
		kdFseek ( m_pFile, 0, KD_SEEK_SET );
	}
}



//! returns name of file
const io::path& CWriteFile::getFileName() const
{
	return m_sName;
}



IWriteFile* createWriteFile(const io::path& fileName, bool append)
{
	CWriteFile*  file = new CWriteFile ( fileName, append );
	if ( file->isOpen ( ) )
	{
		return file;
	}

	file->drop();
	return 0;
}


} // end namespace io
} // end namespace irr

