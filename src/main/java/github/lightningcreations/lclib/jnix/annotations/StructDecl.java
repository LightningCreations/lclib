package github.lightningcreations.lclib.jnix.annotations;

import static java.lang.annotation.ElementType.TYPE;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

import java.lang.annotation.Retention;
import java.lang.annotation.Target;

import github.lightningcreations.lclib.jnix.annotations.Type.CVQualifiers;

@Retention(RUNTIME)
@Target(TYPE)
public @interface StructDecl {
	public enum AccessGroup{
		PUBLIC, PRIVATE, PROTECTED;
	};
	public enum DeclSeqSpecifier{
		STATIC, MUTABLE, VIRTUAL
	};
	public enum RefQualfiers{
		NONE, LVALUE_REF, RVALUE_REF
	};
	public enum ExceptionSpecifier{
		NOEXCEPT_TRUE,NOEXCEPT_FALSE,NONE
	};
	public @interface Field{
		Type type();
		DeclSeqSpecifier[] specifiers()default{};
		AccessGroup group()default AccessGroup.PUBLIC;
		String name();
	};
	public @interface Method{
		Type returnType()default @Type(name="void");
		Type[] parameterTypes()default{};
		DeclSeqSpecifier[] specifiers()default{};
		CVQualifiers[] cvqualfiers()default{};
		ExceptionSpecifier specifier()default ExceptionSpecifier.NONE;
		
	};
	public @interface Supertype{
		String value();
		AccessGroup policy()default AccessGroup.PUBLIC;
		boolean isVirtual()default false;
	};
	String name();
	Field[] fields()default{};
	Supertype[] supertypes()default{};
	Method[] methods()default{};
}
