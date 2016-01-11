/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.ui;

import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.nanovm.internal.ui.config.editor.ConfigCodeScanner;
import org.eclipse.nanovm.internal.ui.config.editor.ConfigPartitionScanner;
import org.eclipse.ui.plugin.AbstractUIPlugin;
import org.osgi.framework.BundleContext;

/**
 * The activator class controls the plug-in life cycle
 * 
 * @author tugstugi@yahoo.com
 */
public class NanoVMUI extends AbstractUIPlugin {

	/** The plug-in ID */
	public static final String PLUGIN_ID = "org.eclipse.nanovm.ui";
	
	/** The unique ID for the classpath container */
    public static final String LIBRARY_CONTAINER_ID= PLUGIN_ID + ".nanovmcontainer";
    
    /** The unique ID for the nanovm project. */
    public static final String NATURE_ID= PLUGIN_ID + ".nanovmnature";
    
    /** The unique ID for the nanovm project builder. */
    public static final String BUILDER_ID= PLUGIN_ID + ".nanovmbuilder";
    
    /** The unique ID for launching. */
    public static final String LAUNCHER = PLUGIN_ID + ".nanovmlauncher";
    
    /** The unique ID for launching configuration. */
    public static final String ATTR_CONFIGURATION_NAME = PLUGIN_ID + ".CONFIG_NAME";
    
    /** The unique ID for main class to be launch. */
    public static final String ATTR_MAIN_CLASS_NAME = PLUGIN_ID + ".CLASS_NAME";
       
	/** The shared instance */
	private static NanoVMUI plugin;
	
	/**
	 * The constructor
	 */
	public NanoVMUI() {
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
	public static NanoVMUI getDefault() {
		return plugin;
	}
	
	private ConfigCodeScanner fConfigCodeScanner = null;
	public ConfigCodeScanner getConfigCodeScanner(){
		if(fConfigCodeScanner == null){
			fConfigCodeScanner = new ConfigCodeScanner();
		}
		return fConfigCodeScanner;
	}
	
	private ConfigPartitionScanner fConfigPartitionScanner = null;
	public ConfigPartitionScanner getConfigPartitionScanner(){
		if(fConfigPartitionScanner == null){
			fConfigPartitionScanner = new ConfigPartitionScanner();
		}
		return fConfigPartitionScanner;
	}
	
	public static void log(Throwable e) {
		log(new Status(IStatus.ERROR, PLUGIN_ID, IStatus.ERROR, e.getMessage(), e)); 
	}
	
	public static void log(IStatus status) {
		getDefault().getLog().log(status);
	}
}
