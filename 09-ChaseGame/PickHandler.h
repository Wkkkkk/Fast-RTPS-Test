/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 1/9/19.
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

#ifndef FAST_RTPS_TEST_PICKHANDLER_H
#define FAST_RTPS_TEST_PICKHANDLER_H


#include <iostream>
#include <string>

#include <osg/Vec3d>
#include <osg/Vec4f>
#include <osg/Geode>
#include <osgViewer/View>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>

class PickHandler : public osgGA::GUIEventHandler {
public:
    explicit PickHandler(osg::Switch *root_node);

    ~PickHandler() final;

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) final;

    void pick(const osgGA::GUIEventAdapter &ea, osgViewer::View *view);

private:
    void cleanup();

    osg::ref_ptr<osg::Switch> root_node_;

    float _mx, _my;
};

#endif //FAST_RTPS_TEST_PICKHANDLER_H
