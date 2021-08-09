'use strict';

const path = require('path');
const childProcess = require('child_process');


module.exports = function (options) {
	if (!process.env.NNTOOL ?? options.toolPath) {
		throw new Error('Neural Network executable not specified. Run with NNTOOL ' +
				'environment variable or specify toolPath parameter.')
	}
	let _exe = path.resolve(process.env.NNTOOL ?? options.toolPath);
	let _nnFile;
	let _inputs;


	const NeuralNetwork = function () {
		// Nothing to do
	};


	NeuralNetwork.prototype.load = function (file) {
		_nnFile = path.resolve(file);
		return this;
	};


	NeuralNetwork.prototype.feed = function (inputs) {
		if (!(inputs instanceof Array)) {
			throw new Error('Input should be an array');
		}
		_inputs = inputs;
		return this;
	};


	NeuralNetwork.prototype.run = function (err, cb) {
		let inString = '1:' + _inputs.length + '\n';
		for (const val of _inputs) {
			inString += val + '\n';
		}

		const nnProcess = childProcess.execFile(_exe, ['-q', '-n', _nnFile], (error, stdout) => {
			if (error && cb) {
				err?.(error);
			}

			const vectorHeight = +stdout.match(/(?<=1:)\d+$/m)
			const vector = [];
			const lines = stdout.split('\n');

			for (let i = 1; i <= vectorHeight; ++i) {
				vector.push(+lines[i]);
			}

			if (cb !== undefined) {
				cb?.(vector);
			} else {  // err callback is optional
				err?.(vector);
			}
		});

		nnProcess.stdin.end(inString);

		return this;
	};


	return new NeuralNetwork;
}
