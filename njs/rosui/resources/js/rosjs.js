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
				if (message.topic === '/world/empty/model/carobot/joint/prismatic_0_joint/state') {
					document.getElementById('x-status').textContent = parseFloat(message.data.position[0]).toFixed(3)
				} else if (message.topic === '/world/empty/model/carobot/joint/prismatic_1_joint/state') {
					document.getElementById('y-status').textContent = parseFloat(message.data.position[0]).toFixed(3)
				} else if (message.topic === '/world/empty/model/carobot/joint/prismatic_2_joint/state') {
					document.getElementById('z-status').textContent = parseFloat(message.data.position[0]).toFixed(3)
				}
			}
			if (message.topic === '/sensiact/encoder/pos') {
				updateConnectionStatus('uros-status', 'Conectado')
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
		topic: '/world/empty/model/carobot/joint/prismatic_0_joint/cmd_pos',
		data: parseFloat(document.getElementById('x-input').value)
	})
	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_1_joint/cmd_pos',
		data: parseFloat(document.getElementById('y-input').value)
	})
	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_2_joint/cmd_pos',
		data: parseFloat(document.getElementById('z-input').value)
	})
	addTask([
		parseFloat(document.getElementById('x-input').value),
		parseFloat(document.getElementById('y-input').value),
		parseFloat(document.getElementById('z-input').value)
	])
})

document.getElementById('home').addEventListener('click', () => {
	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_0_joint/cmd_pos',
		data: parseFloat(0.0)
	})
	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_1_joint/cmd_pos',
		data: parseFloat(0.0)
	})
	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_2_joint/cmd_pos',
		data: parseFloat(0.0)
	})
	document.getElementById('x-input').value = 0.0
	document.getElementById('y-input').value = 0.0
	document.getElementById('z-input').value = 0.0
	addTask([
		parseFloat(0.0),
		parseFloat(0.0),
		parseFloat(0.0)
	])
})

document.getElementById('random').addEventListener('click', () => {
	const randomX = (Math.random() * 0.2 - 0.1).toFixed(3),
		randomY = (Math.random() * 0.2 - 0.1).toFixed(3),
		randomZ = (Math.random() * 0.2 - 0.1).toFixed(3)

	document.getElementById('x-input').value = randomX
	document.getElementById('y-input').value = randomY
	document.getElementById('z-input').value = randomZ

	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_0_joint/cmd_pos',
		data: parseFloat(randomX)
	})

	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_1_joint/cmd_pos',
		data: parseFloat(randomY)
	})

	sendMessage({
		type: 'publish_command',
		topic: '/world/empty/model/carobot/joint/prismatic_2_joint/cmd_pos',
		data: parseFloat(randomZ)
	})
	addTask([
		parseFloat(randomX),
		parseFloat(randomY),
		parseFloat(randomZ)
	])
})

document.getElementById('cancel-works').addEventListener('click', () => {
	const div = document.getElementById('progress'),
		ul = div.querySelector('ul')
	while (ul.children.length > 0) {
		ul.children[0].remove()
	}
	const li = document.createElement('li')
	li.className = 'task'
	li.innerHTML = `<p>Esperando...</p>`
	ul.appendChild(li)
})

document.getElementById('clear-history').addEventListener('click', () => {
	const div = document.getElementById('progress'),
		ul = div.querySelector('ul')
	while (ul.children.length > 0) {
		ul.children[0].remove()
	}
	const li = document.createElement('li')
	li.className = 'task'
	li.innerHTML = `<p>Historial vacío</p>`
	ul.appendChild(li)
})

function addTask(data) {
	const div = document.getElementById('progress'),
		ul = div.querySelector('ul'),
		li = document.createElement('li')
	if (ul.children.length === 1 && ul.children[0].querySelector('p').textContent === 'Esperando...') {
		ul.children[0].remove()
	}
	li.className = 'task'
	li.id = 'task' + (ul.children.length + 1)
	li.innerHTML = `<p class="display" id="li-text">[${data[0].toFixed(3)}, ${data[1].toFixed(3)}, ${data[2].toFixed(3)}]</p>
					<button id="cancel${ul.children.length + 1}" class="cancel">x</button>`
	ul.insertBefore(li, ul.firstChild)
	const cancelButton = li.querySelector('.cancel')
	cancelButton.addEventListener('click', () => {
		li.remove()
		if (ul.children.length === 0) {
			const li = document.createElement('li')
			li.className = 'task'
			li.innerHTML = `<p>Esperando...</p>`
			ul.appendChild(li)
		}
	})
}
