const websocket = new WebSocket('ws://localhost:8080')

websocket.onopen = () => {
	document.getElementById('x-input').disabled = false
	document.getElementById('y-input').disabled = false
	document.getElementById('z-input').disabled = false
	updateConnectionStatus('websocket-status', 'Conectado')
}

websocket.onmessage = (event) => {
	try {
		const message = JSON.parse(event.data)
		handleIncomingMessage(message)
		updateConnectionStatus('ros-status', 'Conectado')
	} catch (error) {
		updateConnectionStatus('ros-status', 'Error')
	}
}

websocket.onclose = () => {
	updateConnectionStatus('websocket-status', 'Desconectado')
	updateConnectionStatus('ros-status', 'Desconectado')
	updateConnectionStatus('uros-status', 'Desconectado')
	updateConnectionStatus('gazebo-status', 'Desconectado')
	document.getElementById('x-input').disabled = true
	document.getElementById('y-input').disabled = true
	document.getElementById('z-input').disabled = true
}

websocket.onerror = (error) => {
	updateConnectionStatus('websocket-status', 'Error')
	updateConnectionStatus('ros-status', 'Error')
	updateConnectionStatus('uros-status', 'Error')
	updateConnectionStatus('gazebo-status', 'Error')
	document.getElementById('x-input').disabled = true
	document.getElementById('y-input').disabled = true
	document.getElementById('z-input').disabled = true
}

function sendMessage(data) {
	if (websocket.readyState === WebSocket.OPEN) {
		websocket.send(JSON.stringify(data))
		Neutralino.debug.log('Mensaje enviado:', data)
	} else {
		Neutralino.debug.log('No se puede enviar el mensaje. El WebSocket no está abierto.')
	}
}

function handleIncomingMessage(message) {
	switch (message.type) {
		case 'info':
			Neutralino.debug.log('Mensaje del servidor:', message.message)
			break
		case 'echo':
			if (message.data.position && message.data.position !== undefined) {
				updateConnectionStatus('gazebo-status', 'Conectado')
			}
			if (message.topic === '/yoba/sensiact/info') {
				updateConnectionStatus('uros-status', 'Conectado')
				document.getElementById('x-status').textContent = parseFloat(message.data.data[0] / 1000).toFixed(3)
				document.getElementById('y-status').textContent = parseFloat(message.data.data[1] / 1000).toFixed(3)
				document.getElementById('z-status').textContent = parseFloat(message.data.data[2] / 1000).toFixed(3)
			}
			break
		default:
			Neutralino.debug.log('Mensaje no manejado:', message)
	}
}

function updateConnectionStatus(elementId, status) {
	const element = document.getElementById(elementId)
	element.textContent = status
}

document.getElementById('go').addEventListener('click', () => {
	sendMessage({
		type: 'publish_command',
		topic: '/yoba/sensiact/cmd_pos',
		data: [
			parseFloat(1000 * document.getElementById('x-input').value),
			parseFloat(1000 * document.getElementById('y-input').value),
			parseFloat(1000 * document.getElementById('z-input').value)
		]
	})
})

document.getElementById('home').addEventListener('click', () => {
	document.getElementById('x-input').value = 0.0
	document.getElementById('y-input').value = 0.0
	document.getElementById('z-input').value = 0.0
	sendMessage({
		type: 'publish_command',
		topic: '/yoba/sensiact/cmd_pos',
		data: [0, 0, 0]
	})
})

document.getElementById('random').addEventListener('click', () => {
	//random between -0,05 to 0,05
	const randomX = Math.random() * (0.05 - (-0.05)) + (-0.05)
		randomY = Math.random() * (0.05 - (-0.05)) + (-0.05)
		randomZ = Math.random() * (0.05 - (-0.05)) + (-0.05)

	document.getElementById('x-input').value = randomX
	document.getElementById('y-input').value = randomY
	document.getElementById('z-input').value = randomZ

	sendMessage({
		type: 'publish_command',
		topic: '/yoba/sensiact/cmd_pos',
		data: [
			parseFloat(1000 * document.getElementById('x-input').value),
			parseFloat(1000 * document.getElementById('y-input').value),
			parseFloat(1000 * document.getElementById('z-input').value)
		]
	})
})
