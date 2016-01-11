/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.project;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectNature;
import org.eclipse.core.runtime.CoreException;

/**
 * NanoVM project nature. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMProjectNature implements IProjectNature {
	protected IProject project;

	public void configure() throws CoreException {		
	}

	public void deconfigure() throws CoreException {
	}

	public IProject getProject() {
		return project;
	}

	public void setProject(IProject project) {	
		this.project = project;
	}
}
