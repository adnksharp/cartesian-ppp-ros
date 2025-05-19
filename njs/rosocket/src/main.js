const ros = require('rclnodejs'),
	wocket = require('ws'),
	NODE_NAME = 'ui_websocket',
	ECHO_TOPICS = [
		'/yoba/carobot/joint_0/state', // Gazebo joint states
		'/yoba/carobot/joint_1/state', 
		'/yoba/carobot/joint_2/state',
		'/yoba/sensiact/info', // uROS hardware state
	],
	PUBLISH_TOPICS = [
		'/yoba/carobot/joint_0/cmd_pos', // Gazebo joint commands
		'/yoba/carobot/joint_1/cmd_pos',
		'/yoba/carobot/joint_2/cmd_pos',
		'/yoba/sensiact/cmd_pos' // uROS pos commands
	],
	ECHO_TOPIC_TYPES = [
		'sensor_msgs/msg/JointState',
		'sensor_msgs/msg/JointState',
		'sensor_msgs/msg/JointState',
		'std_msgs/msg/Int16MultiArray'
	],
	PUBLISH_TOPIC_TYPES = [
		'std_msgs/msg/Float64', 
		'std_msgs/msg/Float64',
		'std_msgs/msg/Float64',
		'std_msgs/msg/Int16MultiArray' 
	]

function log(type, message) {
	const date = new Date().toTimeString().split(' ')[0]
	let msg = ''
	switch (type) {
		case 'info':
			msg = `[\x1b[32mINFO\x1b[0m | ${date}] ${message}`
			break
		case 'warn':
			msg = `[\x1b[33mWARN\x1b[0m | ${date}] ${message}`
			break
		case 'error':
			msg = `[\x1b[31mERROR\x1b[0m | ${date}] ${message}`
			break
		default:
			msg = `[${date}] ${message}`
			break
	}
	console.log(msg)
}

async function start(wws) {
	try {
		await ros.init()
		const node = ros.createNode(NODE_NAME)
		ROSConf(node, wws)
		node.spin()
	} catch (err) {
		log('error', `Nodo ${NODE_NAME} no iniciado: ${err}`)
	}
}

const wws = new wocket.Server({ port: 8080 })
wws.on('connection', client => {
	log('info', 'Client connected')
	client.on('message', message => {
		try {
			const msg = JSON.parse(message)
			switch (msg.type) {
				case 'publish_command':
					publish(msg)
					break
				case 'request_status':
					break
				default:
					log('warn', `Tipo de mensaje no reconocido: ${msg.type}`)
					break
			}
		} catch (err) {
			log('info', `${message}`)
			log('error', `Error al procesar el mensaje: ${err}`)
		}
	})
	client.on('close', () => {
		log('info', 'Client disconnected')
	})
	client.on('error', err => {
		log('error', `Error en el cliente: ${err}`)
	})
	client.send(JSON.stringify({ type: 'info', message: 'Connected to WebSocket server' }))
})

function ROSConf(node, wws) {
	ECHO_TOPICS.forEach((topic, i) => {
		const sub = node.createSubscription(ECHO_TOPIC_TYPES[i], topic, msg => {
			const data = {
				type: 'echo',
				topic: topic,
				data: msg
			}
			wws.clients.forEach(client => {
				if (topic === '/yoba/sensiact/info') {
					for (let i = 0; i < 3; i++) {
						const gz_data = data.data.data[i] / 1000.0
						const gz_topic = `/yoba/carobot/joint_${i}/cmd_pos`
						const gz_pub = node.createPublisher('std_msgs/msg/Float64', gz_topic)
						gz_pub.publish({ data: gz_data })
					}
				}
				client.send(JSON.stringify(data))
			})
		})
	})
	
	function publish(msg) {
		const topic = msg.topic
		//find the topic type on the PUBLISH_TOPICS_TYPES array
		const index = PUBLISH_TOPICS.findIndex(t => t === topic)
		const type = index !== -1 ? PUBLISH_TOPIC_TYPES[index] : null

		if (topic && type) {
			const pub = node.createPublisher(type, topic)
			pub.publish({ data: msg.data })
			log('info', `Publicando en ${topic}`)
		} else {
			log('warn', `Topic ${topic} no encontrado`)
		}
	}
	global.publish = publish
}

start(wws)

process.on('SIGINT', async () => {
	log('info', 'Cerrando...')
	try {
		await node.destroyNode()
		wwsServer.close(() => {
			log('info', 'Servidor WebSocket cerrado.')
			process.exit(0)
		})
		setTimeout(() => {
			log('warn', 'Forzando el cierre del servidor WebSocket.')
			process.exit(1)
		}, 5000)
	} catch (err) {
		process.exit(0)
	}
})
