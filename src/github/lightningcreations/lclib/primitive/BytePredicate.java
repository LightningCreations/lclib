package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

@FunctionalInterface
public interface BytePredicate extends Predicate<Byte> {
	boolean test(byte b);
	default boolean test(Byte b) {
		return test(b.byteValue());
	}
	default BytePredicate and(BytePredicate other) {
		return b->BytePredicate.this.test(b)&&other.test(b);
	}
	default BytePredicate or(BytePredicate other) {
		return b->BytePredicate.this.test(b)&&other.test(b);
	}
	default BytePredicate negate() {
		return b->!BytePredicate.this.test(b);
	}
	static BytePredicate zero() {
		return b->b==0;
	}
	static BytePredicate notZero() {
		return b->b!=0;
	}
	static BytePredicate positive() {
		return b->b>0;
	}
	static BytePredicate negative() {
		return b->b<0;
	}
	static BytePredicate notNegative() {
		return b->b>=0;
	}
	static BytePredicate notPositive() {
		return b->b<=0;
	}
	static BytePredicate alwaysTrue() {
		return b->true;
	}
	static BytePredicate alwaysFalse() {
		return b->false;
	}
}
