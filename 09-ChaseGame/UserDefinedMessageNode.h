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

#ifndef FAST_RTPS_TEST_USERDEFINEDMESSAGENODE_H
#define FAST_RTPS_TEST_USERDEFINEDMESSAGENODE_H

#include "MessageNode.h"

#include "message/data.h"
#include "message/dataPubSubTypes.h"

class StatusMessageNode : public MessageNode {
Q_OBJECT
public:
    StatusMessageNode() = default;

    ~StatusMessageNode() final = default;

    bool init() final;
    Q_DISABLE_COPY(StatusMessageNode)

private:
    PubListener m_pub_listener;
    SubListener<Status> m_sub_listener;

    StatusPubSubType myType;
signals:

    void receiveMessage(QString guid, Status status);

public slots:

    void publishMessage(Status status);
};

class TargetMessageNode : public MessageNode {
Q_OBJECT
public:
    TargetMessageNode() = default;

    ~TargetMessageNode() final = default;

    bool init() final;
    Q_DISABLE_COPY(TargetMessageNode)

private:
    PubListener m_pub_listener;
    SubListener<Target> m_sub_listener;

    TargetPubSubType myType;
signals:

    void receiveMessage(QString guid, Target pos);

public slots:

    void publishMessage(Target target);
};

#endif //FAST_RTPS_TEST_USERDEFINEDMESSAGENODE_H
