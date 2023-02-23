#include "MainWindow.h"

#include "VariableModel.h"
#include "MeasurementModel.h"
#include "FunctionModel.h"
#include "Variable.h"
#include "Config.h"
#include "db.h"
#include "GlobalSignalHandler.h"
#include "ResultsWidget.h"

#include <QTableView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QModelIndex>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QStatusBar>

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle(tr("Error Calculator"));

    ui();
    menu();
    statusbar();

    variableModel = new VariableModel(this);
    variableView->setModel(variableModel);

    measurementModel = new MeasurementModel(this);
    measurementView->setModel(measurementModel);

    connect(
        variableView->selectionModel(),
        &QItemSelectionModel::currentChanged,
        this,
        &MainWindow::onVariableSelectionChanged
    );

    functionModel = new FunctionModel(this);
    functionView->setModel(functionModel);

    connect(
        newFileAction,
        &QAction::triggered,
        this,
        &MainWindow::onNewFileTriggered
    );

    connect(
        openFileAction,
        &QAction::triggered,
        this,
        &MainWindow::onOpenFileTriggered
    );

    connect(
        closeFileAction,
        &QAction::triggered,
        this,
        &MainWindow::onCloseFileTriggered
    );

    connect(
        confSpinBox,
        &QSpinBox::valueChanged,
        this,
        &MainWindow::saveConf
    );

    connect(
        gsh,
        &GlobalSignalHandler::endResetDb,
        this,
        &MainWindow::loadConf
    );

    connect(
        processButton,
        &QPushButton::clicked,
        this,
        &MainWindow::onProcessClicked
    );

    master->setDisabled(true);
}

void MainWindow::ui() {
    // Master widget
    master = new QWidget;
    setCentralWidget(master);

    // Main layout
    auto* vbox = new QVBoxLayout;
    master->setLayout(vbox);

    // Top widgets
    auto* hbox = new QHBoxLayout;
    vbox->addLayout(hbox, 0);

    hbox->addWidget(new QLabel(tr("Confidence coefficient:")), 0);

    confSpinBox = new QSpinBox;
    confSpinBox->setMinimum(0);
    confSpinBox->setMaximum(99);
    confSpinBox->setSuffix(" %");
    hbox->addWidget(confSpinBox, 0);

    hbox->addStretch(1);

    processButton = new QPushButton(tr("Process"));
    hbox->addWidget(processButton);

    // Splitters
    auto* vsplitter = new QSplitter(Qt::Vertical);
    vbox->addWidget(vsplitter, 1);

    auto* hsplitter = new QSplitter(Qt::Horizontal);
    vsplitter->addWidget(hsplitter);

    hsplitter->addWidget(getVariableWidget());
    hsplitter->addWidget(getMeasurementWidget());

    vsplitter->addWidget(getFunctionWidget());
}

QWidget* MainWindow::getVariableWidget() {
    auto* widget = new QWidget;
    auto* vbox = new QVBoxLayout(widget);
    vbox->setContentsMargins(spacing, spacing, spacing, spacing);

    vbox->addWidget(new QLabel(tr("Variables")));

    vbox->addWidget(variableView = new QTableView);
    variableView->setSelectionMode(QAbstractItemView::NoSelection);

    return widget;
}

QWidget* MainWindow::getMeasurementWidget() {
    auto* widget = new QWidget;
    auto* vbox = new QVBoxLayout(widget);
    vbox->setContentsMargins(spacing, spacing, spacing, spacing);

    vbox->addWidget(new QLabel(tr("Measurements")));

    vbox->addWidget(measurementView = new QTableView);
    measurementView->setSelectionMode(QAbstractItemView::NoSelection);

    return widget;
}

QWidget* MainWindow::getFunctionWidget() {
    auto* widget = new QWidget;
    auto* vbox = new QVBoxLayout(widget);
    vbox->setContentsMargins(spacing, spacing, spacing, spacing);

    vbox->addWidget(new QLabel(tr("Functions")));

    vbox->addWidget(functionView = new QTableView);
    functionView->setSelectionMode(QAbstractItemView::NoSelection);

    return widget;
}

void MainWindow::menu() {
    auto* bar = menuBar();

    auto* fileMenu = bar->addMenu(tr("File"));

    fileMenu->addAction(
        newFileAction = new QAction(tr("New"), this)
    );

    fileMenu->addAction(
        openFileAction = new QAction(tr("Open"), this)
    );

    fileMenu->addAction(
        closeFileAction = new QAction(tr("Close"), this)
    );
}

void MainWindow::statusbar() {
    statusBar()->addWidget(
        fileNameStatusLabel = new QLabel(tr("No file opened"))
    );
}

void MainWindow::onVariableSelectionChanged(const QModelIndex& cur) {
    try {
        int varId = Variable::getNth(cur.row()).getId();
        measurementModel->setVarId(varId);
        measurementView->setEnabled(true);
    } catch (const std::overflow_error&) {
        measurementModel->setVarId(-1);
        measurementView->setEnabled(false);
    }
}

void MainWindow::onNewFileTriggered() {
    auto filename = QFileDialog::getSaveFileName(
        this,
        tr("New file"),
        "",
        tr("Experiment file (*.exp)")
    );

    if (filename.isEmpty()) return;

    db::close();
    db::openNew(filename);

    master->setEnabled(true);
    fileNameStatusLabel->setText(filename);
}

void MainWindow::onOpenFileTriggered() {
    auto filename = QFileDialog::getOpenFileName(
        this,
        tr("New file"),
        "",
        tr("Experiment file (*.exp)")
    );

    if (filename.isEmpty()) return;

    db::close();
    db::open(filename);

    master->setEnabled(true);
    fileNameStatusLabel->setText(filename);
}

void MainWindow::onCloseFileTriggered() {
    db::close();

    master->setDisabled(true);
    fileNameStatusLabel->setText(tr("No file opened"));
}

void MainWindow::saveConf() {
    Config::setConf(confSpinBox->value() / 100.0);
}

void MainWindow::loadConf() {
    if (db::isOpened()) {
        confSpinBox->setValue(Config::getConf() * 100);
    }
}

void MainWindow::onProcessClicked() {
    (new ResultsWidget)->show();
}
