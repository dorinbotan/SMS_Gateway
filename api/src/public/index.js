'use strict'
var index = 0;
var ticker;

$(document).ready(function() {
    let vh = window.innerHeight * 0.01;
    document.documentElement.style.setProperty('--vh', `${vh}px`);

    $('#receiver_text_input').on('keyup', function (e) {
        if (e.keyCode == 13) {
            form_submit();
        }
    });
    $('#message_text_input').on('keyup', function (e) {
        if (e.keyCode == 13) {
            form_submit();
        }
    });

    var request = new XMLHttpRequest();
    request.open('GET', `/api/sms`);
    request.onload = function(res) {
        var response = JSON.parse(request.response);

        response.map(msg => feed_message(msg.sender, msg.timestamp, msg.message));

        initialize_ticker(ticker);
    
        feed_scroll();
    };
    request.send();
});

function initialize_ticker(ticker) {
    ticker = new EventSource(`/api/sms/updates?since=${index}`);

    ticker.onmessage = function(event) {
        var data = JSON.parse(event.data);

        if (data.length == 0) return;

        data.map(msg => feed_message(msg.sender, msg.timestamp, msg.message));
        ticker.close();
        initialize_ticker(ticker);
    }
}

function feed_is_scrolled() {
    return $('#message_feed').height() + $('#message_feed').scrollTop() + 80 > $('#message_feed')[0].scrollHeight;
}

function feed_scroll() {
    $('#message_feed').scrollTop($('#message_feed')[0].scrollHeight);
}

function feed_message(sender, timestamp, message) {
    index = timestamp;
    $('#message_feed').append(`
    <div class="message_header">
        <div class="message_sender">${sender}</div>
        <div class="message_timestamp">${new Date(timestamp).toString('HH:MM:ss dd MMM yyyy')}</div>
    </div>
    <div class="message">${message}</div>`);
}

function form_submit() {
    if(!$('#receiver_text_input').val()) {
        $('#receiver_text_input').focus();
        return;
    }

    if(!$('#message_text_input').val()) {
        $('#message_text_input').focus();
        return;
    }

    send_message($('#receiver_text_input').val(), $('#message_text_input').val());

    $('#message_text_input').val('');
}

function send_message(receiver, message) {
    var obj = {
        receiver: receiver.replace(/^00/, '+'),
        message: message
    }

    console.log(`Send ${JSON.stringify(obj)}`);
    
    var request = new XMLHttpRequest();
    request.open('POST', `/api/sms`);
    request.setRequestHeader('Content-Type', 'application/json;charset=UTF-8');
    request.send(JSON.stringify(obj));

    feed_scroll();
}
