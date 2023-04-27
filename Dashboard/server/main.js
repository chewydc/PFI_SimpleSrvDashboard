//---------------------------------------------------------------------------------------------------//
// PROYECTO FINAL DE INGENIERIA                                                                      //
// Diseño y Simulación de una Red de Sensores Inalámbricos basados en tecnología de largo alcance    //
//                                                                                                   //
// Autores:                                                                                          //
//          - Damián del Campo                                                                       //
//          - Jonatan Salini                                                                         //
//                                                                                                   //
//---------------------------------------------------------------------------------------------------//
import express from 'express'
import { Server as HttpServer } from 'http' 
import { Server as IOServer } from 'socket.io'
const app = express()
const httpServer = new HttpServer(app)
const io = new IOServer(httpServer)

const SIZE = process.env.PORT || 5 //Size of the messages table displayed on the webapage

let mensajes = []
app.use(express.json())
app.use(express.static('public'))
app.use(express.urlencoded({ extended: true }))

import mqtt from 'mqtt'
const options = {
        host: 'MQTT_BROKER', //Define the host broker of your choice.
        port: 1883,
}

// initialize the MQTT client
const client = mqtt.connect(options);

// setup the callbacks
client.on('connect', function () {
    console.log('Connected to mqtt broker');
});
client.on('error', function (error) {
    console.log(error);
});
client.on('message', function (topic, message) {
    // called each time a message is received
    console.log('Received message:', topic, message.toString());
    let info = []
    info = message.toString()
    console.log(info)
    let time = new Date()
    let newElem = { nodoOrigen: info[0], dato: info.slice(2), nodoDestino: info[1],fecha: time.toLocaleString() }  
    mensajes.unshift(newElem)
    if(mensajes.length>SIZE) mensajes.pop()
    console.log(newElem)
    io.sockets.emit('updateMsj')
});
client.subscribe('TEST_TOPIC'); //Define the topic of your choice.

//-------------------------------------------------------------------
//Websockets handles
io.on('connection', clientSocket => {
    clientSocket.on('nuevoMensaje', (msj) => {
      logger.info(msj)
      io.sockets.emit('updateMsj')
    })  
})

app.get('/', (req, res) => {
    res.sendFile('index.html', { root: './public/home' })
});

app.get('/mensajes', (req, res) => {
    res.json(mensajes)
});
/* 
// POST feature to load some fake event and test the webpage
app.post('/mensajes', (req, res) => {
    const time = new Date()
    const newElem = { ...req.body, fecha: time.toLocaleString() }  
    mensajes.unshift(newElem)
    if(mensajes.length>SIZE) mensajes.pop()
    console.log(newElem)
    io.sockets.emit('updateMsj')
    res.json('Dato Cargado OK')
});
*/

const PORT = process.env.PORT || 8080
const server = httpServer.listen(PORT, () => {
    console.log(`Servidor escuchando en el puerto ${server.address().port}`)
})
server.on('error', error => console.log(`Error en servidor ${error}`))
