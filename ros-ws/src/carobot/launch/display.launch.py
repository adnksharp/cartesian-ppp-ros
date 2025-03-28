from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
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

    # Incluir el lanzamiento de urdf_launch
    ild: IncludeLaunchDescription = IncludeLaunchDescription(
        PathJoinSubstitution([FindPackageShare('urdf_launch'), 'launch', 'display.launch.py']),
        launch_arguments={
            'urdf_package': 'carobot',
            'urdf_package_path': LaunchConfiguration('model'),
            'rviz_config': LaunchConfiguration('rvizconfig'),
            'jsp_gui': LaunchConfiguration('gui')}.items())
    ld.add_action(ild)

    return ld
