/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.classpath;

import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.Path;
import org.eclipse.core.runtime.Platform;
import org.eclipse.jdt.core.IClasspathContainer;
import org.eclipse.jdt.core.IClasspathEntry;
import org.eclipse.jdt.core.IJavaProject;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.nanovm.runtime.NanoVMRuntime;
import org.eclipse.nanovm.tool.NanoVMTool;
import org.eclipse.nanovm.ui.NanoVMUI;

/**
 * "NanoVM Library" class path container. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMClasspathContainer implements IClasspathContainer {
	protected IJavaProject javaProject;
	
	public NanoVMClasspathContainer(IJavaProject javaProject){
		this.javaProject = javaProject;
	}

	public IClasspathEntry[] getClasspathEntries() {		
		if(Platform.inDevelopmentMode()){
			IFolder runtimeLibraryFolder = javaProject.getProject().getFolder("lib").getFolder(NanoVMRuntime.LIBRARY_NAME);
			if(runtimeLibraryFolder.exists())
				return new IClasspathEntry[]{
					JavaCore.newLibraryEntry(runtimeLibraryFolder.getFullPath(), null, null),
					JavaCore.newLibraryEntry(javaProject.getProject().getFolder("lib").getFile(NanoVMTool.LIBRARY_NAME).getFullPath(), null, null)
				};
		}else{
			IFile runtimeLibraryFile = javaProject.getProject().getFolder("lib").getFile(NanoVMRuntime.LIBRARY_NAME);
			if(runtimeLibraryFile.exists()){
				return new IClasspathEntry[]{
					JavaCore.newLibraryEntry(runtimeLibraryFile.getFullPath(), null, null),
					JavaCore.newLibraryEntry(javaProject.getProject().getFolder("lib").getFile(NanoVMTool.LIBRARY_NAME).getFullPath(), null, null)
				};
			}
		}
		
		return new IClasspathEntry[0];
	}

	public String getDescription() {
		return "NanoVM System Library";
	}

	public int getKind() {
		return IClasspathContainer.K_APPLICATION;
	}

	public IPath getPath() {
		return new Path(NanoVMUI.LIBRARY_CONTAINER_ID);
	}

}
