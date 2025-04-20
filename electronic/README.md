# Esquema elctrónico
![](<../img/electronic PCB 3D.png>)

El proyecto se desarrolló usando [KiCAD](https://www.kicad.org/). ([electronic.kicad_pro](electronic.kicad_pro)) se desarrollo para conectar una placa de ESP32 con 2 controladores de motores L298 y encoders de 3 motores JGA25-371. Los componentes utilizados en el proyecto son:

* 1 **ESP32 S3** WROOM 1 N16R8 DevKit
* 2 Modulos controladores de motores **L298**
* 3 Motores con encoder **JGA25-371**
* 4 LEDs
* 4 Resistencias **220 $\Omega$**
* 1 Fuente de alimentación **12V 2A**

## Esquema
![](<../img/electronic.svg>)

El esquema eléctrico se encuentra dividido en ocho partes:

* 1 Alimentación: En esta sección se encuentra el conector de la fuente de alimentación para el circuito.
* 3 Motores
* 1 LED: LEDS conectados a los pines GPIO del microcontrolador y resistencias de 220 $\Omega$.
* 2 Controladores de motores
* 1 Microcontrolador

### Motores
En esta sección se encuentran los motores con encoder **JGA25-371**. 

* Las etiquetas del encoder se conectan a los pines GPIO del microcontrolador.
* Las etiquetas de alimentación se conectan a la salida de 3.3V del microcontrolador.
* Las etiquetas de los motores se conectan a los controladores de motores.

### Controladores de motores
En esta sección se encuentran los módulos de controladores de motores **L298N**. 

* Las etiquetas de los pines de control de los módulos se conectan a los pines GPIO del microcontrolador.
* Las etiquetas de alimentación se conectan a la fuente de alimentación y la salida de 5V de los controladores a la alimentación del microcontrolador.
* Las etiquetas de los motores se conectan a los motores.

### Microcontrolador
En esta sección se encuentra el microcontrolador **ESP32 S3** y las etiquetas de los pines que se utilizan para la comunicación con el resto de los componentes.

## PCB
![](<../img/electronic PCB.svg>)

La PCB fue diseñada para conectar todos los componentes del circuito. La PCB tiene un tamaño de 68.5x75 mm y fue diseñada para ser fabricada en dos capas. El diseño se encuentra en la carpeta `electronic` ([PCB.kicad_pcb](PCB.kicad_pcb)). Para la PCB se usan los siguientes componentes:

* 1 PCB de dos capas
* 1 Conector de alimentación **Barrel Jack**
* 2 Conectores **JST-B3B** para la alimentación de los controladores de motores.
* 3 Conectores **JST-B4B** para la conexión de los encoders de los motores.
* 2 Conectores **JST-B5B** para la conexión de los pines de control de los controladores de motores.
* 1 Terminal de 44 pines para la conexión del microcontrolador.
* 4 LEDs SMD **5730**
* 4 Resistencias 220 $\Omega$ SMD **1206**
