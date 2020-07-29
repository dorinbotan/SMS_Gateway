'use strict'
const iot_platform = require('./iot_platform');

iot_platform.connect(function() {
    console.log('Connected to Alibaba IoT Platform');
});

/* TODO: (DOBO) replace Receiver with receiver */
/* TODO: (DOBO) replace Message with message */
module.exports = {
    send: function(receiver, message) {
        var msg = {
            Receiver: receiver,
            Message: message
        };

        iot_platform.publish(msg);
    },
    onReceive: function(callback) {
        iot_platform.onMessageReceived(function(message) {
            if (!message) return;
            message = JSON.parse(message);
            
            if (!message['Sender']) return;
            /* Replace phone number's leading '00' with '+' */
            message['Sender'] = message['Sender'].replace(/^00/, '+');

            callback(message['Sender'], message['Message']);
        });
    }
}
