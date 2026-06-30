#ifndef UTILS_H
#define UTILS_H

#define _CRT_SECURE_NO_WARNINGS 

#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

// Función para obtener timestamp actual
inline std::string obtenerTimestampActual() {
    auto ahora =std::chrono::system_clock::now();
    auto tiempo_t=std::chrono::system_clock::to_time_t(ahora);
    std::tm tm =*std::localtime(&tiempo_t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

#endif
