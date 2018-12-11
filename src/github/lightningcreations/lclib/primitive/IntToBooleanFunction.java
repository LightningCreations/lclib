package github.lightningcreations.lclib.primitive;

import java.util.function.IntPredicate;

public interface IntToBooleanFunction {
	boolean apply(int i);
	static IntToBooleanFunction fromPredicate(IntPredicate p) {
		return p::test;
	}
}
