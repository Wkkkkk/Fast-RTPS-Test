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
#include <thread>
#include <iostream>

#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>

#include "RTPSNode.h"

#include <fastrtps/Domain.h>
#include <fastrtps/utils/eClock.h>

using namespace eprosima;
using namespace eprosima::fastrtps;

void communication_thread();

void control_thread();

void display_thread();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("FastRtps_Qt");
    QApplication::setApplicationVersion("1.0.0");

    qRegisterMetaType<Vec3>("Vec3");

    QCommandLineParser parser;
    parser.setApplicationDescription("A Test for fast-rtps message.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("mode",
                                 "working mode: show, normal. default: normal");
    parser.process(app);
    QStringList posArgs = parser.positionalArguments();
    bool show_widget = false;
    if (!posArgs.isEmpty()) {
        const auto &mode = posArgs.at(0);
        if (mode == QLatin1Literal("show")) {
            show_widget = true;
        }
    }
    std::cout << "show_widget: " << show_widget << std::endl;

    RTPSNodeThread rtpsNodeThread;
    rtpsNodeThread.start();  // trigger signal
    while (1) {
        std::cout << "Main thread working..." << std::endl;
        eClock::my_sleep(1000); // Sleep 1 s;
    }

    return app.exec();
}
