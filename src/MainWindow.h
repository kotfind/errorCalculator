#pragma once

#include <QMainWindow>

class QTableView;
class VariableModel;
class MeasurementModel;
class FunctionModel;
class QModelIndex;
class QAction;
class QSpinBox;
class QPushButton;
class QLabel;

class MainWindow : public QMainWindow {
    public:
        MainWindow();

    private:
        void ui();
        QWidget* getVariableWidget(); 
        QWidget* getMeasurementWidget();
        QWidget* getFunctionWidget();

        void menu();
        void statusbar();

        const int spacing = 5;

        QWidget* master;

        QSpinBox* confSpinBox;

        VariableModel* variableModel;
        QTableView* variableView;

        MeasurementModel* measurementModel;
        QTableView* measurementView;

        FunctionModel* functionModel;
        QTableView* functionView;

        QAction* newFileAction;
        QAction* openFileAction;
        QAction* closeFileAction;

        QPushButton* processButton;

        QLabel* fileNameStatusLabel;

    private slots:
        void onVariableSelectionChanged(const QModelIndex& cur);

        void onNewFileTriggered();
        void onOpenFileTriggered();
        void onCloseFileTriggered();

        void saveConf();
        void loadConf();

        void onProcessClicked();
};
