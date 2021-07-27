'use strict';

const express = require('express');
const app = express();


const DEFAULT_PORT = 3000;


app.get('/', (req, res) => {
	res.send('Hello!');
});


app.listen(process.env.PORT ?? DEFAULT_PORT, () => {
	console.log('Listening on port', process.env.PORT ?? DEFAULT_PORT);
});
