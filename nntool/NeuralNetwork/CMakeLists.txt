add_library(NeuralNetwork STATIC)

target_sources(NeuralNetwork PRIVATE
		vector_io.cpp
		Matrix.cpp
		NeuralNetwork.cpp
		)

target_include_directories(NeuralNetwork PUBLIC .)
