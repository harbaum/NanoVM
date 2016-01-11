/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.project;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URI;
import java.net.URL;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.StringTokenizer;

import org.eclipse.core.resources.ICommand;
import org.eclipse.core.resources.IFile;
import org.eclipse.core.resources.IFolder;
import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IProjectDescription;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IResourceStatus;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.OperationCanceledException;
import org.eclipse.core.runtime.Platform;
import org.eclipse.core.runtime.SubProgressMonitor;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.jface.dialogs.DialogSettings;
import org.eclipse.jface.dialogs.ErrorDialog;
import org.eclipse.jface.dialogs.IDialogSettings;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.nanovm.internal.ui.utils.FileUtil;
import org.eclipse.nanovm.runtime.NanoVMRuntime;
import org.eclipse.nanovm.tool.NanoVMTool;
import org.eclipse.nanovm.ui.NanoVMUI;
import org.eclipse.ui.IWorkbench;
import org.eclipse.ui.actions.WorkspaceModifyOperation;
import org.eclipse.ui.dialogs.WizardNewProjectCreationPage;
import org.eclipse.ui.wizards.newresource.BasicNewProjectResourceWizard;

/**
 * NanoVM project wizard. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NewNanoVMProjectWizard extends BasicNewProjectResourceWizard {
	private static final String DIALOG_SETTINGS_KEY = "NewNanoVMProjectWizard";

	private WizardNewProjectCreationPage mainPage;

	private IProject newProject;

	private String defaultProjectName;
	
	public NewNanoVMProjectWizard() {
		this(null);
	}
	
	public NewNanoVMProjectWizard(String defaultProjectName) {
		this.defaultProjectName = defaultProjectName;
		initDialogSettings();
	}
	
	@Override
	public void init(IWorkbench workbench, IStructuredSelection currentSelection) {
        super.init(workbench, currentSelection);
        setNeedsProgressMonitor(true);
        setWindowTitle("New NanoVM Project");
    }

	protected void initDialogSettings() {
		IDialogSettings pluginSettings = NanoVMUI.getDefault().getDialogSettings();
		IDialogSettings wizardSettings = pluginSettings.getSection(DIALOG_SETTINGS_KEY);
		if (wizardSettings == null) {
			wizardSettings = new DialogSettings(DIALOG_SETTINGS_KEY);
			pluginSettings.addSection(wizardSettings);
		}
		setDialogSettings(wizardSettings);
	}

	@Override
	public void addPages() {
		mainPage = new WizardNewProjectCreationPage("basicNewProjectPage");	
		mainPage.setTitle("NanoVM Project");
		mainPage.setDescription("Create a new NanoVM project.");
		if(defaultProjectName != null)
			mainPage.setInitialProjectName(defaultProjectName);
		this.addPage(mainPage);
	}

	@Override
	public IProject getNewProject() {
		return newProject;
	}

	@Override
	public boolean performFinish() {
		createNewNanoVMProject();

		if (newProject == null) {
			return false;
		}

		updatePerspective();
		selectAndReveal(newProject);

		return true;
	}

	private IProject createNewNanoVMProject() {
		if (newProject != null) {
			return newProject;
		}

		// get a project handle
		final IProject newProjectHandle = mainPage.getProjectHandle();

		// get a project descriptor
		URI location = null;
		if (!mainPage.useDefaults()) {
			location = mainPage.getLocationURI();
		}

		IWorkspace workspace = ResourcesPlugin.getWorkspace();
		final IProjectDescription description = workspace.newProjectDescription(newProjectHandle.getName());
		description.setLocationURI(location);
		
		// create the new project operation
		WorkspaceModifyOperation op = new WorkspaceModifyOperation() {
			protected void execute(IProgressMonitor monitor) throws CoreException {
				createNanoVMProject(description, newProjectHandle, monitor);
			}
		};

		// run the new project creation operation
		try {
			getContainer().run(true, true, op);
		} catch (InterruptedException e) {
			return null;
		} catch (InvocationTargetException e) {
			// ie.- one of the steps resulted in a core exception
			Throwable t = e.getTargetException();
			if (t instanceof CoreException) {
				if (((CoreException) t).getStatus().getCode() == IResourceStatus.CASE_VARIANT_EXISTS) {
					MessageDialog.openError(getShell(), "Creation Problems", MessageFormat.format("The underlying file system is case insensitive. There is an existing project which conflicts with ''{0}''.", newProjectHandle.getName()));
				} else {
					ErrorDialog.openError(getShell(), "Creation Problems", null, ((CoreException) t).getStatus());
				}
			} else {
				NanoVMUI.log(e);
			}
			return null;
		}

		newProject = newProjectHandle;

		return newProject;
	}

	void createNanoVMProject(IProjectDescription description, IProject projectHandle, IProgressMonitor monitor) throws CoreException, OperationCanceledException {
		try {
			monitor.beginTask("", 7000);
			
			addNature(JavaCore.NATURE_ID, description, monitor);
			addNature(NanoVMUI.NATURE_ID, description, monitor);
			
			addBuilder(JavaCore.BUILDER_ID, description, monitor);
			//addBuilder(NanoVMUI.BUILDER_ID, description, monitor);
			
			projectHandle.create(description, new SubProgressMonitor(monitor, 1000));
			
			if (monitor.isCanceled()) {
				throw new OperationCanceledException();
			}			

			projectHandle.open(IResource.BACKGROUND_REFRESH, new SubProgressMonitor(monitor, 1000));		
			
			createFolderHierarchy(projectHandle, getFolderHierarchyFromPath("src"), new SubProgressMonitor(monitor, 300));
            createFolderHierarchy(projectHandle, getFolderHierarchyFromPath("bin"), new SubProgressMonitor(monitor, 300));
            createFolderHierarchy(projectHandle, getFolderHierarchyFromPath("config"), new SubProgressMonitor(monitor, 300));
            createFolderHierarchy(projectHandle, getFolderHierarchyFromPath("lib"), new SubProgressMonitor(monitor, 300));
            
            createConfigurationFiles(projectHandle.getFolder("config"), new SubProgressMonitor(monitor, 1000));
            createToolLibrary(projectHandle.getFolder("lib"), new SubProgressMonitor(monitor, 1000));
            createRuntimeLibrary(projectHandle.getFolder("lib"), new SubProgressMonitor(monitor, 1000));
			
			IFile file= projectHandle.getFile(".classpath");
            file.create(createClasspathEntries(), true, new SubProgressMonitor(monitor, 100));            
		} finally {
			monitor.done();
		}
	}
	
	private void addNature(String natureID, IProjectDescription description, IProgressMonitor monitor) throws CoreException {
		if (monitor != null && monitor.isCanceled()) {
			throw new OperationCanceledException();
		}
		if (!description.hasNature(natureID)) {
			String[] prevNatures= description.getNatureIds();
			String[] newNatures= new String[prevNatures.length + 1];
			System.arraycopy(prevNatures, 0, newNatures, 0, prevNatures.length);
			newNatures[prevNatures.length]= natureID;
			description.setNatureIds(newNatures);
		} 
		
		if (monitor != null) {
			monitor.worked(100);
		}
	}
	
	private void addBuilder(String builderID, IProjectDescription description, IProgressMonitor monitor) throws CoreException {
		if (monitor != null && monitor.isCanceled()) {
			throw new OperationCanceledException();
		}
		if (!hasBuilder(builderID, description)) {			
			ICommand[] prevBuilders= description.getBuildSpec();
			ICommand[] newBuilders= new ICommand[prevBuilders.length + 1];
			System.arraycopy(prevBuilders, 0, newBuilders, 0, prevBuilders.length);
			newBuilders[prevBuilders.length]= description.newCommand();
			newBuilders[prevBuilders.length].setBuilderName(builderID);
			description.setBuildSpec(newBuilders);
		}
		
		if (monitor != null) {
			monitor.worked(100);
		}		
	}
	
	private boolean hasBuilder(String builderID, IProjectDescription description){
		for(ICommand command : description.getBuildSpec()){
			if(builderID.equals(command.getBuilderName()))
				return true;
		}
		return false;
	}
	
	private String[] getFolderHierarchyFromPath(String path){
		StringTokenizer tokenizer = new StringTokenizer(path, "/");
		ArrayList<String> tokens = new ArrayList<String>();
		while(tokenizer.hasMoreTokens())
			tokens.add(tokenizer.nextToken());
		return tokens.toArray(new String[tokens.size()]);
	}
	
	private void createFolderHierarchy(IProject projectHandle, String[] folders, IProgressMonitor monitor) throws CoreException {
        IFolder last= null;
        for(String folderName : folders) {
            if(last == null) {
                last= projectHandle.getFolder(folderName);
                if(!last.exists())
                    last.create(true, true, monitor);
            } else {
                IFolder newFolder= last.getFolder(folderName);
                if(!newFolder.exists())
                    newFolder.create(true, true, monitor);
                last= newFolder;
            }
        }
    }
	
	private void createConfigurationFiles(IFolder destination, IProgressMonitor monitor){
		try {
			File configDirectory= new File(FileLocator.resolve(new URL(NanoVMTool.getDefault().getBundle().getEntry("/"), "config/")).getFile());
			File files[] = configDirectory.listFiles();
			monitor.beginTask("Copying configuration files", files.length);
			for(File file : files){
				if(file.getName().toLowerCase().endsWith(NanoVMTool.CONFIGURATION_FILE_EXTENSION) || file.getName().toLowerCase().endsWith(NanoVMTool.NATIVE_FILE_EXTENSION))
				FileUtil.copyFile(file, destination, monitor);
				monitor.worked(1);
			}
		} catch (MalformedURLException e) {
			NanoVMUI.log(e);
		} catch (IOException e) {
			NanoVMUI.log(e);
		}
		monitor.done();
	}
	
	private void createToolLibrary(IFolder destination, IProgressMonitor monitor){
		monitor.beginTask("Copying tool library", 1);
		try {				
			File runtimeToolFile= new File(FileLocator.resolve(new URL(NanoVMTool.getDefault().getBundle().getEntry("/"), "lib/" + NanoVMTool.LIBRARY_NAME)).getFile());				
			FileUtil.copyFile(runtimeToolFile, destination, monitor);
			monitor.worked(1);
		} catch (MalformedURLException e) {
			NanoVMUI.log(e);
		} catch (IOException e) {
			NanoVMUI.log(e);
		}
		monitor.done();
	}
	
	private void createRuntimeLibrary(IFolder destination, IProgressMonitor monitor){
		monitor.beginTask("Copying runtime library", 1);
		try {				
			if(Platform.inDevelopmentMode()){
				File runtimeLibraryDirectory= new File(FileLocator.resolve(new URL(NanoVMRuntime.getDefault().getBundle().getEntry("/"), "bin")).getFile());
				FileUtil.copyDirectory(runtimeLibraryDirectory, destination.getFolder(NanoVMRuntime.LIBRARY_NAME), monitor);
				monitor.worked(1);
			}else{
				File runtimeLibraryFile= new File(FileLocator.resolve(new URL(NanoVMRuntime.getDefault().getBundle().getEntry("/"), NanoVMRuntime.LIBRARY_NAME)).getFile());				
				FileUtil.copyFile(runtimeLibraryFile, destination, monitor);
				monitor.worked(1);
			}
		} catch (MalformedURLException e) {
			NanoVMUI.log(e);
		} catch (IOException e) {
			NanoVMUI.log(e);
		}
		monitor.done();
	}
    
    private InputStream createClasspathEntries(){
        ByteArrayOutputStream buffer= new ByteArrayOutputStream();
        PrintStream classpath= new PrintStream(buffer, true); 
        classpath.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
        classpath.println("<classpath>");
        
        // src entries
        insertEntry(classpath, "src", "src");  
        
        // output folder
        insertEntry(classpath, "output", "bin"); 
        
        // container
        //insertEntry(classpath, "con", "org.eclipse.jdt.launching.JRE_CONTAINER");
        insertEntry(classpath, "con", NanoVMUI.LIBRARY_CONTAINER_ID);
        classpath.print("</classpath>");
        
        return new ByteArrayInputStream(buffer.toByteArray());
    }
    
    private void insertEntry(PrintStream stream, String kind, String path) {
        stream.append("\t<classpathentry kind=\"").append(kind).append("\" path=\"").append(path).println("\"/>");
    }    
}
