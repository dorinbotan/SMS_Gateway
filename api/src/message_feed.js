'use strict'
const MAX_SIZE = 1000;
const CLEAR_AT_ONCE = 100;

var elements = 0;
var feed = [];

function clean() {
    feed = feed.splice(CLEAR_AT_ONCE);
    elements -= CLEAR_AT_ONCE;
}

module.exports = {
    push: function(obj) {
        feed.push(obj);
        elements++;

        if(elements > MAX_SIZE)
            clean();
    },
    get: function(validator) {
        if (!validator) {
            return feed;
        }

        return feed.reduce((prev, cur, index) => { if (validator(cur, index)) prev.push(cur); return prev; }, []);
    }
}
