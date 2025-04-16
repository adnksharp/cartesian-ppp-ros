#!/bin/bash
clear
SYS_LANG="./src/lang/${LANG:0:2}.sh"
if [ ! -f "$SYS_LANG" ]; then
	source ./src/lang/en.sh
else
	source "$SYS_LANG"
fi

if [ "$EUID" -ne 0 ]; then
    printf "${MSG_ROOT_NO}"
    exit 1
fi



DIST="$(. /etc/os-release && echo $PRETTY_NAME)"
printf "${MSG_TITLE}$(echo $DIST | awk -F ' ' '{print $1}')\n"

# for ubuntu distros
if [ "$(echo $DIST | grep -E 'Ubuntu|Debian|Linux Mint')" ]; then
	printf "${MSG_PKG_PRE}"
	apt install software-properties-common lsb-release gnupg curl -y > /dev/null 2>&1
	printf "${MSG_TAB_PRE}"

	printf "${MSG_PKG_UNIV}"
	add-apt-repository universe -y > /dev/null 2>&1
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
		printf "${MSG_ROS_FIND} $i"
		ROS_CHECK="$(apt search ros-$i > /dev/null 2>&1 | wc -l)"
		if [ "$ROS_CHECK" -gt "$PKG_COUNT" ]; then
			ROS_DIST=$i
			printf "${MSG_TAB_ROS}"
			break
		else
			printf "\n"
		fi
	done

	printf "\n"
	while IFS= read -r line; do
		if [[ "$line" == *"ros-"* ]]; then
			line=${line//ROS_DIST/$ROS_DIST}
			printf "${MSG_PKG_INSTALL} $line\n"
			apt install -y $line > /dev/null 2>&1
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
	printf "${MSG_GZ_INSTALL}"
	if [[ "$ROS_DIST" == "rolling" ]]; then
		printf "ionic   "
		apt-get install gz-ionic -y > /dev/null 2>&1
	elif [[ "$ROS_DIST" == "jazzy" ]]; then
		printf "harmonic"
		apt-get install gz-harmonic -y > /dev/null 2>&1
	elif [[ "$ROS_DIST" == "humble" ]]; then
		printf "fortress"
		apt-get install ignition-fortress -y > /dev/null 2>&1
	else
		printf "citadel "
		apt-get install ignition-citadel -y > /dev/null 2>&1
	fi
	printf "${MSG_TAB_GZ}"


	printf "${MSG_FINISH}"
	for i in $(seq 1 60); do
		printf "#"
	done
	printf "\nexport ROS_DOMAIN_ID=42\n"
	printf "export ROS_VERSION=2\n"
	printf "export ROS_PYTHON_VERSION=3\n"
	printf "run-ros() {\n"
	printf "	export ROS_DISTRO=$ROS_DIST\n"
	printf "	source $ROS_SOURCE\n"
	printf "	eval \"\$(register-python-argcomplete ros2)\"\n"
	printf "	eval \"\$(register-python-argcomplete ros2cli)\"\n"
	printf "	eval \"\$(register-python-argcomplete colcon)\"\n"
	printf "}\n"
	for i in $(seq 1 60); do
		printf "#"
	done
	printf "\e[0m\n"
fi
