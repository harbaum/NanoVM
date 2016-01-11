/**
 * Plugin for NanoVM runtime.
 */
package org.eclipse.nanovm.runtime;

import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Plugin;
import org.eclipse.core.runtime.Status;
import org.osgi.framework.BundleContext;

/**
 * The activator class controls the plug-in life cycle
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMRuntime extends Plugin {

	/** The plug-in ID */
	public static final String PLUGIN_ID = "org.eclipse.nanovm.runtime";
	
	/** Runtime libray file name */
	public static final String LIBRARY_NAME = "NanoVMRuntime.jar";

	/** The shared instance */
	private static NanoVMRuntime plugin;
	
	/**
	 * The constructor
	 */
	public NanoVMRuntime() {
		plugin = this;
	}

	@Override
	public void start(BundleContext context) throws Exception {
		super.start(context);
	}

	@Override
	public void stop(BundleContext context) throws Exception {
		plugin = null;
		super.stop(context);
	}

	/**
	 * Returns the shared instance
	 *
	 * @return the shared instance
	 */
	public static NanoVMRuntime getDefault() {
		return plugin;
	}

	public static void log(Throwable e) {
		log(new Status(IStatus.ERROR, PLUGIN_ID, IStatus.ERROR, e.getMessage(), e)); 
	}
	
	public static void log(IStatus status) {
		getDefault().getLog().log(status);
	}
}
