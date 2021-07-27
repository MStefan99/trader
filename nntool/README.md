# Neural Network tool

At the core of this trading bot is a neural network, which predicts stock prices based on the latest data and this tool
contains the actual algorithms for the network. Written in C++ for the best performance, it also provides a simple
command line interface that can be used by other applications.

## Usage

The tool is used from the command line. It has two main modes: training and feedforward. In feedforward mode the tool
expects an array of inputs and produces an array of outputs (format described below), given a trained network. In
training mode you need to supply a set of inputs and a set of corresponding outputs. After the network is trained, it
will be written into a file, which can then be used in feedforward mode.

### Feedforward mode

./tool [-h/--help] -n/--nn nnFile [-i/--in inFile] [-o/--out outFile] [-q/--quiet]

In this mode, the tool will take a column-vector from the input file (or stdin if not specified) and output a result
column-vector to the output file (stdout if not specified). The neural network will be loaded from the specified file.
If you want to disable any output on stdout, you can use the quiet mode.

### Training mode

./tool [-h/--help] -n/--nn nnFile [-i/--in inFile] [-o/--out outFile] [-t/--train] [-v/--verify]
[-p/--topology topology] [-e/--epochs epochs] [-a/--eta eta] [-f/--fast] [-q/--quiet]

Training mode is used to create neural networks for a specific task. In this mode, a neural network is first created,
initialized with random values, trained on the provided training data and then saved to file. For readability, the
format of the data is a bit different here: a tool expects a column-vector of individual inputs. In other words, you can
imagine a table where each row contains inputs for a single example. The amount inputs and outputs must always match
exactly. Additionally, you can specify a topology of the network in X,XX,...,X format, where X is an integer number. The
topology represents the amount of neurons in each layer. The size of the first layer must match the length of each input
example, and the size of the last one must match the length of each output. Learning rate is used to set how fast the
network will change its values. Higher values result in faster training but can result in a network overshooting the
desired result. Fast mode uses multithreading to speed up the training process significantly, however at times the
network may not converge properly, producing bad results. Quiet mode can be used to disable any output on stdout. The
network can be optionally verified on up to 100 examples. If verification is requested without training, the neural
network will be loaded from the file.

### Data representation

All data is represented in CSV format with commas as separators and line breaks as line separators. The only notable
feature is that the first column of the first row should contain the dimensions of the data in the format W:H, where W
is width and H is height represented as integer numbers.  
An example of the format can look like this:

```
2:10
0.0136548,0.0870173
0.029001,0.263561  
-0.0070195,-0.970369  
0.0916812,0.648149  
0.0772818,0.307959  
0.00518893,0.576696  
-0.0341755,-0.429407  
-0.0654803,0.177007  
-0.0392484,0.230445  
-0.0511522,-0.107494  
```
