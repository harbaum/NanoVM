/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.launch;

import java.io.File;
import java.text.MessageFormat;
import java.util.Map;

import org.eclipse.core.resources.IProject;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IPath;
import org.eclipse.core.runtime.IProgressMonitor;
import org.eclipse.core.runtime.NullProgressMonitor;
import org.eclipse.debug.core.ILaunch;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.jdt.core.IJavaProject;
import org.eclipse.jdt.core.JavaCore;
import org.eclipse.jdt.launching.ExecutionArguments;
import org.eclipse.jdt.launching.IJavaLaunchConfigurationConstants;
import org.eclipse.jdt.launching.IVMRunner;
import org.eclipse.jdt.launching.JavaLaunchDelegate;
import org.eclipse.jdt.launching.VMRunnerConfiguration;
import org.eclipse.nanovm.ui.NanoVMUI;

/**
 * Launcher class.
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMLauncher extends JavaLaunchDelegate {

	@Override
	public void launch(ILaunchConfiguration configuration, String mode,
			ILaunch launch, IProgressMonitor monitor) throws CoreException {

		if (monitor == null) {
			monitor = new NullProgressMonitor();
		}

		monitor.beginTask(MessageFormat.format(
				"{0}...", new Object[] { configuration.getName() }), 3); //$NON-NLS-1$
		// check for cancellation
		if (monitor.isCanceled()) {
			return;
		}

		monitor.subTask("Verifying launch attributes...");

		IVMRunner runner = getVMRunner(configuration, mode);

		File workingDir = verifyWorkingDirectory(configuration);
		String workingDirName = null;
		if (workingDir != null) {
			workingDirName = workingDir.getAbsolutePath();
		}

		// Environment variables
		String[] envp = getEnvironment(configuration);

		// Program & VM arguments	

		String projectName = configuration.getAttribute(
				IJavaLaunchConfigurationConstants.ATTR_PROJECT_NAME, "");
		IProject project = ResourcesPlugin.getWorkspace().getRoot().getProject(
				projectName);
		IPath projectPath = project.getLocation();
		IJavaProject javaProject = JavaCore.create(project);
		IPath classPath = ResourcesPlugin.getWorkspace().getRoot()
				.getLocation().append(javaProject.getOutputLocation());
		String configFile = configuration.getAttribute(
				NanoVMUI.ATTR_CONFIGURATION_NAME, "");
		IPath configPath = projectPath.append(configFile);
		String mainClassToLaucn = configuration.getAttribute(
				NanoVMUI.ATTR_MAIN_CLASS_NAME, "");

		String pgmArgs = getProgramArguments(configuration) + " "
				+ "\"" + configPath.toOSString() + "\"" + " " + "\"" + classPath.toOSString() + "\"" + " "
				+ mainClassToLaucn.replace('.', '/');
						
		String vmArgs = getVMArguments(configuration);
		ExecutionArguments execArgs = new ExecutionArguments(vmArgs, pgmArgs);

		// VM-specific attributes
		Map vmAttributesMap = getVMSpecificAttributesMap(configuration);

		// Classpath
		String[] classpath = getClasspath(configuration);

		// Create VM config
		VMRunnerConfiguration runConfig = new VMRunnerConfiguration(
				"NanoVMTool", classpath);
		runConfig.setProgramArguments(execArgs.getProgramArgumentsArray());
		runConfig.setEnvironment(envp);
		runConfig.setVMArguments(execArgs.getVMArgumentsArray());
		runConfig.setWorkingDirectory(workingDirName);
		runConfig.setVMSpecificAttributesMap(vmAttributesMap);

		// Bootpath
		runConfig.setBootClassPath(getBootpath(configuration));

		// check for cancellation
		if (monitor.isCanceled()) {
			return;
		}

		// stop in main
		prepareStopInMain(configuration);

		// done the verification phase
		monitor.worked(1);

		monitor.subTask("Creating source locator..");
		// set the default source locator if required
		setDefaultSourceLocator(launch, configuration);
		monitor.worked(1);

		// Launch the configuration - 1 unit of work
		runner.run(runConfig, launch, monitor);

		// check for cancellation
		if (monitor.isCanceled()) {
			return;
		}

		monitor.done();
	}
}
