cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(DCMST VERSION 1.0 LANGUAGES CXX)

# C++17 obrigatório
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilação
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Flags de debug (opcional)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

# Incluir diretório de headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Encontrar todos os arquivos fonte
file(GLOB SOURCES "src/*.cpp")

# Criar executável
add_executable(programa ${SOURCES})

# Informações de compilação
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")
