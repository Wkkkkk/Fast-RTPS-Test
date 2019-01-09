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

#include "PickHandler.h"
#include "NodeTreeSearch.h"
#include "Common.h"
#include "DataStructure.h"

PickHandler::PickHandler(osg::Switch *root_node) : root_node_(root_node) {

}

PickHandler::~PickHandler() = default;


bool PickHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa) {
    auto view = dynamic_cast<osgViewer::View *>(&aa);
    if (!view) return false;

    switch (ea.getEventType()) {
        case (osgGA::GUIEventAdapter::KEYDOWN): {
            if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Escape) {
                cleanup();
                return true;
            }
            break;
        }
        case (osgGA::GUIEventAdapter::MOVE): {
            _mx = ea.getX();
            _my = ea.getY();

            return false;
        }
        case (osgGA::GUIEventAdapter::RELEASE): {
            if (_mx == ea.getX() && _my == ea.getY()) {
                // only do a pick if the mouse hasn't moved
                pick(ea, view);
            }
            return true;
        }

        default:
            return false;
    }
}


void PickHandler::pick(const osgGA::GUIEventAdapter &ea, osgViewer::View *view) {
    if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {

        osg::ref_ptr<osgUtil::LineSegmentIntersector> picker = new osgUtil::LineSegmentIntersector
                (osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized());
        osgUtil::IntersectionVisitor iv(picker.get());

        static osg::ref_ptr<osg::Switch> self_node = dynamic_cast<osg::Switch *>(
                NodeTreeSearch::findNodeWithName(root_node_, self_node_name));

        self_node->setNodeMask(1);
        view->getCamera()->accept(iv);
        self_node->setNodeMask(0);

        //intersection check
        if (picker->containsIntersections()) {
            osg::Vec3d local_point;

            std::cout << "get target" << std::endl;

            Target target;
            target.find_target(true);
            target.target_pos(Vec3(local_point.x(), local_point.y(), local_point.z()));

            Singleton<Target>::getInstance()->update(Key<Target>("All"), target);
        }
    } //left button
}

void PickHandler::cleanup() {
    Target target;
    target.find_target(false);

    Singleton<Target>::getInstance()->update(Key<Target>("All"), target);
}
