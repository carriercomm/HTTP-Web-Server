import os
import sys
import subprocess
from fabricate import *

MAXCOMPILERDIR = '/network-raid/opt/maxcompiler-2014.1.1/'
CLASSPATH = os.environ['CLASSPATH']

MAXCOMPILER = 'maxjc'
MAXDEBUG = 'source,lines,vars'
MAXSOURCE = '1.6'
MAXTARGET = '1.6'
MAXCLASSPATH = ['/home/mvorkapic/workspace/maxeda/bin/',
		'%s/lib/MaxCompiler.jar' % MAXCOMPILERDIR]

MAXDESTDIR = 'bin'
MAXSRCDIR = 'src'

JAVARTENV = '/network-raid/opt/jdk1.6.0_02/jre/bin/java'
JAVAMAXHEAPSIZE = 'mx12288m' #-Xmx<size> set maximum Java heap size
HTTPSERVERCLASSPATH = MAXCLASSPATH + ['bin'] + [CLASSPATH]
TARGET = 'DFE'
#MAXFILENAME = 'httpServer_2014_1_'
HTTPSERVERSTARTUPCLASS = 'httpServer.httpServerManager'


def build():
    clean()
    compile_java()
    #execute_java()
    execute_java_maxJavaRun()


# compile java
# command
# maxjc -nowarn -source 1.6 -target 1.6 -classpath /home/mvorkapic/workspace/maxeda/bin/:/network-raid/opt/maxcompiler-ticket7202-clang/lib/MaxCompiler.jar -d bin src
def compile_java():
    print "\n*** Compile Java *** \n"
    run(MAXCOMPILER, '-nowarn', '-g:%s' % MAXDEBUG, '-source', '%s' % MAXSOURCE, 
        '-target', '%s' % MAXTARGET, '-classpath', '%s' % ':'.join(MAXCLASSPATH), 
        '-d', '%s' % MAXDESTDIR, MAXSRCDIR)

# run java (create .max file)
# command
# java -Xmx12288m -classpath bin:/home/mvorkapic/workspace/maxeda/bin/:/network-raid/opt/maxcompiler-ticket7202-clang/lib/MaxCompiler.jar httpServer.httpServerManager
# or: CLASSPATH=./bin:/home/mvorkapic/workspace/maxeda/bin:$CLASSPATH maxJavaRun httpServer.httpServerManager target=DFE

#def execute_java():
#    print "\n*** Execute Java *** \n"
#    run(JAVARTENV, '-X%s' % JAVAMAXHEAPSIZE, '-classpath', '%s' % ':'.join(HTTPSERVERCLASSPATH), 
#        HTTPSERVERSTARTUPCLASS, 'target=%s' % TARGET)
##	'target=%s' % TARGET, 'maxFileName=%s' % MAXFILENAME

def execute_java_maxJavaRun():
    print "\n*** Execute Java *** \n"
    #print ' '.join['CLASSPATH=%s' % ':'.join(HTTPSERVERCLASSPATH), 'maxJavaRun', HTTPSERVERSTARTUPCLASS, 'target=%s' % TARGET]
    Temp = ' '.join(['CLASSPATH=%s' % ':'.join(HTTPSERVERCLASSPATH), 'maxJavaRun', HTTPSERVERSTARTUPCLASS, 'target=%s' % TARGET])
#, 'maxFileName=%s' % MAXFILENAME
    subprocess.check_call(Temp, shell=True)
    #subprocess.check_call([)
#	'target=%s' % TARGET, 'maxFileName=%s' % MAXFILENAME


def clean():
    autoclean()

main()