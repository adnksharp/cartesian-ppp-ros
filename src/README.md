# Scripts de Shell
![](../img/shell-install.svg)

## Instalador de ROS
[install-ros.sh](install-ros.sh) Es un script que instala ROS2, Gazebo sim y micro-ROS. Actualmente, está diseñado para distribuciones basadas en Debian, específicamente Ubuntu.

### 0. Instalar dependencias
El script se encarga de instalar paquetes necesarios para la instalación y habilitar el paquete universe.

### 1. Instalar ROS2
El script sigue el método de instalación de ROS2, tomando como base la versión de Ubuntu instalada para instalar la versión correspondiente de ROS2. Actualmente, soporta diferentes versiones de [Ubuntu](http://packages.ros.org/ros2/ubuntu/dists/). El script instala los paquetes almacenados en [deb-packages.txt](deb-packages.txt).

### 2. Instalar Gazebo simulate
Al igual que ROS2 es instalado en función de la versión de Ubuntu instalada, [Gazebo](https://gazebosim.org/docs/harmonic/ros_installation/) es instalado en función de la versión de ROS2 instalada.

### 3. Instalar micro-ROS
[micro-ROS](https://micro.ros.org/docs/tutorials/core/first_application_linux/) se instala de la misma forma que los paquetes de ROS2. Para esto, el script crea un espacio de trabajo en `~/uros-ws` donde descarga el proyecto de micro-ROS y lo compila. El script también crea y compila el agente de micro-ROS. 

### 4. Instalar librería de micro-ROS para Arduino
El script descarga la librería de micro-ROS para Arduino y la instala en el directorio de librerías de Arduino. 
