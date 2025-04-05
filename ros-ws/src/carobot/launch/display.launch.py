from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

def generate_launch_description() -> LaunchDescription:
    ld = LaunchDescription()

    # Obtener la ruta del paquete
    pkg_share = FindPackageShare(package='carobot')
    
    # Construir rutas de archivos
    default_model_path = PathJoinSubstitution(['urdf', 'model.urdf'])
    default_rviz_path = PathJoinSubstitution([pkg_share, 'rviz', 'urdf.rviz'])

    # Argumento para habilitar/deshabilitar GUI
    gui_arg = DeclareLaunchArgument(
        name='gui',
        default_value='true',
        choices=['true', 'false'],
        description='Flag to enable/disable the GUI'
    )
    ld.add_action(gui_arg)

    # Argumento para el archivo de configuración de RViz
    rviz_arg = DeclareLaunchArgument(
        name='rvizconfig',
        default_value=default_rviz_path,
        description='Path to the RViz config file relative to the package'
    )
    ld.add_action(rviz_arg)

    # Argumento para el modelo URDF
    model_arg: DeclareLaunchArgument = DeclareLaunchArgument(
        name='model',
        default_value=default_model_path,
        description='Path to robot urdf file relative to urdf_tutorial package')
    ld.add_action(model_arg)

    ld.add_action(DeclareLaunchArgument(
        name='jsp_gui',
        default_value='true',
        choices=['true', 'false'],
        description='Flag to enable/disable the GUI for joint state publisher'
    ))

    # Nodo de Joint State Publisher
    """
        J1: Junta prismatica de -0.5 a 0
        J2: Junta prismatica de -0.5 a 0
        J3: Junta prismatica de 0 a 0.5
    """
    ld.add_action(Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        condition=UnlessCondition(LaunchConfiguration('jsp_gui'))
    ))

    # Nodo de Joint State Publisher GUI
    ld.add_action(Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        condition=IfCondition(LaunchConfiguration('jsp_gui'))
    ))

    # Incluir el lanzamiento de urdf_launch
    ild: IncludeLaunchDescription = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'carobot',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui')}.items())
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

    # Agregar nodo de gz_create
    gz_bridge_node = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='gz_bridge',
        output='screen',
        arguments=[
            '/world/empty/model/carobot/joint_states@sensor_msgs/msg/JointState@gz.msgs.Model',
            '/world/empty/model/carobot/joint_states/J1@std_msgs/msg/Float64@gz.msgs.Double',
            '/world/empty/model/carobot/joint_states/J2@std_msgs/msg/Float64@gz.msgs.Double',
            '/world/empty/model/carobot/joint_states/J3@std_msgs/msg/Float64@gz.msgs.Double',
        ],
    )
    ld.add_action(gz_bridge_node)

    return ld
