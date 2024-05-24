const WebSocket = require('ws');
const net = require('net');

const wss = new WebSocket.Server({ port: 8080 });

wss.on('connection', function connection(ws) {
    const client = new net.Socket();
    client.connect(23455, '127.0.0.1', function() {
        console.log('Connected to C++ server');
    });

    client.on('data', function(data) {
        ws.send(data);
    });

    ws.on('message', function(message) {
        client.write(message);
    });

    ws.on('close', function() {
        client.destroy();
    });
});
