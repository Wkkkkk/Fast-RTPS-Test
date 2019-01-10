/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 1/8/19.
 * Contact with:wk707060335@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http: *www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "DataStructure.h"
#include "message/data.h"

//static member initialize:
// <data type> <class name>::<member name>=<value>
#define DECLARE_SINGLETON_CLASS(Type) \
template <> \
std::shared_ptr<Singleton<Type>> Singleton<Type>::instance(new Singleton<Type>);

DECLARE_SINGLETON_CLASS(Vec3)
DECLARE_SINGLETON_CLASS(Status)
DECLARE_SINGLETON_CLASS(Target)