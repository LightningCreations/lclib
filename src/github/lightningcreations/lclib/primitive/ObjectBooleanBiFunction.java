package github.lightningcreations.lclib.primitive;

import java.util.function.BiFunction;

public interface ObjectBooleanBiFunction<A, R> extends BiFunction<A,Boolean,R> {
	R apply(A a,boolean b);
	default R apply(A a,Boolean b) {
		return apply(a,!!b);
	}
}
