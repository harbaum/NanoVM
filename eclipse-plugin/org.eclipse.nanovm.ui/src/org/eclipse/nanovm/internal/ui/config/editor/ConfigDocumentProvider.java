/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.config.editor;

import org.eclipse.core.runtime.CoreException;
import org.eclipse.jface.text.IDocument;
import org.eclipse.jface.text.IDocumentPartitioner;
import org.eclipse.jface.text.rules.FastPartitioner;
import org.eclipse.nanovm.ui.NanoVMUI;
import org.eclipse.ui.editors.text.FileDocumentProvider;

/**
 * A document provider for NanoVM configuration and native files.
 * 
 * @author tugstugi@yahoo.com
 */
public class ConfigDocumentProvider extends FileDocumentProvider {
	protected IDocument createDocument(Object element) throws CoreException {
		IDocument document = super.createDocument(element);
		if (document != null) {
			IDocumentPartitioner partitioner = new FastPartitioner(NanoVMUI.getDefault().getConfigPartitionScanner(), ConfigPartitionScanner.CONFIG_PARTITION_TYPES);
			document.setDocumentPartitioner(partitioner);
			partitioner.connect(document);			
		}
		return document;
	}
}
