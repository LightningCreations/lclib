package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

@FunctionalInterface
public interface ShortPredicate extends Predicate<Short> {
	boolean test(short s);
	default boolean test(Short s) {
		return test(s.shortValue());
	}
	default ShortPredicate and(ShortPredicate other) {
		return s->ShortPredicate.this.test(s)&&other.test(s);
	}
	default ShortPredicate or(ShortPredicate other) {
		return s->ShortPredicate.this.test(s)&&other.test(s);
	}
	default ShortPredicate negate() {
		return s->!ShortPredicate.this.test(s);
	}
	static ShortPredicate zero() {
		return b->b==0;
	}
	static ShortPredicate notZero() {
		return b->b!=0;
	}
	static ShortPredicate positive() {
		return b->b>0;
	}
	static ShortPredicate negative() {
		return b->b<0;
	}
	static ShortPredicate notNegative() {
		return b->b>=0;
	}
	static ShortPredicate notPositive() {
		return b->b<=0;
	}
	static ShortPredicate alwaysTrue() {
		return b->true;
	}
	static ShortPredicate alwaysFalse() {
		return b->false;
	}
}
