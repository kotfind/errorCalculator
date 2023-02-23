cmake_minimum_required(VERSION 3.16)

project(errorCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined,address -g3 -O0")

find_package(Qt6 REQUIRED
    Core
    Widgets
    Sql
)

add_subdirectory(mathParser)

qt_standard_project_setup()

add_executable(errorCalculator
    main.cpp
    GlobalSignalHandler.cpp
    db.cpp

    Variable.cpp
    VariableModel.cpp

    Measurement.cpp
    MeasurementModel.cpp

    Function.cpp
    FunctionModel.cpp

    MainWindow.cpp

    Config.cpp

    ResultsWidget.cpp

    MeasurementProcessor.cpp
    MeasurementResultsModel.cpp

    FunctionProcessor.cpp
    FunctionResultsModel.cpp
)

target_link_libraries(errorCalculator
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    MathParser
)
