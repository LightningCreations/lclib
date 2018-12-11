package github.lightningcreations.lclib.primitive;

import java.util.function.LongPredicate;

public interface LongToBooleanFunction {
	boolean apply(long l);
	static LongToBooleanFunction fromPredicate(LongPredicate p) {
		return p::test;
	}
}
