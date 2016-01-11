/**
 * Plugin for NanoVM.
 */
package org.eclipse.nanovm.internal.ui.config.editor;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jface.text.TextAttribute;
import org.eclipse.jface.text.rules.EndOfLineRule;
import org.eclipse.jface.text.rules.IRule;
import org.eclipse.jface.text.rules.IToken;
import org.eclipse.jface.text.rules.IWhitespaceDetector;
import org.eclipse.jface.text.rules.IWordDetector;
import org.eclipse.jface.text.rules.RuleBasedScanner;
import org.eclipse.jface.text.rules.Token;
import org.eclipse.jface.text.rules.WhitespaceRule;
import org.eclipse.jface.text.rules.WordRule;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.RGB;
import org.eclipse.swt.widgets.Display;

/**
 * A configuration keyword scanner for NanoVM configuration and native files.
 * 
 * @author tugstugi@yahoo.com
 */
public class ConfigCodeScanner extends RuleBasedScanner {
	public static final RGB KEYWORD= new RGB(0, 0, 128);
	public static final RGB SINGLE_LINE_COMMENT= new RGB(63, 127, 95);
	public static final RGB DEFAULT= new RGB(0, 0, 0);
	
	private static String[] fgKeywords = {
		"name", "maxsize", "target", "filename", "speed", "native", "class", "field", "method"
	};
	
	public ConfigCodeScanner(){
		IToken keyword = new Token(new TextAttribute(new Color(Display.getCurrent(), KEYWORD), null, SWT.BOLD));
		IToken comment = new Token(new TextAttribute(new Color(Display.getCurrent(), SINGLE_LINE_COMMENT)));
		IToken other = new Token(new TextAttribute(new Color(Display.getCurrent(), DEFAULT)));
		
		List<IRule> rules = new ArrayList<IRule>();
		
		// Add rule for single line comments.
		rules.add(new EndOfLineRule("#", comment)); //$NON-NLS-1$
		
		// Add generic whitespace rule.
		rules.add(new WhitespaceRule(new IWhitespaceDetector(){
			public boolean isWhitespace(char character) {
				return Character.isWhitespace(character);
			}
		}));

		// Add word rule for keywords.
		WordRule wordRule = new WordRule(new IWordDetector(){
			public boolean isWordPart(char character) {		
				return Character.isJavaIdentifierPart(character);
			}
			
			public boolean isWordStart(char character) {
				return Character.isJavaIdentifierStart(character);
			}
		}, other);
		for (int i = 0; i < fgKeywords.length; i++)
			wordRule.addWord(fgKeywords[i], keyword);		
		rules.add(wordRule);

		IRule[] result = new IRule[rules.size()];
		rules.toArray(result);
		setRules(result);
	}
}
