/**
 * 
 */
/**
 * @author Connor Horman
 *
 */
module github.lightningcreations.lclib {
	requires java.base;
	requires java.desktop;
	
	exports github.lightningcreations.lclib;
	exports github.lightningcreations.lclib.stream;
	exports github.lightningcreations.lclib.annotation;
	exports github.lightningcreations.lclib.jnix;
	exports github.lightningcreations.lclib.jnix.annotations;
	exports github.lightningcreations.lclib.jnix.types;
	exports github.lightningcreations.lclib.security;
	exports github.lightningcreations.lclib.primitive;
	exports github.lightningcreations.lclib.numerics;
}