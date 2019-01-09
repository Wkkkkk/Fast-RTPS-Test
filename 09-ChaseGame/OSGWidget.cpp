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

#include <QtWidgets/QGridLayout>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include <osg/Light>
#include <osg/Point>
#include <osg/Material>
#include <osg/Geometry>
#include <osg/LineWidth>
#include <osg/ValueObject>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/ComputeBoundsVisitor>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>

#include "Common.h"
#include "OSGWidget.h"
#include "PickHandler.h"
#include "NodeCallback.h"
#include "NodeTreeInfo.h"
#include "NodeTreeSearch.h"
#include "message/data.h"

OSGWidget::OSGWidget(QWidget *parent) :
        QWidget(parent),
        main_view_(nullptr),
        root_node_(nullptr),
        text_node_(nullptr),
        update_timer_(new QTimer) {}

OSGWidget::~OSGWidget() = default;

void OSGWidget::init() {
    initSceneGraph();
    initHelperNode();
    initCamera();

    QObject::connect(update_timer_.data(), SIGNAL(timeout()), this, SLOT(update()));
    update_timer_->start(15);
}

void OSGWidget::paintEvent(QPaintEvent *) {
    frame();
}


void OSGWidget::initSceneGraph() {
    root_node_ = new osg::Switch;
    root_node_->setName(root_node_name);

    osg::ref_ptr<osg::Switch> test_node = new osg::Switch;
    test_node->setName(test_node_name);
    root_node_->addChild(test_node);

    osg::ref_ptr<osg::Switch> self_node = new osg::Switch;
    self_node->setName(self_node_name);
    root_node_->addChild(self_node);

    osg::ref_ptr<osg::Camera> hud_node = createHUD();
    hud_node->setName(hud_node_name);
    {
        text_node_ = new osg::Switch;
        //text_node_->setName(text_node_name);
        hud_node->addChild(text_node_);
    }
    root_node_->addChild(hud_node);

    osg::ref_ptr<osg::Switch> helper_node = new osg::Switch;
    helper_node->setName(helper_node_name);
    root_node_->addChild(helper_node);
}

void OSGWidget::initCamera() {
    osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::ViewerBase::SingleThreaded;
    this->setThreadingModel(threadingModel);
    this->setKeyEventSetsDone(0);

    auto graphic_window = createGraphicsWindow(0, 0, 2000, 2000);
    auto traits = graphic_window->getTraits();

    main_view_ = new osgViewer::View;
    this->addView(main_view_.get());

    auto camera = main_view_->getCamera();
    camera->setGraphicsContext(graphic_window);
    camera->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
    camera->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective(30.f,
                                             static_cast<double>(traits->width) / static_cast<double>(traits->height),
                                             1.0, 1000.0);
    camera->setNearFarRatio(0.0000002);
    camera->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES);
//    camera->setClearColor(osg::Vec4(0.84313, 0.84313, 0.89804, 1.0));

    //for outline effects
    {
        osg::DisplaySettings::instance()->setMinimumNumStencilBits(1);
        unsigned int clearMask = camera->getClearMask();
        camera->setClearMask(clearMask | GL_STENCIL_BUFFER_BIT);
        camera->setClearStencil(0);
    }

    main_view_->addEventHandler(new osgViewer::StatsHandler);
    main_view_->addEventHandler(new NodeTreeHandler(root_node_));
    main_view_->addEventHandler(new PickHandler(root_node_));
    main_view_->setSceneData(root_node_.get());
    main_view_->setCameraManipulator(new osgGA::TrackballManipulator);

    QWidget *widget = graphic_window->getGLWidget();
    auto grid = new QGridLayout;
    grid->addWidget(widget);
    this->setLayout(grid);
}

osgQt::GraphicsWindowQt *
OSGWidget::createGraphicsWindow(int x, int y, int w, int h, const std::string &name, bool windowDecoration) const {
    osg::DisplaySettings *ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
}

osg::Node *OSGWidget::readModelFromFile(const QFileInfo &file_info) const {
    osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(file_info.filePath().toStdString());
    return node.release();
}

