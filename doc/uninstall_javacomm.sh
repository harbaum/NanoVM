#!/bin/bash
#
# uninstall_javacomm.sh, uninstall javacomm extension under linux
#
# run this script with "sh ./uninstall_javacomm.sh"
#

echo "------------------------------"
echo "- Deinstallation of javacomm -"
echo "------------------------------"
echo

# check for java installation directory
if [ "${JAVA_HOME}" == "" ]; then
    echo
    echo "ERROR:";
    echo "JAVA_HOME environment variable not set.";
    echo "Unable to continue installation!";
    exit 1;
fi;

echo "Uninstalling from " ${JAVA_HOME} " ..."
echo

echo "Uninstalling libParallel.so from ${JAVA_HOME}/jre/lib/i386/"
rm ${JAVA_HOME}/jre/lib/i386/libParallel.so
echo "Uninstalling libSerial.so from ${JAVA_HOME}/jre/lib/i386/"
rm ${JAVA_HOME}/jre/lib/i386/libSerial.so
echo "Uninstalling jcl.jar from ${JAVA_HOME}/jre/lib/ext/"
rm ${JAVA_HOME}/jre/lib/ext/jcl.jar

echo "Uninstalling comm.jar from ${JAVA_HOME}/jre/lib/ext ..."
rm ${JAVA_HOME}/jre/lib/ext/comm.jar
echo "Unregistering driver ..."
rm ${JAVA_HOME}/jre/lib/javax.comm.properties
