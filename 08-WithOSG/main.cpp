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
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("FastRtps_OSG");
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

    MainWindow main_window;
    main_window.setMinimumSize(800, 600);  //graphic_context bugs!
    main_window.createConnect(rtpsNodeThread);
    main_window.showMaximized();

    rtpsNodeThread.init();
    rtpsNodeThread.start();  // trigger signal

    return app.exec();
}
