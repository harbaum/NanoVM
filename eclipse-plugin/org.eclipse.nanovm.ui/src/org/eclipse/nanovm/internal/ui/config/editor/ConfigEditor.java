/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.config.editor;

import org.eclipse.ui.editors.text.TextEditor;

/**
 * A text editor for NanoVM configuration and native files.
 * 
 * @author tugstugi@yahoo.com
 */
public class ConfigEditor extends TextEditor {
	public ConfigEditor(){
		setDocumentProvider(new ConfigDocumentProvider());
	}
	
	@Override
	protected void initializeEditor() {
		super.initializeEditor();
		setSourceViewerConfiguration(new ConfigSourceViewerConfiguration());
	}
}
