// Copyright (C) 2002-2011 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "COSOperator.h"



namespace irr
{

// constructor
COSOperator::COSOperator(const core::stringc& osVersion) : OperatingSystem(osVersion)
{
	#ifdef _DEBUG
	setDebugName("COSOperator");
	#endif
}


//! returns the current operating system version as string.
const core::stringc& COSOperator::getOperatingSystemVersion() const
{
	return OperatingSystem;
}


//! copies text to the clipboard
void COSOperator::copyToClipboard(const c8* text) const
{

}


//! gets text from the clipboard
//! \return Returns 0 if no string is in there.
const c8* COSOperator::getTextFromClipboard() const
{
	return 0;
}


bool COSOperator::getProcessorSpeedMHz(u32* MHz) const
{
	return false;
}

bool COSOperator::getSystemMemory(u32* Total, u32* Avail) const
{
	return false;
}


} // end namespace

