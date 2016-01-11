/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.classpath;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.jdt.core.ClasspathContainerInitializer;
import org.eclipse.jdt.core.IClasspathContainer;
import org.eclipse.jdt.core.IJavaProject;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.nanovm.ui.NanoVMUI;

/**
 * "NanoVM Library" class path container initializer. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMClasspathContainerInitializer extends ClasspathContainerInitializer {

	@Override
	public void initialize(IPath containerPath, IJavaProject project) throws CoreException {
		if(containerPath != null && containerPath.segmentCount() == 1 && NanoVMUI.LIBRARY_CONTAINER_ID.equals(containerPath.segment(0))){
			JavaCore.setClasspathContainer(containerPath, new IJavaProject[] {project}, new IClasspathContainer[] {new NanoVMClasspathContainer(project)}, null);
		}
	}

}
