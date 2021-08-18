
let connectButton = document.getElementById('connect');
let disconnectButton = document.getElementById('disconnect');
let terminalContainer = document.getElementById('terminal');
let sendForm = document.getElementById('send-form');
let inputField = document.getElementById('input');


//Buttons
let forward_button = document.getElementById('forward_button');
let backward_button = document.getElementById('backward_button');
let left_button = document.getElementById('left_button');
let right_button = document.getElementById('right_button');
let light_button = document.getElementById('light_button');
let alarm_button = document.getElementById('alarm_button');
let photo_button = document.getElementById('photo_button');
let emotion_button = document.getElementById('emotion_button');

forward_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('A\n'); //
  send(' '); //
});

backward_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('C\n'); //
  send(' '); //
});

left_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('D\n'); //
  send(' '); //
});

right_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('B\n'); //
  inputField.value = '';  //
  inputField.focus();     //
  send(' '); //
});

light_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('G\n'); //
  inputField.value = '';  //
  inputField.focus();     //
  send(' '); //
});

photo_button.addEventListener('click', function() {
  event.preventDefault(); //
  send(' \n'); //
  inputField.value = '';  //
  inputField.focus();     //
  send(' '); //
});

alarm_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('F\n'); //
  inputField.value = '';  //
  inputField.focus();     //
  send(' '); //
});

emotion_button.addEventListener('click', function() {
  event.preventDefault(); //
  send('E\n'); //
  inputField.value = '';  //
  inputField.focus();     //
  send(' '); //
});


//end Buttons

//arrow keys
var isKeyDown = false;

document.addEventListener('keydown', function(e) {
    switch (e.keyCode) {
        case 37:
          event.preventDefault(); //
          send('D'); //
            break;
        case 38:
        event.preventDefault(); //
        send('A'); //
            break;
        case 39:
        event.preventDefault(); //
        send('B'); //
            break;
        case 40:
        event.preventDefault(); //
        send('C'); //
            break;
    }
});

document.addEventListener('keyup', function(e) {
    switch (e.keyCode) {
        case 37:
          event.preventDefault(); //
          send(' '); //
            break;
        case 38:
        event.preventDefault(); //
        send(' '); //
            break;
        case 39:
        event.preventDefault(); //
        send(' '); //
            break;
        case 40:
        event.preventDefault(); //
        send(' '); //
            break;
    }
});


//end arrow keys

connectButton.addEventListener('click', function() {
  connect();
});

disconnectButton.addEventListener('click', function() {
  disconnect();
});

sendForm.addEventListener('submit', function(event) {
  event.preventDefault(); //
  send(inputField.value); //
  inputField.value = '';  //
  inputField.focus();     //
});


let deviceCache = null;

let characteristicCache = null;


let readBuffer = '';


function connect() {
  return (deviceCache ? Promise.resolve(deviceCache) :
      requestBluetoothDevice()).
      then(device => connectDeviceAndCacheCharacteristic(device)).
      then(characteristic => startNotifications(characteristic)).
      catch(error => log(error));
}

function requestBluetoothDevice() {
  log('Requesting bluetooth device...');

  return navigator.bluetooth.requestDevice({
    filters: [{services: [0xFFE0]}],
  }).
      then(device => {
        log('"' + device.name + '" bluetooth device selected');
        deviceCache = device;
        deviceCache.addEventListener('gattserverdisconnected',
            handleDisconnection);

        return deviceCache;
      });
}

function handleDisconnection(event) {
  let device = event.target;

  log('"' + device.name +
      '" bluetooth device disconnected, trying to reconnect...');

  connectDeviceAndCacheCharacteristic(device).
      then(characteristic => startNotifications(characteristic)).
      catch(error => log(error));
}

function connectDeviceAndCacheCharacteristic(device) {
  if (device.gatt.connected && characteristicCache) {
    return Promise.resolve(characteristicCache);
  }

  log('Connecting to GATT server...');

  return device.gatt.connect().
      then(server => {
        log('GATT server connected, getting service...');

        return server.getPrimaryService(0xFFE0);
      }).
      then(service => {
        log('Service found, getting characteristic...');

        return service.getCharacteristic(0xFFE1);
      }).
      then(characteristic => {
        log('Characteristic found');
        characteristicCache = characteristic;

        return characteristicCache;
      });
}

function startNotifications(characteristic) {
  log('Starting notifications...');

  return characteristic.startNotifications().
      then(() => {
        log('Notifications started');
        characteristic.addEventListener('characteristicvaluechanged',
            handleCharacteristicValueChanged);
      });
}

function handleCharacteristicValueChanged(event) {
  let value = new TextDecoder().decode(event.target.value);

  for (let c of value) {
    if (c === '\n') {
      let data = readBuffer.trim();
      readBuffer = '';

      if (data) {
        receive(data);
      }
    }
    else {
      readBuffer += c;
    }
  }
}

function receive(data) {
  log(data, 'in');
}

function log(data, type = '') {
  terminalContainer.insertAdjacentHTML('beforeend',
      '<div' + (type ? ' class="' + type + '"' : '') + '>' + data + '</div>');
  terminalContainer.scrollTop = terminalContainer.scrollHeight;
}

function disconnect() {
  if (deviceCache) {
    log('Disconnecting from "' + deviceCache.name + '" bluetooth device...');
    deviceCache.removeEventListener('gattserverdisconnected',
        handleDisconnection);

    if (deviceCache.gatt.connected) {
      deviceCache.gatt.disconnect();
      log('"' + deviceCache.name + '" bluetooth device disconnected');
    }
    else {
      log('"' + deviceCache.name +
          '" bluetooth device is already disconnected');
    }
  }

  if (characteristicCache) {
    characteristicCache.removeEventListener('characteristicvaluechanged',
        handleCharacteristicValueChanged);
    characteristicCache = null;
  }

  deviceCache = null;
}

function send(data) {
  data = String(data);

  if (!data || !characteristicCache) {
    return;
  }

  //data += '\n';

  if (data.length > 20) {
    let chunks = data.match(/(.|[\r\n]){1,20}/g);

    writeToCharacteristic(characteristicCache, chunks[0]);

    for (let i = 1; i < chunks.length; i++) {
      setTimeout(() => {
        writeToCharacteristic(characteristicCache, chunks[i]);
      }, i * 100);
    }
  }
  else {
    writeToCharacteristic(characteristicCache, data);
  }

  switch(data){
    case 'A':
    log("Move forward, sent: " + data, 'out');
    break;
    case 'B':
    log("Move right, sent: " + data, 'out');
    break;
    case 'C':
    log("Move backward, sent: " + data, 'out');
    break;
    case 'D':
    log("Move left, sent: " + data, 'out');
    break;
    case 'G':
    log("Toggle light, sent: " + data, 'out');
    break;
    case 'F':
    log("Toggle alarm, sent: " + data, 'out');
    break;
    case 'E':
    log("Communicate, sent: " + data, 'out');
    break;
    default:
    log(data, 'out');
    break;
  }

}

function writeToCharacteristic(characteristic, data) {
  characteristic.writeValue(new TextEncoder().encode(data));
}
