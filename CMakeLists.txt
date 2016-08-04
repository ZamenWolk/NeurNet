cmake_minimum_required(VERSION 3.5)
project(NeurNet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libstdc++ -static-libgcc -static")

include_directories(${PROJECT_SOURCE_DIR}/cereal)


set(SOURCE_FILES main.cpp src/network/NeuralNetwork.cpp src/network/NeuralNetwork.h src/network/NeuralNode.cpp src/network/NeuralNode.h src/util/util.h src/storage/Gene.cpp src/storage/Gene.h src/storage/Genome.cpp src/storage/Genome.h src/util/Rates.cpp src/util/Rates.h src/util/util.cpp 2048/TTFE.cpp 2048/TTFE.h 2048/Cell.cpp 2048/Cell.h exceptions/noCellLeft.h 2048/Direction.h exceptions/endOfGrid.h 2048/TTFENeuralTraining.cpp 2048/TTFENeuralTraining.h 2048/TTFENeuralUse.cpp 2048/TTFENeuralUse.h src/use/Pool.cpp src/use/Pool.h src/use/Specie.cpp src/use/Specie.h 2048/NeuralTrainingV2.cpp 2048/NeuralTrainingV2.h)
add_executable(NeurNet ${SOURCE_FILES})