package github.lightningcreations.lclib.annotation;

import static java.lang.annotation.ElementType.METHOD;
import static java.lang.annotation.ElementType.CONSTRUCTOR;

import java.lang.annotation.Target;

@Target({METHOD, CONSTRUCTOR})
public @interface Constant {

}
