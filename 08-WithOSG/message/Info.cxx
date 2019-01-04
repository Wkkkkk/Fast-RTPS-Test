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
 * @file Info.cpp
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifdef _WIN32
// Remove linker warning LNK4221 on Visual Studio
namespace { char dummy; }
#endif

#include "Info.h"
#include <fastcdr/Cdr.h>

#include <fastcdr/exceptions/BadParamException.h>

using namespace eprosima::fastcdr::exception;

#include <utility>
#include <iostream>

Vec3::Vec3() {
    m_x = 0.0;

    m_y = 0.0;

    m_z = 0.0;

//    std::cout << "Vec3 default constructor is called..." << std::endl;
}

Vec3::~Vec3() {
//    std::cout << "Vec3 destructor is called..." << std::endl;
}

Vec3::Vec3(const Vec3 &x) {
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
//    std::cout << "Vec3 copy constructor is called..." << std::endl;
}

Vec3::Vec3(Vec3 &&x) {
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
//    std::cout << "Vec3 move constructor is called..." << std::endl;
}

Vec3 &Vec3::operator=(const Vec3 &x) {
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
//    std::cout << "Vec3 copy assignment operator is called..." << std::endl;

    return *this;
}

Vec3 &Vec3::operator=(Vec3 &&x) {
    m_x = x.m_x;
    m_y = x.m_y;
    m_z = x.m_z;
//    std::cout << "Vec3 move assignment operator is called..." << std::endl;

    return *this;
}

size_t Vec3::getMaxCdrSerializedSize(size_t current_alignment) {
    size_t initial_alignment = current_alignment;

    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    return current_alignment - initial_alignment;
}

size_t Vec3::getCdrSerializedSize(const Vec3 &data, size_t current_alignment) {
    (void) data;
    size_t initial_alignment = current_alignment;

    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    current_alignment += 8 + eprosima::fastcdr::Cdr::alignment(current_alignment, 8);


    return current_alignment - initial_alignment;
}

void Vec3::serialize(eprosima::fastcdr::Cdr &scdr) const {
    scdr << m_x;
    scdr << m_y;
    scdr << m_z;
}

void Vec3::deserialize(eprosima::fastcdr::Cdr &dcdr) {
    dcdr >> m_x;
    dcdr >> m_y;
    dcdr >> m_z;
}

size_t Vec3::getKeyMaxCdrSerializedSize(size_t current_alignment) {
    size_t current_align = current_alignment;


    return current_align;
}

bool Vec3::isKeyDefined() {
    return false;
}

void Vec3::serializeKey(eprosima::fastcdr::Cdr &scdr) const {
    (void) scdr;


}
