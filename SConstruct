
import os

# Compiler settings:

C_COMPILER = 'clang'
CPP_COMPILER = 'clang++'
ENABLED_C_WARNINGS = '-Werror -Weverything '
ENABLED_CPP_WARNINGS = '-Werror -Weverything '
SUPPRESSED_C_WARNINGS = '  -Wno-reserved-id-macro -Wno-unused-command-line-argument'
SUPPRESSED_CPP_WARNINGS = ' -Wno-c++98-compat '
C_WARNINGS = ENABLED_C_WARNINGS + SUPPRESSED_C_WARNINGS
CPP_WARNINGS = ENABLED_CPP_WARNINGS + SUPPRESSED_CPP_WARNINGS

CPP_FLAGS = '-O2 -pipe -g -fno-omit-frame-pointer -fstrict-overflow \
    -fstrict-aliasing -funsigned-char -feliminate-unused-debug-types \
    -DNDEBUG -std=c++11 -stdlib=libc++ ' + CPP_WARNINGS
C_FLAGS = '-O2 -pipe -g -fno-omit-frame-pointer -fstrict-overflow \
    -fstrict-aliasing -funsigned-char -feliminate-unused-debug-types \
    -DNDEBUG -std=c11 ' + C_WARNINGS


# Setup environments:

# First start with external environment, then replace/append custom arguments
common_env = Environment(ENV=os.environ)
common_env.Replace(CC=C_COMPILER, CXX=CPP_COMPILER)
common_env.Append(CFLAGS=C_FLAGS, CXXFLAGS=CPP_FLAGS)

if ARGUMENTS.get('V') != '1':
    # Invoke scons with V=1 to show verbose build output
    common_env.Append(CCCOMSTR='     [C] $TARGET')
    common_env.Append(CXXCOMSTR='   [C++] $TARGET')
    common_env.Append(ARCOMSTR='    [AR] $TARGET')
    common_env.Append(RANLIBCOMSTR='[RANLIB] $TARGET')
    common_env.Append(LINKCOMSTR='    [LD] $TARGET')
    
    common_env.Append(SHCCCOMSTR='     [C] $TARGET')
    common_env.Append(SHCXXCOMSTR='   [C++] $TARGET')
    common_env.Append(SHLINKCOMSTR='    [LD] $TARGET')

src_env = common_env.Clone()
test_env = common_env.Clone()


# Execute build:

env_dict = {
    'src': src_env,
    'tests': test_env
}

for subdir, env in env_dict.iteritems():
    env.SConscript('%s/SConscript' % subdir,
                    { 'env': env },
                    variant_dir='bin/%s' % subdir,
                    src_dir=subdir,
                    duplicate=False)

