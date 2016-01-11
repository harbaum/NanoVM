/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.launch;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.IWorkspace;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.debug.core.ILaunchConfigurationWorkingCopy;
import org.eclipse.debug.ui.AbstractLaunchConfigurationTab;
import org.eclipse.jdt.core.IJavaElement;
import org.eclipse.jdt.core.IJavaModel;
import org.eclipse.jdt.core.IJavaProject;
import org.eclipse.jdt.core.IType;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.jdt.core.JavaModelException;
import org.eclipse.jdt.core.search.IJavaSearchScope;
import org.eclipse.jdt.core.search.SearchEngine;
import org.eclipse.jdt.launching.IJavaLaunchConfigurationConstants;
import org.eclipse.jdt.ui.IJavaElementSearchConstants;
import org.eclipse.jdt.ui.JavaElementLabelProvider;
import org.eclipse.jdt.ui.JavaUI;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ILabelProvider;
import org.eclipse.jface.window.Window;
import org.eclipse.nanovm.ui.NanoVMUI;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.ElementListSelectionDialog;
import org.eclipse.ui.dialogs.ResourceListSelectionDialog;
import org.eclipse.ui.dialogs.SelectionDialog;

/**
 * "Run" main tab. 
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMLaunchMainTab extends AbstractLaunchConfigurationTab {
	Text fProjText;
	Button fProjButton;
	Text fConfigText;
	Button fConfigButton;
	Text fMainClassText;
	Button fMainClassButton;
	
	public void createControl(Composite parent) {
		Font font = parent.getFont();
		
		Composite comp = new Composite(parent, SWT.NONE);
		setControl(comp);
		GridLayout topLayout = new GridLayout();
		topLayout.verticalSpacing = 0;
		comp.setLayout(topLayout);
		comp.setFont(font);
		
		createProjectEditor(comp);
		createConfigEditor(comp);
		createMainClassEditor(comp);
	}
	
	/**
	 * Creates the widgets for specifying a main type.
	 * 
	 * @param parent the parent composite
	 */
	private void createProjectEditor(Composite parent) {
		Font font= parent.getFont();
		Group group= new Group(parent, SWT.NONE);
		group.setText("NanoVM &Project:");
		GridData gd = new GridData(GridData.FILL_HORIZONTAL);
		group.setLayoutData(gd);
		GridLayout layout = new GridLayout();
		layout.numColumns = 2;
		group.setLayout(layout);
		group.setFont(font);

		fProjText = new Text(group, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		fProjText.setLayoutData(gd);
		fProjText.setFont(font);
		fProjText.addModifyListener(fListener);
		
		fProjButton = createPushButton(group, "&Browse...", null);
		fProjButton.addSelectionListener(fListener);
	}
	
	/**
	 * Creates the widgets for specifying a config file.
	 * 
	 * @param parent the parent composite
	 */
	private void createConfigEditor(Composite parent) {
		Font font= parent.getFont();
		Group group= new Group(parent, SWT.NONE);
		group.setText("NanoVM &Configuration:");
		GridData gd = new GridData(GridData.FILL_HORIZONTAL);
		group.setLayoutData(gd);
		GridLayout layout = new GridLayout();
		layout.numColumns = 2;
		group.setLayout(layout);
		group.setFont(font);

		fConfigText = new Text(group, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		fConfigText.setLayoutData(gd);
		fConfigText.setFont(font);
		fConfigText.addModifyListener(fListener);
		
		fConfigButton = createPushButton(group, "&Browse...", null);
		fConfigButton.addSelectionListener(fListener);
	}
	
	/**
	 * Creates the widgets for specifying a main type.
	 * 
	 * @param parent the parent composite
	 */
	private void createMainClassEditor(Composite parent) {
		Font font= parent.getFont();
		Group mainGroup= new Group(parent, SWT.NONE);
		mainGroup.setText("NanoVM &Main Class");
		GridData gd = new GridData(GridData.FILL_HORIZONTAL);
		mainGroup.setLayoutData(gd);
		GridLayout layout = new GridLayout();
		layout.numColumns = 2;
		mainGroup.setLayout(layout);
		mainGroup.setFont(font);

		fMainClassText = new Text(mainGroup, SWT.SINGLE | SWT.BORDER);
		gd = new GridData(GridData.FILL_HORIZONTAL);
		fMainClassText.setLayoutData(gd);
		fMainClassText.setFont(font);
		fMainClassText.addModifyListener(fListener);
		
		fMainClassButton = createPushButton(mainGroup,"&Search...", null);
		fMainClassButton.addSelectionListener(fListener);
	}

	public void setDefaults(ILaunchConfigurationWorkingCopy configuration) {
		configuration.setAttribute(IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME, "");
		configuration.setAttribute(NanoVMUI.ATTR_CONFIGURATION_NAME, "");
		configuration.setAttribute(NanoVMUI.ATTR_MAIN_CLASS_NAME, "");
	}

	public void initializeFrom(ILaunchConfiguration configuration) {				
		try {	
			this.fProjText.setText( configuration.getAttribute(IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME, ""));	
			this.fConfigText.setText( configuration.getAttribute(NanoVMUI.ATTR_CONFIGURATION_NAME, ""));
			this.fMainClassText.setText( configuration.getAttribute(NanoVMUI.ATTR_MAIN_CLASS_NAME, ""));
		} catch (CoreException ce) {
			setErrorMessage(ce.getMessage());
		}
	}

	public void performApply(ILaunchConfigurationWorkingCopy configuration) {			
		configuration.setAttribute(IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME, fProjText.getText().trim());
		configuration.setAttribute(NanoVMUI.ATTR_CONFIGURATION_NAME, this.fConfigText.getText().trim());
		configuration.setAttribute(NanoVMUI.ATTR_MAIN_CLASS_NAME, this.fMainClassText.getText().trim());
	}

	public String getName() {
		return "Main";
	}

	/**
	 * Show a dialog that lets the user select a project.  This in turn provides
	 * context for the main type, allowing the user to key a main type name, or
	 * constraining the search for main types to the specified project.
	 */
	protected void handleProjectButtonSelected() {
		IJavaProject project = chooseNanoVMProject();
		if (project == null) {
			return;
		}
		
		String projectName = project.getElementName();
		fProjText.setText(projectName);		
	}
	
	/**
	 * A listener which handles widget change events for the controls
	 * in this tab.
	 */
	private class WidgetListener implements ModifyListener, SelectionListener {
		public void modifyText(ModifyEvent e) {
			updateLaunchConfigurationDialog();
		}
		@SuppressWarnings("restriction")
		public void widgetSelected(SelectionEvent event) {
			Object source = event.getSource();
			if (source == fProjButton) {
				handleProjectButtonSelected();
			} else if(source == fMainClassButton){	
				IJavaSearchScope searchScope = SearchEngine.createJavaSearchScope(getNanoVMProjects(), IJavaSearchScope.SOURCES);
				IJavaProject project = getNanoVMProject();
				if(project != null){
					searchScope = SearchEngine.createJavaSearchScope(new IJavaElement[]{project}, IJavaSearchScope.SOURCES);					
				}
				IType[] types = null;
				try {
					types = new org.eclipse.jdt.internal.debug.ui.launcher.MainMethodSearchEngine().searchMainMethods(getLaunchConfigurationDialog(), searchScope, true);
				}//end try 
				catch (InvocationTargetException e) {
					setErrorMessage(e.getMessage());
					return;
				}//end catch 
				catch (InterruptedException e) {
					setErrorMessage(e.getMessage());
					return;
				}//end catch
				SelectionDialog dialog = null;
				try {
					dialog = JavaUI.createTypeDialog(
								getShell(),
								getLaunchConfigurationDialog(),
								SearchEngine.createJavaSearchScope(types),
								IJavaElementSearchConstants.CONSIDER_CLASSES, 
								false,
								"*"); //$NON-NLS-1$
				} catch (JavaModelException e) {
					setErrorMessage(e.getMessage());
					return;
				}//end catch
				dialog.setTitle("NanoVM Main Class Selection");
				dialog.setMessage("Choose a NanoVM main &class to launch:");
				
				if (dialog.open() == Window.OK) {	
					IType type = (IType) dialog.getResult()[0];
					fMainClassText.setText(type.getFullyQualifiedName());
					fProjText.setText(type.getJavaProject().getElementName());
				}				
			} else if(source == fConfigButton){
				IJavaProject project = getNanoVMProject();
				if(project != null){
					SelectionDialog dialog = new ResourceListSelectionDialog(getShell(), project.getProject(), IResource.FILE){
						protected String adjustPattern() {
							String pattern = super.adjustPattern();
							if("".equals(pattern) || pattern.endsWith("*"))
								pattern = pattern + ".config";							
					        return pattern;
						}
					};
					dialog.setTitle("NanoVM Configuration File Selection");
					dialog.setMessage("Choose a NanoVM launch &configuration file:");
					
					if (dialog.open() == Window.OK) {
						IResource resource = (IResource) dialog.getResult()[0];
						fConfigText.setText(resource.getProjectRelativePath().toString());
					}
				}else{
					MessageDialog.openWarning(getShell(), "Warning", "Select first a NanoVM project!");
				}
				
			} else {
				updateLaunchConfigurationDialog();
			}
		}
		public void widgetDefaultSelected(SelectionEvent e) {
		}
	}

	private WidgetListener fListener = new WidgetListener();

	protected IJavaProject chooseNanoVMProject() {
		IJavaProject[] projects;
		try {
			ArrayList<IJavaProject> nanoVMProjects = new ArrayList<IJavaProject>();
			for(IJavaProject project : getJavaModel().getJavaProjects()){
				if(project.getProject().hasNature(NanoVMUI.NATURE_ID))
					nanoVMProjects.add(project);
			}
			projects = nanoVMProjects.toArray(new IJavaProject[nanoVMProjects.size()]);
		} catch (Exception e) {			
			projects= new IJavaProject[0];
			setErrorMessage(e.getMessage());
		}
		ILabelProvider labelProvider= new JavaElementLabelProvider();
		ElementListSelectionDialog dialog= new ElementListSelectionDialog(getShell(), labelProvider);
		dialog.setTitle("NanoVM Project Selection");
		dialog.setMessage("Choose a NanoVM &project to constrain the search for main types:");
		dialog.setElements(projects);
		
		IJavaProject javaProject = getNanoVMProject();
		if (javaProject != null) {
			dialog.setInitialSelections(new Object[] { javaProject });
		}
		if (dialog.open() == Window.OK) {			
			return (IJavaProject) dialog.getFirstResult();
		}			
		return null;		
	}
	
	protected IJavaProject[] getNanoVMProjects(){
		IJavaProject[] projects;
		try {
			ArrayList<IJavaProject> nanoVMProjects = new ArrayList<IJavaProject>();
			for(IJavaProject project : getJavaModel().getJavaProjects()){
				if(project.getProject().hasNature(NanoVMUI.NATURE_ID))
					nanoVMProjects.add(project);
			}
			projects = nanoVMProjects.toArray(new IJavaProject[nanoVMProjects.size()]);
		} catch (Exception e) {			
			projects= new IJavaProject[0];
			setErrorMessage(e.getMessage());
		}
		return projects;
	}
	
	protected IJavaProject getNanoVMProject() {
		String projectName = fProjText.getText().trim();
		if (projectName.length() < 1) {
			return null;
		}
		IJavaProject project = null;
		project = getJavaModel().getJavaProject(projectName);
		return project;		
	}
	
	private IJavaModel getJavaModel(){
		return JavaCore.create(ResourcesPlugin.getWorkspace().getRoot());
	}
	
	@Override
	public boolean isValid(ILaunchConfiguration config) {
		setErrorMessage(null);
		setMessage(null);
		String name = fProjText.getText().trim();
		if (name.length() > 0) {
			IWorkspace workspace = ResourcesPlugin.getWorkspace();
			IStatus status = workspace.validateName(name, IResource.PROJECT);
			if (status.isOK()) {
				IProject project= ResourcesPlugin.getWorkspace().getRoot().getProject(name);
				if (!project.exists()) {
					setErrorMessage("Project '" + name + "' does not exist"); 
					return false;
				}//end if
				if (!project.isOpen()) {
					setErrorMessage("Project '" + name + "' is not open"); 
					return false;
				}//end if
			}//end if 
			else {
				setErrorMessage("Workspace error"); 
				return false;
			}//end else
		}//end if
		name = fConfigText.getText().trim();
		if (name.length() == 0) {
			setErrorMessage("Configuration file not specified"); 
			return false;
		}//end if
		name = fMainClassText.getText().trim();
		if (name.length() == 0) {
			setErrorMessage("Main class not specified"); 
			return false;
		}//end if
		return true;
	}
}
