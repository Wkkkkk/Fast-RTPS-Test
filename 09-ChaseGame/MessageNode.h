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

#ifndef FAST_RTPS_TEST_MESSAGENODE_H
#define FAST_RTPS_TEST_MESSAGENODE_H


#include <memory>
#include <functional>

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QString>

#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/publisher/PublisherListener.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

template<class T>
class SubListener : public eprosima::fastrtps::SubscriberListener {
public:
    SubListener() : n_matched(0), n_msg(0) {};

    ~SubListener() {};

    void onSubscriptionMatched(eprosima::fastrtps::Subscriber *sub, eprosima::fastrtps::rtps::MatchingInfo &info);

    void onNewDataMessage(eprosima::fastrtps::Subscriber *sub);

    inline void
    setNewDataMessageCallback(std::function<void(QString, T)> callback) { m_new_data_message_cb = callback; }

    inline void
    setSubscriptionMatchCallback(std::function<void(QString, bool)> callback) { m_subscription_cb = callback; }

    eprosima::fastrtps::SampleInfo_t m_info;
    int n_matched;
    int n_msg;
    std::function<void(QString, T)> m_new_data_message_cb;
    std::function<void(QString, bool)> m_subscription_cb;
};

class PubListener : public eprosima::fastrtps::PublisherListener {
public:
    PubListener() : n_matched(0) {};

    ~PubListener() {};

    void onPublicationMatched(eprosima::fastrtps::Publisher *pub, eprosima::fastrtps::rtps::MatchingInfo &info);

    inline void
    setPublicationMatchCallback(std::function<void(QString, bool)> callback) { m_publication_cb = callback; }

    int n_matched;
    std::function<void(QString, bool)> m_publication_cb;
};

class MessageNode : public QObject {
Q_OBJECT
public:
    MessageNode();

    virtual ~MessageNode();

    virtual bool init() {}

    Q_DISABLE_COPY(MessageNode)

protected:
    eprosima::fastrtps::Participant *mp_participant;
    eprosima::fastrtps::Publisher *mp_publisher;
    eprosima::fastrtps::Subscriber *mp_subscriber;

private:
    PubListener m_pub_listener;
signals:

    void connectPartner(const QString &guid, bool connect);

public slots:

    virtual void run();
};

#endif //FAST_RTPS_TEST_MESSAGENODE_H
