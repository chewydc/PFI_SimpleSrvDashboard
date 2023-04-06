//---------------------------------------------------------------------------------------------------//
// PROYECTO FINAL DE INGENIERIA                                                                      //
// Diseño y Simulación de una Red de Sensores Inalámbricos basados en tecnología de largo alcance    //
//                                                                                                   //
// Alumnos:                                                                                          //
//          - Damián del Campo                                                                       //
//          - Jonatan Salini                                                                         //
//                                                                                                   //
//---------------------------------------------------------------------------------------------------//
const socket = io.connect();
listarMensajes()

socket.on('updateMsj', () => {
    console.log("evento updateMsj llego al cliente")
    listarMensajes() 
});

async function listarMensajes() {
    const plantillaMsj = await buscarPlantillaMensaje()
    const mensajes = await buscarMensajes()
    const html = armarHTMLmsj(plantillaMsj, mensajes)
    document.getElementById('mensajes').innerHTML = html
}

//---------------------------------------
//Ejecuto API del lado del cliente
function buscarMensajes() {
    return fetch('/mensajes')
        .then(msjs => msjs.json())
}
//---------------------------------------
//Busco plantilla Handlebars
function buscarPlantillaMensaje() {
    return fetch('/plantillas/mensaje.hbs')
        .then(respuesta => respuesta.text())
}
//---------------------------------------
//Armo Html
function armarHTMLmsj(plantillaMsj, mensajes) {
    const render = Handlebars.compile(plantillaMsj);
    const html = render({ mensajes })
   return html
}