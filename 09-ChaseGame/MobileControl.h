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

#ifndef FAST_RTPS_TEST_MOBILECONTROL_H
#define FAST_RTPS_TEST_MOBILECONTROL_H

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QString>

class Status;

class Target;

class Communication;

class MobileControl : public QObject {
Q_OBJECT
public:
    MobileControl();

    ~MobileControl();

signals:

    void updateStatus(const Status &status);

    void updateTarget(const Target &target);

public slots:

    void run();
};


class Control : public QObject {
Q_OBJECT
    QThread workerThread;
public:
    Control();

    ~Control();
    Q_DISABLE_COPY(Control)

    void init();

    void createConnect(const Communication &communication);

    MobileControl *mobileControl;
signals:

    void start();
};


#endif //FAST_RTPS_TEST_MOBILECONTROL_H