void OSGWidget::readDataFromFile(const QFileInfo &file_info) {

    static osg::ref_ptr<osg::Switch> test_node = dynamic_cast<osg::Switch *>(
            NodeTreeSearch::findNodeWithName(root_node_, test_node_name));

    osg::ref_ptr<osg::Node> node = readModelFromFile(file_info);
    osg::ref_ptr<osg::Geode> bbox = calculateBBoxForModel(node);

    osg::ref_ptr<osg::MatrixTransform> matrix = new osg::MatrixTransform;
    matrix->addChild(node);
    matrix->addChild(bbox);
    matrix->setUpdateCallback(new NodeCallback());

    test_node->removeChildren(0, test_node->getNumChildren());
    test_node->addChild(matrix);
}

void OSGWidget::initHelperNode() {
    static osg::ref_ptr<osg::Switch> helper_node = dynamic_cast<osg::Switch *>(
            NodeTreeSearch::findNodeWithName(root_node_, helper_node_name));

    // origin
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        osg::ref_ptr<osg::ShapeDrawable> point_sphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(), 0.5f));
        point_sphere->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
        geode->addDrawable(point_sphere);

        helper_node->addChild(geode);
    }

    // axis
    {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        osg::ref_ptr<osg::Vec3Array> colors = new osg::Vec3Array;

        float radius = 10.0;
        osg::Vec3d axis_center;
        vertices->push_back(axis_center);
        vertices->push_back(axis_center + osg::Vec3(radius, 0, 0));
        vertices->push_back(axis_center + osg::Vec3(0, radius, 0));
        vertices->push_back(axis_center + osg::Vec3(0, 0, radius));

        colors->push_back(osg::Vec3(1, 0, 0));
        colors->push_back(osg::Vec3(0, 1, 0));
        colors->push_back(osg::Vec3(0, 0, 1));

        geom->setVertexArray(vertices);
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE_SET);

        osg::ref_ptr<osg::DrawElementsUInt> line1 = new osg::DrawElementsUInt(osg::DrawElementsUInt::LINES, 2);
        (*line1)[0] = 0;
        (*line1)[1] = 1;
        osg::ref_ptr<osg::DrawElementsUInt> line2 = new osg::DrawElementsUInt(osg::DrawElementsUInt::LINES, 2);
        (*line2)[0] = 0;
        (*line2)[1] = 2;
        osg::ref_ptr<osg::DrawElementsUInt> line3 = new osg::DrawElementsUInt(osg::DrawElementsUInt::LINES, 2);
        (*line3)[0] = 0;
        (*line3)[1] = 3;

        geom->addPrimitiveSet(line1);
        geom->addPrimitiveSet(line2);
        geom->addPrimitiveSet(line3);

        osg::ref_ptr<osg::StateSet> state_set = geode->getOrCreateStateSet();
        osg::ref_ptr<osg::LineWidth> line_width = new osg::LineWidth(3.0);
        state_set->setAttributeAndModes(line_width);
        geode->addDrawable(geom);

        helper_node->addChild(geode);
    }
}

osg::Geode *OSGWidget::calculateBBoxForModel(osg::Node *node) const {
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ComputeBoundsVisitor bounds_visitor;
    node->accept(bounds_visitor);
    osg::BoundingBox bb = bounds_visitor.getBoundingBox();
    float lengthX = bb.xMax() - bb.xMin();
    float lengthY = bb.yMax() - bb.yMin();
    float lengthZ = bb.zMax() - bb.zMin();
    osg::Vec3 center = bb.center();

    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(new osg::Box(center, lengthX, lengthY, lengthZ));
    drawable->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));

    osg::ref_ptr<osg::StateSet> state_set = drawable->getOrCreateStateSet();
    osg::ref_ptr<osg::PolygonMode> polygon = new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,
                                                                  osg::PolygonMode::LINE);
    state_set->setAttributeAndModes(polygon);
    osg::ref_ptr<osg::LineWidth> line_width = new osg::LineWidth(3.0);
    state_set->setAttribute(line_width);
    geode->addDrawable(drawable);

    return geode.release();
}

osg::Camera *OSGWidget::createHUD() {
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;

    camera->setProjectionMatrix(osg::Matrix::ortho2D(0, 1280, 0, 1024));
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setAllowEventFocus(false);

    return camera.release();
}

