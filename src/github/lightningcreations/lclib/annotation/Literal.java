package github.lightningcreations.lclib.annotation;

import static java.lang.annotation.RetentionPolicy.RUNTIME;
import static java.lang.annotation.ElementType.TYPE;

import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;

/**
 * States that a given type is a literal type, that is, it can be represented as a constant.
 * This is based on a future proposal for the JCP, Literal Types and Annotations
 * @author chorm
 */
@Documented
@Retention(RUNTIME)
@Target(TYPE)
public @interface Literal {
}
