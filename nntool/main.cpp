#include <iostream>
#include <fstream>
#include <vector>

#include "vector_io.h"
#include "NeuralNetwork.h"


#define MAX_VERIFY_SIZE 100  // TODO: replace with cli parameter

typedef std::vector<std::string> strings;

static const strings inOptions {"-i", "--input"};
static const strings outOptions {"-o", "--output"};
static const strings nnOptions {"-n", "--nn"};
static const strings trainOptions {"-t", "--train"};
static const strings verifyOptions {"-v", "--verify"};
static const strings epochOptions {"-e", "--epochs"};
static const strings etaOptions {"-a", "--eta"};
static const strings helpOptions {"-h", "--help"};
static const strings topologyOptions {"-p", "--topology"};
static const strings quietOptions {"-q", "--quiet"};
static const strings fastOptions {"-f", "--fast"};
static const strings progressBar {"-b", "--progress-bar"};


static std::pair<bool, strings> getParameter(int argc, char* argv[],
		const strings& names, size_t len = 0) {
	strings results {};
	bool found {};

	for (size_t i {0}; i < argc; ++i) {
		for (const auto& name: names) {
			if (name == argv[i]) {
				for (size_t j {i}; j < argc && j < i + len; ++j) {
					results.emplace_back(argv[i + 1]);
				}
				found = true;
				break;
			}
		}
	}

	return {found, results};
}


template <class T>
static void readFile(const std::string& filename, T& data) {
	std::ifstream file {filename, std::ios::in};

	if (!file) {
		std::cerr << "Could not read file " << filename << std::endl;
		std::exit(1);
	}

	file >> data;
}


template <class T>
static void writeFile(const std::string& filename, T& data) {
	std::ofstream file {filename, std::ios::out};

	if (!file) {
		std::cerr << "Could not write file " << filename << std::endl;
		std::exit(2);
	}

	file << data;
}


static std::vector<size_t> readTopology(const std::string& str) {
	std::vector<size_t> topology {};
	std::string tmp {str};

	while (tmp.find(',') != std::string::npos) {
		topology.push_back(std::stoul(tmp.substr(0, tmp.find(','))));
		tmp.erase(0, tmp.find(',') + 1);
	}
	topology.push_back(std::stoul(tmp));

	return topology;
}