osg::Vec3d OSGWidget::calculateColorForPoint(const osg::Vec3d &point) const {
    static std::vector<osg::Vec3> Colors =
            {osg::Vec3(0.0, 0.0, 1.0), osg::Vec3(0.0, 0.2, 1.0), osg::Vec3(0.0, 0.4, 1.0), osg::Vec3(0.0, 0.6, 1.0),
             osg::Vec3(0.0, 0.8, 1.0), osg::Vec3(0.0, 1.0, 1.0),
             osg::Vec3(0.0, 1.0, 0.8), osg::Vec3(0.0, 1.0, 0.6), osg::Vec3(0.0, 1.0, 0.4), osg::Vec3(0.0, 1.0, 0.2),
             osg::Vec3(0.0, 1.0, 0.0), osg::Vec3(0.2, 1.0, 0.0),
             osg::Vec3(0.4, 1.0, 0.0), osg::Vec3(0.6, 1.0, 0.0), osg::Vec3(0.8, 1.0, 0.0), osg::Vec3(1.0, 1.0, 0.0),
             osg::Vec3(1.0, 0.8, 0.0), osg::Vec3(1.0, 0.6, 0.0),
             osg::Vec3(1.0, 0.4, 0.0), osg::Vec3(1.0, 0.2, 0.0), osg::Vec3(1.0, 0.0, 0.0)
            };
    const int distance_range = 30;

    double distance = point.length();
    int range = static_cast<int>(distance) / distance_range;
    if (range >= Colors.size()) range = Colors.size() - 1;
    if (range == 0) range = 1;

    return Colors[Colors.size() - range];
}

void OSGWidget::createOrRemoveNode(const QString &guid, bool create) {
    static osg::ref_ptr<osg::Switch> test_node = dynamic_cast<osg::Switch *>(
            NodeTreeSearch::findNodeWithName(root_node_, test_node_name));

    if (create) {
        osg::ref_ptr<osg::PositionAttitudeTransform> pos = new osg::PositionAttitudeTransform;
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        osg::ref_ptr<osg::ShapeDrawable> point_sphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(), 0.5f));
        point_sphere->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        geode->addDrawable(point_sphere);

        pos->addChild(geode);
        pos->setName(guid.toStdString());

        test_node->addChild(pos);
    } else {
        osg::ref_ptr<osg::PositionAttitudeTransform> node = dynamic_cast<osg::PositionAttitudeTransform *>(
                NodeTreeSearch::findNodeWithName(test_node, guid.toStdString().c_str()));

        if (node.valid()) {
            test_node->removeChild(node.release());
        }
    }
}

void OSGWidget::updatePositionOfNode(const QString &guid, const Vec3 &pos) {
    static osg::ref_ptr<osg::Switch> test_node = dynamic_cast<osg::Switch *>(
            NodeTreeSearch::findNodeWithName(root_node_, test_node_name));

    osg::ref_ptr<osg::PositionAttitudeTransform> node = dynamic_cast<osg::PositionAttitudeTransform *>(
            NodeTreeSearch::findNodeWithName(test_node, guid.toStdString().c_str()));

    if (!node.valid()) {
        createOrRemoveNode(guid, true);
    }
    node->setPosition(osg::Vec3d(pos.x(), pos.y(), pos.z()));
}

void OSGWidget::updateStatusOfNode(const QString &guid, const Status &status) {
//    std::cout << "updateStatusOfNode: " << guid.toStdString() << " status: " << status << std::endl;
    static osg::ref_ptr<osg::Switch> test_node = dynamic_cast<osg::Switch *>(
            NodeTreeSearch::findNodeWithName(root_node_, test_node_name));

    osg::ref_ptr<osg::PositionAttitudeTransform> node = dynamic_cast<osg::PositionAttitudeTransform *>(
            NodeTreeSearch::findNodeWithName(test_node, guid.toStdString().c_str()));

    if (!node.valid()) {
        osg::ref_ptr<osg::PositionAttitudeTransform> pos = new osg::PositionAttitudeTransform;
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        osg::ref_ptr<osg::ShapeDrawable> point_sphere = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3d(), 0.5f));
        point_sphere->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        geode->addDrawable(point_sphere);

        pos->addChild(geode);
        pos->setName(guid.toStdString());

        test_node->addChild(pos);
        node.swap(pos);
    }

    Vec3 pos = status.position();
    node->setPosition(osg::Vec3d(pos.x(), pos.y(), pos.z()));
}

void OSGWidget::updateTargetOfNode(const QString &guid, const Target &target) {
    std::cout << "updateTargetOfNode: " << guid.toStdString() << " target: " << target << std::endl;
}
