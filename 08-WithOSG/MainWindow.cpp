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

#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QStatusBar>
#include <QtGui/QIcon>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtCore/QFileInfoList>

#include "RTPSNode.h"
#include "OSGWidget.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        osgwidget_(nullptr),
        dock_widget_(nullptr),
        tree_widget_(nullptr),
        open_file_action(nullptr),
        connect_action(nullptr),
        start_action(nullptr),
        end_action(nullptr),
        convert_action(nullptr) {

    this->setWindowTitle("PointCloudViewer");
    initUI();

    osgwidget_ = new OSGWidget(this);
    this->setCentralWidget(osgwidget_);
    osgwidget_->init();
}

MainWindow::~MainWindow() = default;

void MainWindow::initUI() {
    createMenu();
    createToolBar();
    //createDockWidget();
}

void MainWindow::createMenu() {
    open_file_action = new QAction("Open", this);
    open_file_action->setIcon(QIcon(":/images/file_open.png"));
    connect(open_file_action, &QAction::triggered, this, &MainWindow::openFile);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar("Tools");
    toolBar->addAction(open_file_action);

    toolBar->addSeparator();
    //toolBar->addAction(draw_line_action);
}

void MainWindow::createDockWidget() {
    tree_widget_ = new QTreeWidget(this);
    tree_widget_->setColumnCount(1);
    tree_widget_->setHeaderHidden(true);
    //tree_widget_->setColumnWidth(0, 100);
    //tree_widget_->setStyleSheet("QTreeWidget::item {height:25px;");

    //GPS位置
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree_widget_, QStringList(QStringLiteral("GPS位置")));
        item->setExpanded(true);
    }

    //卫星数
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree_widget_, QStringList(QStringLiteral("卫星数")));
        item->setExpanded(true);
    }

    //RTK状态
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree_widget_, QStringList(QStringLiteral("RTK状态")));
        item->setCheckState(0, Qt::CheckState::Unchecked);
    }


    dock_widget_ = new QDockWidget(QStringLiteral("无人机状态"), this);
    dock_widget_->setFixedWidth(200);
    dock_widget_->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dock_widget_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock_widget_->setWidget(tree_widget_);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock_widget_);

    //QTreeWidget connect
    //connect(edit_widget_, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(TreeWidgetClicked(QTreeWidgetItem *, int)));
    //connect(edit_widget_, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(TreeWidgetDoubleClicked(QTreeWidgetItem *, int)));
    //connect(edit_widget_, SIGNAL(itemPressed(QTreeWidgetItem *, int)), this, SLOT(TreeWidgetRightedClicked(QTreeWidgetItem *, int)));
}

void MainWindow::createConnect(const RTPSNodeThread &rtpsNodeThread) {
    connect(&rtpsNodeThread, &RTPSNodeThread::createOrRemoveNodeWithName, osgwidget_, &OSGWidget::createOrRemoveNode);
    connect(&rtpsNodeThread, &RTPSNodeThread::updatePositionOfNodeWithName, osgwidget_,
            &OSGWidget::updatePositionOfNode);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Model"), "/home/zhihui/workspace/data/",
                                                    tr("Image Files (*.osg *.osgt *.osgb)"));
    if (fileName.isEmpty()) return;

    QFileInfo f(fileName);
    osgwidget_->readDataFromFile(f);
}


void MainWindow::updateSatelliteNum(QString num) {
    auto satellite_item = tree_widget_->topLevelItem(SATELLITE_NUM);

    if (!satellite_item->childCount()) {
        auto item = new QTreeWidgetItem(satellite_item);
    }

    auto item = satellite_item->child(0);
    item->setText(0, num);
}

void MainWindow::updateRTKStatus(bool is_valid) {
    auto rtk_item = tree_widget_->topLevelItem(RTK_STATUS);

    auto check_state = is_valid ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    rtk_item->setCheckState(0, check_state);
}