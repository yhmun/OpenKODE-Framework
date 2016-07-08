/* --------------------------------------------------------------------------
 *
 *      File            khr_perfcounter.h
 *      Description     Unified API through which different performance counter 
 *                      suppliers expose  performance counters 
 *      Version         5, 2008-06-20
 *      Author          Young-Hwan Mun
 * 
 * --------------------------------------------------------------------------
 *      
 *      Copyright (c) 2010-2014 XMSoft. All rights reserved.
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

#ifndef __kd_KHR_perfcounter_h_
#define __kd_KHR_perfcounter_h_

#include <KD/kd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KD_INFINITE_COUNTER_VAL_KHR   KDINT64_MAX
#define KD_UNKNOWN_COUNTER_VAL_KHR    -1

// KDCounterInfoKHR : Information on a single performance counter.
typedef struct KDCounterInfoKHR 
{
    const KDchar*   vendorName;
    const KDchar*   name;
    const KDchar*   description;
          KDint64   minValue;
          KDint64   maxValue;
          KDfloat32 defaultScale;
} KDCounterInfoKHR;

// kdGetNumberOfCountersKHR : Return the number of performance counters.
KD_API KDint KD_APIENTRY kdGetNumberOfCountersKHR ( KDvoid );

// kdGetCounterInformationKHR : Retrieve information on a performance counter.
KD_API const KDCounterInfoKHR* KD_APIENTRY kdGetCounterInformationKHR ( KDint index );

// kdActivateCountersKHR : Make counters active.
KD_API KDint KD_APIENTRY kdActivateCountersKHR ( const KDint* indexes, KDint numindexes );

// kdDeactivateCountersKHR : Makes counters inactive.
KD_API KDint KD_APIENTRY kdDeactivateCountersKHR (const KDint* indexes, KDint numindexes );

// kdStartSamplingKHR : Start the performance counters sampling.
KD_API KDint KD_APIENTRY kdStartSamplingKHR ( KDvoid );

// kdStopSamplingKHR : Stop the performance counters sampling.
KD_API KDint KD_APIENTRY kdStopSamplingKHR ( KDvoid );

// kdGetCounterValuesKHR : Retrieves list of counter values.
KD_API KDint KD_APIENTRY kdGetCounterValuesKHR ( const KDint* indexes, KDint numindexes, KDint64* values );

#ifdef __cplusplus
}
#endif

#endif 

