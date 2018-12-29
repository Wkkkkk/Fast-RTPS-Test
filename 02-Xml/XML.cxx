// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file XML.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "XML.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>

using namespace eprosima::fastcdr::exception;

#include <utility>

XMLProfiles::XMLProfiles() {
    m_index = 0;


}

XMLProfiles::~XMLProfiles() {
}

XMLProfiles::XMLProfiles(const XMLProfiles &x) {
    m_index = x.m_index;
    m_message = x.m_message;
}

XMLProfiles::XMLProfiles(XMLProfiles &&x) {
    m_index = x.m_index;
    m_message = std::move(x.m_message);
}

XMLProfiles &XMLProfiles::operator=(const XMLProfiles &x) {
    m_index = x.m_index;
    m_message = x.m_message;

    return *this;
}

XMLProfiles &XMLProfiles::operator=(XMLProfiles &&x) {
    m_index = x.m_index;
    m_message = std::move(x.m_message);

    return *this;
}

size_t XMLProfiles::getMaxCdrSerializedSize(size_t current_alignment) {
    size_t initial_alignment = current_alignment;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + 255 + 1;


    return current_alignment - initial_alignment;
}

size_t XMLProfiles::getCdrSerializedSize(const XMLProfiles &data, size_t current_alignment) {
    (void) data;
    size_t initial_alignment = current_alignment;

    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4);


    current_alignment += 4 + eprosima::fastcdr::Cdr::alignment(current_alignment, 4) + data.message().size() + 1;


    return current_alignment - initial_alignment;
}

void XMLProfiles::serialize(eprosima::fastcdr::Cdr &scdr) const {
    scdr << m_index;
    scdr << m_message;
}

void XMLProfiles::deserialize(eprosima::fastcdr::Cdr &dcdr) {
    dcdr >> m_index;
    dcdr >> m_message;
}

size_t XMLProfiles::getKeyMaxCdrSerializedSize(size_t current_alignment) {
    size_t current_align = current_alignment;


    return current_align;
}

bool XMLProfiles::isKeyDefined() {
    return false;
}

void XMLProfiles::serializeKey(eprosima::fastcdr::Cdr &scdr) const {
    (void) scdr;


}