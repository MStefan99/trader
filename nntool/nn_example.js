'use strict';


const nn = require('./NeuralNetwork');


nn()
		.load('./samples/nn.txt')
		.feed([2.42228, 6.98474])
		.run(res => console.log(res))
		.feed([-5.98768, -2.19725])
		.run(res => console.log(res))
		.feed([-9.01402, -4.51559])
		.run(res => console.log(res))
		.feed([6.58245, -7.01648])
		.run(res => console.log(res));
