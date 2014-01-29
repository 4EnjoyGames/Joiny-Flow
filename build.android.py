import sys

mode = ['debug', 'release', 'debug_release']

def printUsageAndExit():
    print("USAGE:")
    for c in mode:
        print("{0} {1}".format(sys.argv[0], c))

    sys.exit(1)

if len(sys.argv) < 2:
    printUsageAndExit()

ard_mode = sys.argv[1]


from libraries.ADLib.modules_android import *

from libraries.ADLib.buildutil.AndroidProject import *
from libraries.ADLib.buildutil.Modules import  *

modules = Modules()
loadModules(modules)

working_directory = os.path.dirname(os.path.realpath(__file__))

from environment import *

project = AndroidProject(env)
project.modules = modules

#Modules
project.addModule('android_ADAds_AdMob')
project.addModule('android_ADStatistics_Flurry')
project.addModule('android_ADLanguage')
project.addModule('android_ADInfo_Store_GooglePlay')
project.addModule('android_ADNotification')
project.addModule('android_ADBrowser')

project.project_name = 'Joiny'
project.package_name = 'com.x4enjoy.joiny'
project.version_name = "1.01"
project.version_code = "5"
project.resources_compile_task = createResourceCompilerTask(
    os.path.join(working_directory, 'Resources_storage'),
    'joiny',
    'android',
    ['en']
)

project.signature = createAndroidSignature('build.android/joiny.jks',
											'joiny',
                                           'dq0duhPVBsEc25M2Tygj', 'dq0duhPVBsEc25M2Tygj')
project.res_dir = 'build.android/res'
project.working_directory = working_directory
project.addLookUpDir('libraries/ADLib/platform/Android/cpp')
project.addLookUpDir('libraries/ADLib/platform/Android/libs')
project.addLookUpDir('libraries/ADLib/platform/Android/java')
project.addLookUpDir('libraries/ADLib/src')
project.addLookUpDir('Classes')
project.main_activity_definition = project.getMainActivityDeclaration(screenOrientation="sensorPortrait")

project.addCppDir('libraries/ADLib/src')
project.addCppDir('Classes')

project.output_name = 'proj.android'


if ard_mode == 'debug':
    project.debuggable = True
    project.release_build = False
if ard_mode == 'release':
    project.debuggable = False
    project.release_build = True
if ard_mode == 'debug_release':
    project.debuggable = True
    project.release_build = True
project.buildProject()
project.runProject()