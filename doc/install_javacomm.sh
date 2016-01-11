#!/bin/bash
#
# install_javacomm.sh, install javacomm extension under linux
#
# run this script with "sh ./install_javacomm.sh"
#

echo "The usage of javax.comm is deprecated." 
echo "Please use gnu.io instead"
exit 0;

export JAVACOMM=javax_comm-2_0_3-solsparc.tar.Z
export RXTX=rxtx-bins.1.tar.gz 

echo "----------------------------"
echo "- Installation of javacomm -"
echo "----------------------------"
echo
echo "Installing:"
echo "- ${JAVACOMM}"
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

echo "Installing in " ${JAVA_HOME} " ..."
echo

# check for javax.comm package
if [ ! -f ${JAVACOMM} ]; then
    echo
    echo "ERROR:";
    echo ${JAVACOMM}" archive not found in current directory.";
    echo "Please download it from http://java.sun.com/products/javacomm";
    echo "and place it in the current directory."
    exit 1;
fi;

# Check javax.comm archive
MD5=`md5sum ${JAVACOMM} | awk '{print $1}'`
MD5_EXPECT="b8eaa0ab5fa36edfae64194aeacb651b"
if [ "${MD5}" != "${MD5_EXPECT}" ]; then
    echo
    echo "ERROR:";
    echo ${JAVACOMM}" archive md5 sum is invalid:";
    echo "MD5 is: ${MD5}"
    echo "Expected: ${MD5_EXPECT}"
    exit 1;
fi

# fetch rxtx package
if [ ! -f ${RXTX} ]; then
    echo
    echo "Trying to download ${RXTX} ...";
    wget ftp://jarvi.dsl.frii.com/pub/rxtx/${RXTX};
fi;

# Check rxtx archive
MD5=`md5sum ${RXTX} | awk '{print $1}'`
MD5_EXPECT="7b734a5fa91d6265f4e92e442a48be82"
if [ "${MD5}" != "${MD5_EXPECT}" ]; then
    echo
    echo "ERROR:";
    echo ${RXTX}" archive md5 sum is invalid:";
    echo "MD5 is: ${MD5}"
    echo "Expected: ${MD5_EXPECT}"
    exit 1;
fi

RXTXDIR=rxtx-bins.1
RXTXVER=1.4

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

echo "Installing libParallel.so to ${JAVA_HOME}/jre/lib/i386/"
cp ${RXTXDIR}/${RXTXVER}/i386-pc-linux/libParallel.so ${JAVA_HOME}/jre/lib/i386/
echo "Installing libSerial.so to ${JAVA_HOME}/jre/lib/i386/"
cp ${RXTXDIR}/${RXTXVER}/i386-pc-linux/libSerial.so ${JAVA_HOME}/jre/lib/i386/
echo "Installing jcl.jar to ${JAVA_HOME}/jre/lib/exit/"
cp ${RXTXDIR}/${RXTXVER}/jcl.jar ${JAVA_HOME}/jre/lib/ext/

echo "Cleaning up ..."
rm -rf ${RXTXDIR}
echo

# check that target directory doesn'>t exist yet
if [ -e commapi ]; then
    echo
    echo "ERROR:";
    echo "commapi directory already exists in current directory.";
    exit 1;
fi
    
# uncompress javacomm archive
echo "Uncompressing ${JAVACOMM} ..."
tar xfZ ${JAVACOMM}

echo "Installing comm.jar to ${JAVA_HOME}/jre/lib/ext ..."
cp commapi/comm.jar ${JAVA_HOME}/jre/lib/ext
echo "Registering driver ..."
echo "Driver=gnu.io.RXTXCommDriver" > ${JAVA_HOME}/jre/lib/javax.comm.properties

echo "Cleaning up ..."
rm -rf commapi
