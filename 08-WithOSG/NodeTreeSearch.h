/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 11/19/18.
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

#ifndef POINTCLOUDVIEWER_NODETREESEARCH_H
#define POINTCLOUDVIEWER_NODETREESEARCH_H

#include <string>
#include <iostream>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Switch>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeVisitor>

class NodeTreeSearch : public osg::NodeVisitor {
public:
    explicit NodeTreeSearch(const char name[]) :
            osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
            node_(nullptr),
            name_(name) {
    }

    void apply(osg::Switch &search_node) override {
        if (search_node.getName() == name_) {
            node_ = &search_node;
        }
        traverse(search_node);
    }

    void apply(osg::PositionAttitudeTransform &search_node) override {
        if (search_node.getName() == name_) {
            node_ = &search_node;
        }
        traverse(search_node);
    }

    void apply(osg::Geode &search_node) override {
        if (search_node.getName() == name_) {
            node_ = &search_node;
        }
        traverse(search_node);
    }

    void apply(osg::Node &search_node) override {
        if (search_node.getName() == name_) {
            node_ = &search_node;
        }
        traverse(search_node);
    }

    osg::Node *getNode() {
        return node_;
    }

    static osg::Node *findNodeWithName(osg::Switch *root_node, const char name[]) {
        auto visitor = new NodeTreeSearch(name);
        root_node->accept(*visitor);
        auto node = visitor->getNode();

        if (!node) std::cout << "aha?" << std::endl;

        return node;
    }

private:
    osg::Node *node_;
    std::string name_;
};

#endif //POINTCLOUDVIEWER_NODETREESEARCH_H
