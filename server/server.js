// Create the server

const app = require('express')();
const server = require('http').createServer(app);
const io = require('socket.io')(server);

// Incorporate our websites folder
app.use(require('express').static(require('path').join(__dirname, 'website')));

// Function to handle messages
function handlePlay(socket, info) {
  // Print current button pressed to the console log
  console.log(`Current: ${info}`);

  const message = Buffer.from(info); // create new buffer

  // Create UDP socket
  const client = require('dgram').createSocket('udp4');
  
  // Send message to beaglebone
  client.send(message, 0, message.length, 12345, "192.168.7.2", null);

  // Event handler for messages
  client.on('message', (message) => {
      const value = message.toString('utf8');
      socket.emit('value', value);
    });
}

// Handle connection to Node server
io.on('connection', (socket) => {
  // Listen for music
  socket.on('beats', (info) => {
      handlePlay(socket, info);
  });
});

// Listening on port
server.listen(8088, function () {
    console.log('Listening on port 8088...');
});
