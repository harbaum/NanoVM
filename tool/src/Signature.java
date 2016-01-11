package sli.kim.classfile;

/**
* This class provides a useful routine to convert signatures to 
* regular java type declarations.
*/
public class Signature {
	// Signature is not meant to be instantiated
	private Signature() {}

	/**
	* Convert a signature into a normal type declaration.
	* <ul>
	* <li>"I",null -&gt; "int"
	* <li>"Z","foo" -&gt; "boolean foo"
	* <li>"[J","foo" -&gt; "long[] foo"
	* <li>"Lmypackage/MyClass;",null -&gt; "mypackage.MyClass"
	* <li>"()V",null -&gt; "void ()"
	* <li>"(S[Ljava/lang/String;)V","foo" -&gt; "void foo (short, String[])"
	* </ul>
	* @param sig the signature.
	* @param name the name of the field or method.  This can be null.
	*/
	public static String signatureToTypeDecl(String sig, String name) {
		String[] sigOut = new String[1];
		sigOut[0] = sig;
		return _sigToType(sigOut, name);
	}

	private static String _sigToType(String[] sigOut, String name) {
		if (sigOut[0] == null || sigOut[0].length() == 0)
			return "";

		String sig = sigOut[0];
		String result = "";
		
		switch (sig.charAt(0)) {
		case 'V':
			result = "void";
			sig = sig.substring(1);
			break;
		case 'B':
			result = "byte";
			sig = sig.substring(1);
			break;
		case 'S':
			result = "short";
			sig = sig.substring(1);
			break;
		case 'I':
			result = "int";
			sig = sig.substring(1);
			break;
		case 'J':
			result = "long";
			sig = sig.substring(1);
			break;
		case 'F':
			result = "float";
			sig = sig.substring(1);
			break;
		case 'D':
			result = "double";
			sig = sig.substring(1);
			break;
		case 'Z':
			result = "boolean";
			sig = sig.substring(1);
			break;
		case 'L':
			{
				int semi = sig.indexOf(';');
				result = sig.substring(1, semi).replace('/', '.');
				if (result.startsWith("java.lang."))
					result = result.substring(10);
				sig = sig.substring(semi+1);
			}
			break;
		case '[':
			{
				sig = sig.substring(1);
				sigOut[0] = sig;
				result = _sigToType(sigOut, null) + "[]";
				sig = sigOut[0];
			}
			break;
		case '(':
			{
				int closeParen = sig.indexOf(')');
				String[] paramOut = new String[1];
				paramOut[0] = sig.substring(1, closeParen);
				String paramStr = "";
				while (paramOut[0] != null && paramOut[0].length() > 0) {
					paramStr += _sigToType(paramOut, null);
					if (paramOut[0] != null && paramOut[0].length() > 0)
						paramStr += ", ";
				}
				String[] returnOut = new String[1];
				returnOut[0] = sig.substring(closeParen+1);
				String returnStr = _sigToType(returnOut, name);
				result = returnStr + " (" + paramStr + ')';
				name = null;
			}
			break;
		}

		sigOut[0] = sig;
		return result + (name != null ? " " + name : "");
	}

	/**
	* Simple unit test.  Usage:
	* <pre>java sli.kim.classfile.Signature &lt;signature&gt; [&lt;name&gt;]</pre>
	*/
	public static void main(String[] args) {
		if (args.length == 2)
			System.out.println(signatureToTypeDecl(args[0], args[1]));
		else
			System.out.println(signatureToTypeDecl(args[0], null));
	}
}
