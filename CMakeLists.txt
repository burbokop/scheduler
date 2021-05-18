cmake_minimum_required(VERSION 3.14)

project(scheduler LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

include(klibcore/klibcore.cmake)

set(PROJECT_SOURCES
    src/schedulerlogview.cpp
    src/schedulerlogview.h
    src/main.cpp
    src/executor.cpp
    src/executor.h
    src/tools/correlation.cpp
    src/tools/correlation.h
    src/tools/fourier.cpp
    src/tools/fourier.h
    src/tools/optdebug.cpp
    src/tools/optdebug.h
    src/tasks/fouriertask.cpp
    src/tasks/fouriertask.h
    src/tools/range.cpp
    src/tools/range.h
    src/scheduler/erliestdeadlinefirstscheduler.cpp
    src/scheduler/erliestdeadlinefirstscheduler.cpp
    src/tasks/autocorrelationtask.cpp
    src/tasks/autocorrelationtask.h
    src/tools/signalgenerator.cpp
    src/tools/signalgenerator.h
    src/scheduler/scheduler.cpp
    src/scheduler/scheduler.h
    src/scheduler/roundrobinscheduler.cpp
    src/scheduler/roundrobinscheduler.h
    src/scheduler/time.cpp
    src/scheduler/time.h
    src/scheduler/roundindex.cpp
    src/scheduler/roundindex.h
    src/scheduler/task.cpp
    src/scheduler/task.h
    resources.qrc
)

        add_executable(scheduler
          ${PROJECT_SOURCES}
        )
target_link_libraries(scheduler
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Quick
  klibcore
  )
