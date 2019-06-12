package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

@FunctionalInterface
public interface BooleanPredicate extends Predicate<Boolean> {
	boolean test(boolean b);
	default boolean test(Boolean b) {
		return test(b.booleanValue());
	}
	default BooleanPredicate and(BooleanPredicate other) {
		return b->BooleanPredicate.this.test(b)&&other.test(b);
	}
	default BooleanPredicate or(BooleanPredicate other) {
		return b->BooleanPredicate.this.test(b)||other.test(b);
	}
	default BooleanPredicate negate() {
		return b->!BooleanPredicate.this.test(b);
	}
	static BooleanPredicate identity() {
		return b->b;
	}
	static BooleanPredicate inverse() {
		return b->!b;
	}
	static BooleanPredicate alwaysTrue() {
		return b->true;
	}
	static BooleanPredicate alwaysFalse() {
		return b->false;
	}
}
