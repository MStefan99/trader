'use strict';


const nn = require('./NeuralNetwork');


nn()
		.load('../trading-data/data/nn.txt')
		.feed([0.6373,-0.23842])
		.run(res => console.log(res))
		.feed([-0.7073,-0.5676])
		.run(res => console.log(res))
		.feed([-0.8789,0.5338])
		.run(res => console.log(res))
		.feed([0.9828,0.8366])
		.run(res => console.log(res));
