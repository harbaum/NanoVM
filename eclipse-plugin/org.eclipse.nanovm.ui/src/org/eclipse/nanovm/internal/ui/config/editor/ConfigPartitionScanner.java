/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.config.editor;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jface.text.rules.EndOfLineRule;
import org.eclipse.jface.text.rules.IPredicateRule;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.RuleBasedPartitionScanner;
import org.eclipse.jface.text.rules.Token;

/**
 * A partition scanner for NanoVM configuration and native files.
 * 
 * @author tugstugi@yahoo.com
 */
public class ConfigPartitionScanner extends RuleBasedPartitionScanner {
	public final static String[] CONFIG_PARTITION_TYPES= new String[] {};
	
	public ConfigPartitionScanner(){
		List<IRule> rules= new ArrayList<IRule>();
		
		// Add rule for single line comments.
		rules.add(new EndOfLineRule("#", Token.UNDEFINED)); //$NON-NLS-1$
		
		IPredicateRule[] result= new IPredicateRule[rules.size()];
		rules.toArray(result);
		setPredicateRules(result);
	}

}
