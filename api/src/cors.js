'use strict'
const cors = require('cors');

const whitelist = ['http://47.100.32.187', 'http://smsgate.grundfos.cn', 'http://www.smsgate.grundfos.cn' ];

const corsOptions = {
    origin: function(origin, callback) {
        if (whitelist.indexOf(origin) !== -1) {
            callback(null, true);
        } else {
            console.log(`${origin} not allowed by CORS`);
        }
    }
}

module.exports = cors(corsOptions);
module.exports = cors();
