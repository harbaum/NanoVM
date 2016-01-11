@rem
@rem asuro_upload.bat
@rem
@rem batch script to convert and upload class files to the NanoVM on the Asuro
@rem

@rem Set these to the values matching your system!!
set JAVALIB="c:\Program Files\Java\jre1.5.0_04\lib"
set ASUROCLASS=e:\projekte\nanovm\java

java -cp %JAVALIB%\comm.jar;./NanoVMTool.jar NanoVMTool -f COM1 config\Asuro.config %ASUROCLASS% %1