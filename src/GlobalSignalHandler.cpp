#include "GlobalSignalHandler.h"

GlobalSignalHandler::GlobalSignalHandler() : QObject() {}

GlobalSignalHandler* GlobalSignalHandler::instance = new GlobalSignalHandler();
GlobalSignalHandler* gsh = GlobalSignalHandler::instance;
