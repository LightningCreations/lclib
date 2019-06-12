package github.lightningcreations.lclib.primitive;

import java.util.function.DoublePredicate;

public interface DoubleToBooleanFunction {
	boolean apply(double d);
	static DoubleToBooleanFunction fromPredicate(DoublePredicate p) {
		return p::test;
	}
}
