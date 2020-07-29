'use strict'
const mqtt = require('aliyun-iot-mqtt');

const options = {
    productKey:   'a1nexC4w43L',
    deviceName:   'Frontend_API',
    deviceSecret: '5393089f2868bceec69d2702be84ebb7',
    regionId:     'cn-shanghai'
};

const TOPIC_SMS_IN  = `/${options.productKey}/${options.deviceName}/sms/in`;
const TOPIC_SMS_OUT = `/${options.productKey}/${options.deviceName}/sms/out`;

var client;

module.exports = {
    connect: function(callback) {
        client = mqtt.getAliyunIotMqttClient(options);

        if (callback) {
            client.on('connect', callback);
        }
    },
    /* returns message of format:
        {
            sender: '+0000000000',
            message: '...'
        }
    */
    onMessageReceived: function(callback) {
        if (callback && client) {
            client.on('message', function(topic, message) {
                /* Filter out messages comming to topics other than TOPIC_SMS_IN */
                var in_topic = TOPIC_SMS_IN.split('/');
                var topic = topic.split('/');
                for (var i = 0; i < topic.length; i++) {
                    if (in_topic[i] != topic[i]) {
                        return;
                    }
                }

                callback(message);
            });
        }
    },
    /* accepts message of format:
        {
            receiver: '+0000000000',
            message: '...'
        }
    */
    publish: function(message, options = {}, callback = () => {}) {
        if (!client) return;

        if (typeof(message) == 'object') {
            message = JSON.stringify(message);
        }

        options.qos = options.qos || 1;
        options.retain = options.retain || false;

        client.publish(TOPIC_SMS_OUT, message, options, callback);
    },
    disconnect: function() {
        client.end();
    }
}
