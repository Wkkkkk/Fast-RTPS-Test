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
#include <time.h>
#include <stdlib.h>
#include <sstream>

#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/subscriber/Subscriber.h>
#include <fastrtps/attributes/SubscriberAttributes.h>

#include <fastrtps/Domain.h>

#include <fastrtps/utils/eClock.h>

#include "RTPSNode.h"

using namespace eprosima::fastrtps;
using namespace eprosima::fastrtps::rtps;

RTPSNode::RTPSNode() : mp_participant(nullptr), mp_publisher(nullptr), mp_subscriber(nullptr) {}

RTPSNode::~RTPSNode() { Domain::removeParticipant(mp_participant); }

bool RTPSNode::init() {
    eprosima::fastrtps::Domain::loadXMLProfilesFile("Configure.xml");

    // Create RTPSParticipant
    std::string participant_profile_name = "participant_profile";
    mp_participant = Domain::createParticipant(participant_profile_name);
    if (mp_participant == nullptr)
        return false;

    // Register the type
    Domain::registerType(mp_participant, (TopicDataType *) &myType);

    // Register callback before they are called!!
    std::function<void(QString, Vec3)> position_callback = std::bind(&RTPSNode::findPartnerAt, this,
                                                                     std::placeholders::_1,
                                                                     std::placeholders::_2);
    m_sub_listener.setPositionCallBack(position_callback);

    std::function<void(QString, bool)> find_callback = std::bind(&RTPSNode::connectPartner, this, std::placeholders::_1,
                                                                 std::placeholders::_2);
    m_sub_listener.setFindCallBack(find_callback);

    // Create Publisher
    std::string profile_name = "publisher_profile";
    mp_publisher = Domain::createPublisher(mp_participant, profile_name, (PublisherListener *) &m_pub_listener);
    if (mp_publisher == nullptr) return false;
    std::cout << "Publisher created, waiting for Subscribers." << std::endl;

    // Create Subscriber
    std::string subscriber_profile_name = "subscriber_profile";
    mp_subscriber = Domain::createSubscriber(mp_participant, subscriber_profile_name,
                                             (SubscriberListener *) &m_sub_listener);
    if (mp_subscriber == nullptr) return false;
    std::cout << "Subscriber created, waiting for Publishers." << std::endl;
    return true;
}

void RTPSNode::PubListener::onPublicationMatched(Publisher *pub, MatchingInfo &info) {
    (void) pub;

    if (info.status == MATCHED_MATCHING) {
        n_matched++;
        std::cout << "Publisher matched" << std::endl;
    } else {
        n_matched--;
        std::cout << "Publisher unmatched" << std::endl;
    }
}

void RTPSNode::SubListener::onSubscriptionMatched(Subscriber *sub, MatchingInfo &info) {
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

    std::ostringstream os;
    os << info.remoteEndpointGuid;
    QString guid_str = QString::fromStdString(os.str());
    emit m_find_cb(guid_str, connect);
}

void RTPSNode::SubListener::onNewDataMessage(Subscriber *sub) {
    // Take data
    Vec3 st;
    eClock clock;

    if (sub->takeNextData(&st, &m_info)) {
        if (m_info.sampleKind == ALIVE) {
            // Print your structure data here.
            // m_info.sample_identity.writer_guid()
            std::ostringstream os;
            os << m_info.sample_identity.writer_guid();
            QString guid_str = QString::fromStdString(os.str());
            emit m_position_cb(guid_str, std::move(st));
        }
    }
}

void RTPSNode::run() {
    std::cout << "running..." << std::endl;
    while (m_pub_listener.n_matched == 0) {
        eClock::my_sleep(250); // Sleep 250 ms
    }

    // Publication code
    srand((int) time(0));

    Vec3 st;
    st.x() = rand() % 10;
    st.z() = rand() % 10;
    /* Initialize your structure here */

    int msgsent = 0;
    char ch = 'w';
    do {
        if (ch == 'w') {
            st.z()++;
        } else if (ch == 's') {
            st.z()--;
        } else if (ch == 'a') {
            st.x()--;
        } else if (ch == 'd') {
            st.x()++;
        } else {
            std::cout << "Command " << ch << " not recognized, please enter \"y/n\":";
            continue;
        }

        mp_publisher->write(&st);
        ++msgsent;
        std::cout << "Sending sample, count=" << msgsent << ", send another sample?(y-yes,n-stop): ";
    } while (std::cin >> ch);
}
