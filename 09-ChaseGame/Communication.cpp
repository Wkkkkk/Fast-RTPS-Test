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

#include "Communication.h"
#include "UserDefinedMessageNode.h"

Communication::Communication() :
        status_node_(new StatusMessageNode),
        target_node_(new TargetMessageNode) {
}

Communication::~Communication() {
    workerThread.quit();
    workerThread.wait();
}

void Communication::init() {
    connect(&workerThread, &QThread::finished, status_node_, &QObject::deleteLater);
    connect(&workerThread, &QThread::finished, target_node_, &QObject::deleteLater);

//    connect(this, &Communication::start, status_node_, &StatusMessageNode::run);
//    connect(this, &Communication::start, target_node_, &TargetMessageNode::run);

    if (status_node_->init() && target_node_->init()) {
        status_node_->moveToThread(&workerThread);
        target_node_->moveToThread(&workerThread);

        workerThread.start();
        std::cout << "workerThread start..." << std::endl;
    } else {
        std::cout << "failed to create RTPSNode..." << std::endl;
    }
}