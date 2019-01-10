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

#include <math.h>
#include <random>

#include <fastrtps/utils/eClock.h>

#include "MobileControl.h"
#include "Communication.h"
#include "DataStructure.h"
#include "UserDefinedMessageNode.h"

MobileControl::MobileControl() : timer_(new QTimer) {
    timer_->setInterval(250);
    connect(timer_.get(), &QTimer::timeout, this, &MobileControl::update);
}

MobileControl::~MobileControl() = default;

void MobileControl::start() {
    timer_->start();
}

void MobileControl::update() {
//    std::cout << "MobileControl running..." << std::endl;
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> pos_distribution(-10, 10);
    static std::uniform_int_distribution<unsigned> speed_distribution(0, 5);
    static std::uniform_real_distribution<double> dir_distribution(-M_PI, M_PI);

    static double angle = 0;
    static Vec3 postion(pos_distribution(e), 0, pos_distribution(e));
    static unsigned int speed = speed_distribution(e);
    Vec3 direction;

    // update target
    Target self_target = Singleton<Target>::getInstance()->findByID(Key<Target>(cur_guid));
    emit updateTarget(self_target);

    // update direction
    bool self_find_target = self_target.find_target();
    if (self_find_target) {
        std::cout << "find self target: " << self_target << std::endl;

        Vec3 target_pos = self_target.target_pos();

        direction.x() = target_pos.x() - postion.x();
        direction.z() = target_pos.z() - postion.z();
//        std::cout << "MobileControl find target: " << cur_guid << " " << self_target << std::endl;
    } else {
        std::vector<Target> targets = Singleton<Target>::getInstance()->findByFilter(
                [](const Target &target) { return target.find_target(); });

        if (targets.empty()) {
            angle = dir_distribution(e);
            direction.x() = std::sin(angle);
            direction.z() = std::cos(angle);
        } else {
            Target target = targets.front();
            Vec3 target_pos = target.target_pos();

            direction.x() = target_pos.x() - postion.x();
            direction.z() = target_pos.z() - postion.z();
//            std::cout << "Other MobileControl find target: " << target << std::endl;
        }
    }
    // update positon
    direction.normalize();
    postion = postion + direction * speed;

    // update status
    Status status;
    status.position(postion);
    status.direction(direction);
//        std::cout << "MobileControl update status: " << status << std::endl;
    emit updateStatus(status);
}

void MobileControl::receiveStatus(GUID_t guid, const Status &status) {
    Singleton<Status>::getInstance()->update(Key<Status>(guid), status);

//    Status b = Singleton<Status>::getInstance()->findByID(Key<Status>("guid"));
}

void MobileControl::receiveTarget(GUID_t guid, const Target &target) {
//    std::cout << cur_guid << "receiveTarget from: " << guid << " " << target << std::endl;
    Singleton<Target>::getInstance()->update(Key<Target>(guid), target);
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
    mobileControl->timer_->moveToThread(&workerThread);
    workerThread.start();

    std::cout << "Control start..." << std::endl;
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

    mobileControl->cur_guid = communication.target_node_->getGUID();

    std::cout << "createConnect cur guid is: " << mobileControl->cur_guid << std::endl;
}
