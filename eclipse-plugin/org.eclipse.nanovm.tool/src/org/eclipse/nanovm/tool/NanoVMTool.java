/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.tool;

import org.eclipse.core.runtime.Plugin;
import org.osgi.framework.BundleContext;

/**
 * The activator class controls the plug-in life cycle
 * @author tugstugi@yahoo.com
 */
public class NanoVMTool extends Plugin {

	/** The plug-in ID */
	public static final String PLUGIN_ID = "org.eclipse.nanovm.tool";
	
	/** Tool libray file name */
	public static final String LIBRARY_NAME = "NanoVMTool.jar";
	
	/** Configuration file extension */
	public static final String CONFIGURATION_FILE_EXTENSION = ".config";
	
	/** Native file extension */
	public static final String NATIVE_FILE_EXTENSION = ".native";

	// The shared instance
	private static NanoVMTool plugin;

	/**
	 * The constructor
	 */
	public NanoVMTool() {
		plugin = this;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.core.runtime.Plugins#start(org.osgi.framework.BundleContext)
	 */
	public void start(BundleContext context) throws Exception {
		super.start(context);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.eclipse.core.runtime.Plugin#stop(org.osgi.framework.BundleContext)
	 */
	public void stop(BundleContext context) throws Exception {
		plugin = null;
		super.stop(context);
	}

	/**
	 * Returns the shared instance
	 * 
	 * @return the shared instance
	 */
	public static NanoVMTool getDefault() {
		return plugin;
	}
}
