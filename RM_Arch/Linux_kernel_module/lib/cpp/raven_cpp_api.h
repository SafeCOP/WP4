/**
 * @file array_cpp_api.h
 *
 * This file simply aggregates all necessary includes to use the raven C++ api.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_RAVEN_CPP_API_H
#define KERNEL_RAVEN_CPP_API_H

#include "../c/raven_api.h"
#include "factories/Factory.h"
#include "factories/EventBufferFactory.h"
#include "factories/EventReaderFactory.h"
#include "factories/EventWriterFactory.h"
#include "Event.h"
#include "EventBuffer.h"
#include "EventBufferInfo.h"
#include "EventReader.h"
#include "EventWriter.h"
#include "IEventReader.h"
#include "SynchronizedEventReader.h"

#endif //KERNEL_RAVEN_CPP_API_H
