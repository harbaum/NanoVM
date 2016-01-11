/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.nanovm.ui.NanoVMUI;

/**
 * Util class. 
 * 
 * @author tugstugi@yahoo.com
 */
public class FileUtil {
	
	public static void copyDirectory(File source, IFolder destination, IProgressMonitor monitor){
		if(!source.isDirectory())
			return;
		if(!destination.exists())
			try {
				destination.create(true, true, monitor);
			} catch (CoreException e) {
				NanoVMUI.log(e);
				return;
			}
		
		File files[] = source.listFiles();
		for(File file : files){
			if(file.isFile())
				copyFile(file, destination, monitor);
			else
				copyDirectory(file, destination.getFolder(file.getName()), monitor);
		}
	}
	
	public static void copyFile(File file, IFolder destination, IProgressMonitor monitor){
		String name = file.getName();
		IFile newFile = destination.getFile(name);
		FileInputStream input = null;
		try {
			input = new FileInputStream(file);
			if(newFile.exists()){
				newFile.setContents(input, true, false, monitor);
			}else{
				newFile.create(input, true, monitor);
			}
		} catch (Exception e){
			e.printStackTrace();
		}finally{
			if(input != null){
				try {
					input.close();
				} catch (IOException e) {
					NanoVMUI.log(e);
				}
			}
		}
		monitor.worked(1);
	}
}
