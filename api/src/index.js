#!/usr/bin/node
'use strict'
const sms = require('./sms');
const CORS = require('./cors');
const message_feed = require('./message_feed');
const express = require('express');
const bodyParser = require('body-parser');
const basicAuth = require('express-basic-auth');

const PORT = 80;

var app = express();

app.use(basicAuth({
    users: { 'grundfos': '5s2p7Kffy6jcwlHoGwmyr7IZCbqmAYtPXoWjQ0UmaGl' },
    challenge: true
}));
app.use(CORS);
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(__dirname + '/public'));

sms.onReceive((sender, message) => {
    console.log(`=> ${sender}: ${message}`);

    var msg = {
        receiver: 'web',
        sender: sender,
        message: message,
        timestamp: Date.now()
    }

    message_feed.push(msg);
});

app.get('/api/sms', CORS, function(req, res) {
    res.send(JSON.stringify(message_feed.get()));
});

app.get('/api/sms/updates', CORS, function(req, res) {
    var validator = (function() {
        if (req.query['since']) {
            return function(cur, index) {
                return cur.timestamp > req.query['since'];
            }
        }
    })();

    res.setHeader('Content-Type', 'text/event-stream');
    res.send(`data: ${JSON.stringify(message_feed.get(validator))}\n\n`);
});

app.post('/api/sms', CORS, function(req, res) {
    if (!req.body.receiver || !req.body.message) {
        res.sendStatus(400);
        return;
    }

    var msg = {
        receiver: req.body.receiver,
        sender: 'web',
        message: req.body.message,
        timestamp: Date.now()
    }
    message_feed.push(msg);

    sms.send(req.body.receiver, req.body.message)
    console.log(`<= ${req.body.receiver}: ${req.body.message}`);

    res.send();
});

app.listen(PORT, '0.0.0.0', function() {
    console.log(`Listening on port ${PORT}`);
});

process.on('SIGINT', () => process.exit());
