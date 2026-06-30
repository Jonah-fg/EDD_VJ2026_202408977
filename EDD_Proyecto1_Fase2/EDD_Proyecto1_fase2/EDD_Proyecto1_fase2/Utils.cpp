#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string obtenerTimestampActual() {
    auto ahora=std::chrono::system_clock::now();
    auto tiempo_t=std::chrono::system_clock::to_time_t(ahora);
    std::tm tm =*std::localtime(&tiempo_t);
    std::ostringstream oss;
    oss<<std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

#endif