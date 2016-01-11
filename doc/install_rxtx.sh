#!/bin/bash
#
# install_rxtx.sh, install gnu.io/rxtx extension under linux
#
# run this script with "sh ./install_rxtx.sh"
#

export RXTX=rxtx-2.1-7pre17-i686-pc-linux-gnu.tar.gz

echo "----------------------------"
echo "- Installation of rxtx     -"
echo "----------------------------"
echo
echo "Installing:"
echo "- ${RXTX}"
echo

# check for java installation directory
if [ "${JAVA_HOME}" == "" ]; then
    echo
    echo "ERROR:";
    echo "JAVA_HOME environment variable not set.";
    echo "Unable to continue installation!";
    exit 1;
fi;

echo "Checking for jcl.jar in ${JAVA_HOME}/jre/lib/ext/"
if [ -f ${JAVA_HOME}/jre/lib/ext/jcl.jar ]; then
    echo
    echo "ERROR:";
    echo "It seems you already have installed javax.comm or the";
    echo "like. Please remove it first. If you installed it using";
    echo "the install_javacomm.sh script, then can remove it";
    echo "with the uninstall_javacomm.sh script.";
    exit 1;
fi;

echo "Installing in " ${JAVA_HOME} " ..."
echo

# fetch rxtx package
if [ ! -f ${RXTX} ]; then
    echo
    echo "Trying to download ${RXTX} ...";
    wget ftp://jarvi.dsl.frii.com/pub/rxtx/${RXTX};
fi;

# Check rxtx archive
MD5=`md5sum ${RXTX} | awk '{print $1}'`
MD5_EXPECT="28e3d775786e1ce8c847eb99d992ef69"
if [ "${MD5}" != "${MD5_EXPECT}" ]; then
    echo
    echo "ERROR:";
    echo ${RXTX}" archive md5 sum is invalid:";
    echo "MD5 is: ${MD5}"
    echo "Expected: ${MD5_EXPECT}"
    exit 1;
fi

RXTXDIR=rxtx-2.1-7pre17-i686-pc-linux-gnu

# check that target directory doesn'>t exist yet
if [ -e ${RXTXDIR} ]; then
    echo
    echo "ERROR:";
    echo "${RXTXDIR} directory already exists in current directory.";
    exit 1;
fi

# uncompress rxtx archive
echo "Uncompressing ${RXTX} ..."
tar xfz ${RXTX}

# copy librxtxSerial.so to %JAVA_HOME%/jre/lib/i386,
# (%JAVA_HOME% is the folder where JRE is installed on your system; e.g. /usr/local/j2sdk1.4.1_01)
 
# copy RXTXcomm.jar to %JAVA_HOME%/jre/lib/ext


echo "Installing libParallel.so to ${JAVA_HOME}/jre/lib/i386/"
cp ${RXTXDIR}/librxtxParallel.so ${JAVA_HOME}/jre/lib/i386/
echo "Installing libSerial.so to ${JAVA_HOME}/jre/lib/i386/"
cp ${RXTXDIR}/librxtxSerial.so ${JAVA_HOME}/jre/lib/i386/
echo "Installing RXTXcomm.jar to ${JAVA_HOME}/jre/lib/ext/"
cp ${RXTXDIR}/RXTXcomm.jar ${JAVA_HOME}/jre/lib/ext/

echo "Cleaning up ..."
rm -rf ${RXTXDIR}
echo

