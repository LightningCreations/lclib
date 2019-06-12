package github.lightningcreations.lclib.jnix.annotations;

import static java.lang.annotation.ElementType.TYPE;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

import java.lang.annotation.Retention;
import java.lang.annotation.Target;

@Retention(RUNTIME)
@Target(TYPE)
public @interface Type {
	public enum CVQualifiers{
		CONST, VOLATILE, CV, NONE
	};
	String name();
	CVQualifiers qualifiers() default CVQualifiers.NONE;
	
}
