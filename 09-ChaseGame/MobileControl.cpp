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
#include "DataStructure.h"
#include "UserDefinedMessageNode.h"

MobileControl::MobileControl() : timer_(new QTimer) {
    timer_->setInterval(100);
    connect(timer_.get(), &QTimer::timeout, this, &MobileControl::update);
}

MobileControl::~MobileControl() = default;


//TODO: 1. MobileControl 不能堵塞自己的线程
//TODO: 2. MobileControl 如果以QTimer去update, QTimer不能在其他线程开启
//TODO: 3. MobileControl update会覆盖新的动作

void MobileControl::start() {
    QTimer *local_timer = new QTimer;
    connect(local_timer, &QTimer::timeout, this, &MobileControl::update);
    local_timer->setInterval(100);

    local_timer->start();
}

void MobileControl::update() {
//    std::cout << "MobileControl running..." << std::endl;
    srand((int) time(0));

    static double angle = 0;
    static Vec3 postion(rand() % 10, 0, rand() % 10);

    Target target = Singleton<Target>::getInstance()->findByID(Key<Target>("All"));
    bool find_target = target.find_target();

    Vec3 direction;
    if (!find_target) {
        angle += 0.1;
        direction.x() = std::sin(angle);
        direction.z() = std::cos(angle);
    } else {
        Vec3 target_pos = target.target_pos();

        direction.x() = target_pos.x() - postion.x();
        direction.z() = target_pos.z() - postion.z();
        direction.x() = direction.x() / 10;
        direction.z() = direction.z() / 10;

        std::cout << "MobileControl find target: " << target << std::endl;
        emit updateTarget(target);
    }
    postion = postion + direction;

    Status status;
    status.position(postion);
    status.direction(direction);
//        std::cout << "MobileControl update status: " << status << std::endl;
    emit updateStatus(status);
}

void MobileControl::receiveStatus(QString guid, const Status &status) {
    Singleton<Status>::getInstance()->update(Key<Status>(guid), status);

//    Status b = Singleton<Status>::getInstance()->findByID(Key<Status>("guid"));
}

void MobileControl::receiveTarget(QString guid, const Target &target) {
    Singleton<Target>::getInstance()->update(Key<Target>("All"), target);
}

Control::Control() : mobileControl(new MobileControl) {

}

Control::~Control() {
    workerThread.quit();
    workerThread.wait();
}

void Control::init() {
    connect(&workerThread, &QThread::finished, mobileControl, &QObject::deleteLater);
    connect(this, &Control::start, mobileControl, &MobileControl::start);
    mobileControl->moveToThread(&workerThread);
    workerThread.start();

    std::cout << "Control running..." << std::endl;
}

void Control::createConnect(const Communication &communication) {
    connect(mobileControl, &MobileControl::updateStatus, communication.status_node_,
            &StatusMessageNode::publishMessage);
    connect(mobileControl, &MobileControl::updateTarget, communication.target_node_,
            &TargetMessageNode::publishMessage);

    connect(communication.status_node_, &StatusMessageNode::receiveMessage, mobileControl,
            &MobileControl::receiveStatus);
    connect(communication.target_node_, &TargetMessageNode::receiveMessage, mobileControl,
            &MobileControl::receiveTarget);

    mobileControl->cur_guid = communication.status_node_->getGUID();

    std::cout << "cur guid is: " << mobileControl->cur_guid.toStdString() << std::endl;
}
