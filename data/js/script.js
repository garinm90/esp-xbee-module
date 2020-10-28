const wsQueue = [];
let wsBusy = false;
let wsTimerId;
const SH = document.getElementById('SH');
const SL = document.getElementById('SL');
const CH = document.getElementById('CH');
const ID = document.getElementById('ID');
const NI = document.getElementById('NI');
const saveConfig = document.getElementById('save');
let divErrorMsg = document.createElement('div');
const target = document.location.host;
const socket = new WebSocket('ws://192.168.86.168/ws');
const settingsPayload = {
  CH: '',
  ID: '',
  NI: '',
};

saveConfig.addEventListener('click', (e) => {
  if (CH.validity.valid && ID.validity.valid && NI.validity.valid) {
    // Why? Because we have a string that displays a decimal value.
    // So we turn it to an integer first then back to a string in HEX.
    settingsPayload.CH = parseInt(CH.value).toString(16);
    settingsPayload.ID = parseInt(ID.value).toString(16);
    settingsPayload.NI = NI.value;
    // console.log(settingsPayload.CH);
    // console.log(settingsPayload.ID);
    // console.log(settingsPayload.NI);
    console.log(settingsPayload);
    submitSettings(settingsPayload);
  } else {
    console.log('invalid settings!');
  }
});

CH.addEventListener('input', (e) => {
  if (CH.validity.valid) {
    CH.classList.add('is-valid');
    CH.classList.remove('is-invalid');
  } else {
    CH.classList.add('is-invalid');
    CH.classList.remove('is-valid');
  }
});

ID.addEventListener('input', (e) => {
  if (ID.validity.valid) {
    ID.classList.add('is-valid');
    ID.classList.remove('is-invalid');
  } else {
    ID.classList.add('is-invalid');
    ID.classList.remove('is-valid');
  }
});

NI.addEventListener('input', (e) => {
  if (NI.validity.valid) {
    NI.classList.add('is-valid');
    NI.classList.remove('is-invalid');
  } else {
    NI.classList.add('is-invalid');
    NI.classList.remove('is-valid');
  }
});

wsConnect();

function wsConnect() {
  socket.onopen = (e) => {
    // socket.send('S');
    wsEnqueue('S', socket);
  };

  socket.onmessage = (e) => {
    if (typeof e.data == 'string') {
      const cmd = e.data.substring(0, 1);
      const payload = JSON.parse(e.data.substring(1));
      switch (cmd) {
        case 'S':
          getSettings(payload);
          break;
        default:
          console.log('Unkown command: ' + e.data);
          break;
      }
    } else {
      console.log('Unknown Data Type');
    }
    wsReadyToSend();
  };
}

function wsEnqueue(message, socket) {
  let wsQueueIndex = wsQueue.findIndex(wsCheckQueue, message);
  if (wsQueueIndex === -1) {
    wsQueue.push(message);
  } else {
    wsQueue[wsQueueIndex] = message;
  }
  wsProcessQueue(socket);
}

function wsCheckQueue(value) {
  return value.substring(0, 1) == this.substring(0, 1);
}

function wsProcessQueue(socket) {
  if (wsBusy) {
    console.log('WS queue busy: ' + wsQueue);
  } else {
    wsBusy = true;
    message = wsQueue.shift();
  }
  wsTimerId = setTimeout(wsReadyToSend, 1000);
  socket.send(message);
}

function wsReadyToSend() {
  clearTimeout(wsTimerId);
  wsBusy = false;
  if (wsQueue.length > 0) {
    wsBusy.wsProcessQueue();
  } else {
    console.log('WS Queue is empty');
  }
}

function getSettings(payload) {
  SH.innerText = payload['SH'];
  SL.innerText = payload['SL'];
  console.log(CH.value);
  CH.value = parseInt(payload['CH'], 16).toString();
  ID.value = parseInt(payload['ID'], 16).toString();
  // NI.value = parseInt(payload['NI'], 16).toString();
  NI.value = String.fromCharCode(parseInt(payload['NI'], 16));
  // NI.value = payload['NIsp'];
}

function submitSettings(payload) {
  payload = 'U' + JSON.stringify(payload);
  console.log(payload);
  wsEnqueue(payload, socket);
}
