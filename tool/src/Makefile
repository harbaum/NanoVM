#
# Makefile for NanoVMTool
#

APP = NanoVMTool
VERSION = 1.5

all: ../$(APP).jar

CLASSPATH = ../../java/examples
NATIVEPATH = ../../java/native

JAVAFILES = AccessFlags.java CodeInfo.java ConstPoolEntry.java FieldInfo.java \
            MethodInfo.java AttributeInfo.java CodeTranslator.java \
	    ConstPoolEntryError.java InnerClassInfo.java NanoVMTool.java \
	    ClassFileReader.java CommonInfo.java ConvertException.java \
	    LineNumberInfo.java NativeMapper.java ClassInfo.java \
	    Config.java Debug.java LocalVariableInfo.java UVMWriter.java \
	    ClassLoader.java ConstPool.java ExceptionInfo.java \
	    MethodIdTable.java Uploader.java NVMComm2.java

# compile target code
$(CLASSPATH)/%.class: $(CLASSPATH)/%.java
	javac -classpath $(CLASSPATH):$(NATIVEPATH) $<

%.class: %.java
	echo "public class Version {" > Version.java
	echo "  public static String version = \"V$(VERSION)\";" >> Version.java
	echo "}" >> Version.java
	javac $<

../$(APP).jar: $(APP).class
	jar cmf $(APP).mf ../$(APP).jar *.class

# convert and upload a class file (should be moved to vm/target Makefile)
asuro-%: $(CLASSPATH)/%.class $(APP).class
	java $(APP) ../config/Asuro.config $(CLASSPATH) $*

mega8-%: $(CLASSPATH)/%.class $(APP).class
	java  $(APP) ../config/Mega8.config $(CLASSPATH) $*

clean:
	rm -f *.class *~ 
