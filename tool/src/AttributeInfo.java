/**
* A class for storing information about a class/field/method attribute.
* This class is usually only used for non-standard attributes.  Standard
* attributes such as SourceFile, Code, LineNumberTable, LocalVariableTable,
* Exceptions, and ConstantValue are handled with specialized methods in
* the appropriate Info class.
*
* @see ClassInfo
* @see FieldInfo
* @see MethodInfo
*/
public class AttributeInfo {
	public String name;
	public byte[] data;

	public AttributeInfo(String name, byte[] data) {
		this.name = name;
		this.data = data;
	}

	public String getName() {
		return name;
	}

	public byte[] getData() {
		return data;
	}
}
