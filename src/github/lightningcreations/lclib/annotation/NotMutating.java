package github.lightningcreations.lclib.annotation;

import static java.lang.annotation.ElementType.METHOD;
import static java.lang.annotation.RetentionPolicy.SOURCE;

import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;

/**
 * Defines that a method will not mutate the object.
 * The underlying state of the object will not be affected by this call
 */
@Documented
@Retention(SOURCE)
@Target(METHOD)
public @interface NotMutating {

}
