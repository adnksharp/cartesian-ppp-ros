#!/bin/bash
clear
SYS_LANG="./src/lang/${LANG:0:2}.sh"
if [ ! -f "$SYS_LANG" ]; then
	source ./src/lang/en.sh
else
	source "$SYS_LANG"
fi

DIST="$(. /etc/os-release && echo $PRETTY_NAME)"
printf "${MSG_TITLE}$(echo $DIST | awk -F ' ' '{print $1}')\n"

# for ubuntu distros
if [ "$(echo $DIST | grep -E 'Ubuntu|Debian|Linux Mint')" ]; then
	printf "${MSG_PKG_PRE}"
	sudo apt install software-properties-common lsb-release gnupg curl -y > /dev/null 2>&1
	printf "${MSG_TAB_PRE}"

	printf "${MSG_PKG_UNIV}"
	sudo add-apt-repository universe -y > /dev/null 2>&1
	printf "${MSG_TAB_UNIV}"

	printf "${MSG_PKG_KEYS}"
	sudo apt update > /dev/null 2>&1
	sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg > /dev/null 2>&1
	sudo curl https://packages.osrfoundation.org/gazebo.gpg --output /usr/share/keyrings/pkgs-osrf-archive-keyring.gpg > /dev/null 2>&1
	printf "${MSG_TAB_KEYS}"

	printf "${MSG_PKG_ADD}"
	echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null 2>&1
	echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/pkgs-osrf-archive-keyring.gpg] http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/gazebo-stable.list > /dev/null 2>&1
	printf "${MSG_TAB_ADD}"

	printf "${MSG_PKG_UPD}"
	sudo apt update > /dev/null 2>&1
	printf "${MSG_TAB_UPD}"

	printf "\n"
	ROS_DISTROS=("beta3" "humble" "jazzy" "rolling" "galactic" "foxy" "dashing" "crystal" "ardent" "bouncy" "aero" "zesty" "xenial")
	ROS_DIST="alpha1"
	PKG_COUNT=50
	for i in "${ROS_DISTROS[@]}"; do
		printf "${MSG_ROS_FIND} $i ${MSG_TAB_ROS}"
		ROS_CHECK="$(apt search ros-$i > /dev/null 2>&1 | wc -l)"
		if [ "$ROS_CHECK" -gt "$PKG_COUNT" ]; then
			ROS_DIST=$i
			break
		fi
	done

	printf "\n"
	while IFS= read -r line; do
		if [[ "$line" == *"ros-"* ]]; then
			line=${line//ROS_DIST/$ROS_DIST}
			printf "${MSG_PKG_INSTALL} $line\n"
			sudo apt install -y $line > /dev/null 2>&1
		fi
	done < ./src/deb-packages.txt
	
	printf "\033[F\033[K\033[F\033[K"
	printf "${MSG_ROS_INSTALL} $ROS_DIST"
	ROS_SOURCE="/opt/ros/$ROS_DIST/setup.bash"
	if [ -f "$ROS_SOURCE" ]; then
		printf "${MSG_TAB_ROS_OK}"
	else
		printf "${MSG_TAB_ROS_FAIL}"
		exit 1
	fi

	if [[ "$ROS_DIST" == "rolling" ]]; then
		gz_print="ionic   "
		gz_install="gz-ionic"
	elif [[ "$ROS_DIST" == "jazzy" ]]; then
		gz_print="harmonic"
		gz_install="gz-harmonic"
	elif [[ "$ROS_DIST" == "humble" ]]; then
		gz_print="fortress"
		gz_install="ignition-fortress"
	else
		gz_print="citadel "
		gz_install="ignition-citadel"
	fi
	printf "${MSG_GZ_INSTALL}${gz_print}${MSG_TAB_GZ}"
	sudo apt install $gz_install -y > /dev/null 2>&1
	printf "${MSG_GZ_INSTALLED}${gz_print}${MSG_TAB_GZ_OK}"

	SSHELL="$(echo $SHELL | awk -F '/' '{print $NF}')"
	ROS_DISTRO="$ROS_DIST"

	source /opt/ros/$ROS_DISTRO/setup.$SSHELL

	printf "${MSG_UROS_DOWNLOAD}"
	rm -rf ~/uros-ws
	mkdir -p ~/uros-ws
	cd ~/uros-ws
	git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup > /dev/null 2>&1
	printf "${MSG_TAB_UROSDOWN}"

	printf "${MSG_UROS_INSTALL}"
	sudo apt update > /dev/null 2>&1 && rosdep update > /dev/null 2>&1
	rosdep install --from-paths src --ignore-src -y > /dev/null 2>&1
	colcon build > /dev/null 2>&1
	source install/local_setup.$SSHELL
	printf "${MSG_TAB_UROSINS}"

	printf "${MSG_UROS_AGENT_CR}"
	ros2 run micro_ros_setup create_agent_ws.sh > /dev/null 2>&1
	printf "${MSG_TAB_UROSCR}"
	
	printf "${MSG_UROS_AGENT_BLD}"
	ros2 run micro_ros_setup build_agent.sh > /dev/null 2>&1
	source install/local_setup.$SSHELL
	printf "${MSG_TAB_UROSBLD}"

	printf "${MSG_UROS_INO}"
	rm -rf ~/Arduino/libraries/micro_ros_arduino
	git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_arduino.git ~/Arduino/libraries/micro_ros_arduino > /dev/null 2>&1
	printf "${MSG_TAB_UROSINO}"

	printf "${MSG_FINISH} \e[93m~/.${SSHELL}rc\e[90m:\e[0m\n"
	for i in $(seq 1 60); do
		printf "#"
	done
	printf "\nexport ROS_DOMAIN_ID=42\n"
	printf "export ROS_VERSION=2\n"
	printf "export ROS_PYTHON_VERSION=3\n"
	printf "run-ros() {\n"
	AD
	printf "	export ROS_DISTRO=$ROS_DIST\n"
	printf "	source /opt/ros/$ROS_DISTRO/setup.$SSHELL\n"
	printf "	eval \"\$(register-python-argcomplete ros2)\"\n"
	printf "	eval \"\$(register-python-argcomplete ros2cli)\"\n"
	printf "	eval \"\$(register-python-argcomplete colcon)\"\n"
	printf "}\n"
	printf "run-uros() {\n"
	printf "	export ROS_DISTRO=$ROS_DIST\n"
	printf "	source ~/uros_ws/install/local_setup.$SSHELL\n"
	printf "	eval \"\$(register-python-argcomplete ros2)\"\n"
	printf "	eval \"\$(register-python-argcomplete ros2cli)\"\n"
	printf "	eval \"\$(register-python-argcomplete colcon)\"\n"
	printf "}\n"
	printf "alias ros-agent=\"ros2 run micro_ros_agent micro_ros_agent\"\n"
	for i in $(seq 1 60); do
		printf "#"
	done
	printf "\e[0m\n"
fi
