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

#include <fastrtps/utils/eClock.h>

#include "MobileControl.h"
#include "Communication.h"
#include "UserDefinedMessageNode.h"

MobileControl::MobileControl() = default;

MobileControl::~MobileControl() = default;

void MobileControl::run() {
    std::cout << "MobileControl running..." << std::endl;

    double angle = 0;
    Status status;
    Vec3 postion;
    Vec3 direction;
    std::string message;

    srand((int) time(0));
    postion.x() = rand() % 10;
    postion.z() = rand() % 10;

    while (1) {
        angle += 0.2;
        direction.x() = std::sin(angle);
        direction.z() = std::cos(angle);

        postion = postion + direction;

        status.position(postion);
        status.direction(direction);
//        std::cout << "MobileControl update status: " << status << std::endl;
        emit updateStatus(status);

        eprosima::fastrtps::eClock::my_sleep(250); // Sleep 250 ms
    }
}

Control::Control() : mobileControl(new MobileControl) {
}

Control::~Control() {
    workerThread.quit();
    workerThread.wait();
}

void Control::init() {
    connect(&workerThread, &QThread::finished, mobileControl, &QObject::deleteLater);
    connect(this, &Control::start, mobileControl, &MobileControl::run);
    mobileControl->moveToThread(&workerThread);
    workerThread.start();

    std::cout << "Control running..." << std::endl;
}

void Control::createConnect(const Communication &communication) {
    connect(mobileControl, &MobileControl::updateStatus, communication.status_node_,
            &StatusMessageNode::publishMessage);
    connect(mobileControl, &MobileControl::updateTarget, communication.target_node_,
            &TargetMessageNode::publishMessage);
}
