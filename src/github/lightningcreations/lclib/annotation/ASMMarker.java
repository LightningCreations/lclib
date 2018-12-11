package github.lightningcreations.lclib.annotation;

import static java.lang.annotation.ElementType.ANNOTATION_TYPE;

import java.lang.annotation.Documented;
import java.lang.annotation.Target;

/**
 * Defines an annotation that is used to bind to Bytecode Modifications 
 * @author Connor
 */
@Documented
@Target(ANNOTATION_TYPE)
public @interface ASMMarker {

}
