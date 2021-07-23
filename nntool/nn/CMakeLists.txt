add_library(NeuralNetwork STATIC)

target_sources(NeuralNetwork PRIVATE
		Matrix.cpp
		NeuralNetwork.cpp
		)

target_include_directories(NeuralNetwork PUBLIC .)
