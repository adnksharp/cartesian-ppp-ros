from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description() -> LaunchDescription:
    ld = LaunchDescription()

    # Obtener la ruta del paquete
    pkg_share = FindPackageShare(package='carobot')
    
    # Construir rutas de archivos
    default_model_path = PathJoinSubstitution([pkg_share, 'urdf', 'model.urdf'])
    default_rviz_path = PathJoinSubstitution([pkg_share, 'rviz', 'urdf.rviz'])

    # Argumento para habilitar/deshabilitar GUI
    gui_arg = DeclareLaunchArgument(
        name='gui',
        default_value='false',
        choices=['true', 'false'],
        description='Flag to enable/disable the GUI'
    )
    ld.add_action(gui_arg)

    # Argumento para el archivo de configuración de RViz
    rviz_arg = DeclareLaunchArgument(
        name='rvizconfig',
        default_value=default_rviz_path,
        description='Path to the RViz configuration file'
    )
    ld.add_action(rviz_arg)

    # Argumento para el modelo URDF
    model_arg: DeclareLaunchArgument = DeclareLaunchArgument(
        name='model',
        default_value=default_model_path,
        description='Path to the URDF model file'
    )
    ld.add_action(model_arg)

    # Topico de conexión entre gazebo y ROS2
    jsp_node = Node(
        package='carobot',
        executable='pos_get.py',
        name='robot_position_publisher',
    )
    ld.add_action(jsp_node)

    # Incluir el lanzamiento de urdf_launch
    ild: IncludeLaunchDescription = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'carobot',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui'),
            'use_gui': 'false',
        }.items(),
    )
    ld.add_action(ild)

    # Agregar nodo de gz_create
    gz_create_node = Node(
        package='ros_gz_sim',
        executable='create',
        name='spawner',
        output='screen',
        arguments=[
            '-name', 'carobot', 
            '-topic', '/robot_description',
        ]
    )

    ld.add_action(gz_create_node)

    # Agregar nodo de gz_bridge
    gz_bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='robot_gz_bridge',
        output='screen',
        arguments=[
            '/yoba/carobot/joint_0/cmd_pos@std_msgs/msg/Float64]gz.msgs.Double',
            '/yoba/carobot/joint_0/state@sensor_msgs/msg/JointState[gz.msgs.Model',
            '/yoba/carobot/joint_1/cmd_pos@std_msgs/msg/Float64]gz.msgs.Double',
            '/yoba/carobot/joint_1/state@sensor_msgs/msg/JointState[gz.msgs.Model',
            '/yoba/carobot/joint_2/cmd_pos@std_msgs/msg/Float64]gz.msgs.Double',
            '/yoba/carobot/joint_2/state@sensor_msgs/msg/JointState[gz.msgs.Model',
        ],
    )
    ld.add_action(gz_bridge_node)

    # Ejecutar gz sim -r -v 4 empty.sdf
    gz_sim_node = ExecuteProcess(
        cmd=['gz', 'sim', '-r', '-v', '4', 'empty.sdf'],
        output='screen'
    )
    ld.add_action(gz_sim_node)

    return ld
