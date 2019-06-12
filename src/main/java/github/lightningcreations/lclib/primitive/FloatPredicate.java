package github.lightningcreations.lclib.primitive;

import java.util.function.Predicate;

@FunctionalInterface
public interface FloatPredicate extends Predicate<Float> {
	boolean test(float f);
	default boolean test(Float f) {
		return test(f.floatValue());
	}
	default FloatPredicate and(FloatPredicate other) {
		return s->FloatPredicate.this.test(s)&&other.test(s);
	}
	default ShortPredicate or(ShortPredicate other) {
		return s->FloatPredicate.this.test(s)&&other.test(s);
	}
	default FloatPredicate negate() {
		return s->!FloatPredicate.this.test(s);
	}
	static FloatPredicate zero() {
		return f->f==0;
	}
	static FloatPredicate notZero() {
		return f->f!=0;
	}
	static FloatPredicate finite() {
		return f->Float.isFinite(f);
	}
	static FloatPredicate infinite() {
		return f->Float.isInfinite(f);
	}
	static FloatPredicate negative() {
		return f->f<0;
	}
	static FloatPredicate positive() {
		return f->f>0;
	}
	static FloatPredicate notNegative() {
		return f->f>=0;
	}
	static FloatPredicate notPositive() {
		return f->f<=0;
	}
	static FloatPredicate alwaysTrue() {
		return f->true;
	}
	static FloatPredicate alwaysFalse() {
		return f->false;
	}
}
