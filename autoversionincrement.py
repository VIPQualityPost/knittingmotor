# see: https://stackoverflow.com/a/74589004

import sys, re, datetime, shutil, os
Import("env")

PATH_VERSION = './include/Version.h'
MAJOR, MINOR, PATCH, BUILD = 0, 1, 2, 3

# copy firmware + version.h to auto incremented firmware folder
def post_program_action(source, target, env):
    src = target[0].get_abspath()
    src_h = os.path.join(env["PROJECT_DIR"], "include/Version.h") 
 
    with open(PATH_VERSION, 'r') as reader:
      # Find "MAJOR.MINOR.PATCH+BUILD" from the first line
      line = re.search(r'"([^"]*)"', reader.readline()).group()[1:-1]
      # Extract old values for MAJOR.MINOR.PATCH+BUILD
      versions = re.split('\.|\+', line)
      versiondir = '%s.%s.%s+%s' % (versions[MAJOR], versions[MINOR], versions[PATCH], versions[BUILD])

    dest = os.path.join(env["PROJECT_DIR"], "firmware", versiondir)

    if not os.path.exists(dest):
      os.makedirs(dest)

    print("Copy firmware...")
    print("fw source:"+src)
    print("fw dest:"+dest)
 
    shutil.copy(src, dest)
    shutil.copy(src_h,dest)

# write new v ersion information to  Version.h after build
def versioning(source, target, env):
  # Read
  with open(PATH_VERSION, 'r') as reader:
    # Find "MAJOR.MINOR.PATCH+BUILD" from the first line
    line = re.search(r'"([^"]*)"', reader.readline()).group()[1:-1]
    # Extract old values for MAJOR.MINOR.PATCH+BUILD
    versions = re.split('\.|\+', line)
    # Increment value
    versions[BUILD] = int(versions[BUILD]) + 1

    # Write
    with open(PATH_VERSION, 'w') as writer:
      time = datetime.datetime.now()

      datestamp = time.strftime('%Y-%m-%d')
      timestamp = time.strftime('%H:%M')
      version = '%s.%s.%s+%d' % (versions[MAJOR], versions[MINOR], versions[PATCH], versions[BUILD])
      versionFull = version + ' %s %s' % (datestamp, timestamp)

      writer.writelines([
        '#define VERSION "%s"' % version,
        '\n#define VERSION_MAJOR %s' % versions[MAJOR],
        '\n#define VERSION_MINOR %s' % versions[MINOR],
        '\n#define VERSION_PATCH %s' % versions[PATCH],
        '\n#define VERSION_BUILD %s' % versions[BUILD],
        '\n#define VERSION_DATE "%s"' % datestamp,
        '\n#define VERSION_TIME "%s"' % timestamp,
        '\n#define VERSION_FULL "%s"' % versionFull
      ])

      print('Release: ' + version)

# register post build actions
env.AddPostAction("buildprog", versioning)
env.AddPostAction("$BUILD_DIR/firmware.hex", post_program_action)
