
$(document).ready(function() {
    var socket = io.connect();
    var count;

    // Show error box if the program is not running
    $('#error-box').show();
    
    // Show error box if program not responding for about 3 seconds
    function handleErrors() {
        socket.emit('beats', 'update');
        
        count++;
        if (count > 2) {
            $('#error-box').show();
        } 
    }

    // Look for all beat options
    $('#modeNone').click(() => {
        socket.emit('beats', 'none');
        $('#modeid').text('None');
    });
    $('#modeRock1').click(() => {
        socket.emit('beats', 'rock1');
        $('#modeid').text('Rock 1');
    });
    $('#modeRock2').click(() => {
        socket.emit('beats', 'rock2');
        $('#modeid').text('Rock 2');
    });

    $('#volumeDown').click(() => {
        let volume = Number($('#volumeid').val());
        if (volume >= 5) {
            $('#volumeid').val(volume - 5);
            socket.emit('beats', 'vol_decr');
        }
    });
    $('#volumeUp').click(() => {
        let volume = Number($('#volumeid').val());
        if (volume <= 95) {
            $('#volumeid').val(volume + 5);
            socket.emit('beats', 'vol_incr');
        }
    });

    $('#tempoDown').click(() => {
        let tempo = Number($('#tempoid').val());
        if (tempo >= 45) {
            $('#tempoid').val(tempo - 5);
            socket.emit('beats', 'tempo_decr');
        }
    });
    $('#tempoUp').click(() => {
        let tempo = Number($('#tempoid').val());
        if (tempo <= 295) {
            $('#tempoid').val(tempo + 5);
            socket.emit('beats', 'tempo_incr');
        }
    });

    $('#hihat').click(() => socket.emit('beats', 'hihat'));
    $('#snare').click(() => socket.emit('beats', 'snare'));
    $('#base').click(() => socket.emit('beats', 'base'));

    $('#stop').click(() => socket.emit('beats', 'stop'));


    // Input from program with updated mode, vol, tempo, time so we can update
    // Help from chat gpt with the structure of this socket.on and the split function
    socket.on('value', (update) => {
        count = 0; // program is running so decrease count back to 0 (if it hits 3, error box will show)

        // Get input from program (chat gpt helped with this)
        let [command, mode, vol, tempo, time] = update.split(' ');
        
        // Update website values
        if (command === 'values') {
            $('#error-box').hide();
            $('#stop').show();
            $('#status span').text(time);
            $('#volumeid').val(vol);
            $('#tempoid').val(tempo);
            
            if (mode == '1') {
                $('#modeid').text('None');
            } else if (mode == '2') {
                $('#modeid').text('Rock 1');
            } else if (mode == '3') {
                $('#modeid').text('Rock 2');
            }
        }
    });

    // Check error box every second (chat gpt helped with calling this function every 1 second)
    setInterval(handleErrors, 1000);
});
