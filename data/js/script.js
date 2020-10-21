const socket = new WebSocket('ws://192.168.86.168/ws');
const SH = document.getElementById('SH');
const SL = document.getElementById('SL');
const CH = document.getElementById('CH');
const ID = document.getElementById('ID');
const NI = document.getElementById('NI');

socket.onopen = (e) => {
  socket.send('S');
};

socket.onmessage = (e) => {
  console.log('Message recieved');
  const payload = JSON.parse(e.data.substring(1));
  SH.innerText = payload['SH'];
  SL.innerText = payload['SL'];
  // CH.innerHTML = payload['CH'];
  CH.value = payload['CH'];
  ID.value = payload['ID'];
  NI.value = payload['NI'];
};

// while(!socket.readyState){
//   console.log('Connecting.....')
// }
