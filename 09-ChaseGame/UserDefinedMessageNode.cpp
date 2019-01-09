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
#include <fastrtps/participant/Participant.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/publisher/Publisher.h>
#include <fastrtps/subscriber/Subscriber.h>

#include <fastrtps/Domain.h>

#include "UserDefinedMessageNode.h"

bool StatusMessageNode::init() {
    eprosima::fastrtps::Domain::loadXMLProfilesFile("StatusMessage.xml");

    // Create RTPSParticipant
    std::string participant_profile_name = "participant_status";
    mp_participant = Domain::createParticipant(participant_profile_name);
    if (mp_participant == nullptr)
        return false;

    // Register the type
    Domain::registerType(mp_participant, (TopicDataType *) &myType);

    // Register callback before they are called!!
    std::function<void(QString, Status)> new_message_callback = std::bind(&StatusMessageNode::receiveMessage, this,
                                                                          std::placeholders::_1, std::placeholders::_2);
    m_sub_listener.setNewDataMessageCallback(new_message_callback);

    std::function<void(QString, bool)> match_callback = std::bind(&MessageNode::connectPartner, this,
                                                                  std::placeholders::_1, std::placeholders::_2);
    m_sub_listener.setSubscriptionMatchCallback(match_callback);

    // Create Publisher
    std::string profile_name = "publisher_status";
    mp_publisher = Domain::createPublisher(mp_participant, profile_name, (PublisherListener *) &m_pub_listener);
    if (mp_publisher == nullptr) return false;
    std::cout << "Publisher created, waiting for Subscribers." << std::endl;

    // Create Subscriber
    std::string subscriber_profile_name = "subscriber_status";
    mp_subscriber = Domain::createSubscriber(mp_participant, subscriber_profile_name,
                                             (SubscriberListener *) &m_sub_listener);
    if (mp_subscriber == nullptr) return false;
    std::cout << "Subscriber created, waiting for Publishers." << std::endl;
    return true;
}

void StatusMessageNode::publishMessage(Status status) {
    static int msgsent = 0;

    mp_publisher->write(&status);
    ++msgsent;
//    std::cout << "StatusMessageNode sending sample, count=" << msgsent << std::endl;
}

bool TargetMessageNode::init() {
    eprosima::fastrtps::Domain::loadXMLProfilesFile("TargetMessage.xml");

    // Create RTPSParticipant
    std::string participant_profile_name = "participant_target";
    mp_participant = Domain::createParticipant(participant_profile_name);
    if (mp_participant == nullptr)
        return false;

    // Register the type
    Domain::registerType(mp_participant, (TopicDataType *) &myType);

    // Register callback before they are called!!
    std::function<void(QString, Target)> new_message_callback = std::bind(&TargetMessageNode::receiveMessage, this,
                                                                          std::placeholders::_1, std::placeholders::_2);
    m_sub_listener.setNewDataMessageCallback(new_message_callback);

    std::function<void(QString, bool)> match_callback = std::bind(&MessageNode::connectPartner, this,
                                                                  std::placeholders::_1, std::placeholders::_2);
    m_sub_listener.setSubscriptionMatchCallback(match_callback);

    // Create Publisher
    std::string profile_name = "publisher_target";
    mp_publisher = Domain::createPublisher(mp_participant, profile_name, (PublisherListener *) &m_pub_listener);
    if (mp_publisher == nullptr) return false;
    std::cout << "Publisher created, waiting for Subscribers." << std::endl;

    // Create Subscriber
    std::string subscriber_profile_name = "subscriber_target";
    mp_subscriber = Domain::createSubscriber(mp_participant, subscriber_profile_name,
                                             (SubscriberListener *) &m_sub_listener);
    if (mp_subscriber == nullptr) return false;
    std::cout << "Subscriber created, waiting for Publishers." << std::endl;
    return true;
}

void TargetMessageNode::publishMessage(Target target) {
    static int msgsent = 0;

    mp_publisher->write(&target);
    ++msgsent;
    std::cout << "TargetMessageNode sending sample2: " << msgsent << " " << target << std::endl;
}
