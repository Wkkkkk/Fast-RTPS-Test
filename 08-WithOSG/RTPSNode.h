/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 1/3/19.
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

#ifndef FAST_RTPS_TEST_RTPSNODE_H
#define FAST_RTPS_TEST_RTPSNODE_H

#include <memory>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QString>

#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>

#include "message/InfoPubSubTypes.h"

class RTPSNode : public QObject {
Q_OBJECT
public:
    RTPSNode();

    virtual ~RTPSNode();

    bool init();

    Q_DISABLE_COPY(RTPSNode)

private:
    eprosima::fastrtps::Participant *mp_participant;
    eprosima::fastrtps::Publisher *mp_publisher;
    eprosima::fastrtps::Subscriber *mp_subscriber;

    class PubListener : public eprosima::fastrtps::PublisherListener {
    public:
        PubListener() : n_matched(0) {};

        ~PubListener() {};

        void onPublicationMatched(eprosima::fastrtps::Publisher *pub, eprosima::fastrtps::rtps::MatchingInfo &info);

        int n_matched;
    } m_pub_listener;

    class SubListener : public eprosima::fastrtps::SubscriberListener {
    public:
        SubListener() : n_matched(0), n_msg(0) {};

        ~SubListener() {};

        void onSubscriptionMatched(eprosima::fastrtps::Subscriber *sub, eprosima::fastrtps::rtps::MatchingInfo &info);

        void onNewDataMessage(eprosima::fastrtps::Subscriber *sub);

        inline void setPositionCallBack(std::function<void(QString, Vec3)> callback) { m_position_cb = callback; }

        inline void setFindCallBack(std::function<void(QString, bool)> callback) { m_find_cb = callback; }

        eprosima::fastrtps::SampleInfo_t m_info;
        int n_matched;
        int n_msg;
        std::function<void(QString, Vec3)> m_position_cb;
        std::function<void(QString, bool)> m_find_cb;
    } m_sub_listener;

    Vec3PubSubType myType;

public slots:

    void run();

signals:

    void connectPartner(const QString &guid, bool connect);

    void findPartnerAt(const QString &guid, const Vec3 &pos);
};

class RTPSNodeThread : public QObject {
Q_OBJECT
    QThread workerThread;
public:
    RTPSNodeThread() {}

    ~RTPSNodeThread() {
        workerThread.quit();
        workerThread.wait();
    }

    void init() {
        RTPSNode *worker = new RTPSNode;
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &RTPSNodeThread::start, worker, &RTPSNode::run);
        connect(worker, &RTPSNode::connectPartner, this, &RTPSNodeThread::createConnect);
        connect(worker, &RTPSNode::findPartnerAt, this, &RTPSNodeThread::diliverPartnerPos);

        if (worker->init()) {
            worker->moveToThread(&workerThread);
            workerThread.start();
            std::cout << "workerThread start..." << std::endl;
        } else {
            std::cout << "failed to create RTPSNode..." << std::endl;
        }
    }
    Q_DISABLE_COPY(RTPSNodeThread)

public slots:

    void diliverPartnerPos(const QString &guid, const Vec3 &pos) {
        std::cout << "receiver message: " << pos.x() << " " << pos.y() << " " << pos.z() << " from: "
                  << guid.toStdString() << std::endl;

        emit updatePositionOfNodeWithName(guid, pos);
    };

    void createConnect(const QString &guid, bool connect) {
        std::string type = "disconnect";
        if (connect) type = "connect";
        std::cout << type << " with: " << guid.toStdString() << std::endl;

        emit createOrRemoveNodeWithName(guid, connect);
    };
signals:
    void start();

    void createOrRemoveNodeWithName(QString guid, bool create);

    void updatePositionOfNodeWithName(QString guid, const Vec3 &pos);
};

#endif //FAST_RTPS_TEST_RTPSNODE_H
