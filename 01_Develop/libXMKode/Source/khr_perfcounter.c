/* --------------------------------------------------------------------------
 *
 *      File            khr_perfcounter.c
 *      Description     Unified API through which different performance counter 
 *                      suppliers expose  performance counters 
 *      Version         5, 2008-06-20
 *      Author          Y.H Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (C) 2010-2012 XMSoft. All rights reserved.
 * 
 *      Contact Email: xmsoft77@gmail.com 
 * 
 * --------------------------------------------------------------------------
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * -------------------------------------------------------------------------- */

#include "kd_library.h"

// kdGetNumberOfCountersKHR : Return the number of performance counters.
KD_API KDint KD_APIENTRY kdGetNumberOfCountersKHR ( KDvoid )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdGetCounterInformationKHR : Retrieve information on a performance counter.
KD_API const KDCounterInfoKHR* KD_APIENTRY kdGetCounterInformationKHR ( KDint index )
{	
	kdSetError ( KD_ENOSYS );

	return 0;
}

// kdActivateCountersKHR : Make counters active.
KD_API KDint KD_APIENTRY kdActivateCountersKHR ( const KDint* indexes, KDint numindexes )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdDeactivateCountersKHR : Makes counters inactive.
KD_API KDint KD_APIENTRY kdDeactivateCountersKHR (const KDint* indexes, KDint numindexes )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdStartSamplingKHR : Start the performance counters sampling.
KD_API KDint KD_APIENTRY kdStartSamplingKHR ( KDvoid )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdStopSamplingKHR : Stop the performance counters sampling.
KD_API KDint KD_APIENTRY kdStopSamplingKHR ( KDvoid )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}

// kdGetCounterValuesKHR : Retrieves list of counter values.
KD_API KDint KD_APIENTRY kdGetCounterValuesKHR ( const KDint* indexes, KDint numindexes, KDint64* values )
{
	kdSetError ( KD_ENOSYS );

	return -1;
}