int main(int argc, char* argv[]) {
	auto help {getParameter(argc, argv, helpOptions, 0)};

	if (help.first) {
		std::cout << "Trader NN Tool" << std::endl
				<< "This tool is a core component of a trader application, making stock price predictions."
				<< std::endl << std::endl
				<< "Usage:" << std::endl
				<< "\t ./tool [-h/--help] -n/--nn nnFile [-i/--in inFile] [-o/--out outFile] "
				<< "[-t/--train] [-v/--verify] [-p/--topology topology] [-e/--epochs epochs] "
				<< "[-a/--eta eta] [-f/--fast] [-q/--quiet]" << std::endl
				<< "Options:" << std::endl
				<< "\t-h, --help    Displays this page" << std::endl
				<< "\t-n, --nn nnFile    Path to the neural network file" << std::endl
				<< "\t-i, --inFile inFile    Path to the input file"
				<< "\t-o, --outFile outFile    Path to the output file"
				<< "\t-t, --train    Runs the tool in the training mode. Requires output file" << std::endl
				<< "\t-v, --verify    Verifies the training results on up to 100 examples" << std::endl
				<< "\t-p, --topology topology    Topology to use for the neural network" << std::endl
				<< "\t-e, --epochs    Number of epochs to use during training" << std::endl
				<< "\t-a, --eta    Eta (learning rate) of the network" << std::endl
				<< "\t-q, --quiet    Do not print anything to stdout except the end result" << std::endl
				<< "\t-f, --fast    Enables multithreading. May severely impact accuracy in some cases" 
				<< "\t-b, --progress-bar Enable progress bar printout." << std::endl;
		std::exit(0);
	}

	auto train {getParameter(argc, argv, trainOptions, 0)};
	auto verify {getParameter(argc, argv, verifyOptions, 0)};
	auto inFilename {getParameter(argc, argv, inOptions, 1)};
	auto outFilename {getParameter(argc, argv, outOptions, 1)};
	auto nnFilename {getParameter(argc, argv, nnOptions, 1)};
	auto quiet {getParameter(argc, argv, quietOptions, 0)};
	auto fast {getParameter(argc, argv, fastOptions, 0)};
	auto progress {getParameter(argc, argv, progressBar, 0)};

	if (!nnFilename.first) {
		std::cerr << "No neural network file specified" << std::endl;
		std::exit(3);
	}

	if (train.first || verify.first) {  // Training mode
		Rows in {};
		Rows out {};
		std::vector<size_t> topology {};

		try {
			if (inFilename.first) {  // Reading inputs from file
				readFile(inFilename.second.front(), in);
			} else {  // Reading inputs from stdin if not passed as an argument
				if (!quiet.first) {
					std::cerr << "No input file specified. Rerun the tool with '-i' option or use stdin: " << std::endl;
				}
				std::cin >> in;
			}
		} catch (const std::exception& e) {
			std::cerr << "An error occurred while reading inputs from file: " << e.what() << std::endl;
			std::exit(-1);
		}

		try {
			if (outFilename.first) {  // Reading outputs from file
				readFile(outFilename.second.front(), out);
			} else {  // Reading outputs from stdin if not passed as an argument
				if (!quiet.first) {
					std::cout << "No output file specified. Rerun the tool with '-o' option or use stdin: " << std::endl;
				}
				std::cin >> out;
			}
		} catch (const std::exception& e) {
			std::cerr << "An error occurred while reading outputs from file: " << e.what() << std::endl;
			std::exit(-1);
		}

		if (train.first) {
			auto topologyInput {getParameter(argc, argv, topologyOptions, 1)};
			auto epochInput {getParameter(argc, argv, epochOptions, 1)};
			auto etaInput {getParameter(argc, argv, etaOptions, 1)};

			if (topologyInput.first) {  // Reading topology
				topology = readTopology(topologyInput.second.front());
			} else {  // Using default topology if not specified
				if (!quiet.first) {
					std::cout << "No topology specified. Using default: "
							<< in.front().size() << ",10," << out.front().size() << std::endl;
				}
				topology.push_back(in.front().size());
				topology.push_back(10);
				topology.push_back(out.front().size());
			}

			NeuralNetwork nn {topology};
			try {
				if (fast.first) {
					nn.fastTrain(in, out, etaInput.first? std::stof(etaInput.second.front()) : 0.001f,
							epochInput.first? std::stoull(epochInput.second.front()) : 10);
				} else {
					nn.train(in, out, etaInput.first? std::stof(etaInput.second.front()) : 0.001f,
							epochInput.first? std::stoull(epochInput.second.front()) : 10,
							progress.first);
				}
			} catch (const std::exception& e) {
				std::cerr << "An error occurred while training: " << e.what() << std::endl;
				std::exit(-1);
			}
			writeFile(nnFilename.second.front(), nn);
		}

		if (verify.first) {
			NeuralNetwork nn {};

			try {
				readFile(nnFilename.second.front(), nn);
			} catch (const std::exception& e) {
				std::cerr << "An error occurred while loading neural network: " << e.what() << std::endl;
				std::exit(-1);
			}

			float error {};
			size_t verifySize {in.size() < MAX_VERIFY_SIZE? in.size() : MAX_VERIFY_SIZE};

			for (size_t i {0}; i < verifySize; ++i) {
				error += NeuralNetwork::error(nn.feedforward(in[i]), out[i]);
			}

			std::cout << "Average error: " << error / static_cast<float>(verifySize) << std::endl;
		}
	} else {  // Feedforward mode
		Column in {};
		NeuralNetwork nn;

		try {
			readFile(nnFilename.second.front(), nn);
		} catch (const std::exception& e) {
			std::cerr << "An error occurred while loading neural network: " << e.what() << std::endl;
			std::exit(-1);
		}

		try {
			if (!inFilename.first) {  // Reading inputs from stdin if not passed as an argument
				if (!quiet.first) {
					std::cout << "No input file specified. Rerun the tool with '-i' option or use stdin: " << std::endl;
				}
				std::cin >> in;
			} else {
				readFile(inFilename.second.front(), in);
			}
		} catch (const std::exception& e) {
			std::cerr << "An error occurred while reading inputs from file: " << e.what() << std::endl;
			std::exit(-1);
		}

		Column out {nn.feedforward(in)};

		if (!outFilename.first) {  // Printing result to stdout
			std::cout << out;
		} else {
			writeFile(outFilename.second.front(), out);
		}
	}
	return 0;
}
