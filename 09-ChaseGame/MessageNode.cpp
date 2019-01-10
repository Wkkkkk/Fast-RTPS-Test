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
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/subscriber/Subscriber.h>

#include <fastrtps/Domain.h>

#include <fastrtps/utils/eClock.h>

#include "MessageNode.h"
#include "message/dataPubSubTypes.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

template
class SubListener<Status>;

template
class SubListener<Target>;

MessageNode::MessageNode() : mp_participant(nullptr), mp_publisher(nullptr), mp_subscriber(nullptr) {}

MessageNode::~MessageNode() {
    if (mp_participant) {
        Domain::removeSubscriber(mp_subscriber);
        Domain::removePublisher(mp_publisher);
        Domain::removeParticipant(mp_participant);
        mp_participant = nullptr;
    }
}

void MessageNode::run() {
    while (m_pub_listener.n_matched == 0) {
        eClock::my_sleep(250); // Sleep 250 ms
    }
}

GUID_t MessageNode::getGUID() {
    if (mp_publisher) {
        return mp_publisher->getGuid();
    }
    return GUID_t();
}

void PubListener::onPublicationMatched(Publisher *pub, MatchingInfo &info) {
    (void) pub;

    if (info.status == MATCHED_MATCHING) {
        n_matched++;
        std::cout << "Publisher matched" << std::endl;
    } else {
        n_matched--;
        std::cout << "Publisher unmatched" << std::endl;
    }
}

template<class T>
void SubListener<T>::onSubscriptionMatched(Subscriber *sub, MatchingInfo &info) {
    (void) sub;

    bool connect = false;
    if (info.status == MATCHED_MATCHING) {
        n_matched++;
        std::cout << "Subscriber matched" << std::endl;
        connect = true;
    } else {
        n_matched--;
        std::cout << "Subscriber unmatched" << std::endl;
    }

    emit m_subscription_cb(info.remoteEndpointGuid, connect);
}

template<class T>
void SubListener<T>::onNewDataMessage(Subscriber *sub) {
    // Take data
    T st;
    eClock clock;

    if (sub->takeNextData(&st, &m_info)) {
        if (m_info.sampleKind == ALIVE) {
            // Print your structure data here.
            // m_info.sample_identity.writer_guid()

            emit m_new_data_message_cb(m_info.sample_identity.writer_guid(), std::move(st));
        }
    }
}
