#include <iostream>
#include <fstream>
#include <vector>

#include "vector_io.h"
#include "NeuralNetwork.h"


typedef std::vector<std::string> strings;

static const strings inOptions {"-i", "--input"};
static const strings outOptions {"-o", "--output"};
static const strings nnOptions {"-n", "--nn"};
static const strings trainOptions {"-t", "--train"};
static const strings epochOptions {"-e", "--epochs"};
static const strings etaOptions {"-a", "--eta"};
static const strings helpOptions {"-h", "--help"};
static const strings topologyOptions {"-p", "--topology"};
static const strings quietOptions {"-q", "--quiet"};
static const strings fastOptions {"-f", "--fast"};
// TODO: add verification mode


static std::pair<bool, strings> getParameter(int argc, char* argv[],
		const strings& names, size_t len = 1) {
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
		          << "[-t/--train] [-p/--topology topology] [-e/--epochs epochs] "
		          << "[-a/--eta eta] [-f/--fast] [-q/--quiet]" << std::endl
		          << "Options:" << std::endl
		          << "\t-h, --help    Displays this page" << std::endl
		          << "\t-n, --nn nnFile    Path to the neural network file" << std::endl
		          << "\t-i, --inFile inFile    Path to the input file"
		          << "\t-o, --outFile outFile    Path to the output file"
		          << "\t-t, --train    Runs the tool in the training mode. Requires output file" << std::endl
		          << "\t-p, --topology topology    Topology to use for the neural network" << std::endl
		          << "\t-e, --epochs    Number of epochs to use during training" << std::endl
		          << "\t-a, --eta    Eta (learning rate) of the network" << std::endl
		          << "\t-q, --quiet    Do not print anything to stdout except the end result" << std::endl
		          << "\t-f, --fast    Enables multithreading. May severely impact accuracy in some cases" << std::endl;
		std::exit(0);
	}

	auto train {getParameter(argc, argv, trainOptions, 0)};
	auto inFilename {getParameter(argc, argv, inOptions)};
	auto outFilename {getParameter(argc, argv, outOptions)};
	auto nnFilename {getParameter(argc, argv, nnOptions)};
	auto quiet {getParameter(argc, argv, quietOptions, 0)};
	auto fast {getParameter(argc, argv, fastOptions, 0)};

	if (!train.first) {  // Feedforward mode
		Column in {};

		if (!nnFilename.first) {  // Checking if NN file specified
			std::cout << "No NN specified, please rerun with 'nn' option";
			std::exit(5);
		}

		NeuralNetwork nn;
		readFile(nnFilename.second.front(), nn);

		if (!inFilename.first) {  // Reading inputs from stdin if not passed as an argument
			if (!quiet.first) {
				std::cout << "No input file specified. Rerun the tool with 'in' option or use stdin: " << std::endl;
			}
			std::cin >> in;
		} else {
			readFile(inFilename.second.front(), in);
		}

		Column out {nn.feedforward(in)};

		if (!outFilename.first) {  // Printing result to stdout
			std::cout << out;
		} else {
			writeFile(outFilename.second.front(), out);
		}
	} else {  // Training mode
		if (!nnFilename.first) {
			std::cerr << "No neural network file specified, neural network won't be saved!" << std::endl;
			std::exit(5);
		}

		Rows in {};
		Rows out {};
		std::vector<size_t> topology {};

		if (inFilename.first) {  // Reading inputs from file
			readFile(inFilename.second.front(), in);
		} else {  // Reading inputs from stdin if not passed as an argument
			if (!quiet.first) {
				std::cerr << "No input file specified. Rerun the tool with 'in' option or use stdin: " << std::endl;
			}
			std::cin >> in;
		}

		if (outFilename.first) {  // Reading outputs from file
			readFile(outFilename.second.front(), out);
		} else {  // Reading outputs from stdin if not passed as an argument
			if (!quiet.first) {
				std::cout << "No output file specified. Rerun the tool with 'out' option or use stdin: " << std::endl;
			}
			std::cin >> out;
		}

		auto topologyInput {getParameter(argc, argv, topologyOptions)};
		auto epochInput {getParameter(argc, argv, epochOptions)};
		auto etaInput {getParameter(argc, argv, etaOptions)};

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
		if (fast.first) {
			nn.fastTrain(in, out, etaInput.first? std::stof(etaInput.second.front()) : 0.001f,
					epochInput.first? std::stoull(epochInput.second.front()) : 10);
		} else {
			nn.train(in, out, etaInput.first? std::stof(etaInput.second.front()) : 0.001f,
					epochInput.first? std::stoull(epochInput.second.front()) : 10);
		}
		writeFile(nnFilename.second.front(), nn);
	}
	return 0;
}
