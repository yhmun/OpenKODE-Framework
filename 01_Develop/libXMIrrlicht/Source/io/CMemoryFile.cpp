// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CMemoryFile.h"
#include "irrString.h"

namespace irr
{
namespace io
{


CMemoryFile::CMemoryFile(void* memory, long len, const io::path& fileName, bool d)
: m_pFile(0), m_nFileSize ( len ), m_sFilename(fileName), m_pMemory ( memory ), m_bDeleteMemoryWhenDropped(d)
{
	#ifdef _DEBUG
	setDebugName("CMemoryFile");
	#endif

	m_pFile = kdFmemopen ( memory, len, "rb" );
}


CMemoryFile::~CMemoryFile()
{
	if (m_bDeleteMemoryWhenDropped)
	{
		delete [] (c8*)m_pMemory;
	}

	if ( m_pFile )
	{
		kdFclose ( m_pFile );
	}
}


//! returns how much was read
s32 CMemoryFile::read(void* buffer, u32 sizeToRead)
{
	return (s32) ( m_pFile ? kdFread ( buffer, sizeToRead, 1, m_pFile ) : 0 );
}

//! returns how much was written
s32 CMemoryFile::write(const void* buffer, u32 sizeToWrite)
{
	return (s32) ( m_pFile ? kdFwrite ( buffer, sizeToWrite, 1, m_pFile ) : 0 );
}

//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool CMemoryFile::seek(long finalPos, bool relativeMovement)
{
	return kdFseek ( m_pFile, finalPos, relativeMovement ? KD_SEEK_CUR : KD_SEEK_SET ) == 0;
}

//! returns size of file
long CMemoryFile::getSize() const
{
	return m_nFileSize;
}

//! returns where in the file we are.
long CMemoryFile::getPos() const
{
	return kdFtell ( m_pFile );
}

//! returns name of file
const io::path& CMemoryFile::getFileName() const
{
	return m_sFilename;
}

KDFile* CMemoryFile::getFile ( ) const
{
	return m_pFile;
}

IReadFile* createMemoryReadFile(void* memory, long size, const io::path& fileName, bool deleteMemoryWhenDropped)
{
	CMemoryFile* file = new CMemoryFile(memory, size, fileName, deleteMemoryWhenDropped);
	return file;
}


} // end namespace io
} // end namespace irr

