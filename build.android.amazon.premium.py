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


from build_android_config_premium import *
project.addModule('android_ADInfo_Store_AmazonStore')
project.addModule('android_ADInApp_Amazon')
project.addModule('android_ADVirtualCurrency_None')

project.output_name = 'proj.android.amazon.premium'

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