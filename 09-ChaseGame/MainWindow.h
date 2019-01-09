/*
 * Copyright (c) 2018 Ally of Intelligence Technology Co., Ltd. All rights reserved.
 *
 * Created by WuKun on 1/4/19.
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

#ifndef FAST_RTPS_TEST_MAINWINDOW_H
#define FAST_RTPS_TEST_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QLabel>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

class OSGWidget;

class RTPSNodeThread;

class Communication;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() final;

    void createConnect(const RTPSNodeThread &rtpsNodeThread);

    void createConnect(const Communication &communication);

    Q_DISABLE_COPY(MainWindow);
private:
    void initUI();

    void createMenu();

    void createToolBar();

    void createDockWidget();

    //core widget
    OSGWidget *osgwidget_;

    //other widgets
    QDockWidget *dock_widget_;
    QTreeWidget *tree_widget_;
    enum TREE_WIDGET_ITEM {
        GPS_LOCATION,
        SATELLITE_NUM,
        RTK_STATUS
    };

    //items
    QAction *open_file_action;
    QAction *connect_action;
    QAction *start_action;
    QAction *end_action;
    QAction *convert_action;

public slots:

    void openFile();
};


#endif //FAST_RTPS_TEST_MAINWINDOW_H
