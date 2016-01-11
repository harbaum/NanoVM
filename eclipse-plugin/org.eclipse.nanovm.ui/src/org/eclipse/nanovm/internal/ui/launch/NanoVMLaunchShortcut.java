/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.launch;

import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.debug.core.DebugPlugin;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationType;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.core.ILaunchManager;
import org.eclipse.debug.ui.DebugUITools;
import org.eclipse.debug.ui.ILaunchShortcut;
import org.eclipse.jdt.core.IJavaProject;
import org.eclipse.jdt.core.IPackageFragmentRoot;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.jdt.core.JavaModelException;
import org.eclipse.jdt.launching.IJavaLaunchConfigurationConstants;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.nanovm.tool.NanoVMTool;
import org.eclipse.nanovm.ui.NanoVMUI;
import org.eclipse.ui.IEditorInput;
import org.eclipse.ui.IEditorPart;
import org.eclipse.ui.IFileEditorInput;

/**
 * Launcher shortcut. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMLaunchShortcut implements ILaunchShortcut{

	public void launch(ISelection selection, String mode) {	
		if (selection instanceof IStructuredSelection) {
			searchAndLaunch(((IStructuredSelection) selection).toArray(), mode);
		}
	}

	public void launch(IEditorPart editor, String mode) {
		IEditorInput input = editor.getEditorInput();
        if (input instanceof IFileEditorInput) {
            searchAndLaunch( new Object[]{((IFileEditorInput)input).getFile()}, mode);  
        }
	}

	protected void searchAndLaunch(Object[] search, String mode) {        
		for (int i = 0; i < search.length; i++)	{			
            Object obj = search[i];
            
            IProject project = null;
            IJavaProject javaProject = null;
            if(obj instanceof IResource){
            	project = ((IResource) obj).getProject();
            	javaProject = JavaCore.create(project);
            }else{
            	return;
            }
            
            IResource r = (IResource) obj;
            
            try {
                IPackageFragmentRoot sourceRoot = getSourceRoot(r.getFullPath(), javaProject);            
                IPath path = r.getFullPath().removeFirstSegments(sourceRoot.getPath().segmentCount());                                
                String p = path.toString();
                p = p.substring(0,p.lastIndexOf('.'));
                p = p.replace('/','.');
                
                String mainType = p; 
                ILaunchConfiguration configuration = findLaunchConfiguration(javaProject, mainType, mainType);
                
                DebugUITools.launch(configuration, mode);
            } catch (CoreException e) {
            	NanoVMUI.log(e);
            }
		}
	}
	
	private static IPackageFragmentRoot getSourceRoot(IPath path, IJavaProject project) throws JavaModelException {
		IPackageFragmentRoot[] roots = project.getAllPackageFragmentRoots();
		for (int i = 0; i < roots.length; i++) {
			IPackageFragmentRoot r = roots[i];
			if (r.getKind() == IPackageFragmentRoot.K_SOURCE && r.getPath().isPrefixOf(path))
				return r;
		}
		return null;
	}
	
	protected ILaunchConfiguration findLaunchConfiguration(IJavaProject project, String name, String mainType) throws CoreException{
        ILaunchConfigurationType configType = createLCType();        
        ILaunchConfiguration[] configs= DebugPlugin.getDefault().getLaunchManager().getLaunchConfigurations(configType);        
        for (ILaunchConfiguration c : configs) {
            if (c.getAttribute(IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME,"").equals(project.getJavaProject().getProject().getName()) &&
                c.getAttribute(NanoVMUI.ATTR_MAIN_CLASS_NAME, "").equals(mainType)) {
                return c;
            }
        }
        
        return createConfiguration(project, name, mainType);
    }
    
    protected ILaunchConfiguration createConfiguration(IJavaProject project, String name, String mainType) {                
        ILaunchConfiguration config= null;
        try {
            ILaunchConfigurationType configType = createLCType();
            ILaunchConfigurationWorkingCopy wc = configType.newInstance(null, DebugPlugin.getDefault().getLaunchManager().generateUniqueLaunchConfigurationNameFrom(name)); 
            wc.setAttribute(IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME, project.getElementName());
            wc.setAttribute(NanoVMUI.ATTR_CONFIGURATION_NAME, getFirstConfigurationFile(project));
            wc.setAttribute(NanoVMUI.ATTR_MAIN_CLASS_NAME, mainType);
            config= wc.doSave();        
        } catch (CoreException ce) {
        	NanoVMUI.log(ce);
        }
        return config;
    }

    private ILaunchConfigurationType createLCType() {
        ILaunchManager lm= DebugPlugin.getDefault().getLaunchManager();            
        ILaunchConfigurationType configType= lm.getLaunchConfigurationType(NanoVMUI.LAUNCHER);
        return configType;
    }
    
    private String getFirstConfigurationFile(IJavaProject project){
    	IFolder configFolder = project.getProject().getFolder("config");
    	try {
			for(IResource resource : configFolder.members()){
				if(resource.getName().toLowerCase().endsWith(NanoVMTool.CONFIGURATION_FILE_EXTENSION))
					return resource.getProjectRelativePath().toString();
			}
		} catch (CoreException e) {
			NanoVMUI.log(e);
		}
    	return "";
    }
}
